#include "GameOfLife.h"

#include <Cool/Random/Random.h>
#include <Cool/App/RenderState.h>
#include <iostream>

GameOfLife::GameOfLife(unsigned int width, unsigned int height)
	: m_width(width), m_height(height), m_ssbo0(0), m_ssbo1(1), m_computeShader("shaders/simulation.comp"),
	  m_shader({
		ShaderCode(ShaderType::Vertex, "Cool/Renderer_Fullscreen/fullscreen.vert"),
		ShaderCode(ShaderType::Fragment, "shaders/rendering.frag")
	  })
{
	restart();
}

void GameOfLife::restart() {
	std::vector<int> v;
	size_t N = m_width * m_height;
	v.reserve(N);
	for (size_t i = 0; i < N; ++i) {
		//v.push_back(Random::get0to1() < 0.5 ? 0 : 1);
		v.push_back(i % m_cellGen == 1 ? 1 : 0);
	}
	m_ssbo0.uploadData(v);
	m_ssbo1.uploadData(v);
}

void GameOfLife::update() {
	m_bFlipFlop = !m_bFlipFlop;
	m_computeShader.get().bind();
	m_computeShader.get().setUniform1i("u_bFlipFlop", m_bFlipFlop);
	m_computeShader.compute(m_width, m_height);
}

void GameOfLife::render() {
	m_renderer.begin();
	{
		m_shader.bind();
		m_shader.setUniform2f("u_resolution", { m_width, m_height });
		m_shader.setUniform1i("u_resolutionX", m_width);
		m_shader.setUniform1i("u_resolutionY", m_height);
		m_shader.setUniform1i("u_bFlipFlop", m_bFlipFlop);
		m_shader.setUniform1f("u_cellRoundness", m_cellRoundness);
		m_shader.setUniform1f("u_cellThreshold", m_cellThreshold);
		m_shader.setUniform1f("u_cellBlur", m_cellBlur);
		m_renderer.render();
	}
	m_renderer.end(GL_NEAREST);
}

void GameOfLife::renderToConsole() {
	std::cout << "----------------------------------------------" << std::endl;
	std::vector<int> v;
	size_t N = m_width * m_height;
	v.resize(N);
	if (m_bFlipFlop)
		m_ssbo0.downloadData(v);
	else
		m_ssbo1.downloadData(v);
	for (unsigned int y = 0; y < m_height; ++y) {
		for (unsigned int x = 0; x < m_width; ++x) {
			std::cout << v[x + y * m_width] << " ";
		}
		std::cout << std::endl;
	}
}

void GameOfLife::ImGui() {
	ImGui::SliderFloat("Cell Roundness", &m_cellRoundness, 0.f, 10.f);
	ImGui::SliderFloat("Cell Threshold", &m_cellThreshold, 0.f, 10.f);
	ImGui::SliderFloat("Cell Blur", &m_cellBlur, 0.f, 1.f);
	ImGui::InputInt("Cell Gen", &m_cellGen);
}