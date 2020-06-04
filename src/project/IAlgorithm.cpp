#include "IAlgorithm.h"

IAlgorithm::IAlgorithm()
    : m_elements(10000),
      m_sleepDelay(sf::seconds(0.01f)),
      m_state(State::WaitingForStart),
      m_isActive(true),
      m_visType(VisType::Bars)
{
    m_name.setFont(*FontMgr::Get("res/arial.ttf"));
}

void IAlgorithm::Draw(const sf::FloatRect &rect)
{
    switch (m_visType)
    {
    case VisType::Bars:
    {
        sf::RectangleShape shape;
        sf::Vector2f size(rect.width / m_elements.size(), 0.0f);
        sf::Vector2f positionOffset(rect.left, rect.top);
        if (size.x > 2.0f && size.y > 2.0f)
        {
            shape.setOutlineThickness(1);
            shape.setOutlineColor(sf::Color(150, 150, 150));
        }
        float heightMult = rect.height / m_elements.size();
        for (size_t i = 0; i < m_elements.size(); i++)
        {
            size.y = m_elements[i].value * heightMult;
            shape.setFillColor(m_elements[i].color);
            shape.setPosition(positionOffset.x + size.x * static_cast<float>(i), positionOffset.y + rect.height - size.y);
            shape.setSize(size);
            Camera::Draw(shape);
        }
        break;
    }
    case VisType::Circles:
    {
        sf::VertexArray vertexArray(sf::TriangleFan, 1 + 2 * m_elements.size());
        float maxRadius = static_cast<float>(std::min(rect.width / 2, rect.height / 2));
        float maxValue = m_elements.size();
        float angleDelta = 2.0f * PI<> / m_elements.size();
        float heightMult = maxRadius / m_elements.size();
        sf::Vector2f rectMid = Lib::Mid(rect);

        vertexArray[0] = sf::Vertex(rectMid, sf::Color(255, 255, 255));
        for (size_t i = 0; i < m_elements.size() * 2; i += 2)
        {
            sf::Vector2f line0 = vl::Rotate(sf::Vector2f(0.0f, -m_elements[i / 2].value * heightMult) + rectMid, angleDelta * static_cast<float>(i), rectMid);
            sf::Vector2f line1 = vl::Rotate(sf::Vector2f(0.0f, -m_elements[i / 2].value * heightMult) + rectMid, angleDelta * static_cast<float>(i + 1), rectMid);
            vertexArray[i + 1] = sf::Vertex(line0, m_elements[i / 2].color);
            vertexArray[i + 2] = sf::Vertex(line1, m_elements[i / 2].color);
        }
        Camera::Draw(vertexArray);
        break;
    }
    case VisType::Spectrum:
    {
        sf::VertexArray vertexArray(sf::Quads, 4 * m_elements.size());
        sf::Vector2f size(rect.width / m_elements.size(), rect.height / 4.0f);
        sf::Vector2f positionOffset(rect.left, rect.top + rect.height / 3.0f);
        sf::FloatRect frect(positionOffset, size);

        auto MapColor = [](long value, long maxValue) {
            float a = ((float)value / (float)maxValue) / 0.2f;
            sf::Int8 X = std::floor(a);
            sf::Int8 Y = std::floor(255 * (a - X));
            sf::Int8 r = 0, g = 0, b = 0;
            switch (X)
            {
            case 0:
                r = 255;
                g = Y;
                b = 0;
                break;
            case 1:
                r = 255 - Y;
                g = 255;
                b = 0;
                break;
            case 2:
                r = 0;
                g = 255;
                b = Y;
                break;
            case 3:
                r = 0;
                g = 255 - Y;
                b = 255;
                break;
            case 4:
                r = Y;
                g = 0;
                b = 255;
                break;
            case 5:
                r = 255;
                g = 0;
                b = 255;
                break;
            default:
                break;
            }
            return sf::Color(r, g, b);
        };

        for (size_t i = 0; i < m_elements.size() * 4; i += 4)
        {
            sf::Color color = MapColor(m_elements[i / 4].value, m_elements.size());
            vertexArray[i + 0] = sf::Vertex(sf::Vector2f(frect.left, frect.top), color);
            vertexArray[i + 1] = sf::Vertex(sf::Vector2f(frect.left + frect.width, frect.top), color);
            vertexArray[i + 2] = sf::Vertex(sf::Vector2f(frect.left + frect.width, frect.top + frect.height), color);
            vertexArray[i + 3] = sf::Vertex(sf::Vector2f(frect.left, frect.top + frect.height), color);
            frect.left += size.x;
        }
        Camera::Draw(vertexArray);
    }
    default:
        break;
    }
}

void IAlgorithm::DrawName(const sf::FloatRect &rect)
{
    m_name.setString(GetName());
    m_name.setPosition(rect.left + rect.width / 2.0f, rect.top - 35.0f);
    Camera::DrawText(m_name, TextAlign::Middle);
}

void IAlgorithm::Start()
{
    if (m_state == State::WaitingForStart)
    {
        CollectSorter();
        m_state = State::Sorting;
        m_sorter = std::thread(SortThreadFn, this);
    }
}

void IAlgorithm::Restart()
{
    if (m_state == State::Sorting || m_state == State::Paused || m_state == State::Finished)
    {
        CollectSorter();
        m_state = State::WaitingForStart;
        m_elements = m_elementsRestart;
    }
}

void IAlgorithm::Pause()
{
    if (m_state == State::Sorting)
        m_state = State::Paused;
}

void IAlgorithm::Resume()
{
    if (m_state == State::Paused)
        m_state = State::Sorting;
}

void IAlgorithm::Reset()
{
    CollectSorter();
    m_state = State::WaitingForStart;
    m_elements = m_elementsReset;
}

void IAlgorithm::Resize(size_t size)
{
    if (m_elements.size() == size)
        return;

    m_elements.clear();
    m_elementsRestart.clear();
    m_elementsReset.clear();
    m_elements.resize(size);
    m_elementsRestart.resize(size);
    m_elementsReset.resize(size);

    long n = 1;
    std::generate(m_elements.begin(), m_elements.end(), [&n] {
        return Element(n++);
    });
    m_elementsRestart = m_elements;
    m_elementsReset = m_elements;
}

void IAlgorithm::PopPushUntil(size_t size)
{
    CollectSorter();
    while (m_elementsReset.size() != size)
    {
        if (size < m_elementsReset.size())
            m_elementsReset.pop_back();
        else if (size > m_elementsReset.size())
            m_elementsReset.push_back(Element(m_elementsReset.size() + 1));
    }
    m_elements = m_elementsReset;
    m_elementsRestart = m_elementsReset;
}

void IAlgorithm::Shuffle(std::mt19937 generator)
{
    Reset();
    std::shuffle(m_elementsRestart.begin(), m_elementsRestart.end(), generator);
    m_elements = m_elementsRestart;
}

void IAlgorithm::PauseCheck()
{
    while (m_state == State::Paused && m_state != State::BeingCollected)
        sf::sleep(sf::seconds(0.01f));
}

std::chrono::nanoseconds calc_overhead()
{
    using namespace std::chrono;
    constexpr size_t tests = 1001;
    constexpr auto timer = 200us;

    auto init = [&timer]() {
        auto end = steady_clock::now() + timer;
        while (steady_clock::now() < end)
            ;
    };

    time_point<steady_clock> start;
    nanoseconds dur[tests];

    for (auto &d : dur)
    {
        start = steady_clock::now();
        init();
        d = steady_clock::now() - start - timer;
    }
    std::sort(std::begin(dur), std::end(dur));
    // get the median value or something a little less as in this example:
    return dur[tests / 3];
}

// initialize the overhead constant that will be used in busy_sleep()
static const std::chrono::nanoseconds overhead = calc_overhead();

inline void busy_sleep(std::chrono::nanoseconds t)
{
    auto end = std::chrono::steady_clock::now() + t - overhead;
    while (std::chrono::steady_clock::now() < end)
        ;
}

void IAlgorithm::SleepDelay()
{
    std::this_thread::sleep_for(std::chrono::microseconds(m_sleepDelay.asMicroseconds()));
}

void IAlgorithm::SortThreadFn()
{
    Sort();
    if (m_state != State::BeingCollected)
        OnFinish();
}

void IAlgorithm::OnFinish()
{
    for (auto &bar : m_elements)
        bar.color = sf::Color::Green;
    m_state = State::Finished;
}

void IAlgorithm::CollectSorter()
{
    auto tmpState = m_state;
    m_state = State::BeingCollected;
    if (m_sorter.joinable())
        m_sorter.join();
    m_state = tmpState;
}