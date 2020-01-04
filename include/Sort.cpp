#include "Sort.hpp"

Sort::Sort(std::string const &sortName)
    : m_stepTime(1000),
      m_currentWait(0.0f),
      m_isActive(true),
      m_isPaused(true),
      m_isFinished(false),
      m_isBeingCollected(false),
      m_sortName(sortName)
{
}

void Sort::WorkerUpdate(sf::Time const &dt)
{
  while (!m_isBeingCollected)
  {
    if (!m_isPaused && !m_isFinished)
    {
      Step();
      if (!m_isBeingCollected)
      {
        m_isPaused = true;
        m_isFinished = true;
        for (auto &bar : *m_allBars)
        {
          bar->SetColor(sf::Color::Green);
        }
      }
    }
  }
}

void Sort::ContinueIfNotPaused()
{
  while (m_isPaused)
  {
    SleepDelay();
  };
}

void Sort::SleepDelay()
{
  std::this_thread::sleep_for(std::chrono::microseconds(m_stepTime));
}