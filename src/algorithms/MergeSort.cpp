#include "MergeSort.h"

void MergeSort::Sort()
{
    Split(0, m_elements.size() - 1);
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
    std::vector<Element> sorted;

    size_t iCurrLeft = iLeftStart;
    size_t iCurrRight = iRightStart;

    while ((iCurrLeft != iLeftEnd + 1 || iCurrRight != iRightEnd + 1) && m_state != State::BeingCollected)
    {
        PauseCheck();
        if (iCurrLeft != iLeftEnd + 1)
            m_elements[iCurrLeft].color = sf::Color::Red;
        if (iCurrRight != iRightEnd + 1)
            m_elements[iCurrRight].color = sf::Color::Red;

        if (iCurrLeft == iLeftEnd + 1)
        {
            sorted.push_back(m_elements[iCurrRight]);
            iCurrRight++;
        }
        else if (iCurrRight == iRightEnd + 1)
        {
            sorted.push_back(m_elements[iCurrLeft]);
            iCurrLeft++;
        }
        else if (m_elements[iCurrLeft].value <= m_elements[iCurrRight].value)
        {
            sorted.push_back(m_elements[iCurrLeft]);
            iCurrLeft++;
        }
        else if (m_elements[iCurrLeft].value > m_elements[iCurrRight].value)
        {
            sorted.push_back(m_elements[iCurrRight]);
            iCurrRight++;
        }
        SleepDelay();
    }

    for (size_t i = 0; iLeftStart + i != iRightEnd + 1 && m_state != State::BeingCollected; i++)
    {
        PauseCheck();
        m_elements[iLeftStart + i] = sorted[i];
        m_elements[iLeftStart + i].color = sf::Color::White;
    }
}