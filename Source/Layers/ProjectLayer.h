#pragma once

#include "Layers/BaseLayer.h"

#include "AlgorithmManager.h"

namespace Se
{
class ProjectLayer : public BaseLayer
{
public:
	void OnAttach(Shared<BatchLoader>& loader) override;
	void OnDetach() override;

	void OnUpdate() override;
	void OnGuiRender() override;

	void OnRenderTargetResize(const sf::Vector2f& newSize) override;

private:
	Shared<AlgorithmManager> _algorithmManager;
};
}
