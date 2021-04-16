#include "App.h"

#include <Cool/App/RenderState.h>
#include <Cool/App/Input.h>
#include <Cool/ExportImage/AsPNG.h>
#include <Cool/String/String.h>
#include <Cool/File/File.h>

App::App()
	: m_gameOfLife(200, 200)
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
	m_gameOfLife.render();
	//exportFramebuffer(m_gameOfLife.renderer().renderBuffer(), (File::RootDir + "/export/" + String::ToString(m_frameCount, 5) + ".png").c_str());
	//m_frameCount++;
	//m_gameOfLife.update();
	static auto timeOfLastUpdate = std::chrono::steady_clock::now();
	auto now = std::chrono::steady_clock::now();
	std::chrono::duration<float> elapsedTime = now - timeOfLastUpdate;
	if (elapsedTime.count() > 1.f / m_updateSpeed) {
		m_gameOfLife.update();
		timeOfLastUpdate = now;
	}
}

void App::ImGuiWindows() {
#ifndef NDEBUG
	if (m_bShow_Debug) {
		ImGui::Begin("Debug", &m_bShow_Debug);
		ImGui::Text("Application average %.1f FPS", ImGui::GetIO().Framerate);
		ImGui::Checkbox("Show Demo Window", &m_bShow_ImGuiDemo);
		ImGui::End();
	}
	if (m_bShow_ImGuiDemo) // Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
		ImGui::ShowDemoWindow(&m_bShow_ImGuiDemo);
#endif
	ImGui::Begin("Game of Life");
	ImGui::SliderFloat("Simulation Speed", &m_updateSpeed, 0.f, 60.f);
	m_gameOfLife.ImGui();
	if (ImGui::Button("Restart"))
		m_gameOfLife.restart();
	ImGui::End();
}

void App::ImGuiMenus() {

}