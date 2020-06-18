#include "AlgorithmMgr.h"

AlgorithmMgr::AlgorithmMgr()
{
    Add(new BubbleSort());
    Add(new SelectionSort());
    Add(new InsertionSort());
    Add(new ShellSort());
    Add(new MergeSort());
    Add(new HeapSort());
    Add(new QuickSort());
    Add(new GnomeSort());
    Add(new RadixSort());

    GenerateDrawContainers();
    Resize(100);
}

AlgorithmMgr::~AlgorithmMgr()
{
    Reset();
    for (auto &algorithm : m_algorithms)
    {
        delete algorithm;
    }
    m_algorithms.clear();
}

void AlgorithmMgr::Draw()
{
    int i = 0;
    for (auto &algorithm : m_algorithms)
    {
        if (i < m_drawContainers.size() && algorithm->IsActive())
        {
            algorithm->Draw(m_drawContainers[i]);
            algorithm->DrawName(m_drawContainers[i]);
            i++;
        }
    }
}

void AlgorithmMgr::Add(IAlgorithm *algorithm)
{
    m_algorithms.emplace_back(algorithm);
}

void AlgorithmMgr::Activate(const std::string &name)
{
    for (auto &algorithm : m_algorithms)
    {
        if (algorithm->GetName() == name)
        {
            algorithm->Activate();
            GenerateDrawContainers();
            break;
        }
    }
}

void AlgorithmMgr::Deactivate(const std::string &name)
{
    for (auto &algorithm : m_algorithms)
    {
        if (algorithm->GetName() == name)
        {
            algorithm->Deactivate();
            GenerateDrawContainers();
            break;
        }
    }
}

void AlgorithmMgr::ActivateSpectrum() noexcept
{
    for (auto &algorithm : m_algorithms)
        algorithm->ActivateSpectrum();
}

void AlgorithmMgr::DeactivateSpectrum() noexcept
{
    for (auto &algorithm : m_algorithms)
        algorithm->DeactivateSpectrum();
}

void AlgorithmMgr::Start()
{
    for (auto &algorithm : m_algorithms)
        algorithm->Start();
}
void AlgorithmMgr::Restart()
{
    for (auto &algorithm : m_algorithms)
        algorithm->Restart();
}
void AlgorithmMgr::Pause()
{
    for (auto &algorithm : m_algorithms)
        algorithm->Pause();
}
void AlgorithmMgr::Resume()
{
    for (auto &algorithm : m_algorithms)
        algorithm->Resume();
}
void AlgorithmMgr::Reset()
{
    for (auto &algorithm : m_algorithms)
        algorithm->Reset();
}

void AlgorithmMgr::Resize(size_t size)
{
    for (auto &algorithm : m_algorithms)
        algorithm->Resize(size);
}

void AlgorithmMgr::PopPushUntil(size_t size)
{
    for (auto &algorithm : m_algorithms)
    {
        algorithm->Reset();
        algorithm->PopPushUntil(size);
    }
}

void AlgorithmMgr::Shuffle()
{
    std::random_device rd;
    std::mt19937 generator(rd()); // Set a common random seed

    for (auto &algorithm : m_algorithms)
        algorithm->Shuffle(generator);
}

void AlgorithmMgr::SetSleepDelay(sf::Time delay)
{
    for (auto &algorithm : m_algorithms)
        algorithm->SetSleepDelay(delay);
}

void AlgorithmMgr::SetVisType(IAlgorithm::VisType visType)
{
    for (auto &algorithm : m_algorithms)
        algorithm->SetVisType(visType);
    m_visType = visType;
}

void AlgorithmMgr::GenerateDrawContainers()
{
    m_drawContainers.clear();

    auto winSize = Lib::ConvertTo<float>(Window::GetSize());
    sf::FloatRect container(-Camera::GetOffset(), sf::Vector2f(winSize.x - 200.0f, winSize.y));

    int nWidth, nHeight;
    int nActiveContainers = GetActiveContainers();

    switch (nActiveContainers)
    {
    case 1:
    {
        nWidth = 1;
        nHeight = 1;
        break;
    }
    case 2:
    {
        nWidth = 2;
        nHeight = 1;
        break;
    }
    case 3:
    {
        nWidth = 2;
        nHeight = 2;
        break;
    }
    case 4:
    {
        nWidth = 2;
        nHeight = 2;
        break;
    }
    case 5:
    {
        nWidth = 3;
        nHeight = 2;
        break;
    }
    case 6:
    {
        nWidth = 3;
        nHeight = 2;
        break;
    }
    case 7:
    {
        nWidth = 3;
        nHeight = 3;
        break;
    }
    case 8:
    {
        nWidth = 3;
        nHeight = 3;
        break;
    }
    case 9:
    {
        nWidth = 3;
        nHeight = 3;
        break;
    }
    default:
        break;
    }
    sf::Vector2f size(container.width / static_cast<float>(nWidth), container.height / static_cast<float>(nWidth));
    sf::Vector2f padding(15.0f, 40.0f);
    sf::Vector2f divide(container.width / nWidth, container.height / nHeight);
    for (int y = 0, nBoxes = 0; y < nHeight && nBoxes < nActiveContainers; y++)
    {
        float extraOffsetY = 0.0f;
        if (y == 0)
        {
            float difference = nWidth - nHeight;
            extraOffsetY = difference > 0 ? size.y * difference / 2.0f : 0.0f;
        }
        for (int x = 0; x < nWidth && nBoxes < nActiveContainers; x++, nBoxes++)
        {
            float extraOffsetX = 0.0f;
            if (y == nHeight - 1)
            {
                float extraSpace = (nWidth * nHeight) - nActiveContainers;
                extraOffsetX = extraSpace > 0 ? divide.x * extraSpace - (divide.x * extraSpace) / 2.0f : 0.0f;
            }
            m_drawContainers.emplace_back(sf::FloatRect(sf::Vector2f(container.left + x * divide.x + extraOffsetX, container.top + y * divide.y + extraOffsetY) + padding, size - padding * 2.0f));
        }
    }
}

int AlgorithmMgr::GetActiveContainers()
{
    return std::count_if(m_algorithms.begin(), m_algorithms.end(), [](auto &alg) { return alg->IsActive(); });
}