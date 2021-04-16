#include <Cool/Log/Log.h>
#include <Cool/App/AppManager.h>

#include "App.h"

int main() {
	Cool::Log::Initialize();
	Cool::AppManager appManager("GPGPU");
	App app;
	return appManager.run(app);
}