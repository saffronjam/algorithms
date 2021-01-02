#include "Algorithm.h"

#include <SFML/Graphics.hpp>

namespace Se
{
struct ContainerGroup
{
    ArrayList<Element> &elements;
    ArrayList<Element> &elementsRestart;
    ArrayList<Element> &elementsReset;
};

Algorithm::Algorithm(String name)
        : _name(Move(name)),
          _sleepDelay(sf::seconds(0.01f)),
          _state(State::WaitingForStart),
          _isActive(true),
          _minorDelay(false),
          _minorDelayTimer(0),
          _visType(VisType::Bars),
          _usingSpectrumColors(false),
          _barsVA(sf::Quads, 0)
{
    _nameText.setFont(*FontStore::Get("res/Fonts/segoeui.ttf"));
    _nameText.setCharacterSize(18);
    SetImage("res/Images/sample_image_forest.jpg");
}

void Algorithm::Draw(Scene &scene, const sf::FloatRect &rect)
{
    switch (_visType)
    {
        case VisType::Bars:
        {
            DrawBars(scene, rect);
            break;
        }
        case VisType::Circles:
        {
            DrawCircles(scene, rect);
            break;
        }
        case VisType::Hoops:
        {
            DrawHoops(scene, rect);
            break;
        }
        case VisType::Line:
        {
            DrawLine(scene, rect);
            break;
        }
        case VisType::ScatterPlot:
        {
            DrawScatterPlot(scene, rect);
            break;
        }
        case VisType::Image:
        {
            DrawImage(scene, rect);
            break;
        }
        default:
            break;
    }
}

void Algorithm::DrawName(Scene &scene, const sf::FloatRect &rect)
{
    _nameText.setString(GetName());
    _nameText.setPosition(rect.left + rect.width / 2.0f, rect.top - 35.0f);
    scene.Submit(_nameText, TextAlign::Middle);
}

void Algorithm::Start()
{
    if (_state == State::WaitingForStart)
    {
        CollectSorter();
        _state = State::Sorting;
        _sorter = std::thread([this]
                              { SortThreadFn(); });
    }
}

void Algorithm::Restart()
{
    if (_state == State::Sorting || _state == State::Paused || _state == State::Finished)
    {
        CollectSorter();
        _state = State::WaitingForStart;
        _elements = _elementsRestart;
    }
}

void Algorithm::Pause()
{
    if (_state == State::Sorting)
        _state = State::Paused;
}

void Algorithm::Resume()
{
    if (_state == State::Paused)
        _state = State::Sorting;
}

void Algorithm::Reset()
{
    CollectSorter();
    _state = State::WaitingForStart;
    _elements = _elementsReset;
    _elementsRestart = _elementsReset;
}

void Algorithm::Resize(size_t size)
{
    if (_elements.size() == size)
        return;

    _elements.clear();
    _elementsRestart.clear();
    _elementsReset.clear();
    _elements.resize(size);
    _elementsRestart.resize(size);
    _elementsReset.resize(size);

    long n = 1;
    std::generate(_elements.begin(), _elements.end(), [&n]
    {
        return Element(n++);
    });

    _elementsRestart = _elements;
    _elementsReset = _elements;

    // Resize draw cache
    _barsVA.resize(size * 4);
}

void Algorithm::SoftResize(size_t size)
{
    CollectSorter();
    while (_elementsReset.size() != size)
    {
        if (size < _elementsReset.size())
        {
            _elementsReset.pop_back();
        }
        else if (size > _elementsReset.size())
        {
            const auto pixelCoord = GetPixelCoord(_elementsReset.size());
            _elementsReset.push_back(Element(_elementsReset.size() + 1));
        }
    }
    _elements = _elementsReset;
    _elementsRestart = _elementsReset;

    // Resize draw cache
    _barsVA.resize(size * 4);
}

void Algorithm::SetImage(const std::string &filepath)
{
    Reset();
    _image = *ImageStore::Get(filepath);
    _imageRenderTexture.create(_image.getSize().x, _image.getSize().y);
}

void Algorithm::Shuffle(std::mt19937 generator)
{
    Reset();
    std::shuffle(_elementsRestart.begin(), _elementsRestart.end(),
                 generator);
    _elements = _elementsRestart;
}

void Algorithm::SetSleepDelay(sf::Time delay) noexcept
{
    _sleepDelay = delay;
    _minorDelay = (_sleepDelay.asMicroseconds() < 1000);
}

void Algorithm::SetVisType(VisType visType) noexcept
{
    _visType = visType;
}

ArrayList<Element> &Algorithm::GetElements()
{
    return _elements;
}

ArrayList<Element> &Algorithm::GetRestartElements()
{
    return _elementsRestart;
}

ArrayList<Element> &Algorithm::GetResetElements()
{
    return _elementsReset;
}

void Algorithm::SetValue(Element &element, long value)
{
    element.value = value;
    const auto pixelCoord = GetPixelCoord(element.value - 1l);
    element.pixel = _image.getPixel(pixelCoord.x, pixelCoord.y);
}

void Algorithm::SetColor(Element &element, const sf::Color &color)
{
    element.color = color;
}

void Algorithm::SwapElements(Element &first, Element &second)
{
    std::swap(first, second);
}

void Algorithm::PauseCheck()
{
    while (_state == State::Paused && _state != State::BeingCollected)
        sf::sleep(sf::seconds(0.01f));
}

void Algorithm::SleepDelay()
{
    if (!_minorDelay)
    {
        std::this_thread::sleep_for(std::chrono::microseconds(_sleepDelay.asMicroseconds()));
    }
    else
    {
        _minorDelayTimer += _sleepDelay.asMicroseconds();
        while (_minorDelayTimer > 1000)
        {
            std::this_thread::sleep_for(std::chrono::microseconds(1000));
            _minorDelayTimer -= 1000;
        }
    }
}

sf::Vector2u Algorithm::GetPixelCoord(size_t index) const
{
    const auto width = _image.getSize().x;
    const auto x = index % width;
    const auto y = std::floor(index / width);
    return sf::Vector2u(x, y);
}

sf::FloatRect Algorithm::GetScaledPixel(size_t index, size_t max) const
{
    const float width = _image.getSize().x;
    const float height = _image.getSize().y;

    const float sqrtMax = std::sqrt(max);
    float nPixelsX = std::floor(sqrtMax);
    const float newMax = std::pow(nPixelsX, 2);

    float scaledPixelWidth = width / nPixelsX;
    float scaledPixelHeight = height / nPixelsX;

    if (index >= newMax)
    {
        index = newMax - 1;
    }

    const sf::FloatRect scaledPixel(
            scaledPixelWidth * (index % static_cast<int>(nPixelsX)),
            scaledPixelHeight * std::floor(index / nPixelsX),
            std::max(1.0f, scaledPixelWidth),
            std::max(1.0f, scaledPixelHeight)
    );

    return scaledPixel;
}

sf::Vector2u Algorithm::GetClosestPixelCoord(size_t index, size_t max) const
{
    const float width = _image.getSize().x;
    const float height = _image.getSize().y;

    const float sqrtMax = std::sqrt(max);
    float nPixelsX = std::floor(sqrtMax);
    const float newMax = std::pow(nPixelsX, 2);

    float scaledPixelWidth = width / nPixelsX;
    float scaledPixelHeight = height / nPixelsX;

    if (index >= newMax)
    {
        index = newMax - 1;
    }

    const Uint32 x = scaledPixelWidth * (index % static_cast<int>(nPixelsX));
    const Uint32 y = scaledPixelHeight * std::floor(index / nPixelsX);

    return {x, y};
}

void Algorithm::SortThreadFn()
{
    bool alreadySorted = VerifyElements();

    if (!alreadySorted)
    {
        Sort();
        VerifyElements();
    }
    if (_state != State::BeingCollected)
    {
        OnFinish();
    }

}

void Algorithm::OnFinish()
{
    _state = State::Finished;
}

void Algorithm::CollectSorter()
{
    auto tmpState = _state;
    _state = State::BeingCollected;
    if (_sorter.joinable())
        _sorter.join();
    _state = tmpState;
}

void Algorithm::DrawBars(Scene &scene, const sf::FloatRect &rect)
{
    sf::Vector2f size(rect.width / _elements.size(), 0.0f);
    const sf::Vector2f positionOffset(rect.left, rect.top);
    const float heightMult = rect.height / _elements.size();

    for (size_t i = 0; i < _barsVA.getVertexCount(); i += 4)
    {
        auto color = GetElementColor(i / 4);
        color.a = 50;
        const auto &value = _elements[i / 4].value;
        size.y = value * heightMult;

        const sf::Vector2f position(positionOffset.x + size.x * (static_cast<float>(i) / 4),
                                    positionOffset.y + rect.height - size.y);

        const auto adjustedSize = sf::Vector2f{std::max(1.0f, size.x), std::max(1.0f, size.y)};

        _barsVA[i].color = color;
        _barsVA[i].position = position;
        _barsVA[i + 1].color = color;
        _barsVA[i + 1].position = position + sf::Vector2f{adjustedSize.x, 0.0f};
        _barsVA[i + 2].color = color;
        _barsVA[i + 2].position = position + adjustedSize;
        _barsVA[i + 3].color = color;
        _barsVA[i + 3].position = position + sf::Vector2f{0.0f, adjustedSize.y};

    }

    scene.Submit(_barsVA);
}

void Algorithm::DrawCircles(Scene &scene, const sf::FloatRect &rect)
{
    sf::VertexArray vertexArray(sf::TriangleFan, 1 + 2 * _elements.size());
    float maxRadius = static_cast<float>(std::min(rect.width / 2, rect.height / 2));
    float angleDelta = 2.0f * PI<> / (_elements.size() * 2.0f);
    float heightMult = maxRadius / _elements.size();
    sf::Vector2f rectMid = GenUtils::Mid(rect);

    vertexArray[0] = sf::Vertex(rectMid, sf::Color(255, 255, 255));
    for (size_t i = 0; i < _elements.size() * 2; i += 2)
    {
        sf::Vector2f line0 = vl::Rotate(sf::Vector2f(0.0f, -_elements[i / 2].value * heightMult) + rectMid,
                                        angleDelta * static_cast<float>(i), rectMid);
        sf::Vector2f line1 = vl::Rotate(sf::Vector2f(0.0f, -_elements[i / 2].value * heightMult) + rectMid,
                                        angleDelta * static_cast<float>(i + 1), rectMid);
        vertexArray[i + 1] = sf::Vertex(line0, GetElementColor(i / 2));
        vertexArray[i + 2] = sf::Vertex(line1, GetElementColor(i / 2));
    }
    scene.Submit(vertexArray);
}

void Algorithm::DrawHoops(Scene &scene, const sf::FloatRect &rect)
{
    while (_hoopsShapes.size() < _elements.size())
    {
        _hoopsShapes.emplace_back();
        _hoopsShapes.back().setOutlineThickness(1.0f);
        _hoopsShapes.back().setFillColor(sf::Color::Transparent);
    }

    sf::Vector2f rectMid = GenUtils::Mid(rect);
    float radiusMult = rect.height / _elements.size() / 3.0f;

    for (size_t i = 0; i < _elements.size(); i++)
    {

        float radius = _elements[i].value * radiusMult;

        sf::Color color = GetElementColor(i);
        color.a = 50;

        _hoopsShapes[i].setOutlineColor(color);
        _hoopsShapes[i].setPosition(rectMid.x - radius, rect.top + radiusMult * static_cast<float>(i));
        _hoopsShapes[i].setRadius(radius);

        scene.Submit(_hoopsShapes[i]);
    }
}

void Algorithm::DrawLine(Scene &scene, const sf::FloatRect &rect)
{
    sf::VertexArray line(sf::PrimitiveType::LineStrip);
    float yMult = rect.height / static_cast<float>(_elements.size());
    float xMult = rect.width / static_cast<float>(_elements.size());
    sf::Vector2f offset(rect.left, rect.top);
    for (size_t i = 0; i < _elements.size(); i++)
    {
        float x = offset.x + static_cast<float>(i) * xMult;
        float y = offset.y + rect.height - _elements[i].value * yMult;
        line.append(sf::Vertex(sf::Vector2f(x, y), GetElementColor(i)));
    }
    scene.Submit(line);
}

void Algorithm::DrawScatterPlot(Scene &scene, const sf::FloatRect &rect)
{
    float yMult = rect.height / static_cast<float>(_elements.size());
    float xMult = rect.width / static_cast<float>(_elements.size());
    sf::Vector2f offset(rect.left, rect.top);

    if (xMult < 1.0f)
    {
        sf::VertexArray scatterPlot(sf::PrimitiveType::Points);
        for (size_t i = 0; i < _elements.size(); i++)
        {
            float x = offset.x + static_cast<float>(i) * xMult;
            float y = offset.y + rect.height - _elements[i].value * yMult;
            scatterPlot.append(sf::Vertex(sf::Vector2f(x, y), GetElementColor(i)));
        }
        scene.Submit(scatterPlot);
    }
    else
    {
        sf::CircleShape circleShape(xMult / 2.0f);
        for (size_t i = 0; i < _elements.size(); i++)
        {
            circleShape.setFillColor(GetElementColor(i));
            float x = offset.x + static_cast<float>(i) * xMult;
            float y = offset.y + rect.height - _elements[i].value * yMult;
            circleShape.setPosition(x, y);
            scene.Submit(circleShape);
        }
    }
}

void Algorithm::DrawImage(Scene &scene, const sf::FloatRect &rect)
{
    sf::Vector2f offset(rect.left, rect.top);

    const size_t imageWidth = _image.getSize().x;
    const size_t imageHeight = _image.getSize().y;
    const size_t nPixels = imageWidth * imageHeight;
    sf::VertexArray pointArray(sf::PrimitiveType::Points, nPixels);

    sf::FloatRect lostRect(0.0f, 0.0f, 0.0f, 0.0f);
    sf::Vector2f lostCoord(0.0f, 0.0f);

    for (size_t i = 0; i < _elements.size(); i++)
    {
        const auto pixelRect = GetScaledPixel(i, _elements.size());
        auto pixelCoord = GetClosestPixelCoord(_elements[i].value - 1, _elements.size());

        const sf::IntRect nonLostPixelRect(static_cast<int>(pixelRect.left + static_cast<float>(lostRect.left)),
                                           static_cast<int>(pixelRect.top + static_cast<float>(lostRect.top)),
                                           static_cast<int>( pixelRect.width + static_cast<float>(lostRect.width)),
                                           static_cast<int>( pixelRect.height + static_cast<float>(lostRect.height)));

        const sf::Vector2i nonLostCoord(static_cast<int>(pixelCoord.x + static_cast<float>(lostCoord.x)),
                                        static_cast<int>(pixelCoord.y + static_cast<float>(lostCoord.y)));

        lostRect.left = static_cast<int>(pixelRect.left - static_cast<float>(nonLostPixelRect.left));
        lostRect.top = static_cast<int>(pixelRect.top - static_cast<float>(nonLostPixelRect.top));
        lostRect.width = static_cast<int>(pixelRect.width - static_cast<float>(nonLostPixelRect.width));
        lostRect.height = static_cast<int>(pixelRect.height - static_cast<float>(nonLostPixelRect.height));

        lostCoord.x = static_cast<int>(pixelCoord.x - static_cast<float>(nonLostCoord.x));
        lostCoord.y = static_cast<int>(pixelCoord.y - static_cast<float>(nonLostCoord.y));

        for (int width = 0; width < nonLostPixelRect.width + 1; width++)
        {
            for (int height = 0; height < nonLostPixelRect.height + 1; height++)
            {
                auto destIndex = (nonLostPixelRect.top + height) * imageWidth + nonLostPixelRect.left + width;
                if (destIndex < nPixels)
                {
                    pointArray[destIndex].position = sf::Vector2f(nonLostPixelRect.left + width,
                                                                  nonLostPixelRect.top + height);
                    pointArray[destIndex].color = _image.getPixel(nonLostCoord.x + width,
                                                                  nonLostCoord.y + height);

//                    pointArray[destIndex].position = sf::Vector2f(nonLostCoord.x + width,
//                                                                  nonLostCoord.y + height);
//                    pointArray[destIndex].color = _image.getPixel(nonLostPixelRect.left + width,
//                                                                  nonLostPixelRect.top + height);
                }
            }
        }
    }
    _imageRenderTexture.clear();
    _imageRenderTexture.draw(pointArray);
    _imageRenderTexture.display();
    sf::Sprite renderTextureSprite(_imageRenderTexture.getTexture());
    renderTextureSprite.setPosition(renderTextureSprite.getPosition() + offset);
    renderTextureSprite.setScale(rect.width / imageWidth, rect.height / imageHeight);
    scene.Submit(renderTextureSprite);
}

sf::Color Algorithm::GetElementColor(size_t index)
{
    if (_usingSpectrumColors)
    {
        return GenUtils::ValueToSpectrum(_elements[index].value, static_cast<long>(_elements.size()));
    }
    else
    {
        return _elements[index].color;
    }
}

bool Algorithm::VerifyElements()
{
    for (auto &element : _elements)
    {
        SetColor(element, sf::Color::White);
    }
    bool alreadySorted = true;
    for (size_t i = 0; i < _elements.size() - 1 && _state != State::BeingCollected; i++)
    {
        if (GetValue(i) < GetValue(i + 1))
        {
            if (_visType != VisType::Image)
            {
                SetColor(i, sf::Color::Green);
            }
        }
        else
        {
            alreadySorted = false;
            break;
        }
        SleepDelay();
    }
    if (!alreadySorted)
    {
        if (_visType != VisType::Image)
        {
            for (auto &element : _elements)
            {
                SetColor(element, sf::Color::White);
            }
        }
    }
    else
    {
        if (_visType != VisType::Image)
        {
            SetColor(_elements.back(), sf::Color::Green);
        }
    }

    return alreadySorted;
}

}
