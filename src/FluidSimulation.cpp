#include "FluidSimulation.h"

#include <Cool/Random/Random.h>
#include <Cool/App/RenderState.h>

FluidSimulation::FluidSimulation(unsigned int width, unsigned int height)
	: _width(width), _height(height), _ssbo0(0), _ssbo1(1), _compute_shader("shaders/simulation.comp"),
	  _shader({
		ShaderCode(ShaderType::Vertex, "Cool/Renderer_Fullscreen/fullscreen.vert"),
		ShaderCode(ShaderType::Fragment, "shaders/rendering.frag")
	  })
{
	restart();
}

void FluidSimulation::restart() {
	std::vector<int> v;
	size_t N = _width * _height;
	v.reserve(N);
	for (size_t i = 0; i < N; ++i) {
		v.push_back(Random::get0to1() < 0.5 ? 0 : 1);
	}
	_ssbo0.uploadData(v);
	_ssbo1.uploadData(v);
}

void FluidSimulation::update() {
	_flipflop = !_flipflop;
	_compute_shader.get().bind();
	_compute_shader.get().setUniform1i("u_bFlipFlop", _flipflop);
	_compute_shader.compute(_width, _height);
}

void FluidSimulation::render() {
	_renderer.begin();
	{
		_shader.bind();
		_shader.setUniform2f("u_resolution", { _width, _height });
		_shader.setUniform1i("u_resolutionX", _width);
		_shader.setUniform1i("u_resolutionY", _height);
		_shader.setUniform1i("u_bFlipFlop", _flipflop);
		_renderer.render();
	}
	_renderer.end(GL_NEAREST);
}

void FluidSimulation::ImGui() {

}