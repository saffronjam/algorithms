#include "HeapSort.h"

namespace Se
{
HeapSort::HeapSort() :
	Algorithm("Heap Sort")
{
}

void HeapSort::Sort()
{
	for (long i = static_cast<long>(Elements().size() / 2 - 1); i >= 0 && _state != State::BeingCollected; i--)
	{
		PauseCheck();
		Heapify(Elements().size(), i);
	}

	for (long i = static_cast<long>(Elements().size() - 1); i >= 0 && _state != State::BeingCollected; i--)
	{
		PauseCheck();
		SwapElements(i, 0);
		SetColor(i, sf::Color::Green);
		Heapify(i, 0);
		for (long j = i; j >= 0; j--)
		{
			SetColor(j, sf::Color::White);
		}
		SetColor(i, sf::Color::Green);
	}
}

void HeapSort::Heapify(size_t maxSizeCheck, size_t parent)
{
	size_t largest = parent; //assume parent is largest
	const size_t leftChild = 2 * parent + 1;
	const size_t rightChild = 2 * parent + 2;

	if (leftChild < maxSizeCheck && _state != State::BeingCollected)
	{
		SetColor(leftChild, sf::Color::Red);
		if (ValueByIndex(leftChild) > ValueByIndex(largest))
		{
			largest = leftChild;
		}
	}

	if (rightChild < maxSizeCheck && _state != State::BeingCollected)
	{
		SetColor(rightChild, sf::Color::Red);
		if (ValueByIndex(rightChild) > ValueByIndex(largest))
		{
			largest = rightChild;
		}
	}

	if (largest != parent && _state != State::BeingCollected)
	{
		SleepDelay();
		PauseCheck();
		SwapElements(parent, largest);
		Heapify(maxSizeCheck, largest);
	}
}
}
