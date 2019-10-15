#include "EngineHeader.h"
#include <Windows.h>
#include "Engine/SignalAndSlot.h"
#include <iostream>
#include <functional>
#include <string>
#include <algorithm>

struct foo
{
	void f(int i) { std::cout << "foo" << i; }

	void f2() { std::cout << "foo2"; }
};

foo fb;

std::shared_ptr<foo> fb2(new foo);
std::weak_ptr<foo> fb3(fb2);



class EventManager
{
	struct Observer { virtual ~Observer() {} virtual bool invoke() = 0; };

	typedef std::unique_ptr<Observer> OPtr;
	typedef std::vector<OPtr> Observers;
public:

	// Callback observers of "name"
	// Returns the number of observers so invoked
	size_t signal(std::string const& name) const
	{
		auto const it = _observers.find(name);
		if (it == _observers.end()) { return 0; }

		Observers& obs = it->second;

		size_t count = 0;
		auto invoker = [&count](OPtr const& p) -> bool {
			bool const invoked = p->invoke();
			count += invoked;
			return not invoked; // if not invoked, remove it!
		};

		obs.erase(std::remove_if(obs.begin(), obs.end(), invoker), obs.end());

		if (obs.empty()) { _observers.erase(it); }

		return count;
	}

	// Registers a function callback on event "name"
	void Register(std::string const& name, void (*f)())
	{
		_observers[name].push_back(OPtr(new ObserverFunc(f)));
	}

	// Registers an object callback on event "name"
	template <typename T>
	void Register(std::string const& name, std::shared_ptr<T> const& p, void (T::*f)())
	{
		_observers[name].push_back(OPtr(new ObserverMember<T>(p, f)));
	}

private:
	struct ObserverFunc : Observer
	{
		ObserverFunc(void (*f)()) : _f(f) {}

		virtual bool invoke() override { _f(); return true; }

		void (*_f)();
	};

	template <typename T>
	struct ObserverMember : Observer
	{
		ObserverMember(std::weak_ptr<T> p, void (T::*f)()) : _p(p), _f(f) {}

		virtual bool invoke() override
		{
			std::shared_ptr<T> p = _p.lock();
			if (not p) { return false; }
			auto ttt = p.get();

			(ttt->*_f)();
			return true;
		}

		std::weak_ptr<T> _p;
		void (T::*_f)();
	};

	// mutable because we remove observers lazily
	mutable std::unordered_map<std::string, Observers> _observers;
}; // class EventManager

//-----------------------------------------------------------------------------
class GameApp
{
public:
	bool Init() 
	{ 
		Signal<int> sig;

		sig.Connect<foo>(fb2, &foo::f);

		sig.Connect(fb, &foo::f);

		sig(10);

		fb2.reset();

		sig(20);


		////fb3.lock()->*f(2);

		//EventManager em;

		//em.Register<foo>("test", fb2, &foo::f2);



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