#include "ShellSort.h"

void ShellSort::Sort()
{
    for (size_t gap = m_elements.size() / 2; gap > 0 && m_state != State::BeingCollected; gap /= 2)
    {
        for (size_t i = gap; i < m_elements.size() && m_state != State::BeingCollected; i++)
        {
            PauseCheck();
            m_elements[i].color = sf::Color::Red;
            Element tmp = m_elements[i];
            size_t j;
            for (j = i; j >= gap && m_elements[j - gap].value > tmp.value && m_state != State::BeingCollected; j -= gap)
            {
                m_elements[j].color = sf::Color::Red;
                m_elements[j - gap].color = sf::Color::Red;
                m_elements[j] = m_elements[j - gap];
                SleepDelay();
                m_elements[j].color = sf::Color::White;
                m_elements[j - gap].color = sf::Color::White;
            }
            SleepDelay();
            m_elements[j] = tmp;
            m_elements[i].color = sf::Color::White;
            m_elements[j].color = sf::Color::White;
        }
    }
}