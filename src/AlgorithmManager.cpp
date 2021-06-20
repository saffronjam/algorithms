#include "AlgorithmManager.h"

namespace Se
{
AlgorithmManager::AlgorithmManager() :
	_visTypeNames({"Bars", "Number Line", "Circles", "Hoops", "Line", "Scatter Plot", "Image"}),
	_paletteComboBoxNames({"Rainbow", "Fiery", "UV"}),
	_numberGeneratorTypeComboBoxNames({"Linear", "Quadratic", "Random"})
{
	Add(CreateUnique<BubbleSort>());
	Add(CreateUnique<SelectionSort>());
	Add(CreateUnique<InsertionSort>());
	Add(CreateUnique<GnomeSort>());
	Add(CreateUnique<ShellSort>());
	Add(CreateUnique<MergeSort>());
	Add(CreateUnique<HeapSort>());
	Add(CreateUnique<QuickSort>());
	Add(CreateUnique<RadixSort>());

	for (const auto& algorithm : _algorithms)
	{
		_algorithmNames.push_back(algorithm->GetName().c_str());
	}

	Resize(_elements);

	_gnomeSound = CreateUnique<sf::Sound>(*SoundBufferStore::Get("gnomed.wav", false));
}

AlgorithmManager::~AlgorithmManager()
{
	Reset();
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

	for (auto& algoritm : _algorithms)
	{
		algoritm->OnUpdate();
	}
}

void AlgorithmManager::OnRender(Scene& scene)
{
	int i = 0;
	for (auto& algorithm : _algorithms)
	{
		if (i < _drawContainers.size() && algorithm->Active())
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
	                  GuiPropertyFlag_Slider | GuiPropertyFlag_Logarithmic))
	{
		SetSleepDelay(sf::microseconds(_sleepDelayMicroseconds));
	}

	Gui::EndPropertyGrid();

	ImGui::Separator();

	Gui::BeginPropertyGrid("Elements");


	if (Gui::Property("Elements", _elements, "%.0f", 1, 10000, 1,
	                  GuiPropertyFlag_Slider | GuiPropertyFlag_Logarithmic))
	{
		_wantSoftResize = true;
	}

	ImGui::Text("Generator");
	ImGui::NextColumn();
	ImGui::PushItemWidth(-1);
	if (ImGui::Combo("##Generator", &_numberGeneratorTypeInt, _numberGeneratorTypeComboBoxNames.data(),
	                 _numberGeneratorTypeComboBoxNames.size()))
	{
		SetNumberGeneratorType(static_cast<Algorithm::NumberGeneratorType>(_numberGeneratorTypeInt));
	}
	ImGui::NextColumn();

	if (_numberGeneratorTypeInt == static_cast<int>(Algorithm::NumberGeneratorType::Random))
	{
		Gui::Property("Generate", [this]()
		{
			Resize(_elements);
		}, true);
	}

	Gui::EndPropertyGrid();

	ImGui::Separator();

	Gui::BeginPropertyGrid("Palette");

	if (Gui::Property("Use Palette", _usePalette))
	{
		UsePalette(_usePalette);
	}
	ImGui::Text("Palette");
	ImGui::NextColumn();
	ImGui::PushItemWidth(-1);
	if (ImGui::Combo("##Palette", &_activePaletteInt, _paletteComboBoxNames.data(), _paletteComboBoxNames.size()))
	{
		SetPalette(static_cast<Algorithm::Palette>(_activePaletteInt));
	}
	ImGui::NextColumn();
	Gui::EndPropertyGrid();
	ImGui::Dummy({1.0f, 2.0f});


	Gui::Image(CurrentPaletteTexture(), sf::Vector2f(ImGui::GetContentRegionAvailWidth(), 9.0f));

	ImGui::Separator();

	ImGui::Text("Algorithms");
	Gui::BeginPropertyGrid("Checkboxes");

	for (auto& algorithm : _algorithms)
	{
		bool active = algorithm->Active();
		if (Gui::Property(algorithm->GetName(), active))
		{
			active ? Activate(algorithm) : Deactivate(algorithm);
		}
	}

	Gui::EndPropertyGrid();
}

void AlgorithmManager::OnViewportResize(const sf::Vector2f& size)
{
	_wantNewDrawContainers = true;
}

void AlgorithmManager::Add(Unique<Algorithm> algorithm)
{
	_algorithms.emplace_back(Move(algorithm));
	_algorithms.back()->Activate();
}

void AlgorithmManager::Activate(const Unique<Algorithm>& algorithm)
{
	algorithm->Activate();
	_wantNewDrawContainers = true;

	OnAlgorithmStateChange();
}

void AlgorithmManager::Deactivate(const Unique<Algorithm>& algorithm)
{
	algorithm->Deactivate();
	_wantNewDrawContainers = true;

	OnAlgorithmStateChange();
}

void AlgorithmManager::UsePalette(bool use)
{
	for (auto& algorithm : _algorithms)
	{
		algorithm->UsePalette(use);
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
	List<Element>& newElements = _algorithms.front()->RestartElements();

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

	_algorithms.front()->Elements() = newElements;

	for (int i = 1; i < _algorithms.size(); i++)
	{
		_algorithms[i]->Reset();
		_algorithms[i]->RestartElements() = newElements;
		_algorithms[i]->Elements() = newElements;
	}
}

auto AlgorithmManager::GetVisType() const -> Algorithm::VisType { return _visType; }

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

void AlgorithmManager::SetPalette(Algorithm::Palette palette)
{
	for (auto& algorithm : _algorithms)
	{
		algorithm->SetPalette(palette);
	}
}

void AlgorithmManager::SetNumberGeneratorType(Algorithm::NumberGeneratorType numberGeneratorType)
{
	for (auto& algorithm : _algorithms)
	{
		algorithm->Reset();
		algorithm->SetNumberGeneratorType(numberGeneratorType);
		algorithm->Resize(_elements);
	}
}

auto AlgorithmManager::Algorithms() const -> const List<Unique<Algorithm>>& { return _algorithms; }

void AlgorithmManager::GenerateDrawContainers(const Scene& scene)
{
	_drawContainers.clear();

	const sf::FloatRect container(-scene.Camera().Offset(), scene.ViewportPane().ViewportSize());

	int nWidth, nHeight;
	const int nActiveContainers = ActiveContainers();

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
	default:
	{
		nWidth = 0;
		nHeight = 0;
		break;
	}
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

int AlgorithmManager::ActiveContainers()
{
	return std::count_if(_algorithms.begin(), _algorithms.end(), [](auto& alg)
	{
		return alg->Active();
	});
}

const sf::Texture& AlgorithmManager::CurrentPaletteTexture()
{
	const auto image = _algorithms[0]->GetCurrentPaletteImage();
	static sf::Texture texture;
	texture.loadFromImage(image);
	return texture;
}

void AlgorithmManager::OnAlgorithmStateChange()
{
	if (ActiveContainers() == 1 && _visType == Algorithm::VisType::Image)
	{
		for (auto& algorithm : _algorithms)
		{
			if (algorithm->Active() && algorithm->GetName() == "Gnome Sort")
			{
				algorithm->SetImage("res/Images/gnomed.png");
				_gnomeSound->play();
				_gnomeActive = true;
				break;
			}
		}
	}
	else if (_gnomeActive)
	{
		_gnomeSound->stop();
		for (auto& algorithm : _algorithms)
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
