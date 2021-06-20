#include "ProjectLayer.h"

namespace Se
{
void ProjectLayer::OnAttach(std::shared_ptr<BatchLoader>& loader)
{
	BaseLayer::OnAttach(loader);

	_algorithmManager = CreateShared<AlgorithmManager>();
}

void ProjectLayer::OnDetach()
{
	BaseLayer::OnDetach();
}

void ProjectLayer::OnUpdate()
{
	BaseLayer::OnUpdate();

	_algorithmManager->OnUpdate(_scene);
	_algorithmManager->OnRender(_scene);
}

void ProjectLayer::OnGuiRender()
{
	BaseLayer::OnGuiRender();

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Gui"))
		{
			ImGui::MenuItem("View System", nullptr, &_viewSystem);
			ImGui::MenuItem("View Demo", nullptr, &_viewDemo);
			if (ImGui::BeginMenu("Theme"))
			{
				if (ImGui::MenuItem("Dark"))
				{
					Gui::SetStyle(GuiStyle::Dark);
				}
				if (ImGui::MenuItem("Light"))
				{
					Gui::SetStyle(GuiStyle::Light);
				}
				if (ImGui::MenuItem("Visual Studio"))
				{
					Gui::SetStyle(GuiStyle::VisualStudio);
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	if (ImGui::Begin("Project"))
	{
		_algorithmManager->OnGuiRender();
	}
	ImGui::End();

	if (_viewDemo)
	{
		ImGui::ShowDemoWindow();
	}
}

void ProjectLayer::OnRenderTargetResize(const sf::Vector2f& newSize)
{
	BaseLayer::OnRenderTargetResize(newSize);
	_algorithmManager->OnViewportResize(newSize);
	_scene.OnRenderTargetResize(newSize);
}
}
