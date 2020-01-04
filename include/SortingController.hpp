#pragma once

#include <stdint.h>
#include <thread>

#include "BubbleSort.hpp"
#include "InsertionSort.hpp"
#include "SelectionSort.hpp"
#include "ShellSort.hpp"
#include "MergeSort.hpp"
#include "HeapSort.hpp"
#include "QuickSort.hpp"

class SortingController
{
public:
    enum SortingAlgorithm : char
    {
        ALG_BubbleSort = 0,
        ALG_SelectionSort,
        ALG_InsertionSort,
        ALG_ShellSort,
        ALG_MergeSort,
        ALG_HeapSort,
        ALG_QuickSort,
        ALG_Count
    };

    enum SortingNumber : char
    {
        NUM_10,
        NUM_100,
        NUM_1000,
        NUM_10000
    };

public:
    SortingController(Graphics &gfx, SortingAlgorithm const &algorithmType, sf::Time const &dt);
    ~SortingController();

    void Update();
    void Draw();

    int &GetStepTimeRef() { return m_timeStep; };
    bool GetIsActive(SortingAlgorithm const &algorithmType) { return m_allAlgorithms[algorithmType]->GetIsActive(); }

    void StartAll();
    void PauseAll();
    void ResetAll();
    void NewValues();

    bool &GetActiveAlgorithmRef(SortingAlgorithm const &type);
    bool *GetSortingNumberPtr(SortingNumber const &number);

    void PositionSortingContainers();
    void GenerateRandomBars();

private:
    void CollectAllWorkers();
    void SendOutWorkers();

    int NumberOfActiveAlgorithms();

private:
    Graphics &m_gfx;
    sf::Time const &m_dt;

    std::vector<std::thread> m_allWorkers;
    std::vector<BarContainer> m_allBarContainers;
    std::vector<std::unique_ptr<Sort>> m_allAlgorithms;
    std::vector<std::vector<std::shared_ptr<BarContainer::Bar>>> m_allBars;

    int m_timeStep;
    size_t m_nBars;
    bool m_sorting10;
    bool m_sorting100;
    bool m_sorting1000;
    bool m_sorting10000;

    SortingAlgorithm m_algorithmType;
};