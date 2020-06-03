#include "SelectionSort.h"

void SelectionSort::Sort()
{
    for (size_t i = 0; i < m_elements.size() - 1 && m_state != State::BeingCollected; i++)
    {
        PauseCheck();
        size_t indexFound = i;
        m_elements[i].color = sf::Color::Blue;
        for (size_t j = i + 1; j < m_elements.size() && m_state != State::BeingCollected; j++)
        {
            PauseCheck();
            m_elements[j].color = sf::Color::Yellow;
            if (m_elements[j].value < m_elements[indexFound].value)
            {
                indexFound = j;
                m_elements[indexFound].color = sf::Color::Red;
            }
            SleepDelay();
        }
        std::swap(m_elements[i], m_elements[indexFound]);
        for (size_t j = i + 1; j < m_elements.size(); j++)
        {
            m_elements[j].color = sf::Color::White;
        }
        m_elements[i].color = sf::Color::Green;
    }
}