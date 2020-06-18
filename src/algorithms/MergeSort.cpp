#include "MergeSort.h"

void MergeSort::Sort()
{
    Split(0, GetContainer().size() - 1);
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
    std::vector<long> sorted;

    size_t iCurrLeft = iLeftStart;
    size_t iCurrRight = iRightStart;

    while ((iCurrLeft != iLeftEnd + 1 || iCurrRight != iRightEnd + 1) && m_state != State::BeingCollected)
    {
        PauseCheck();
        if (iCurrLeft != iLeftEnd + 1)
            SetColor(iCurrLeft, sf::Color::Red);
        if (iCurrRight != iRightEnd + 1)
            SetColor(iCurrRight, sf::Color::Red);

        if (iCurrLeft == iLeftEnd + 1)
        {
            sorted.push_back(GetValue(iCurrRight));
            iCurrRight++;
        }
        else if (iCurrRight == iRightEnd + 1)
        {
            sorted.push_back(GetValue(iCurrLeft));
            iCurrLeft++;
        }
        else if (GetValue(iCurrLeft) <= GetValue(iCurrRight))
        {
            sorted.push_back(GetValue(iCurrLeft));
            iCurrLeft++;
        }
        else if (GetValue(iCurrLeft) > GetValue(iCurrRight))
        {
            sorted.push_back(GetValue(iCurrRight));
            iCurrRight++;
        }
        SleepDelay();
    }

    for (size_t i = 0; iLeftStart + i != iRightEnd + 1 && m_state != State::BeingCollected; i++)
    {
        PauseCheck();
        SetValue(iLeftStart + i, sorted[i]);
        SetColor(iLeftStart + i, sf::Color::White);
    }
}