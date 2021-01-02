#include "ProjectLayer.h"

namespace Se
{
void ProjectLayer::OnAttach(std::shared_ptr<BatchLoader> &loader)
{
    BaseLayer::OnAttach(loader);

    _algorithmManager = std::make_shared<AlgorithmManager>();
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

    if (ImGui::Begin("Project", nullptr))
    {
        _algorithmManager->OnGuiRender();
    }
    ImGui::End();

    if(_showSystemWindows)
    {
        ImGui::ShowDemoWindow();
    }
    _dockSpace.End();
}

void ProjectLayer::OnRenderTargetResize(const sf::Vector2f &newSize)
{
    BaseLayer::OnRenderTargetResize(newSize);
    _algorithmManager->OnViewportResize(newSize);
    _scene.OnRenderTargetResize(newSize);
}
}
