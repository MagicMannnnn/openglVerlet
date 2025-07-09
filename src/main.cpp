
#include "app/appContext.hpp"
#include "core/window.hpp"
#include <app/application.hpp>

int main() {
	AppContext ctx;
	Window window(ctx, "Balls");
	App app(ctx, window);
	
	app.run();
	
	return 0;
}