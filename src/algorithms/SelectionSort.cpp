#include "SelectionSort.h"

void SelectionSort::Sort()
{
    for (size_t i = 0; i < m_bars.size() - 1 && m_state != State::BeingCollected; i++)
    {
        PauseCheck();
        size_t indexFound = i;
        m_bars[i].color = sf::Color::Blue;
        for (size_t j = i + 1; j < m_bars.size() && m_state != State::BeingCollected; j++)
        {
            PauseCheck();
            m_bars[j].color = sf::Color::Yellow;
            if (m_bars[j].value < m_bars[indexFound].value)
            {
                indexFound = j;
                m_bars[indexFound].color = sf::Color::Red;
            }
            SleepDelay();
        }
        std::swap(m_bars[i], m_bars[indexFound]);
        for (size_t j = i + 1; j < m_bars.size(); j++)
        {
            m_bars[j].color = sf::Color::White;
        }
        m_bars[i].color = sf::Color::Green;
    }
}