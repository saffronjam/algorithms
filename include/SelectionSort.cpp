#include "SelectionSort.hpp"

SelectionSort::SelectionSort(std::string const &sortName)
    : Sort(sortName)
{
}

void SelectionSort::Step()
{
    for (size_t i = 0; i < m_allBars->size() - 1 && !m_isBeingCollected; i++)
    {
        ContinueIfNotPaused();
        size_t indexFound = i;
        (*m_allBars)[i]->SetColor(sf::Color::Blue);
        for (size_t j = i + 1; j < m_allBars->size() && !m_isBeingCollected; j++)
        {
            ContinueIfNotPaused();
            (*m_allBars)[j]->SetColor(sf::Color::Yellow);
            if ((*m_allBars)[j]->GetValue() < (*m_allBars)[indexFound]->GetValue())
            {
                indexFound = j;
                (*m_allBars)[indexFound]->SetColor(sf::Color::Red);
            }
            SleepDelay();
        }
        std::swap((*m_allBars)[i], (*m_allBars)[indexFound]);
        for (size_t j = i + 1; j < m_allBars->size(); j++)
        {
            (*m_allBars)[j]->SetColor(sf::Color::White);
        }
        (*m_allBars)[i]->SetColor(sf::Color::Green);
    }
}