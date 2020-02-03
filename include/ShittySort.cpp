#include "ShittySort.hpp"

ShittySort::ShittySort(std::string const &sortName)
    : Sort(sortName)
{
}

void ShittySort::Step()
{
    double currentCheck;
    do
    {
        currentCheck = (*m_allBars)[0]->GetValue();
        for (size_t i = 0; i < m_allBars->size() && !m_isBeingCollected; i++)
        {
            ContinueIfNotPaused();
            if ((*m_allBars)[i]->GetValue() >= currentCheck)
            {
                currentCheck = (*m_allBars)[i]->GetValue();
            }
            else
            {
                int randomIndex = Random::Generate((int)0, (int)m_allBars->size() - 1);
                if ((*m_allBars)[randomIndex]->GetValue() > (*m_allBars)[randomIndex + 1]->GetValue())
                {
                    std::swap((*m_allBars)[randomIndex], (*m_allBars)[randomIndex + 1]);
                    (*m_allBars)[randomIndex]->SetColor(sf::Color::Yellow);
                    (*m_allBars)[randomIndex + 1]->SetColor(sf::Color::Yellow);
                }
                else
                {
                    (*m_allBars)[randomIndex]->SetColor(sf::Color::Red);
                    (*m_allBars)[randomIndex + 1]->SetColor(sf::Color::Red);
                }
                SleepDelay();
                (*m_allBars)[randomIndex]->SetColor(sf::Color::White);
                (*m_allBars)[randomIndex + 1]->SetColor(sf::Color::White);
                break;
            }
        }
    } while (currentCheck != m_allBars->back()->GetValue() && !m_isBeingCollected);
}
