#include "EngineHeader.h"
#include <Windows.h>

алимер и оузер

//-----------------------------------------------------------------------------
class GameApp
{
public:
	bool Init() { return true; }
	bool Frame() { return true; }
	bool Update() { return true; }
	void Close() {}
};
//-----------------------------------------------------------------------------
int WINAPI wWinMain(
	[[maybe_unused]] _In_ HINSTANCE hInstance,
	[[maybe_unused]] _In_opt_ HINSTANCE hPrevInstance,
	[[maybe_unused]] _In_ LPWSTR lpCmdLine,
	[[maybe_unused]] _In_ int nCmdShow)
{
	ApplicationConfig config;
	return Application::Run<GameApp>(config);
}
//-----------------------------------------------------------------------------