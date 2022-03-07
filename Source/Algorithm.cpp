#include "Algorithm.h"

#include <SFML/Graphics.hpp>

namespace Se
{
struct ContainerRefGroup
{
	std::vector<Element>& elements;
	std::vector<Element>& elementsRestart;
	std::vector<Element>& elementsReset;
};

Algorithm::Algorithm(std::string name) :
	_name(std::move(name)),
	_sleepDelay(sf::seconds(0.01f)),
	_minorDelay(false),
	_minorDelayTimer(0),
	_state(State::WaitingForStart),
	_isActive(true),
	_nameTextFont(FontStore::Get("segoeui.ttf")),
	_visType(VisType::Bars),
	_barsVA(sf::Quads, 0),
	_numberLineVA(sf::Quads, 0),
	_colorTransitionTimer(0.0f),
	_colorTransitionDuration(0.7f)
{
	_nameText.setFont(*_nameTextFont);
	_nameText.setCharacterSize(18);
	SetImage("sample_image_forest.jpg");

	_palettes.push_back(ImageStore::Get("Pals/rainbowLarge.png"));
	_palettes.push_back(ImageStore::Get("Pals/fieryLarge.png"));
	_palettes.push_back(ImageStore::Get("Pals/uvLarge.png"));

	_currentPalette.create(PaletteWidth, 1, _palettes[static_cast<int>(_desiredPalette)]->getPixelsPtr());
	for (int i = 0; i < PaletteWidth; i++)
	{
		const auto pix = _currentPalette.getPixel(i, 0);
		_colorsStart[i] = {
			static_cast<float>(pix.r) / 255.0f, static_cast<float>(pix.g) / 255.0f, static_cast<float>(pix.b) / 255.0f,
			static_cast<float>(pix.a) / 255.0f
		};
	}
	_colorsCurrent = _colorsStart;
}

void Algorithm::OnUpdate()
{
	if (_colorTransitionTimer <= _colorTransitionDuration)
	{
		const float delta = (std::sin((_colorTransitionTimer / _colorTransitionDuration) * PI<> - PI<> / 2.0f) + 1.0f) /
			2.0f;
		for (int x = 0; x < PaletteWidth; x++)
		{
			const auto pix = _palettes[static_cast<int>(_desiredPalette)]->getPixel(x, 0);
			const TransitionColor goalColor = {
				static_cast<float>(pix.r) / 255.0f, static_cast<float>(pix.g) / 255.0f,
				static_cast<float>(pix.b) / 255.0f, static_cast<float>(pix.a) / 255.0f
			};
			const auto& startColor = _colorsStart[x];
			auto& currentColor = _colorsCurrent[x];
			currentColor.r = startColor.r + delta * (goalColor.r - startColor.r);
			currentColor.g = startColor.g + delta * (goalColor.g - startColor.g);
			currentColor.b = startColor.b + delta * (goalColor.b - startColor.b);
			_currentPalette.setPixel(x, 0, {
				                         static_cast<sf::Uint8>(currentColor.r * 255.0f),
				                         static_cast<sf::Uint8>(currentColor.g * 255.0f),
				                         static_cast<sf::Uint8>(currentColor.b * 255.0f),
				                         static_cast<sf::Uint8>(currentColor.a * 255.0f)
			                         });
		}
		_colorTransitionTimer += Global::Clock::FrameTime().asSeconds();
	}
}

void Algorithm::Draw(Scene& scene, const sf::FloatRect& rect)
{
	switch (_visType)
	{
	case VisType::Bars:
	{
		DrawBars(scene, rect);
		break;
	}
	case VisType::NumberLine:
	{
		DrawNumberLine(scene, rect);
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
	}
}

void Algorithm::DrawName(Scene& scene, const sf::FloatRect& rect)
{
	_nameText.setString(Name());
	_nameText.setPosition(rect.left + rect.width / 2.0f, rect.top - 35.0f);
	scene.Submit(_nameText, TextAlign::Middle);
}

void Algorithm::Activate()
{
	_isActive = true;
}

void Algorithm::Deactivate()
{
	_isActive = false;
}

auto Algorithm::Active() const -> bool
{
	return _isActive;
}

void Algorithm::Start()
{
	if (_state == State::WaitingForStart)
	{
		CollectSorter();
		_state = State::Sorting;
		_sorter = std::thread([this]
		{
			SortThreadFn();
		});
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
	if (_state == State::Sorting) _state = State::Paused;
}

void Algorithm::Resume()
{
	if (_state == State::Paused) _state = State::Sorting;
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
	_elements.clear();
	_elementsRestart.clear();
	_elementsReset.clear();
	_elements.resize(size);
	_elementsRestart.resize(size);
	_elementsReset.resize(size);

	const auto generator = Generator();
	for (int i = 0; i < _elements.size(); i++)
	{
		_elements[i].Value = generator(i);
	}

	std::sort(std::execution::par, _elements.begin(), _elements.end(), [](const Element& l, const Element& r)
	{
		return l.Value < r.Value;
	});

	_elementsRestart = _elements;
	_elementsReset = _elements;

	// Resize draw cache
	_barsVA.resize(size * 4);
	_numberLineVA.resize(size * 4);
}

void Algorithm::SoftResize(size_t size)
{
	CollectSorter();
	const auto generator = Generator();
	while (_elementsReset.size() != size)
	{
		if (size < _elementsReset.size())
		{
			_elementsReset.pop_back();
		}
		else if (size > _elementsReset.size())
		{
			_elementsReset.push_back(Element(generator(_elementsReset.size())));
		}
	}
	_elements = _elementsReset;
	_elementsRestart = _elementsReset;

	// Resize draw cache
	_barsVA.resize(size * 4);
	_numberLineVA.resize(size * 4);
}

void Algorithm::SetImage(const std::string& filepath)
{
	Reset();
	_image = ImageStore::Get(filepath);
	_imageRenderTexture.create(_image->getSize().x, _image->getSize().y);
}

void Algorithm::Shuffle(Random::Engine generator)
{
	Reset();
	std::ranges::shuffle(_elementsRestart, generator);
	_elements = _elementsRestart;
}

auto Algorithm::Name() const -> const std::string& { return _name; }

void Algorithm::SetSleepDelay(sf::Time delay)
{
	_sleepDelay = delay;
	_minorDelay = (_sleepDelay.asMicroseconds() < 1000);
}

void Algorithm::SetVisType(VisType visType)
{
	_visType = visType;
}

void Algorithm::SetNumberGeneratorType(NumberGeneratorType numberGeneratorType)
{
	_numberGeneratorType = numberGeneratorType;
}

void Algorithm::UsePalette(bool use)
{
	_usePalette = use;
}

void Algorithm::SetPalette(Palette palette)
{
	_desiredPalette = palette;
	_colorTransitionTimer = 0.0f;
	_colorsStart = _colorsCurrent;
}

auto Algorithm::PaletteImage() -> const sf::Image&
{
	return _currentPalette;
}

auto Algorithm::Elements() -> std::vector<Element>&
{
	return _elements;
}

auto Algorithm::RestartElements() -> std::vector<Element>&
{
	return _elementsRestart;
}

auto Algorithm::ResetElements() -> std::vector<Element>&
{
	return _elementsReset;
}

auto Algorithm::ElementByIndex(size_t index) -> Element&
{
	return Elements()[index];
}

auto Algorithm::ValueByIndex(size_t index) -> long
{
	return Elements()[index].Value;
}

void Algorithm::SetValue(size_t index, long value)
{
	SetValue(ElementByIndex(index), value);
}

void Algorithm::SetColor(size_t index, const sf::Color& color)
{
	SetColor(ElementByIndex(index), color);
}

void Algorithm::SwapElements(size_t iFirst, size_t iSecond)
{
	SwapElements(ElementByIndex(iFirst), ElementByIndex(iSecond));
}

void Algorithm::SetValue(Element& element, long value)
{
	element.Value = value;
}

void Algorithm::SetColor(Element& element, const sf::Color& color)
{
	element.Color = color;
}

void Algorithm::SwapElements(Element& first, Element& second)
{
	std::swap(first, second);
}

void Algorithm::PauseCheck()
{
	while (_state == State::Paused && _state != State::BeingCollected) sleep(sf::seconds(0.01f));
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

auto Algorithm::Generator() -> std::function<long(size_t)>
{
	switch (_numberGeneratorType)
	{
	case NumberGeneratorType::Linear:
	{
		return [](size_t index)
		{
			return index + 1;
		};
	}
	case NumberGeneratorType::Quadratic:
	{
		return [](size_t index)
		{
			return std::pow(index, 2);
		};
	}
	case NumberGeneratorType::Random:
	{
		return [this](size_t index) { return Random::Integer(1l, static_cast<long>(_elements.size())); };
	}
	}
	Debug::Break("Invalid NumberGeneratorType");
	return {};
}

auto Algorithm::HighestElementValue() -> long
{
	switch (_numberGeneratorType)
	{
	case NumberGeneratorType::Linear:
	{
		return _elements.size();
	}
	case NumberGeneratorType::Quadratic:
	{
		return std::pow(_elements.size(), 2);
	}
	case NumberGeneratorType::Random:
	{
		return _elements.size();
	}
	}
	Debug::Break("Invalid NumberGeneratorType");
	return {};
}

auto Algorithm::PixelCoord(size_t index) const -> sf::Vector2u
{
	const auto width = _image->getSize().x;
	const auto x = index % width;
	const auto y = std::floor(index / width);
	return sf::Vector2u(x, y);
}

auto Algorithm::ScaledPixel(size_t index, size_t max) const -> sf::FloatRect
{
	const float width = _image->getSize().x;
	const float height = _image->getSize().y;

	const float sqrtMax = std::sqrt(max);
	const float nPixelsX = std::floor(sqrtMax);
	const float newMax = std::pow(nPixelsX, 2);

	const float scaledPixelWidth = width / nPixelsX;
	const float scaledPixelHeight = height / nPixelsX;

	if (index >= newMax)
	{
		index = newMax - 1;
	}

	const sf::FloatRect scaledPixel(scaledPixelWidth * (index % static_cast<int>(nPixelsX)),
	                                scaledPixelHeight * std::floor(index / nPixelsX), std::max(1.0f, scaledPixelWidth),
	                                std::max(1.0f, scaledPixelHeight));

	return scaledPixel;
}

auto Algorithm::ClosestPixelCoord(size_t index, size_t max) const -> sf::Vector2u
{
	const float width = _image->getSize().x;
	const float height = _image->getSize().y;

	const float sqrtMax = std::sqrt(max);
	const float nPixelsX = std::floor(sqrtMax);
	const float newMax = std::pow(nPixelsX, 2);

	const float scaledPixelWidth = width / nPixelsX;
	const float scaledPixelHeight = height / nPixelsX;

	if (index >= newMax)
	{
		index = newMax - 1;
	}

	const uint x = scaledPixelWidth * (index % static_cast<int>(nPixelsX));
	const uint y = scaledPixelHeight * std::floor(index / nPixelsX);

	return {x, y};
}

void Algorithm::SortThreadFn()
{
	const bool alreadySorted = VerifyElements();

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
	const auto tmpState = _state;
	_state = State::BeingCollected;
	if (_sorter.joinable()) _sorter.join();
	_state = tmpState;
}

void Algorithm::DrawBars(Scene& scene, const sf::FloatRect& rect)
{
	sf::Vector2f size(rect.width / _elements.size(), 0.0f);
	const sf::Vector2f positionOffset(rect.left, rect.top);
	const float heightMult = rect.height / HighestElementValue();

	for (size_t i = 0; i < _barsVA.getVertexCount(); i += 4)
	{
		auto color = ElementColorByIndex(i / 4);
		color.a = 255 - static_cast<float>(_elements.size()) / static_cast<float>(MaxElements) * 240;
		const auto& value = _elements[i / 4].Value;
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

void Algorithm::DrawNumberLine(Scene& scene, const sf::FloatRect& rect)
{
	const float size = rect.width / _elements.size();
	const float constainedSize = std::min(size, rect.height * 0.4f);
	const sf::Vector2f positionOffset(rect.left, rect.top);

	for (size_t i = 0; i < _numberLineVA.getVertexCount(); i += 4)
	{
		const size_t boxIndex = i / 4;

		auto color = ElementColorByIndex(boxIndex);
		color.a = 255 - static_cast<float>(_elements.size()) / static_cast<float>(MaxElements) * 240;

		sf::Vector2f position(positionOffset.x + constainedSize * (static_cast<float>(boxIndex)),
		                      positionOffset.y + rect.height / 2.0f - constainedSize / 2.0f);

		if (constainedSize < size)
		{
			const float halfTotalLength = rect.width / 2.0f;
			const float halfCurrentAccLength = constainedSize * static_cast<float>(_elements.size()) / 2.0f;

			position.x += halfTotalLength - halfCurrentAccLength;
		}

		const int diff = boxIndex - _numberLineTextList.size();
		if (diff >= 0)
		{
			_numberLineTextList.emplace_back("", *_nameTextFont);
			_numberLineTextList.back().setFillColor(sf::Color::White);
			_numberLineTextList.back().setOutlineThickness(1);
			_numberLineTextList.back().setOutlineColor(sf::Color::Black);
		}

		const auto& value = _elements[boxIndex].Value;
		_numberLineTextList[boxIndex].setPosition(position + sf::Vector2f(constainedSize / 2.0f, 0.0f));
		_numberLineTextList[boxIndex].setCharacterSize(constainedSize * 0.65f);
		_numberLineTextList[boxIndex].setString(std::to_string(value));

		const auto adjustedSize = std::max(1.0f, constainedSize);

		_numberLineVA[i].color = color;
		_numberLineVA[i].position = position;
		_numberLineVA[i + 1].color = color;
		_numberLineVA[i + 1].position = position + sf::Vector2f{adjustedSize, 0.0f};
		_numberLineVA[i + 2].color = color;
		_numberLineVA[i + 2].position = position + sf::Vector2f(adjustedSize, adjustedSize);
		_numberLineVA[i + 3].color = color;
		_numberLineVA[i + 3].position = position + sf::Vector2f{0.0f, adjustedSize};
	}

	scene.Submit(_numberLineVA);
	for (size_t i = 0; i < _numberLineVA.getVertexCount() / 4; i++)
	{
		scene.Submit(_numberLineTextList[i], TextAlign::Middle);
	}
}

void Algorithm::DrawCircles(Scene& scene, const sf::FloatRect& rect)
{
	sf::VertexArray vertexArray(sf::TriangleFan, 1 + 2 * _elements.size());
	const float maxRadius = static_cast<float>(std::min(rect.width / 2, rect.height / 2));
	const float angleDelta = 2.0f * PI<> / (_elements.size() * 2.0f);
	const float heightMult = maxRadius / HighestElementValue();
	const sf::Vector2f rectMid = GenUtils::Mid(rect);

	vertexArray[0] = sf::Vertex(rectMid, sf::Color(255, 255, 255));
	for (size_t i = 0; i < _elements.size() * 2; i += 2)
	{
		sf::Vector2f line0 = VecUtils::Rotate(sf::Vector2f(0.0f, -_elements[i / 2].Value * heightMult) + rectMid,
		                                      angleDelta * static_cast<float>(i), rectMid);
		sf::Vector2f line1 = VecUtils::Rotate(sf::Vector2f(0.0f, -_elements[i / 2].Value * heightMult) + rectMid,
		                                      angleDelta * static_cast<float>(i + 1), rectMid);
		vertexArray[i + 1] = sf::Vertex(line0, ElementColorByIndex(i / 2));
		vertexArray[i + 2] = sf::Vertex(line1, ElementColorByIndex(i / 2));
	}
	scene.Submit(vertexArray);
}

void Algorithm::DrawHoops(Scene& scene, const sf::FloatRect& rect)
{
	while (_hoopsShapes.size() < _elements.size())
	{
		_hoopsShapes.emplace_back();
		_hoopsShapes.back().setOutlineThickness(1.0f);
		_hoopsShapes.back().setFillColor(sf::Color::Transparent);
	}

	const sf::Vector2f rectMid = GenUtils::Mid(rect);
	const float radiusMult = rect.height / _elements.size() / 3.0f;

	for (size_t i = 0; i < _elements.size(); i++)
	{
		const float radius = _elements[i].Value * radiusMult;

		sf::Color color = ElementColorByIndex(i);
		color.a = 50;

		_hoopsShapes[i].setOutlineColor(color);
		_hoopsShapes[i].setPosition(rectMid.x - radius, rect.top + radiusMult * static_cast<float>(i));
		_hoopsShapes[i].setRadius(radius);

		scene.Submit(_hoopsShapes[i]);
	}
}

void Algorithm::DrawLine(Scene& scene, const sf::FloatRect& rect)
{
	sf::VertexArray line(sf::PrimitiveType::LineStrip);
	const float xMult = rect.width / static_cast<float>(_elements.size());
	const float yMult = rect.height / static_cast<float>(HighestElementValue());
	const sf::Vector2f offset(rect.left, rect.top);
	for (size_t i = 0; i < _elements.size(); i++)
	{
		const float x = offset.x + static_cast<float>(i) * xMult;
		const float y = offset.y + rect.height - _elements[i].Value * yMult;
		line.append(sf::Vertex(sf::Vector2f(x, y), ElementColorByIndex(i)));
	}
	scene.Submit(line);
}

void Algorithm::DrawScatterPlot(Scene& scene, const sf::FloatRect& rect)
{
	const float xMult = rect.width / static_cast<float>(_elements.size());
	const float yMult = rect.height / static_cast<float>(HighestElementValue());
	const sf::Vector2f offset(rect.left, rect.top);

	if (xMult < 1.0f)
	{
		sf::VertexArray scatterPlot(sf::PrimitiveType::Points);
		for (size_t i = 0; i < _elements.size(); i++)
		{
			const float x = offset.x + static_cast<float>(i) * xMult;
			const float y = offset.y + rect.height - _elements[i].Value * yMult;
			scatterPlot.append(sf::Vertex(sf::Vector2f(x, y), ElementColorByIndex(i)));
		}
		scene.Submit(scatterPlot);
	}
	else
	{
		sf::CircleShape circleShape(xMult / 2.0f);
		for (size_t i = 0; i < _elements.size(); i++)
		{
			circleShape.setFillColor(ElementColorByIndex(i));
			const float x = offset.x + static_cast<float>(i) * xMult;
			const float y = offset.y + rect.height - _elements[i].Value * yMult;
			circleShape.setPosition(x, y);
			scene.Submit(circleShape);
		}
	}
}

void Algorithm::DrawImage(Scene& scene, const sf::FloatRect& rect)
{
	const sf::Vector2f offset(rect.left, rect.top);

	const size_t imageWidth = _image->getSize().x;
	const size_t imageHeight = _image->getSize().y;
	const size_t nPixels = imageWidth * imageHeight;
	sf::VertexArray pointArray(sf::PrimitiveType::Points, nPixels);

	sf::FloatRect lostRect(0.0f, 0.0f, 0.0f, 0.0f);
	sf::Vector2f lostCoord(0.0f, 0.0f);

	for (size_t i = 0; i < _elements.size(); i++)
	{
		const auto pixelRect = ScaledPixel(i, _elements.size());
		const auto pixelCoord = ClosestPixelCoord(_elements[i].Value - 1, _elements.size());

		const sf::IntRect nonLostPixelRect(static_cast<int>(pixelRect.left + static_cast<float>(lostRect.left)),
		                                   static_cast<int>(pixelRect.top + static_cast<float>(lostRect.top)),
		                                   static_cast<int>(pixelRect.width + static_cast<float>(lostRect.width)),
		                                   static_cast<int>(pixelRect.height + static_cast<float>(lostRect.height)));

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
				const auto destIndex = (nonLostPixelRect.top + height) * imageWidth + nonLostPixelRect.left + width;
				const auto srcX = nonLostCoord.x + width;
				const auto srcY = nonLostCoord.y + height;
				if (destIndex < nPixels && srcX < imageWidth && srcY < imageHeight)
				{
					pointArray[destIndex].position = sf::Vector2f(nonLostPixelRect.left + width,
					                                              nonLostPixelRect.top + height);
					pointArray[destIndex].color = _image->getPixel(srcX, srcY);

					auto tintBlend = ElementColorByIndex(i);
					tintBlend.r = (255 - tintBlend.r) * 0.3f;
					tintBlend.g = (255 - tintBlend.g) * 0.3f;
					tintBlend.b = (255 - tintBlend.b) * 0.3f;
					pointArray[destIndex].color += tintBlend;
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

auto Algorithm::ElementColorByIndex(size_t index) -> sf::Color
{
	if (_usePalette)
	{
		const auto& element = ElementByIndex(index);
		const auto mappedValue = GenUtils::Map(element.Value, 0l, static_cast<long>(_elements.size()), 0l,
		                                       static_cast<long>(PaletteWidth - 1));
		return _currentPalette.getPixel(mappedValue, 0);
	}
	return _elements[index].Color;
}

auto Algorithm::VerifyElements() -> bool
{
	for (auto& element : _elements)
	{
		SetColor(element, sf::Color::White);
	}
	bool alreadySorted = true;
	for (size_t i = 0; i < _elements.size() - 1 && _state != State::BeingCollected; i++)
	{
		if (ValueByIndex(i) < ValueByIndex(i + 1))
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
			for (auto& element : _elements)
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
