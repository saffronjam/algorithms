#pragma once

#include <thread>
#include <string>
#include <vector>
#include <random>
#include <algorithm>

#include <SFML/System/Sleep.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>

#include "Camera.h"
#include "Clock.h"
#include "Bar.h"
#include "FontMgr.h"

class IAlgorithm
{
public:
    enum class State
    {
        Sorting,
        Paused,
        Finished,
        BeingCollected
    };

public:
    IAlgorithm();

    void Draw(const sf::FloatRect &rect);
    void DrawName(const sf::FloatRect &rect);

    void Activate() noexcept { m_isActive = true; }
    void Deactivate() noexcept { m_isActive = false; }
    bool IsActive() const noexcept { return m_isActive; }

    void Start();
    void Pause();
    void Resume();
    void Reset();

    void Resize(size_t size);
    void Shuffle(std::mt19937 generator);

    virtual std::string GetName() = 0;
    void SetSleepTime(sf::Time seconds) noexcept { m_sleepTime = seconds; }

protected:
    virtual void Sort() = 0;

    void PauseCheck();
    void SleepDelay();

private:
    // Calls overridden Sort() and later OnFinish() upon finish
    void SortThreadFn();
    void OnFinish();

protected:
    std::vector<Bar> m_bars;
    std::vector<Bar> m_barsCopy;
    std::thread m_sorter;

    sf::Time m_sleepTime;
    State m_state;
    bool m_isActive;

    sf::Text m_name;
};