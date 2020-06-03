#include "IAlgorithm.h"

IAlgorithm::IAlgorithm()
    : m_bars(10000),
      m_sleepDelay(sf::seconds(0.01f)),
      m_state(State::WaitingForStart),
      m_isActive(true)
{
    m_name.setFont(*FontMgr::Get("res/arial.ttf"));
}

void IAlgorithm::Draw(const sf::FloatRect &rect)
{
    sf::RectangleShape shape;
    sf::Vector2f size(rect.width / m_bars.size(), 0.0f);
    sf::Vector2f positionOffset(rect.left, rect.top);
    if (size.x > 2.0f && size.y > 2.0f)
    {
        shape.setOutlineThickness(1);
        shape.setOutlineColor(sf::Color(150, 150, 150));
    }
    float heightMult = rect.height / m_bars.size();
    for (size_t i = 0; i < m_bars.size(); i++)
    {
        size.y = m_bars[i].value * heightMult;
        shape.setFillColor(m_bars[i].color);
        shape.setPosition(positionOffset.x + size.x * static_cast<float>(i), positionOffset.y + rect.height - size.y);
        shape.setSize(size);
        Camera::Draw(shape);
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
        m_bars = m_barsRestart;
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
    m_bars = m_barsReset;
}

void IAlgorithm::Resize(size_t size)
{
    if (m_bars.size() == size)
        return;

    m_bars.clear();
    m_barsRestart.clear();
    m_barsReset.clear();
    m_bars.resize(size);
    m_barsRestart.resize(size);
    m_barsReset.resize(size);

    long n = 1;
    std::generate(m_bars.begin(), m_bars.end(), [&n] {
        return Bar(n++);
    });
    m_barsRestart = m_bars;
    m_barsReset = m_bars;
}

void IAlgorithm::PopPushUntil(size_t size)
{
    CollectSorter();
    while (m_barsReset.size() != size)
    {
        if (size < m_barsReset.size())
            m_barsReset.pop_back();
        else if (size > m_barsReset.size())
            m_barsReset.push_back(Bar(m_barsReset.size() + 1));
    }
    m_bars = m_barsReset;
    m_barsRestart = m_barsReset;
}

void IAlgorithm::Shuffle(std::mt19937 generator)
{
    Reset();
    std::shuffle(m_barsRestart.begin(), m_barsRestart.end(), generator);
    m_bars = m_barsRestart;
}

void IAlgorithm::PauseCheck()
{
    while (m_state == State::Paused && m_state != State::BeingCollected)
        sf::sleep(sf::seconds(0.1f));
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
    for (auto &bar : m_bars)
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