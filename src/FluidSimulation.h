#pragma once

#include <Cool/OpenGL/SSBO.h>
#include <Cool/OpenGL/ComputeShader.h>
#include <Cool/Renderer_Fullscreen/Renderer_Fullscreen.h>

using namespace Cool;

class FluidSimulation {
public:
	FluidSimulation(unsigned int width, unsigned int height);
	~FluidSimulation() = default;

	void restart();
	void update();
	void render();
	void ImGui();
	inline Renderer_Fullscreen& renderer() { return _renderer; }

private:
	unsigned int _width;
	unsigned int _height;
	SSBO<int> _ssbo0; // We use two SSBOs to write in one of them while we read the other
	SSBO<int> _ssbo1;
	ComputeShader<16, 16, 1> _compute_shader;
	bool _flipflop = false;

	Renderer_Fullscreen _renderer;
	Shader _shader;
};