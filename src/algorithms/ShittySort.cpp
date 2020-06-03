#include "ShittySort.h"

void ShittySort::Sort()
{
    double currentCheck;
    do
    {
        currentCheck = m_bars[0].value;
        for (size_t i = 0; i < m_bars.size() && m_state != State::BeingCollected; i++)
        {
            PauseCheck();
            if (m_bars[i].value >= currentCheck)
            {
                currentCheck = m_bars[i].value;
            }
            else
            {
                int randomIndex = Random::Generate((int)0, (int)m_bars.size() - 1);
                if (m_bars[randomIndex].value > m_bars[randomIndex + 1].value)
                {
                    std::swap(m_bars[randomIndex], m_bars[randomIndex + 1]);
                    m_bars[randomIndex].color = sf::Color::Yellow;
                    m_bars[randomIndex + 1].color = sf::Color::Yellow;
                }
                else
                {
                    m_bars[randomIndex].color = sf::Color::Red;
                    m_bars[randomIndex + 1].color = sf::Color::Red;
                }
                SleepDelay();
                m_bars[randomIndex].color = sf::Color::White;
                m_bars[randomIndex + 1].color = sf::Color::White;
                break;
            }
        }
    } while (currentCheck != m_bars.back().value && m_state != State::BeingCollected);
}