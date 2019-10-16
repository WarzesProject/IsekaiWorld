#include "EngineHeader.h"
#include <Windows.h>
#include <iostream>
#include <functional>
#include <string>
#include <algorithm>
#include "Engine/EventTemp.h"
#include "Engine/SignalAndSlot.h"

class FooB1 : public EventListener//: public EventKeyListener
{
public:
	~FooB1() { delete i; i = 0; }
	void Key(uint32_t key, bool press)// override
	{
		std::cout << "hell1" << key << *i << std::endl;
	}

	int *i = new int(10);
};

//-----------------------------------------------------------------------------
class GameApp
{
public:
	bool Init() 
	{ 
		FooB1 *f1 = new FooB1;


		//Signal<uint32_t, bool> sig2;
		//sig2.Connect(*f1, &FooB1::Key);
		//sig2(10, true);

		EventSignal<uint32_t, bool> sig3;
		sig3.Connect(f1, &FooB1::Key);
		sig3(10, true);
		sig3(20, true);
		delete f1; f1 = nullptr;
		sig3(30, true);


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