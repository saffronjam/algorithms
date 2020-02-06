#include "RadixSort.hpp"

RadixSort::RadixSort(std::string const &sortName)
    : Sort(sortName)
{
}

void RadixSort::Step()
{
    double h = HighestValue();
    for (int exp = 1; (int)(h / exp) > 0 && !m_isBeingCollected; exp *= 10)
        CountSort(exp);
}
void RadixSort::CountSort(int const &exponent)
{
    std::vector<std::shared_ptr<BarContainer::Bar>> outBucket(m_allBars->size());
    std::array<size_t, 10> count = {0};

    for (size_t i = 0; i < m_allBars->size() && !m_isBeingCollected; i++)
    {
        ContinueIfNotPaused();
        count[((size_t)(*m_allBars)[i]->GetValue() / exponent) % 10]++;
    }

    for (int i = 1; i < 10 && !m_isBeingCollected; i++)
    {
        ContinueIfNotPaused();
        count[i] += count[i - 1];
    }

    for (int i = m_allBars->size() - 1; i >= 0 && !m_isBeingCollected; i--)
    {
        ContinueIfNotPaused();
        outBucket[count[((size_t)(*m_allBars)[i]->GetValue() / exponent) % 10] - 1] = (*m_allBars)[i];
        count[((size_t)(*m_allBars)[i]->GetValue() / exponent) % 10]--;
    }

    for (size_t i = 0; i < m_allBars->size() && !m_isBeingCollected; i++)
    {
        (*m_allBars)[i]->SetColor(sf::Color::Red);
        SleepDelay();
        SleepDelay();
        SleepDelay();
        ContinueIfNotPaused();
        (*m_allBars)[i] = outBucket[i];
        (*m_allBars)[i]->SetColor(sf::Color::White);
    }
    if (m_isBeingCollected)
        *m_allBars = outBucket;
}

double RadixSort::HighestValue()
{
    int highest = m_allBars->front()->GetValue();
    for (auto &bar : *m_allBars)
        bar->GetValue() > highest ? highest = bar->GetValue() : 0;
    return highest;
}