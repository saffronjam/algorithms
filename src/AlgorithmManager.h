#pragma once

#include <SFML/Audio/Sound.hpp>

#include <Saffron.h>

// ----- Algorithms -----
#include "Algorithms/BubbleSort.h"
#include "Algorithms/SelectionSort.h"
#include "Algorithms/InsertionSort.h"
#include "Algorithms/ShellSort.h"
#include "Algorithms/MergeSort.h"
#include "Algorithms/HeapSort.h"
#include "Algorithms/QuickSort.h"
#include "Algorithms/GnomeSort.h"
#include "Algorithms/RadixSort.h"
// ----------------------

namespace Se
{
class AlgorithmManager
{
public:
	AlgorithmManager();
	~AlgorithmManager();
	AlgorithmManager(const AlgorithmManager&) = delete;
	const AlgorithmManager& operator()(const AlgorithmManager&) = delete;

	void OnUpdate(const Scene& scene);
	void OnRender(Scene& scene);
	void OnGuiRender();
	void OnViewportResize(const sf::Vector2f& size);

	void Add(Unique<Algorithm> algorithm);

	void Activate(const Unique<Algorithm>& algorithm);
	void Deactivate(const Unique<Algorithm>& algorithm);

	void UsePalette(bool use);

	void Start();
	void Restart();
	void Pause();
	void Resume();
	void Reset();

	void Resize(size_t size);
	void SoftResize(size_t size);

	void Shuffle();
	void CustomShuffle(int degree);

	auto GetVisType() const -> Algorithm::VisType;

	void SetSleepDelay(sf::Time delay);
	void SetVisType(Algorithm::VisType visType);
	void SetPalette(Algorithm::Palette palette);
	void SetNumberGeneratorType(Algorithm::NumberGeneratorType numberGeneratorType);

	auto Algorithms() const -> const List<Unique<Algorithm>>&;

private:
	void GenerateDrawContainers(const Scene& scene);
	auto ActiveContainers() -> int;
	auto CurrentPaletteTexture() -> const sf::Texture&;

	void OnAlgorithmStateChange();

private:
	List<Unique<Algorithm>> _algorithms;
	List<sf::FloatRect> _drawContainers;

	// A cache used for getter
	Algorithm::VisType _visType;

	bool _wantNewDrawContainers = true;
	bool _wantSoftResize = true;

	// Gui cache
	float _elements = 100;
	float _sleepDelayMicroseconds = 10000.0f;
	bool _usePalette = false;
	int _activeVisTypeIndex = static_cast<int>(Algorithm::VisType::Bars);
	List<const char*> _visTypeNames;
	List<const char*> _algorithmNames;
	int _customShuffleDegree = 10;

	List<const char*> _paletteComboBoxNames;
	int _activePaletteInt = static_cast<int>(Algorithm::Palette::Rainbow);

	List<const char*> _numberGeneratorTypeComboBoxNames;
	int _numberGeneratorTypeInt = static_cast<int>(Algorithm::NumberGeneratorType::Linear);

	bool _gnomeActive = false;
	Unique<sf::Sound> _gnomeSound;
};
}
