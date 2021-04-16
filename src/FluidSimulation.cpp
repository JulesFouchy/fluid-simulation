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
	std::vector<float> v;
	size_t N = _width * _height;
	v.reserve(N);
	for (int x = 0; x < _width; ++x) {
		for (int y = 0; y < _height; ++y) {
			float d = sqrt((x - _width / 2) * (x - _width / 2) + (y - _height / 2) * (y - _height / 2)) / _height * 2.f;
			v.push_back(exp(-d*d * 15.f));
		}
	}
	_ssbo0.upload_data(v);
	_ssbo1.upload_data(v);
}

void FluidSimulation::update() {
	_flipflop = !_flipflop;
	_compute_shader->bind();
	_compute_shader->set_uniform("u_bFlipFlop", _flipflop);
	_compute_shader.compute(_width, _height);
}

void FluidSimulation::render() {
	_renderer.begin();
	{
		_shader.bind();
		_shader.set_uniform("u_resolution", { _width, _height });
		_shader.set_uniform("u_resolutionX", _width);
		_shader.set_uniform("u_resolutionY", _height);
		_shader.set_uniform("u_bFlipFlop", _flipflop);
		_renderer.render();
	}
	_renderer.end(GL_NEAREST);
}

void FluidSimulation::ImGui() {

}