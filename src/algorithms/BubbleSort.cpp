#include "BubbleSort.h"

void BubbleSort::Sort()
{
    m_state = State::Sorting;
    for (size_t i = 0; i < m_elements.size() && m_state != State::BeingCollected; i++)
    {
        for (size_t j = 0; j < m_elements.size() - i - 1 && m_state != State::BeingCollected; j++)
        {
            PauseCheck();
            m_elements[j].color = sf::Color::Red;
            m_elements[j + 1].color = sf::Color::Red;

            if (m_elements[j].value > m_elements[j + 1].value)
            {
                std::swap(m_elements[j], m_elements[j + 1]);
            }
            SleepDelay();

            m_elements[j].color = sf::Color::White;
            m_elements[j + 1].color = sf::Color::White;
        }
        m_elements[m_elements.size() - i - 1].color = sf::Color::Green;
    }
}