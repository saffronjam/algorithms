#include "BubbleSort.h"

void BubbleSort::Sort()
{
    m_state = State::Sorting;
    for (size_t i = 0; i < m_bars.size() && m_state != State::BeingCollected; i++)
    {
        for (size_t j = 0; j < m_bars.size() - i - 1 && m_state != State::BeingCollected; j++)
        {
            PauseCheck();
            m_bars[j].color = sf::Color::Red;
            m_bars[j + 1].color = sf::Color::Red;

            if (m_bars[j].value > m_bars[j + 1].value)
            {
                std::swap(m_bars[j], m_bars[j + 1]);
            }
            SleepDelay();

            m_bars[j].color = sf::Color::White;
            m_bars[j + 1].color = sf::Color::White;
        }
        m_bars[m_bars.size() - i - 1].color = sf::Color::Green;
    }
}