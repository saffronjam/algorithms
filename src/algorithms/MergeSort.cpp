#include "MergeSort.h"

void MergeSort::Sort()
{
    Split(0, m_bars.size() - 1);
}

void MergeSort::Split(size_t iStart, size_t iEnd)
{
    if (iStart != iEnd && m_state != State::BeingCollected)
    {
        size_t iMiddle = iStart + std::floor((iEnd - iStart) / 2);
        Split(iStart, iMiddle);
        Split(iMiddle + 1, iEnd);
        Merge(iStart, iMiddle, iMiddle + 1, iEnd);
    }
}

void MergeSort::Merge(size_t iLeftStart, size_t iLeftEnd, size_t iRightStart, size_t iRightEnd)
{
    std::vector<Bar> sorted;

    size_t iCurrLeft = iLeftStart;
    size_t iCurrRight = iRightStart;

    while ((iCurrLeft != iLeftEnd + 1 || iCurrRight != iRightEnd + 1) && m_state != State::BeingCollected)
    {
        PauseCheck();
        if (iCurrLeft != iLeftEnd + 1)
            m_bars[iCurrLeft].color = sf::Color::Red;
        if (iCurrRight != iRightEnd + 1)
            m_bars[iCurrRight].color = sf::Color::Red;

        if (iCurrLeft == iLeftEnd + 1)
        {
            sorted.push_back(m_bars[iCurrRight]);
            iCurrRight++;
        }
        else if (iCurrRight == iRightEnd + 1)
        {
            sorted.push_back(m_bars[iCurrLeft]);
            iCurrLeft++;
        }
        else if (m_bars[iCurrLeft].value <= m_bars[iCurrRight].value)
        {
            sorted.push_back(m_bars[iCurrLeft]);
            iCurrLeft++;
        }
        else if (m_bars[iCurrLeft].value > m_bars[iCurrRight].value)
        {
            sorted.push_back(m_bars[iCurrRight]);
            iCurrRight++;
        }
        SleepDelay();
    }

    for (size_t i = 0; iLeftStart + i != iRightEnd + 1 && m_state != State::BeingCollected; i++)
    {
        PauseCheck();
        m_bars[iLeftStart + i] = sorted[i];
        m_bars[iLeftStart + i].color = sf::Color::White;
    }
}