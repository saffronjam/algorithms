#define SAFFRON_ENTRY_POINT
#include "ProjectApp.h"

namespace Se
{
Unique<App> CreateApplication()
{
	return CreateUnique<ProjectApp>(AppProperties{ "Algorithms", 1700, 720 });
}

ProjectApp::ProjectApp(const AppProperties& properties) :
	App(properties),
	_projectLayer(CreateShared<ProjectLayer>())
{
}

void ProjectApp::OnInit()
{
	PushLayer(_projectLayer);
}

void ProjectApp::OnUpdate()
{
}
}
