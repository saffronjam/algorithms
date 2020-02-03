#include "Application.hpp"
#include <stdlib.h>
#include <time.h>

Application::Application(sf::Time &dt)
	: m_gfx(m_camera),
	  m_dt(dt),
	  m_iu(m_gfx, m_camera),
	  m_ui(m_gfx, m_iu),
	  m_camera(m_gfx, m_camera_follow, m_camera_controller),
	  m_camera_controller(m_gfx, m_camera, m_iu),
	  m_camera_follow(0.0f, 0.0f),
	  m_sortingController(m_gfx, SortingController::SortingAlgorithm::ALG_MergeSort, m_dt)
{
	srand(time(NULL));
	m_ui.CreateStaticRadioButtonSet(0,
									{m_sortingController.GetSortingNumberPtr(SortingController::SortingNumber::NUM_10),
									 m_sortingController.GetSortingNumberPtr(SortingController::SortingNumber::NUM_100),
									 m_sortingController.GetSortingNumberPtr(SortingController::SortingNumber::NUM_1000),
									 m_sortingController.GetSortingNumberPtr(SortingController::SortingNumber::NUM_10000)},
									{false, true, false, false},
									{sf::Vector2f(Graphics::ScreenWidth - 700, 10.0f),
									 sf::Vector2f(Graphics::ScreenWidth - 680, 10.0f),
									 sf::Vector2f(Graphics::ScreenWidth - 660, 10.0f),
									 sf::Vector2f(Graphics::ScreenWidth - 640, 10.0f)},
									std::bind(&SortingController::GenerateRandomBars, &m_sortingController));

	std::function<void()> invokeOnTrueAndFalse = std::bind(&SortingController::PositionSortingContainers, &m_sortingController);
	m_ui.CreateStaticCheckbox(0, m_sortingController.GetActiveAlgorithmRef(SortingController::SortingAlgorithm::ALG_BubbleSort), true, sf::Vector2f(Graphics::ScreenWidth - 590, 10.0f), invokeOnTrueAndFalse, invokeOnTrueAndFalse);
	m_ui.CreateStaticCheckbox(1, m_sortingController.GetActiveAlgorithmRef(SortingController::SortingAlgorithm::ALG_SelectionSort), true, sf::Vector2f(Graphics::ScreenWidth - 570, 10.0f), invokeOnTrueAndFalse, invokeOnTrueAndFalse);
	m_ui.CreateStaticCheckbox(2, m_sortingController.GetActiveAlgorithmRef(SortingController::SortingAlgorithm::ALG_InsertionSort), true, sf::Vector2f(Graphics::ScreenWidth - 550, 10.0f), invokeOnTrueAndFalse, invokeOnTrueAndFalse);
	m_ui.CreateStaticCheckbox(3, m_sortingController.GetActiveAlgorithmRef(SortingController::SortingAlgorithm::ALG_ShellSort), true, sf::Vector2f(Graphics::ScreenWidth - 530, 10.0f), invokeOnTrueAndFalse, invokeOnTrueAndFalse);
	m_ui.CreateStaticCheckbox(4, m_sortingController.GetActiveAlgorithmRef(SortingController::SortingAlgorithm::ALG_MergeSort), true, sf::Vector2f(Graphics::ScreenWidth - 510, 10.0f), invokeOnTrueAndFalse, invokeOnTrueAndFalse);
	m_ui.CreateStaticCheckbox(5, m_sortingController.GetActiveAlgorithmRef(SortingController::SortingAlgorithm::ALG_HeapSort), true, sf::Vector2f(Graphics::ScreenWidth - 490, 10.0f), invokeOnTrueAndFalse, invokeOnTrueAndFalse);
	m_ui.CreateStaticCheckbox(6, m_sortingController.GetActiveAlgorithmRef(SortingController::SortingAlgorithm::ALG_QuickSort), true, sf::Vector2f(Graphics::ScreenWidth - 470, 10.0f), invokeOnTrueAndFalse, invokeOnTrueAndFalse);
	m_ui.CreateStaticCheckbox(7, m_sortingController.GetActiveAlgorithmRef(SortingController::SortingAlgorithm::ALG_BogoSort), true, sf::Vector2f(Graphics::ScreenWidth - 450, 10.0f), invokeOnTrueAndFalse, invokeOnTrueAndFalse);
	m_sortingController.PositionSortingContainers();
	m_ui.CreateStaticButton(0, std::bind(&SortingController::StartAll, &m_sortingController), "Start", sf::Vector2f(Graphics::ScreenWidth - 390, 10.0f));
	m_ui.CreateStaticButton(1, std::bind(&SortingController::PauseAll, &m_sortingController), "Pause", sf::Vector2f(Graphics::ScreenWidth - 340, 10.0f));
	m_ui.CreateStaticButton(2, std::bind(&SortingController::ResetAll, &m_sortingController), "Reset", sf::Vector2f(Graphics::ScreenWidth - 290, 10.0f));
	m_ui.CreateStaticButton(3, std::bind(&SortingController::NewValues, &m_sortingController), "New", sf::Vector2f(Graphics::ScreenWidth - 240, 10.0f));
	m_ui.CreateStaticIntSlider(1, m_sortingController.GetStepTimeRef(), 1, 50000, 1000, sf::Vector2f(Graphics::ScreenWidth - 230, 10.0f));
}

void Application::Go()
{
	HandleEvents();
	m_gfx.ClearFrame();
	UpdateLogic();
	RenderFrame();
	m_gfx.EndFrame();
}

void Application::HandleEvents()
{
	while (m_gfx.GetRenderWindow().pollEvent(m_event))
	{
		// Close window: exit
		if (m_event.type == sf::Event::Closed)
		{
			m_gfx.GetRenderWindow().close();
		}
		else if (m_event.type == sf::Event::MouseWheelMoved)
		{
			float delta_move = m_event.mouseWheel.delta;
			float zoom_strength = 0.03f;
			m_camera.SetZoom(m_camera.GetZoom() + sf::Vector2f(delta_move, delta_move) * zoom_strength);
		}
	}
}

void Application::UpdateLogic()
{
	m_iu.Update();
	m_camera_controller.Update(m_dt);
	m_camera.Update(m_dt);
	m_ui.Update(m_dt);

	m_sortingController.Update();
}

void Application::RenderFrame()
{
	m_iu.DrawMouseCoordinates();
	m_ui.Draw();

	m_sortingController.Draw();
}