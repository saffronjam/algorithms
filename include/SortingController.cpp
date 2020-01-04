#include "SortingController.hpp"

#include "Random.hpp"

SortingController::SortingController(Graphics &gfx, SortingAlgorithm const &algorithmType, sf::Time const &dt)
    : m_gfx(gfx),
      m_dt(dt),
      m_nBars(10),
      m_sorting10(false),
      m_sorting100(true),
      m_sorting1000(false),
      m_sorting10000(false),
      m_algorithmType(algorithmType)
{
    m_allWorkers.reserve(SortingAlgorithm::ALG_Count);
    m_allBarContainers.reserve(SortingAlgorithm::ALG_Count);
    m_allAlgorithms.reserve(SortingAlgorithm::ALG_Count);
    m_allBars.resize(SortingAlgorithm::ALG_Count);

    m_allAlgorithms.push_back(std::make_unique<BubbleSort>(BubbleSort("Bubble Sort")));
    m_allAlgorithms.push_back(std::make_unique<SelectionSort>(SelectionSort("Selection Sort")));
    m_allAlgorithms.push_back(std::make_unique<InsertionSort>(InsertionSort("Insertion Sort")));
    m_allAlgorithms.push_back(std::make_unique<ShellSort>(ShellSort("Shell Sort")));
    m_allAlgorithms.push_back(std::make_unique<MergeSort>(MergeSort("Merge Sort")));
    m_allAlgorithms.push_back(std::make_unique<HeapSort>(HeapSort("Heap Sort")));
    m_allAlgorithms.push_back(std::make_unique<QuickSort>(QuickSort("Quick Sort")));

    m_allAlgorithms[SortingAlgorithm::ALG_BubbleSort]->SetIsActive(true);
    m_allAlgorithms[SortingAlgorithm::ALG_SelectionSort]->SetIsActive(true);
    m_allAlgorithms[SortingAlgorithm::ALG_InsertionSort]->SetIsActive(true);
    m_allAlgorithms[SortingAlgorithm::ALG_ShellSort]->SetIsActive(true);
    m_allAlgorithms[SortingAlgorithm::ALG_MergeSort]->SetIsActive(true);
    m_allAlgorithms[SortingAlgorithm::ALG_HeapSort]->SetIsActive(true);
    m_allAlgorithms[SortingAlgorithm::ALG_QuickSort]->SetIsActive(true);

    for (int i = 0; i < SortingAlgorithm::ALG_Count; i++)
    {
        m_allWorkers.push_back(std::thread());

        m_allBarContainers.push_back(BarContainer(m_gfx, m_allBars[i]));

        m_allAlgorithms[i]->SetBarContainer(m_allBarContainers[i].GetAllBarsPtr());

        m_allBarContainers[i].SetLabel(m_allAlgorithms[i]->GetName());
    }

    GenerateRandomBars();

    PositionSortingContainers();
}

SortingController::~SortingController()
{
    CollectAllWorkers();
}

void SortingController::Update()
{
    for (int i = 0; i < SortingAlgorithm::ALG_Count; i++)
    {
        if (m_allAlgorithms[i]->GetIsActive())
        {
            m_allBarContainers[i].Update();
            m_allAlgorithms[i]->SetTimeStep(m_timeStep);
        }
    }
}

void SortingController::Draw()
{
    for (int i = 0; i < SortingAlgorithm::ALG_Count; i++)
    {
        if (m_allAlgorithms[i]->GetIsActive())
        {
            m_allBarContainers[i].Draw();
        }
    }
}

void SortingController::StartAll()
{
    for (auto &algorithm : m_allAlgorithms)
    {
        if (algorithm->GetIsActive())
        {
            algorithm->SetIsPaused(false);
        }
    }
}

void SortingController::PauseAll()
{
    for (auto &algorithm : m_allAlgorithms)
    {
        if (algorithm->GetIsActive())
        {
            algorithm->SetIsPaused(true);
        }
    }
}

void SortingController::ResetAll()
{
    CollectAllWorkers();
    for (int i = 0; i < SortingAlgorithm::ALG_Count; i++)
    {
        m_allAlgorithms[i]->SetIsPaused(true);
        m_allAlgorithms[i]->SetIsFinished(false);
        if (m_allBarContainers.size())
            m_allBarContainers[i].Reset();
    }
    SendOutWorkers();
}

void SortingController::NewValues()
{
    GenerateRandomBars();
}

bool &SortingController::GetActiveAlgorithmRef(SortingAlgorithm const &type)
{
    return m_allAlgorithms[type]->GetIsActiveRef();
}

bool *SortingController::GetSortingNumberPtr(SortingNumber const &number)
{
    switch (number)
    {
    case SortingNumber::NUM_10:
        return &m_sorting10;
        break;
    case SortingNumber::NUM_100:
        return &m_sorting100;
        break;
    case SortingNumber::NUM_1000:
        return &m_sorting1000;
        break;
    case SortingNumber::NUM_10000:
        return &m_sorting10000;
        break;
    default:
        return nullptr;
    }
}

void SortingController::PositionSortingContainers()
{
    ResetAll();
    CollectAllWorkers();

    const int nActiveContainers = NumberOfActiveAlgorithms();
    std::vector<sf::IntRect> allVisualizationBoxes;

    switch (nActiveContainers)
    {
    case 1:
    {
        sf::Vector2i size(2 * Camera::GetOffset());
        sf::Vector2i offset(40, 60);
        allVisualizationBoxes.push_back(sf::IntRect(-Camera::GetOffset() + offset, size - offset * 2));
        break;
    }
    case 2:
    {
        sf::Vector2i size(Camera::GetOffset());
        sf::Vector2i offset(40, 40);
        allVisualizationBoxes.push_back(sf::IntRect(sf::Vector2i(-Camera::OffsetX, -Camera::OffsetY / 2) + offset, size - offset * 2));
        allVisualizationBoxes.push_back(sf::IntRect(sf::Vector2i(0, -Camera::OffsetY / 2) + offset, size - offset * 2));
        break;
    }
    case 3:
    {
        sf::Vector2i size(Camera::GetOffset());
        sf::Vector2i offset(40, 60);
        allVisualizationBoxes.push_back(sf::IntRect(-Camera::GetOffset() + offset, size - offset * 2));
        allVisualizationBoxes.push_back(sf::IntRect(sf::Vector2i(0, -Camera::OffsetY) + offset, size - offset * 2));
        allVisualizationBoxes.push_back(sf::IntRect(sf::Vector2i(-Camera::OffsetX / 2, 0) + offset, size - offset * 2));
        break;
    }
    case 4:
    {
        sf::Vector2i size(Camera::GetOffset());
        sf::Vector2i offset(40, 60);
        allVisualizationBoxes.push_back(sf::IntRect(-Camera::GetOffset() + offset, size - offset * 2));
        allVisualizationBoxes.push_back(sf::IntRect(sf::Vector2i(0, -Camera::OffsetY) + offset, size - offset * 2));
        allVisualizationBoxes.push_back(sf::IntRect(sf::Vector2i(-Camera::OffsetX, 0) + offset, size - offset * 2));
        allVisualizationBoxes.push_back(sf::IntRect(sf::Vector2i(0, 0) + offset, size - offset * 2));
        break;
    }
    case 5:
    {
        sf::Vector2i size(2 * Camera::GetOffset() / 3);
        sf::Vector2i offset(40, 40);
        allVisualizationBoxes.push_back(sf::IntRect(sf::Vector2i(-Camera::OffsetX, -5 * Camera::OffsetY / 6) + offset, size - offset * 2));
        allVisualizationBoxes.push_back(sf::IntRect(sf::Vector2i(-Camera::OffsetX / 3, -5 * Camera::OffsetY / 6) + offset, size - offset * 2));
        allVisualizationBoxes.push_back(sf::IntRect(sf::Vector2i(Camera::OffsetX / 3, -5 * Camera::OffsetY / 6) + offset, size - offset * 2));
        allVisualizationBoxes.push_back(sf::IntRect(sf::Vector2i(-2 * Camera ::OffsetX / 3, 0) + offset, size - offset * 2));
        allVisualizationBoxes.push_back(sf::IntRect(sf::Vector2i(0, 0) + offset, size - offset * 2));
        break;
    }
    case 6:
    {
        sf::Vector2i size(2 * Camera::GetOffset() / 3);
        sf::Vector2i offset(40, 40);
        allVisualizationBoxes.push_back(sf::IntRect(sf::Vector2i(-Camera::OffsetX, -5 * Camera::OffsetY / 6) + offset, size - offset * 2));
        allVisualizationBoxes.push_back(sf::IntRect(sf::Vector2i(-Camera::OffsetX / 3, -5 * Camera::OffsetY / 6) + offset, size - offset * 2));
        allVisualizationBoxes.push_back(sf::IntRect(sf::Vector2i(Camera::OffsetX / 3, -5 * Camera::OffsetY / 6) + offset, size - offset * 2));
        allVisualizationBoxes.push_back(sf::IntRect(sf::Vector2i(-Camera::OffsetX, 0) + offset, size - offset * 2));
        allVisualizationBoxes.push_back(sf::IntRect(sf::Vector2i(-Camera::OffsetX / 3, 0) + offset, size - offset * 2));
        allVisualizationBoxes.push_back(sf::IntRect(sf::Vector2i(Camera::OffsetX / 3, 0) + offset, size - offset * 2));
        break;
    }
    case 7:
    {
        sf::Vector2i size(2 * Camera::GetOffset() / 3);
        sf::Vector2i offset(40, 60);
        allVisualizationBoxes.push_back(sf::IntRect(sf::Vector2i(-Camera::OffsetX, -Camera::OffsetY) + offset, size - offset * 2));
        allVisualizationBoxes.push_back(sf::IntRect(sf::Vector2i(-Camera::OffsetX / 3, -Camera::OffsetY) + offset, size - offset * 2));
        allVisualizationBoxes.push_back(sf::IntRect(sf::Vector2i(Camera::OffsetX / 3, -Camera::OffsetY) + offset, size - offset * 2));
        allVisualizationBoxes.push_back(sf::IntRect(sf::Vector2i(-Camera::OffsetX, -Camera::OffsetY / 3) + offset, size - offset * 2));
        allVisualizationBoxes.push_back(sf::IntRect(sf::Vector2i(-Camera::OffsetX / 3, -Camera::OffsetY / 3) + offset, size - offset * 2));
        allVisualizationBoxes.push_back(sf::IntRect(sf::Vector2i(Camera::OffsetX / 3, -Camera::OffsetY / 3) + offset, size - offset * 2));
        allVisualizationBoxes.push_back(sf::IntRect(sf::Vector2i(-Camera::OffsetX / 3, Camera::OffsetY / 3) + offset, size - offset * 2));
        break;
    }
    default:
        break;
    }

    for (int i = 0, j = 0; i < SortingAlgorithm::ALG_Count; i++)
    {
        if (m_allAlgorithms[i]->GetIsActive())
        {
            m_allBarContainers[i].SetVisualizationBox(allVisualizationBoxes[j]);
            m_allBarContainers[i].CorrectBarSizes();
            j++;
        }
    }

    SendOutWorkers();
}

void SortingController::GenerateRandomBars()
{
    ResetAll();

    CollectAllWorkers();

    if (m_sorting10000)
    {
        m_nBars = 10000;
    }
    else if (m_sorting1000)
    {
        m_nBars = 1000;
    }
    else if (m_sorting100)
    {
        m_nBars = 100;
    }
    else if (m_sorting10)
    {
        m_nBars = 10;
    }

    Random randomizeValue;
    randomizeValue.SetLowerBound(0);
    randomizeValue.SetUpperBound(10000);

    std::vector<double> newRandom;
    for (size_t i = 0; i < m_nBars; i++)
    {
        newRandom.push_back(randomizeValue());
    }

    for (size_t i = 0; i < SortingAlgorithm::ALG_Count; i++)
    {
        m_allBars[i].clear();
        for (size_t j = 0; j < newRandom.size(); j++)
        {
            m_allBars[i].push_back(std::make_shared<BarContainer::Bar>(newRandom[j]));
        }
        m_allBarContainers[i].ComputeResetValues();
    }

    ResetAll();
}

void SortingController::CollectAllWorkers()
{
    for (int i = 0; i < SortingAlgorithm::ALG_Count; i++)
    {

        if (m_allAlgorithms[i]->GetIsActive() && m_allWorkers[i].joinable())
        {
            m_allAlgorithms[i]->SetIsBeingCollected(true);
            m_allWorkers[i].join();
            m_allAlgorithms[i]->SetIsBeingCollected(false);
        }
    }
}

void SortingController::SendOutWorkers()
{
    for (int i = 0; i < SortingAlgorithm::ALG_Count; i++)
    {
        if (m_allAlgorithms[i]->GetIsActive())
        {
            if (BubbleSort *bubbleSort = dynamic_cast<BubbleSort *>(m_allAlgorithms[i].get()))
            {
                m_allWorkers[i] = std::thread(&BubbleSort::WorkerUpdate, bubbleSort, std::ref(m_dt));
            }
            else if (SelectionSort *selectionSort = dynamic_cast<SelectionSort *>(m_allAlgorithms[i].get()))
            {
                m_allWorkers[i] = std::thread(&SelectionSort::WorkerUpdate, selectionSort, std::ref(m_dt));
            }
            else if (InsertionSort *insertionSort = dynamic_cast<InsertionSort *>(m_allAlgorithms[i].get()))
            {
                m_allWorkers[i] = std::thread(&InsertionSort::WorkerUpdate, insertionSort, std::ref(m_dt));
            }
            else if (ShellSort *shellSort = dynamic_cast<ShellSort *>(m_allAlgorithms[i].get()))
            {
                m_allWorkers[i] = std::thread(&ShellSort::WorkerUpdate, shellSort, std::ref(m_dt));
            }
            else if (MergeSort *mergeSort = dynamic_cast<MergeSort *>(m_allAlgorithms[i].get()))
            {
                m_allWorkers[i] = std::thread(&MergeSort::WorkerUpdate, mergeSort, std::ref(m_dt));
            }
            else if (HeapSort *heapSort = dynamic_cast<HeapSort *>(m_allAlgorithms[i].get()))
            {
                m_allWorkers[i] = std::thread(&HeapSort::WorkerUpdate, heapSort, std::ref(m_dt));
            }
            else if (QuickSort *quickSort = dynamic_cast<QuickSort *>(m_allAlgorithms[i].get()))
            {
                m_allWorkers[i] = std::thread(&QuickSort::WorkerUpdate, quickSort, std::ref(m_dt));
            }
        }
    }
}

int SortingController::NumberOfActiveAlgorithms()
{
    int n = 0;
    for (auto &algorithm : m_allAlgorithms)
    {
        if (algorithm->GetIsActive())
        {
            n++;
        }
    }
    return n;
}