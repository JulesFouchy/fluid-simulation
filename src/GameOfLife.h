#pragma once

#include <Cool/OpenGL/SSBO.h>
#include <Cool/OpenGL/ComputeShader.h>
#include <Cool/Renderer_Fullscreen/Renderer_Fullscreen.h>

using namespace Cool;

class GameOfLife {
public:
	GameOfLife(unsigned int width, unsigned int height);
	~GameOfLife() = default;

	void restart();
	void update();
	void render();
	void renderToConsole();
	void ImGui();
	inline Renderer_Fullscreen& renderer() { return m_renderer; }

private:
	unsigned int m_width;
	unsigned int m_height;
	SSBO<int> m_ssbo0; // We use two SSBOs to write in one of them while we read the other
	SSBO<int> m_ssbo1;
	ComputeShader<16, 16, 1> m_computeShader;
	bool m_bFlipFlop = false;

	Renderer_Fullscreen m_renderer;
	Shader m_shader;
	float m_cellRoundness = 1.972f;
	float m_cellThreshold = 0.367;
	float m_cellBlur = 0.367f;
	int m_cellGen = 3;
};