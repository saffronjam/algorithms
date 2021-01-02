#pragma once

#include <thread>
#include <string>
#include <vector>
#include <random>
#include <algorithm>

#include <SFML/System/Sleep.hpp>
#include <SFML/Graphics.hpp>

#include <Saffron.h>

#include "Element.h"

namespace Se
{
struct ContainerGroup;

class Algorithm
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
    Algorithm(String name);

    virtual ~Algorithm() = default;

    void Draw(Scene &scene, const sf::FloatRect &rect);

    void DrawName(Scene &scene, const sf::FloatRect &rect);

    void Activate() noexcept { _isActive = true; }

    void Deactivate() noexcept { _isActive = false; }

    bool IsActive() const noexcept { return _isActive; }

    void ActivateSpectrum() noexcept { _usingSpectrumColors = true; }

    void DeactivateSpectrum() noexcept { _usingSpectrumColors = false; }

    void Start();

    void Restart();

    void Pause();

    void Resume();

    void Reset();

    void Resize(size_t size);

    void SoftResize(size_t size);

    void SetImage(const std::string &filepath);

    void Shuffle(std::mt19937 generator);

    const String &GetName() const { return _name; }

    void SetSleepDelay(sf::Time delay) noexcept;

    void SetVisType(VisType visType) noexcept;

    ArrayList<Element> &GetElements();

    ArrayList<Element> &GetRestartElements();

    ArrayList<Element> &GetResetElements();

protected:

    virtual void Sort() = 0;

    Element &GetElement(size_t index) { return GetElements()[index]; }

    long GetValue(size_t index) { return GetElements()[index].value; }

    void SetValue(Element &element, long value);

    void SetValue(size_t index, long value) { SetValue(GetElement(index), value); }

    void SetColor(Element &element, const sf::Color &color);

    void SetColor(size_t index, const sf::Color &color) { SetColor(GetElement(index), color); }

    void SwapElements(Element &first, Element &second);

    void SwapElements(size_t iFirst, size_t iSecond) { SwapElements(GetElement(iFirst), GetElement(iSecond)); }

    void PauseCheck();

    void SleepDelay();

private:
    sf::Vector2u GetPixelCoord(size_t index) const;

    sf::FloatRect GetScaledPixel(size_t index, size_t max) const;

    sf::Vector2u GetClosestPixelCoord(size_t index, size_t max) const;

    // Calls overridden Sort() and later OnFinish() upon finish
    void SortThreadFn();

    void OnFinish();

    void CollectSorter();

    void DrawBars(Scene &scene, const sf::FloatRect &rect);

    void DrawCircles(Scene &scene, const sf::FloatRect &rect);

    void DrawHoops(Scene &scene, const sf::FloatRect &rect);

    void DrawLine(Scene &scene, const sf::FloatRect &rect);

    void DrawScatterPlot(Scene &scene, const sf::FloatRect &rect);

    void DrawImage(Scene &scene, const sf::FloatRect &rect);

    // Used as a wrapper to check if drawing in spectrum mode or not
    sf::Color GetElementColor(size_t index);

    bool VerifyElements();

protected:
    String _name;

    std::thread _sorter;

    sf::Image _image;
    sf::RenderTexture _imageRenderTexture;

    sf::Time _sleepDelay;
    bool _minorDelay;
    sf::Int64 _minorDelayTimer;

    State _state;
    bool _isActive;

    sf::Text _nameText;
    VisType _visType;
    bool _usingSpectrumColors;

private:
    ArrayList<Element> _elements;
    ArrayList<Element> _elementsRestart;
    ArrayList<Element> _elementsReset;

    // Shapes cache
    sf::VertexArray _barsVA;
    ArrayList<sf::CircleShape> _hoopsShapes;
};

}