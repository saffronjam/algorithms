#pragma once

#include <chrono>
#include <thread>
#include <string>

#include <SFML/System/Time.hpp>

#include "BarContainer.hpp"

using BarIter = std::vector<std::shared_ptr<BarContainer::Bar>>::iterator;

class Sort
{
public:
    Sort(std::string const &sortName);

    virtual void WorkerUpdate(sf::Time const &dt);

    void SetBarContainer(std::vector<std::shared_ptr<BarContainer::Bar>> *const barContainer) { m_allBars = barContainer; }
    void SetTimeStep(float const &timeStep) { m_stepTime = timeStep; }
    void SetIsActive(bool const &isActive) { m_isActive = isActive; }
    void SetIsPaused(bool const &isPaused) { m_isPaused = isPaused; }
    void SetIsFinished(bool const &isFinished) { m_isFinished = isFinished; }
    void SetIsBeingCollected(bool const &isBeingCollected) { m_isBeingCollected = isBeingCollected; }

    bool GetIsActive() { return m_isActive; }
    bool GetIsPaused() { return m_isPaused; }
    std::string GetName() { return m_sortName; }

    bool &GetIsActiveRef() { return m_isActive; }

protected:
    virtual void Step() = 0;
    void ContinueIfNotPaused();
    void SleepDelay();

protected:
    std::vector<std::shared_ptr<BarContainer::Bar>> *m_allBars;

    int m_stepTime;
    float m_currentWait;

    bool m_isActive;
    bool m_isPaused;
    bool m_isFinished;
    bool m_isBeingCollected;

    std::string m_sortName;
};