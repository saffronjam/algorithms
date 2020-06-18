#pragma once

#include <thread>
#include <string>
#include <vector>
#include <random>
#include <algorithm>

#include <SFML/System/Sleep.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "Camera.h"
#include "Clock.h"
#include "Element.h"
#include "FontMgr.h"

struct ContainerGroup;

class IAlgorithm
{
public:
    enum class VisType
    {
        Bars,
        Circles,
        Hoops,
        Line,
        ScatterPlot,
        Image
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
    void SetImage(const std::string &filepath);

    void Shuffle(std::mt19937 generator);

    virtual std::string GetName() = 0;
    void SetSleepDelay(sf::Time delay) noexcept;
    void SetVisType(VisType visType) noexcept;

protected:
    std::vector<Element> &GetContainer();
    virtual void Sort() = 0;

    Element &GetElement(size_t index) { return GetContainer()[index]; }
    long GetValue(size_t index) { return GetContainer()[index].value; }

    void SetValue(Element &element, long value);
    void SetValue(size_t index, long value) { SetValue(GetElement(index), value); }
    void SetColor(Element &element, const sf::Color &color);
    void SetColor(size_t index, const sf::Color &color) { SetColor(GetElement(index), color); }
    void SwapElements(Element &first, Element &second);
    void SwapElements(size_t iFirst, size_t iSecond) { SwapElements(GetElement(iFirst), GetElement(iSecond)); }

    void PauseCheck();
    void SleepDelay();

private:
    ContainerGroup GetActiveContainerGroup();

    sf::Vector2u GetPixelCoord(size_t index);

    // Calls overridden Sort() and later OnFinish() upon finish
    void SortThreadFn();
    void OnFinish();
    void CollectSorter();

    void DrawBars(const sf::FloatRect &rect);
    void DrawCircles(const sf::FloatRect &rect);
    void DrawHoops(const sf::FloatRect &rect);
    void DrawLine(const sf::FloatRect &rect);
    void DrawScatterPlot(const sf::FloatRect &rect);
    void DrawImage(const sf::FloatRect &rect);

    // Used as a wrapper to check if drawing in spectrum mode or not
    sf::Color GetElementColor(size_t index);

protected:
    std::thread m_sorter;

    sf::Image m_image;
    sf::RenderTexture m_imageRenderTexture;

    sf::Time m_sleepDelay;
    bool m_minorDelay;
    sf::Int64 m_minorDelayTimer;

    State m_state;
    bool m_isActive;

    sf::Text m_name;
    VisType m_visType;
    bool m_usingSpectrumColors;

private:
    std::vector<Element> m_elements;
    std::vector<Element> m_elementsRestart;
    std::vector<Element> m_elementsReset;
    std::vector<Element> m_imageElements;
    std::vector<Element> m_imageElementsRestart;
    std::vector<Element> m_imageElementsReset;
};