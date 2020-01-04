#include "InsertionSort.hpp"

InsertionSort::InsertionSort(std::string const &sortName)
    : Sort(sortName)
{
}

void InsertionSort::Step()
{
    for (size_t i = 1; i < m_allBars->size() && !m_isBeingCollected; i++)
    {
        for (size_t j = i; j > 0 && !m_isBeingCollected; j--)
        {
            ContinueIfNotPaused();
            if ((*m_allBars)[j]->GetValue() < (*m_allBars)[j - 1]->GetValue())
            {
                (*m_allBars)[j]->SetColor(sf::Color::Red);
                (*m_allBars)[j - 1]->SetColor(sf::Color::Yellow);
                SleepDelay();
                std::swap((*m_allBars)[j], (*m_allBars)[j - 1]);
                (*m_allBars)[j]->SetColor(sf::Color::White);
            }
            else
            {
                SleepDelay();
                break;
            }
        }
        for (int j = i; j >= 0; j--)
        {
            (*m_allBars)[j]->SetColor(sf::Color::Cyan);
        }
    }
}