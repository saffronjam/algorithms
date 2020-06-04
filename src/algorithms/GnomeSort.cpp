#include "GnomeSort.h"

void GnomeSort::Sort()
{
    for (int i = 0; i < m_elements.size() && m_state != State::BeingCollected;)
    {
        if (i == 0)
        {
            m_elements[i].color = sf::Color::Cyan;
            i++;
        }
        if (m_elements[i].value >= m_elements[i - 1].value && m_state != State::BeingCollected)
        {
            PauseCheck();
            m_elements[i].color = sf::Color::Cyan;
            i++;
        }
        else
        {
            std::swap(m_elements[i], m_elements[i - 1]);
            m_elements[i].color = sf::Color::Red;
            m_elements[i - 1].color = sf::Color::Red;
            PauseCheck();
            m_elements[i].color = sf::Color::White;
            i--;
        }
        SleepDelay();
    }
}