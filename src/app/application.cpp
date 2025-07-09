
#include <app/application.hpp>
#include <chrono>
#include <thread>

App::App(AppContext& ctx, Window& window) : m_context(ctx), m_window(window), m_ballManager(), m_backround()
{

}

void App::run() {

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);
	

	m_context.createShaders();
	m_context.updateDeltaTime();
	m_context.updateProjection();

	//m_ballManager.addBall({ -0.8f, 0.79f }, { -0.801f, 0.8f}, 0.04f);

	int ballSpawnCounter = 0;

	int ballCounter = 0;

	//std::chrono::high_resolution_clock::time_point start, end;
	const std::chrono::duration<double, std::milli> frame_time(m_context.getFixedDeltaTime() * 1000.f);

	while (!m_window.shouldClose()) {

		m_context.updateDeltaTime();
		auto start = std::chrono::high_resolution_clock::now();

		m_window.pollEvents();
		m_window.processInput();
		
		gameLoop();

		render();

		auto end = std::chrono::high_resolution_clock::now();
		auto elapsed = end - start;
		if (elapsed < frame_time) {
			auto sleep_duration = frame_time - elapsed;


			// Adjust sleep duration to ensure we don't sleep for too short a time
			if (sleep_duration > std::chrono::milliseconds(1)) {
				std::this_thread::sleep_for(sleep_duration);
				end = std::chrono::high_resolution_clock::now();
				elapsed = end - start;				
			}

			ballSpawnCounter++;

			if (ballSpawnCounter == 10) {
				m_ballManager.addBall({ -0.8f, 0.8f }, 0.01f);
				m_ballManager.addBall({ -0.6f, 0.8f }, 0.01f);
				m_ballManager.addBall({ -0.4f, 0.8f }, 0.01f);
				m_ballManager.addBall({ -0.2f, 0.8f }, 0.01f);
				m_ballManager.addBall({ 0.f, 0.8f }, 0.01f);
				m_ballManager.addBall({ 0.8f, 0.8f }, 0.01f);
				m_ballManager.addBall({ 0.6f, 0.8f }, 0.01f);
				m_ballManager.addBall({ 0.4f, 0.8f }, 0.01f);
				m_ballManager.addBall({ 0.2f, 0.8f }, 0.01f);


				ballSpawnCounter = 0;
				ballCounter += 9;
				std::cout << "balls: " << ballCounter << "  ,dt: " << m_context.deltaTime << "\n";
			}
			
		}

		


		

		
	}
}


void App::gameLoop() {

	m_ballManager.update(m_context.getFixedDeltaTime());

}


void App::render() const {
	m_window.clear();

	
	m_ballManager.draw(m_context);
	m_backround.draw(m_context);

	m_window.swapBuffers();
}