#include "FluidSimulation.h"

#include <Cool/Random/Random.h>
#include <Cool/App/RenderState.h>

FluidSimulation::FluidSimulation(unsigned int width, unsigned int height)
	: m_width(width), m_height(height), m_ssbo0(0), m_ssbo1(1), m_computeShader("shaders/simulation.comp"),
	  m_shader({
		ShaderCode(ShaderType::Vertex, "Cool/Renderer_Fullscreen/fullscreen.vert"),
		ShaderCode(ShaderType::Fragment, "shaders/rendering.frag")
	  })
{
	restart();
}

void FluidSimulation::restart() {
	std::vector<int> v;
	size_t N = m_width * m_height;
	v.reserve(N);
	for (size_t i = 0; i < N; ++i) {
		v.push_back(Random::get0to1() < 0.5 ? 0 : 1);
	}
	m_ssbo0.uploadData(v);
	m_ssbo1.uploadData(v);
}

void FluidSimulation::update() {
	m_bFlipFlop = !m_bFlipFlop;
	m_computeShader.get().bind();
	m_computeShader.get().setUniform1i("u_bFlipFlop", m_bFlipFlop);
	m_computeShader.compute(m_width, m_height);
}

void FluidSimulation::render() {
	m_renderer.begin();
	{
		m_shader.bind();
		m_shader.setUniform2f("u_resolution", { m_width, m_height });
		m_shader.setUniform1i("u_resolutionX", m_width);
		m_shader.setUniform1i("u_resolutionY", m_height);
		m_shader.setUniform1i("u_bFlipFlop", m_bFlipFlop);
		m_renderer.render();
	}
	m_renderer.end(GL_NEAREST);
}

void FluidSimulation::ImGui() {

}