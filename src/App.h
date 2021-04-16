#pragma once

#include <Cool/App/IApp.h>

#include "GameOfLife.h"

using namespace Cool;

class App : public Cool::IApp {
public:
	App();
	~App() = default;

	void update() override;
	void ImGuiWindows() override;
	void ImGuiMenus() override;

private:
	GameOfLife m_gameOfLife;
	float m_updateSpeed = 30.f;
	int m_frameCount = 0;

private:
#ifndef NDEBUG
	bool m_bShow_Debug = true;
	bool m_bShow_ImGuiDemo = false;
#endif
};