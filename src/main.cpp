#include <Cool/Log/Log.h>
#include <Cool/App/AppManager.h>
#include <Cool/App/OpenGLWindowingSystem.h>

#include "App.h"

int main() {
	Cool::Log::Initialize();
	Cool::OpenGLWindowingSystem windowingSystem(4, 3);
	Cool::OpenGLWindow mainWindow = windowingSystem.createWindow("Flux", 1280, 720);
#ifdef NDEBUG
	glfwMaximizeWindow(mainWindow.get());
#endif
	// App
	App app;
	Cool::AppManager appManager(mainWindow, app);
	appManager.run();
}