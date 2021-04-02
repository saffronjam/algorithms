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

	void ActivateSpectrum() noexcept;
	void DeactivateSpectrum() noexcept;

	void Start();
	void Restart();
	void Pause();
	void Resume();
	void Reset();

	void Resize(size_t size);
	void SoftResize(size_t size);

	void Shuffle();
	void CustomShuffle(int degree);

	Algorithm::VisType GetVisType() const noexcept { return _visType; }

	void SetSleepDelay(sf::Time delay);
	void SetVisType(Algorithm::VisType visType);

	const ArrayList<Unique<Algorithm>>& GetAlgorithms() const noexcept { return _algorithms; }

private:
	void GenerateDrawContainers(const Scene& scene);
	int GetActiveContainers();

	void OnAlgorithmStateChange();

private:
	ArrayList<Unique<Algorithm>> _algorithms;
	ArrayList<sf::FloatRect> _drawContainers;

	// A cache used for getter
	Algorithm::VisType _visType;

	bool _wantNewDrawContainers = true;
	bool _wantSoftResize = true;

	// Gui cache
	float _elements = 100;
	float _sleepDelayMicroseconds = 10000.0f;
	bool _spectrum = false;
	int _activeVisTypeIndex = static_cast<int>(Algorithm::VisType::Bars);
	ArrayList<const char*> _visTypeNames;
	ArrayList<const char*> _algorithmNames;
	int _customShuffleDegree = 10;

	bool _gnomeActive = false;
	sf::Sound _gnomeSound;
};
}
