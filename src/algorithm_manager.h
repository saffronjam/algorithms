#pragma once

#include <memory>

#include <SFML/Audio/Sound.hpp>

#include <saffron.h>

// ----- Algorithms -----
#include "algorithms/bubble_sort.h"
#include "algorithms/selection_sort.h"
#include "algorithms/insertion_sort.h"
#include "algorithms/shell_sort.h"
#include "algorithms/merge_sort.h"
#include "algorithms/heap_sort.h"
#include "algorithms/quick_sort.h"
#include "algorithms/gnome_sort.h"
#include "algorithms/radix_sort.h"
// ----------------------

namespace algorithms
{
using namespace saffron;
class AlgorithmManager
{
public:
	AlgorithmManager();
	~AlgorithmManager();
	AlgorithmManager(const AlgorithmManager&) = delete;
	auto operator=(const AlgorithmManager&) -> AlgorithmManager& = delete;

	void OnUpdate(const Scene& scene);
	void OnRender(Scene& scene);
	void OnGuiRender();
	void OnViewportResize(const sf::Vector2f& size);

	void Add(std::unique_ptr<Algorithm> algorithm);

	void Activate(const std::unique_ptr<Algorithm>& algorithm);
	void Deactivate(const std::unique_ptr<Algorithm>& algorithm);

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

	auto Algorithms() const -> const std::vector<std::unique_ptr<Algorithm>>&;

private:
	void GenerateDrawContainers(const Scene& scene);
	auto ActiveContainers() -> int;
	auto CurrentPaletteTexture() -> const sf::Texture&;

	void OnAlgorithmStateChange();

private:
	std::vector<std::unique_ptr<Algorithm>> _algorithms;
	std::vector<sf::FloatRect> _drawContainers;

	// A cache used for getter
	Algorithm::VisType _visType;

	bool _wantNewDrawContainers = true;
	bool _wantSoftResize = true;

	// Gui cache
	float _elements = 100;
	float _sleepDelayMicroseconds = 10000.0f;
	bool _usePalette = false;
	int _activeVisTypeIndex = static_cast<int>(Algorithm::VisType::Bars);
	std::vector<const char*> _visTypeNames;
	std::vector<const char*> _algorithmNames;
	int _customShuffleDegree = 10;

	std::vector<const char*> _paletteComboBoxNames;
	int _activePaletteInt = static_cast<int>(Algorithm::Palette::Rainbow);

	std::vector<const char*> _numberGeneratorTypeComboBoxNames;
	int _numberGeneratorTypeInt = static_cast<int>(Algorithm::NumberGeneratorType::Linear);

	bool _gnomeActive = false;
	std::shared_ptr<sf::Sound> _gnomeSound;
};
}
