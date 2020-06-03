#include "IAlgorithm.h"

IAlgorithm::IAlgorithm()
    : m_bars(10000),
      m_sleepTime(sf::seconds(0.01f)),
      m_state(State::Paused),
      m_isActive(true)
{
    m_name.setFont(*FontMgr::Get("res/arial.ttf"));
}

void IAlgorithm::Draw(const sf::FloatRect &rect)
{
    sf::RectangleShape shape;
    sf::Vector2f size(rect.width / m_bars.size(), 0.0f);
    sf::Vector2f positionOffset(rect.left, rect.top);
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
    m_state = State::Sorting;
    m_sorter = std::thread(SortThreadFn, this);
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
    m_bars = m_barsCopy;
    m_state = State::BeingCollected;
    if (m_sorter.joinable())
        m_sorter.join();
}

void IAlgorithm::Resize(size_t size)
{
    if (m_bars.size() == size)
        return;

    m_bars.clear();
    m_barsCopy.clear();
    m_bars.resize(size);
    m_barsCopy.resize(size);

    long n = 1;
    std::generate(m_bars.begin(), m_bars.end(), [&n] {
        Bar bar;
        bar.value = n++;
        return bar;
    });
    m_barsCopy = m_bars;
}

void IAlgorithm::Shuffle(std::mt19937 generator)
{
    Reset();
    for (auto &bar : m_bars)
        bar.color = sf::Color::White;
    std::shuffle(m_bars.begin(), m_bars.end(), generator);
    m_barsCopy = m_bars;
}

void IAlgorithm::PauseCheck()
{
    while (m_state == State::Paused)
        ;
}

void IAlgorithm::SleepDelay()
{
    sf::sleep(m_sleepTime);
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