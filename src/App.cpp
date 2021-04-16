#include "App.h"

#include <Cool/App/RenderState.h>
#include <Cool/App/Input.h>
#include <Cool/ExportImage/AsPNG.h>
#include <Cool/String/String.h>
#include <Cool/File/File.h>

App::App()
	: _fluid_simulation(200, 200)
{
	RenderState::setPreviewAspectRatio(1.f);
	RenderState::setPreviewAspectRatioControl(true);
	//RenderState::setIsExporting(true);
	//RenderState::setExportSize(1080, 1080);
}

void exportFramebuffer(FrameBuffer& frameBuffer, const char* filepath) {
	frameBuffer.bind();
	RectSize size = RenderState::Size();
	unsigned char* data = new unsigned char[4 * size.width() * size.height()];
	glReadPixels(0, 0, size.width(), size.height(), GL_RGBA, GL_UNSIGNED_BYTE, data);
	frameBuffer.unbind();
	// Write png
	ExportImage::AsPNG(filepath, size.width(), size.height(), data);
	//
	delete[] data;
}

void App::update() {
	_fluid_simulation.render();
	//exportFramebuffer(m_gameOfLife.renderer().renderBuffer(), (File::RootDir + "/export/" + String::ToString(_frame_count, 5) + ".png").c_str());
	//_frame_count++;
	//m_gameOfLife.update();
	static auto timeOfLastUpdate = std::chrono::steady_clock::now();
	auto now = std::chrono::steady_clock::now();
	std::chrono::duration<float> elapsedTime = now - timeOfLastUpdate;
	if (elapsedTime.count() > 1.f / _update_speed) {
		_fluid_simulation.update();
		timeOfLastUpdate = now;
	}
}

void App::ImGuiWindows() {
#ifndef NDEBUG
	if (_show_debug) {
		ImGui::Begin("Debug", &_show_debug);
		ImGui::Text("Application average %.1f FPS", ImGui::GetIO().Framerate);
		ImGui::Checkbox("Show Demo Window", &_show_ImGuiDemo);
		ImGui::End();
	}
	if (_show_ImGuiDemo) // Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
		ImGui::ShowDemoWindow(&_show_ImGuiDemo);
#endif
	ImGui::Begin("Game of Life");
	ImGui::SliderFloat("Simulation Speed", &_update_speed, 0.f, 60.f);
	_fluid_simulation.ImGui();
	if (ImGui::Button("Restart"))
		_fluid_simulation.restart();
	ImGui::End();
}

void App::ImGuiMenus() {

}