#include "EngineHeader.h"
#include <Windows.h>
#include <iostream>
#include <functional>
#include <string>
#include <algorithm>
#include "Engine/SignalAndSlot.h"
#include "Engine/EventSignal.h"

class FooB1 : public EventListener
{
public:
	void Key(uint32_t key, bool press)
	{
		std::cout << "hell " << key << std::endl;
	}
};

//-----------------------------------------------------------------------------
class GameApp
{
public:
	bool Init() 
	{ 
		FooB1 *f1 = new FooB1;


		
		EventSignal<uint32_t, bool> sig3;
		sig3.Connect(f1, &FooB1::Key);
		sig3(10, true);
		sig3.Disconnect(f1);
		sig3(20, true);
		sig3.Connect(f1, &FooB1::Key);

		sig3(30, true);
		sig3(40, true);
		sig3(50, true);
		delete f1; f1 = nullptr;
		sig3(60, true);
		
		


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
	Configuration config;
	return Application::Run<GameApp>(config);
}
//-----------------------------------------------------------------------------