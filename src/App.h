#pragma once

#include <Cool/App/IApp.h>

#include "FluidSimulation.h"

using namespace Cool;

class App : public Cool::IApp {
public:
	App();
	~App() = default;

	void update() override;
	void ImGuiWindows() override;
	void ImGuiMenus() override;

private:
	FluidSimulation _fluid_simulation;
	float _update_speed = 30.f;
	int _frame_count = 0;

private:
#ifndef NDEBUG
	bool _show_debug = true;
	bool _show_ImGuiDemo = false;
#endif
};