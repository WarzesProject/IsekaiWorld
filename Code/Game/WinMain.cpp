#include "EngineHeader.h"
#include <Windows.h>
#include <iostream>
#include <functional>
#include <string>
#include <algorithm>
#include "Engine/EventTemp.h"




class FooB : 
	public EventResizeEventListener, 
	public EventKeyListener
{
public:
	void Resize(uint32_t w, uint32_t h) final
	{
		std::cout << w << " " << h << std::endl;

	}

	void Key(uint32_t key, bool press) final
	{
		if (press)
			std::cout << key << " down" << std::endl;
		else
			std::cout << key << " up" << std::endl;
	}
};


class FooB1 : public EventKeyListener
{
public:
	void Key(uint32_t key, bool press) override
	{
		std::cout << "hell1" << key;
	}
};

class FooB2 : public EventKeyListener
{
public:
	void Key(uint32_t key, bool press) override
	{
		std::cout << "hell2" << key;
	}
};

class FooB3 : public EventKeyListener
{
public:
	void Key(uint32_t key, bool press) override
	{
		std::cout << "hell3" << key;
	}
};


//-----------------------------------------------------------------------------
class GameApp
{
public:
	bool Init() 
	{ 
		EventSignal<EventKeyListener, uint32_t, bool> sigKey;

		FooB1 *f1 = new FooB1;
		FooB2 *f2 = new FooB2;
		FooB3 *f3 = new FooB3;

		sigKey.Connect(f1, &FooB1::Key);
		sigKey.Connect(f2, &FooB2::Key);
		sigKey.Connect(f3, &FooB3::Key);

		sigKey(10, true);








		/*FooB *fff = new FooB;
		FooB *fff2 = new FooB;
		FooB *fff3 = new FooB;
		
		std::function<void(uint32_t, uint32_t)> funca = &FooB::Key;



		EventSignal<EventResizeEventListener, uint32_t, uint32_t> sigResize;
		EventSignal<EventKeyListener, uint32_t, bool> sigKey;
		
		sigResize.Connect(fff, &FooB::Resize);
		sigKey.Connect(fff, &FooB::Key);

		sigResize(10, 20);
		sigKey(10, true);
		delete fff; fff = nullptr;
		sigResize(40, 50);
		sigKey(10, false);*/

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