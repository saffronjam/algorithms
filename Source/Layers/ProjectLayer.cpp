#include "ProjectLayer.h"

namespace Se
{
void ProjectLayer::OnAttach(Shared<Batch>& batch)
{
	BaseLayer::OnAttach(batch);

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
	
	if (ImGui::Begin("Project"))
	{
		_algorithmManager->OnGuiRender();
	}
	ImGui::End();
}

void ProjectLayer::OnRenderTargetResize(const sf::Vector2f& newSize)
{
	BaseLayer::OnRenderTargetResize(newSize);
	_algorithmManager->OnViewportResize(newSize);
	_scene.OnRenderTargetResize(newSize);
}
}
