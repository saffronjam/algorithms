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
#include "Element.h"
#include "FontMgr.h"

class IAlgorithm
{
public:
    enum class VisType
    {
        Bars,
        Circles,
        Hoops
    };

    enum class State
    {
        Sorting,
        WaitingForStart,
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

    void ActivateSpectrum() noexcept { m_usingSpectrumColors = true; }
    void DeactivateSpectrum() noexcept { m_usingSpectrumColors = false; }

    void Start();
    void Restart();
    void Pause();
    void Resume();
    void Reset();

    void Resize(size_t size);
    void PopPushUntil(size_t size);

    void Shuffle(std::mt19937 generator);

    virtual std::string GetName() = 0;
    void SetSleepDelay(sf::Time delay) noexcept { m_sleepDelay = delay; }
    void SetVisType(VisType visType) noexcept { m_visType = visType; }

protected:
    virtual void Sort() = 0;

    void PauseCheck();
    void SleepDelay();

private:
    // Calls overridden Sort() and later OnFinish() upon finish
    void SortThreadFn();
    void OnFinish();
    void CollectSorter();

    void DrawBars(const sf::FloatRect &rect);
    void DrawCircles(const sf::FloatRect &rect);
    void DrawHoops(const sf::FloatRect &rect);

    // Used as a wrapper to check if drawing in spectrum mode or not
    sf::Color GetElementColor(size_t index);

protected:
    std::vector<Element> m_elements;
    std::vector<Element> m_elementsRestart;
    std::vector<Element> m_elementsReset;
    std::thread m_sorter;

    sf::Time m_sleepDelay;
    State m_state;
    bool m_isActive;

    sf::Text m_name;
    VisType m_visType;
    bool m_usingSpectrumColors;
};