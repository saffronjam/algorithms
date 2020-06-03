#include "InsertionSort.h"

void InsertionSort::Sort()
{
    for (size_t i = 1; i < m_bars.size() && m_state != State::BeingCollected; i++)
    {
        for (size_t j = i; j > 0 && m_state != State::BeingCollected; j--)
        {
            PauseCheck();
            if (m_bars[j].value < m_bars[j - 1].value)
            {
                m_bars[j].color = sf::Color::Red;
                m_bars[j - 1].color = sf::Color::Yellow;
                SleepDelay();
                std::swap(m_bars[j], m_bars[j - 1]);
                m_bars[j].color = sf::Color::White;
            }
            else
            {
                SleepDelay();
                break;
            }
        }
        for (int j = i; j >= 0; j--)
        {
            m_bars[j].color = sf::Color::Cyan;
        }
    }
}