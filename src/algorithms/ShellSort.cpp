#include "ShellSort.h"

void ShellSort::Sort()
{
    for (size_t gap = m_bars.size() / 2; gap > 0 && m_state != State::BeingCollected; gap /= 2)
    {
        for (size_t i = gap; i < m_bars.size() && m_state != State::BeingCollected; i++)
        {
            PauseCheck();
            m_bars[i].color = sf::Color::Red;
            Bar tmp = m_bars[i];
            size_t j;
            for (j = i; j >= gap && m_bars[j - gap].value > tmp.value && m_state != State::BeingCollected; j -= gap)
            {
                m_bars[j].color = sf::Color::Red;
                m_bars[j - gap].color = sf::Color::Red;
                m_bars[j] = m_bars[j - gap];
                SleepDelay();
                m_bars[j].color = sf::Color::White;
                m_bars[j - gap].color = sf::Color::White;
            }
            SleepDelay();
            m_bars[j] = tmp;
            m_bars[i].color = sf::Color::White;
            m_bars[j].color = sf::Color::White;
        }
    }
}