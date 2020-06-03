#include "RadixSort.h"

void RadixSort::Sort()
{
    long h = HighestValue();
    for (size_t exp = 1; (size_t)(h / exp) > 0 && m_state != State::BeingCollected; exp *= 10)
        CountSort(exp);
}

void RadixSort::CountSort(size_t exponent)
{

    std::vector<Bar> outBucket(m_bars.size());
    std::array<size_t, 10> count = {0};

    // Store count of occurrences in count[]
    for (size_t i = 0; i < m_bars.size() && m_state != State::BeingCollected; i++)
    {
        PauseCheck();
        count[(m_bars[i].value / exponent) % 10]++;
    }

    // Change count[i] so that count[i] now contains actual
    //  position of this digit in output[]
    for (size_t i = 1; i < 10 && m_state != State::BeingCollected; i++)
    {
        PauseCheck();
        count[i] += count[i - 1];
    }

    // Build the output array
    for (long i = (long)m_bars.size() - 1; i >= 0 && m_state != State::BeingCollected; i--)
    {
        PauseCheck();
        outBucket[count[(m_bars[i].value / exponent) % 10] - 1] = m_bars[i];
        count[(m_bars[i].value / exponent) % 10]--;
    }

    // Copy the output array to arr[], so that arr[] now
    // contains sorted numbers according to current digit
    for (size_t i = 0; i < m_bars.size() && m_state != State::BeingCollected; i++)
    {
        m_bars[i].color = sf::Color::Red;
        SleepDelay();
        SleepDelay();
        SleepDelay();
        PauseCheck();
        m_bars[i] = outBucket[i];
        m_bars[i].color = sf::Color::White;
    }

    if (m_state != State::BeingCollected)
        m_bars = outBucket;
}

long RadixSort::HighestValue()
{
    return std::max_element(m_bars.begin(), m_bars.end(), [](const Bar &first, const Bar &second) {
               return first.value < second.value;
           })
        ->value;
}