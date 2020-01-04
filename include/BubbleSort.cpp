#include "BubbleSort.hpp"

BubbleSort::BubbleSort(std::string const &sortName)
    : Sort(sortName)
{
}

void BubbleSort::Step()
{
    for (size_t i = 0; i < m_allBars->size() && !m_isBeingCollected; i++)
    {
        for (size_t j = 0; j < m_allBars->size() - i - 1 && !m_isBeingCollected; j++)
        {
            ContinueIfNotPaused();
            (*m_allBars)[j]->SetColor(sf::Color::Red);
            (*m_allBars)[j + 1]->SetColor(sf::Color::Red);
            if ((*m_allBars)[j]->GetValue() > (*m_allBars)[j + 1]->GetValue())
            {
                std::swap((*m_allBars)[j], (*m_allBars)[j + 1]);
            }
            SleepDelay();
            (*m_allBars)[j]->SetColor(sf::Color::White);
            (*m_allBars)[j + 1]->SetColor(sf::Color::White);
        }
        (*m_allBars)[m_allBars->size() - i - 1]->SetColor(sf::Color::Green);
    }
}