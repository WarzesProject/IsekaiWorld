#include "EngineHeader.h"
//-----------------------------------------------------------------------------
class GameApp
{
public:
	bool Init() 
	{ 		
		return true; 
	}
	bool Frame() { return true; }
	bool Update() { return true; }
	void Close() {}
};
//-----------------------------------------------------------------------------
#if SE_PLATFORM_WINDOWS
int WINAPI wWinMain(
	[[maybe_unused]] _In_ HINSTANCE hInstance,
	[[maybe_unused]] _In_opt_ HINSTANCE hPrevInstance,
	[[maybe_unused]] _In_ LPWSTR lpCmdLine,
	[[maybe_unused]] _In_ int nCmdShow )
#else
int main(
	[[maybe_unused]] int argc, 
	[[maybe_unused]] char *argv[])
#endif
{
	Configuration config;
	return Application::Run<GameApp>(config);
}
//-----------------------------------------------------------------------------