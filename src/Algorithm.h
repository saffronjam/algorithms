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
	explicit Algorithm(String name);

	virtual ~Algorithm() = default;

	void Draw(Scene& scene, const sf::FloatRect& rect);
	void DrawName(Scene& scene, const sf::FloatRect& rect);

	void Activate() noexcept;
	void Deactivate() noexcept;
	bool IsActive() const noexcept;

	void ActivateSpectrum() noexcept;
	void DeactivateSpectrum() noexcept;

	void Start();
	void Restart();
	void Pause();
	void Resume();
	void Reset();

	void Resize(size_t size);
	void SoftResize(size_t size);

	void SetImage(const String& filepath);

	void Shuffle(Random::Engine generator);

	const String& GetName() const { return _name; }

	void SetSleepDelay(sf::Time delay) noexcept;
	void SetVisType(VisType visType) noexcept;

	ArrayList<Element>& GetElements();
	ArrayList<Element>& GetRestartElements();
	ArrayList<Element>& GetResetElements();

protected:
	virtual void Sort() = 0;

	Element& GetElement(size_t index) { return GetElements()[index]; }

	long GetValue(size_t index) { return GetElements()[index].value; }

	void SetValue(Element& element, long value);

	void SetValue(size_t index, long value) { SetValue(GetElement(index), value); }

	void SetColor(Element& element, const sf::Color& color);

	void SetColor(size_t index, const sf::Color& color) { SetColor(GetElement(index), color); }

	void SwapElements(Element& first, Element& second);

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

	void DrawBars(Scene& scene, const sf::FloatRect& rect);
	void DrawCircles(Scene& scene, const sf::FloatRect& rect);
	void DrawHoops(Scene& scene, const sf::FloatRect& rect);
	void DrawLine(Scene& scene, const sf::FloatRect& rect);
	void DrawScatterPlot(Scene& scene, const sf::FloatRect& rect);
	void DrawImage(Scene& scene, const sf::FloatRect& rect);

	// Used as a wrapper to check if drawing in spectrum mode or not
	sf::Color GetElementColor(size_t index);

	bool VerifyElements();

protected:
	String _name;

	Thread _sorter;

	Shared<sf::Image> _image;
	sf::RenderTexture _imageRenderTexture;

	sf::Time _sleepDelay;
	bool _minorDelay;
	sf::Int64 _minorDelayTimer;

	State _state;
	bool _isActive;

	Shared<sf::Font> _nameTextFont;
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

	static constexpr int MaxElements = 10000;
};
}
