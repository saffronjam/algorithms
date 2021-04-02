#include "AlgorithmManager.h"

namespace Se
{
AlgorithmManager::AlgorithmManager()
{
	Add(new BubbleSort());
	Add(new SelectionSort());
	Add(new InsertionSort());
	Add(new GnomeSort());
	Add(new ShellSort());
	Add(new MergeSort());
	Add(new HeapSort());
	Add(new QuickSort());
	Add(new RadixSort());

	_visTypeNames.push_back("Bars");
	_visTypeNames.push_back("Circles");
	_visTypeNames.push_back("Hoops");
	_visTypeNames.push_back("Line");
	_visTypeNames.push_back("Scatter Plot");
	_visTypeNames.push_back("Image");

	for (const auto* algorithm : _algorithms)
	{
		_algorithmNames.push_back(algorithm->GetName().c_str());
	}

	Resize(_elements);

	_gnomeSound = sf::Sound(*SoundBufferStore::Get("gnomed.wav"));
}

AlgorithmManager::~AlgorithmManager()
{
	Reset();
	for (auto& algorithm : _algorithms)
	{
		delete algorithm;
	}
	_algorithms.clear();
}

void AlgorithmManager::OnUpdate(const Scene& scene)
{
	if (_wantSoftResize)
	{
		SoftResize(_elements);
		_wantSoftResize = false;
	}

	if (_wantNewDrawContainers)
	{
		GenerateDrawContainers(scene);
		_wantNewDrawContainers = false;
	}
}

void AlgorithmManager::OnRender(Scene& scene)
{
	int i = 0;
	for (auto& algorithm : _algorithms)
	{
		if (i < _drawContainers.size() && algorithm->IsActive())
		{
			algorithm->Draw(scene, _drawContainers[i]);
			algorithm->DrawName(scene, _drawContainers[i]);
			i++;
		}
	}
}

void AlgorithmManager::OnGuiRender()
{
	ImGui::Columns(3, "BasicControls");
	if (ImGui::Button("Start", {ImGui::GetContentRegionAvailWidth(), 0}))
	{
		Start();
	}
	ImGui::NextColumn();
	if (ImGui::Button("Restart", {ImGui::GetContentRegionAvailWidth(), 0}))
	{
		Restart();
	}
	ImGui::NextColumn();
	if (ImGui::Button("Reset", {ImGui::GetContentRegionAvailWidth(), 0}))
	{
		Reset();
	}
	ImGui::NextColumn();
	ImGui::Columns(2, "PauseResume");
	if (ImGui::Button("Pause", {ImGui::GetContentRegionAvailWidth(), 0}))
	{
		Pause();
	}
	ImGui::NextColumn();
	if (ImGui::Button("Resume", {ImGui::GetContentRegionAvailWidth(), 0}))
	{
		Resume();
	}
	ImGui::Columns(1);

	ImGui::Separator();

	ImGui::Columns(1, "NormalShuffle");
	if (ImGui::Button("Shuffle", {ImGui::GetContentRegionAvailWidth(), 0}))
	{
		Shuffle();
	}
	ImGui::Columns(2, "CustomShuffle");
	if (ImGui::Button("Custom Shuffle", {ImGui::GetContentRegionAvailWidth(), 0}))
	{
		CustomShuffle(_customShuffleDegree);
	}
	ImGui::NextColumn();
	ImGui::PushItemWidth(-1);
	ImGui::SliderInt("##CustomShuffleSlider", &_customShuffleDegree, 1, 100, "%d%%");
	ImGui::NextColumn();
	ImGui::Columns(1);

	ImGui::Separator();

	Gui::BeginPropertyGrid("MainControllers");
	ImGui::Text("Visualization Type");
	ImGui::NextColumn();
	ImGui::PushItemWidth(-1);
	if (ImGui::Combo("##Visualization Type", &_activeVisTypeIndex, _visTypeNames.data(), _visTypeNames.size()))
	{
		SetVisType(static_cast<Algorithm::VisType>(_activeVisTypeIndex));
	}
	ImGui::NextColumn();


	if (Gui::Property("Sleep delay (microseconds)", _sleepDelayMicroseconds, 0.0f, 1000000.0f, 1.0f,
	                  Gui::PropertyFlag_Slider | Gui::PropertyFlag_Logarithmic))
	{
		SetSleepDelay(sf::microseconds(_sleepDelayMicroseconds));
	}

	if (Gui::Property("Elements", _elements, "%.0f", 1, 10000, 1,
	                  Gui::PropertyFlag_Slider | Gui::PropertyFlag_Logarithmic))
	{
		_wantSoftResize = true;
	}

	if (Gui::Property("Spectrum", _spectrum))
	{
		_spectrum ? ActivateSpectrum() : DeactivateSpectrum();
	}

	Gui::EndPropertyGrid();

	ImGui::Separator();

	ImGui::Text("Algorithms");
	Gui::BeginPropertyGrid("Checkboxes");

	Algorithm* lastActive = nullptr;
	for (auto* algorithm : _algorithms)
	{
		bool active = algorithm->IsActive();
		if (Gui::Property(algorithm->GetName(), active))
		{
			active ? Activate(algorithm) : Deactivate(algorithm);
		}

		if (algorithm->IsActive())
		{
			lastActive = algorithm;
		}
	}
	if (GetActiveContainers() == 1 && lastActive && lastActive->GetName() == "Gnome Sort")
	{
		if (!_gnomeActive)
		{
		}
	}

	Gui::EndPropertyGrid();
}

void AlgorithmManager::OnViewportResize(const sf::Vector2f& size)
{
	_wantNewDrawContainers = true;
}

void AlgorithmManager::Add(Algorithm* algorithm)
{
	_algorithms.emplace_back(algorithm);
	_algorithms.back()->Activate();
}

void AlgorithmManager::Activate(Algorithm* algorithm)
{
	algorithm->Activate();
	_wantNewDrawContainers = true;

	OnAlgorithmStateChange();
}

void AlgorithmManager::Deactivate(Algorithm* algorithm)
{
	algorithm->Deactivate();
	_wantNewDrawContainers = true;

	OnAlgorithmStateChange();
}

void AlgorithmManager::ActivateSpectrum() noexcept
{
	for (auto& algorithm : _algorithms)
	{
		algorithm->ActivateSpectrum();
	}
}

void AlgorithmManager::DeactivateSpectrum() noexcept
{
	for (auto& algorithm : _algorithms)
	{
		algorithm->DeactivateSpectrum();
	}
}

void AlgorithmManager::Start()
{
	for (auto& algorithm : _algorithms)
	{
		algorithm->Start();
	}
}

void AlgorithmManager::Restart()
{
	for (auto& algorithm : _algorithms)
	{
		algorithm->Restart();
	}
}

void AlgorithmManager::Pause()
{
	for (auto& algorithm : _algorithms)
	{
		algorithm->Pause();
	}
}

void AlgorithmManager::Resume()
{
	for (auto& algorithm : _algorithms)
	{
		algorithm->Resume();
	}
}

void AlgorithmManager::Reset()
{
	for (auto& algorithm : _algorithms)
	{
		algorithm->Reset();
	}
}

void AlgorithmManager::Resize(size_t size)
{
	for (auto& algorithm : _algorithms)
	{
		algorithm->Reset();
		algorithm->Resize(size);
	}
}

void AlgorithmManager::SoftResize(size_t size)
{
	for (auto& algorithm : _algorithms)
	{
		algorithm->Reset();
		algorithm->SoftResize(size);
	}
}

void AlgorithmManager::Shuffle()
{
	Random::Device rd;
	const Random::Engine generator(rd()); // Set a common random seed

	for (auto& algorithm : _algorithms)
	{
		algorithm->Shuffle(generator);
	}
}

void AlgorithmManager::CustomShuffle(int degree)
{
	_algorithms.front()->Reset();
	ArrayList<Element>& newElements = _algorithms.front()->GetRestartElements();

	const auto degreePecentage = static_cast<float>(degree) / 100.0f;
	const int noElements = newElements.size();

	for (int i = 0; i < newElements.size(); i++)
	{
		// 10 iterations of swapping
		for (int j = 0; j < 10; j++)
		{
			const int newIndex = i + Random::Integer(static_cast<int>(-degreePecentage * noElements),
			                                         static_cast<int>(degreePecentage * noElements));
			if (newIndex >= 0 && newIndex < noElements)
			{
				std::swap(newElements[i], newElements[newIndex]);
			}
		}
	}

	_algorithms.front()->GetElements() = newElements;

	for (int i = 1; i < _algorithms.size(); i++)
	{
		_algorithms[i]->Reset();
		_algorithms[i]->GetRestartElements() = newElements;
		_algorithms[i]->GetElements() = newElements;
	}
}

void AlgorithmManager::SetSleepDelay(sf::Time delay)
{
	for (auto& algorithm : _algorithms) algorithm->SetSleepDelay(delay);
}

void AlgorithmManager::SetVisType(Algorithm::VisType visType)
{
	if (visType == _visType)
	{
		return;
	}

	for (auto& algorithm : _algorithms)
	{
		algorithm->SetVisType(visType);
	}
	_visType = visType;
	OnAlgorithmStateChange();
}

void AlgorithmManager::GenerateDrawContainers(const Scene& scene)
{
	_drawContainers.clear();

	const sf::FloatRect container(-scene.GetCamera().GetOffset(), scene.GetViewportPane().GetViewportSize());

	int nWidth, nHeight;
	const int nActiveContainers = GetActiveContainers();

	switch (nActiveContainers)
	{
	case 1:
	{
		nWidth = 1;
		nHeight = 1;
		break;
	}
	case 2:
	{
		nWidth = 2;
		nHeight = 1;
		break;
	}
	case 3:
	case 4:
	{
		nWidth = 2;
		nHeight = 2;
		break;
	}
	case 5:
	case 6:
	{
		nWidth = 3;
		nHeight = 2;
		break;
	}
	case 7:
	case 8:
	case 9:
	{
		nWidth = 3;
		nHeight = 3;
		break;
	}
	default: SE_CORE_FALSE_ASSERT("Invalid container group amount");
		nWidth = 0;
		nHeight = 0;
		break;
	}
	const sf::Vector2f size(container.width / static_cast<float>(nWidth),
	                        container.height / static_cast<float>(nWidth));
	const sf::Vector2f padding(15.0f, 40.0f);
	const sf::Vector2f divide(container.width / nWidth, container.height / nHeight);
	for (int y = 0, nBoxes = 0; y < nHeight && nBoxes < nActiveContainers; y++)
	{
		float extraOffsetY = 0.0f;
		if (y == 0)
		{
			const float difference = nWidth - nHeight;
			extraOffsetY = difference > 0 ? size.y * difference / 2.0f : 0.0f;
		}
		for (int x = 0; x < nWidth && nBoxes < nActiveContainers; x++, nBoxes++)
		{
			float extraOffsetX = 0.0f;
			if (y == nHeight - 1)
			{
				const float extraSpace = (nWidth * nHeight) - nActiveContainers;
				extraOffsetX = extraSpace > 0 ? divide.x * extraSpace - (divide.x * extraSpace) / 2.0f : 0.0f;
			}
			_drawContainers.emplace_back(sf::FloatRect(sf::Vector2f(container.left + x * divide.x + extraOffsetX,
			                                                        container.top + y * divide.y + extraOffsetY) +
			                                           padding, size - padding * 2.0f));
		}
	}
}

int AlgorithmManager::GetActiveContainers()
{
	return std::count_if(_algorithms.begin(), _algorithms.end(), [](auto& alg)
	{
		return alg->IsActive();
	});
}

void AlgorithmManager::OnAlgorithmStateChange()
{
	if (GetActiveContainers() == 1 && _visType == Algorithm::VisType::Image)
	{
		for (auto* algorithm : _algorithms)
		{
			if (algorithm->IsActive() && algorithm->GetName() == "Gnome Sort")
			{
				algorithm->SetImage("res/Images/gnomed.png");
				_gnomeSound.play();
				_gnomeActive = true;
				break;
			}
		}
	}
	else if (_gnomeActive)
	{
		_gnomeSound.stop();
		for (auto* algorithm : _algorithms)
		{
			if (algorithm->GetName() == "Gnome Sort")
			{
				algorithm->SetImage("res/Images/sample_image_forest.jpg");
				break;
			}
		}
		_gnomeActive = false;
	}
}
}
