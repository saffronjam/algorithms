#include "MergeSort.hpp"

#include "Math.hpp"

MergeSort::MergeSort(std::string const &sortName)
    : Sort(sortName)
{
}

void MergeSort::Step()
{
    Split(m_allBars->begin(), m_allBars->end() - 1);
}

void MergeSort::Split(BarIter start, BarIter end)
{
    //if end==start means that there is only ONE elemtent in the range, DONT SPLIT MORE - Already sorted
    if (start != end && !m_isBeingCollected)
    {
        BarIter middle = start + std::floor((end - start) / 2); //get floored
        Split(start, middle);
        Split(middle + 1, end);
        Merge(start, middle, middle + 1, end);
    }
}

void MergeSort::Merge(BarIter leftStart, BarIter leftEnd, BarIter rightStart, BarIter rightEnd)
{
    std::vector<BarContainer::Bar> sorted;

    BarIter currLeft = leftStart;
    BarIter currRight = rightStart;

    while ((currLeft != leftEnd + 1 || currRight != rightEnd + 1) && !m_isBeingCollected)
    {
        ContinueIfNotPaused();
        if (currLeft != leftEnd + 1)
            currLeft->get()->SetColor(sf::Color::Red);
        if (currRight != rightEnd + 1)
            currRight->get()->SetColor(sf::Color::Red);

        if (currLeft == leftEnd + 1)
        {
            sorted.push_back(**currRight);
            currRight++;
        }
        else if (currRight == rightEnd + 1)
        {
            sorted.push_back(**currLeft);
            currLeft++;
        }
        else if (currLeft->get()->GetValue() <= currRight->get()->GetValue())
        {
            sorted.push_back(**currLeft);
            currLeft++;
        }
        else if (currLeft->get()->GetValue() > currRight->get()->GetValue())
        {
            sorted.push_back(**currRight);
            currRight++;
        }
        SleepDelay();
    }

    for (size_t i = 0; leftStart + i != rightEnd + 1 && !m_isBeingCollected; i++)
    {
        ContinueIfNotPaused();
        **(leftStart + i) = sorted[i];
        (leftStart + i)->get()->SetColor(sf::Color::White);
    }
}