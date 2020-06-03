#include "ShittySort.h"

void ShittySort::Sort()
{
    double currentCheck;
    do
    {
        currentCheck = m_elements[0].value;
        for (size_t i = 0; i < m_elements.size() && m_state != State::BeingCollected; i++)
        {
            PauseCheck();
            if (m_elements[i].value >= currentCheck)
            {
                currentCheck = m_elements[i].value;
            }
            else
            {
                int randomIndex = Random::Generate((int)0, (int)m_elements.size() - 1);
                if (m_elements[randomIndex].value > m_elements[randomIndex + 1].value)
                {
                    std::swap(m_elements[randomIndex], m_elements[randomIndex + 1]);
                    m_elements[randomIndex].color = sf::Color::Yellow;
                    m_elements[randomIndex + 1].color = sf::Color::Yellow;
                }
                else
                {
                    m_elements[randomIndex].color = sf::Color::Red;
                    m_elements[randomIndex + 1].color = sf::Color::Red;
                }
                SleepDelay();
                m_elements[randomIndex].color = sf::Color::White;
                m_elements[randomIndex + 1].color = sf::Color::White;
                break;
            }
        }
    } while (currentCheck != m_elements.back().value && m_state != State::BeingCollected);
}