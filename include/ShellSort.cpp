#include "ShellSort.hpp"

ShellSort::ShellSort(std::string const &sortName)
    : Sort(sortName)
{
}

void ShellSort::Step()
{
    for (size_t gap = m_allBars->size() / 2; gap > 0 && !m_isBeingCollected; gap /= 2)
    {
        for (size_t i = gap; i < m_allBars->size() && !m_isBeingCollected; i++)
        {
            ContinueIfNotPaused();
            (*m_allBars)[i]->SetColor(sf::Color::Red);
            BarContainer::Bar temp = *(*m_allBars)[i];
            size_t j;
            for (j = i; j >= gap && (*m_allBars)[j - gap]->GetValue() > temp.GetValue() && !m_isBeingCollected; j -= gap)
            {
                (*m_allBars)[j]->SetColor(sf::Color::Red);
                (*m_allBars)[j - gap]->SetColor(sf::Color::Red);
                *(*m_allBars)[j] = *(*m_allBars)[j - gap];
                SleepDelay();
                (*m_allBars)[j]->SetColor(sf::Color::White);
                (*m_allBars)[j - gap]->SetColor(sf::Color::White);
            }
            SleepDelay();
            *(*m_allBars)[j] = temp;
            (*m_allBars)[i]->SetColor(sf::Color::White);
            (*m_allBars)[j]->SetColor(sf::Color::White);
        }
    }
}