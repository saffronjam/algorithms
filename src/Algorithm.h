#pragma once

#include <SFML/Graphics.hpp>

#include <Saffron.h>

#include "Element.h"

namespace Se
{
struct ContainerRefGroup;

class Algorithm
{
public:
	enum class VisType
	{
		Bars,
		NumberLine,
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

	enum class Palette
	{
		Rainbow,
		Fiery,
		UV
	};

	enum class NumberGeneratorType
	{
		Linear,
		Quadratic,
		Random
	};

private:
	struct TransitionColor
	{
		float r;
		float g;
		float b;
		float a;
	};

public:
	explicit Algorithm(String name);

	virtual ~Algorithm() = default;

	void OnUpdate();

	void Draw(Scene& scene, const sf::FloatRect& rect);
	void DrawName(Scene& scene, const sf::FloatRect& rect);

	void Activate();
	void Deactivate();
	bool IsActive() const;

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

	void SetSleepDelay(sf::Time delay);
	void SetVisType(VisType visType);
	void SetNumberGeneratorType(NumberGeneratorType numberGeneratorType);

	void UsePalette(bool use);
	void SetPalette(Palette palette);
	const sf::Image& GetCurrentPaletteImage();

	ArrayList<Element>& GetElements();
	ArrayList<Element>& GetRestartElements();
	ArrayList<Element>& GetResetElements();

protected:
	virtual void Sort() = 0;

	Element& GetElement(size_t index);

	long GetValue(size_t index);

	void SetValue(Element& element, long value);
	void SetValue(size_t index, long value);
	void SetColor(Element& element, const sf::Color& color);
	void SetColor(size_t index, const sf::Color& color);

	void SwapElements(Element& first, Element& second);
	void SwapElements(size_t iFirst, size_t iSecond);

	void PauseCheck();
	void SleepDelay();

private:
	Function<long(size_t)> GetGenerator();
	long GetHighestElementValue();

	sf::Vector2u GetPixelCoord(size_t index) const;
	sf::FloatRect GetScaledPixel(size_t index, size_t max) const;
	sf::Vector2u GetClosestPixelCoord(size_t index, size_t max) const;

	// Calls overridden Sort() and later OnFinish() upon finish
	void SortThreadFn();
	void OnFinish();
	void CollectSorter();

	void DrawBars(Scene& scene, const sf::FloatRect& rect);
	void DrawNumberLine(Scene& scene, const sf::FloatRect& rect);
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

private:
	static constexpr int MaxElements = 10000;
	static constexpr int PaletteWidth = 2048;

	ArrayList<Element> _elements;
	ArrayList<Element> _elementsRestart;
	ArrayList<Element> _elementsReset;

	NumberGeneratorType _numberGeneratorType = NumberGeneratorType::Linear;

	// Shapes cache
	sf::VertexArray _barsVA;
	ArrayList<sf::CircleShape> _hoopsShapes;
	sf::VertexArray _numberLineVA;
	ArrayList<sf::Text> _numberLineTextList;

	// Palette
	sf::Texture _paletteTexture;
	Palette _desiredPalette = Palette::Rainbow;
	sf::Image _currentPalette;
	Array<TransitionColor, PaletteWidth> _colorsStart;
	Array<TransitionColor, PaletteWidth> _colorsCurrent;
	float _colorTransitionTimer;
	float _colorTransitionDuration;
	ArrayList<Shared<sf::Image>> _palettes;
	bool _usePalette = false;
};
}
