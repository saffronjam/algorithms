#pragma once

#include <set>
#include <numeric>

#include "Window.h"
#include "Random.h"

// ----- Algorithms -----
#include "BubbleSort.h"
#include "SelectionSort.h"
#include "InsertionSort.h"
#include "ShellSort.h"
#include "MergeSort.h"
#include "HeapSort.h"
#include "QuickSort.h"
#include "ShittySort.h"
#include "RadixSort.h"
// ----------------------

class AlgorithmMgr
{
public:
    AlgorithmMgr();
    ~AlgorithmMgr();
    AlgorithmMgr(const AlgorithmMgr &) = delete;
    const AlgorithmMgr &operator()(const AlgorithmMgr &) = delete;

    void Draw();

    void Add(IAlgorithm *algorithm);

    void Start();
    void Pause();
    void Resume();
    void Reset();

    void Resize(size_t size);
    void Shuffle();

private:
    void GenerateDrawContainers();
    int GetActiveContainers();

private:
    std::vector<IAlgorithm *> m_algorithms;
    std::vector<sf::FloatRect> m_drawContainers;
    std::vector<Bar> m_currentBars;
};