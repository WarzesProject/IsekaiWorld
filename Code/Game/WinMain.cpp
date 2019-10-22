#include "EngineHeader.h"
#include <Windows.h>

#include "Core/MemoryAllocator.h"

template <typename T, typename A = StdAlloc<T>>
using Vector = std::vector<T, A>;

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
int WINAPI wWinMain(
	[[maybe_unused]] _In_ HINSTANCE hInstance,
	[[maybe_unused]] _In_opt_ HINSTANCE hPrevInstance,
	[[maybe_unused]] _In_ LPWSTR lpCmdLine,
	[[maybe_unused]] _In_ int nCmdShow)
{
	Vector<int> v = { 1, 2, 3 };




	Configuration config;
	return Application::Run<GameApp>(config);
}
//-----------------------------------------------------------------------------