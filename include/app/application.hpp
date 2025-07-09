#pragma once

#include <app/appContext.hpp>
#include <core/window.hpp>
#include <app/ballManager.hpp>
#include <app/backround.hpp>

class App {
public:
	App(AppContext& ctx, Window& window);

	void run();
	void gameLoop();
	void render() const;

private:
	AppContext& m_context;
	Window& m_window;
	BallManager<200> m_ballManager;
	Backround m_backround;




};