#pragma once

#include "Error.h"
#include "ApplicationConfig.h"

class Application
{
public:	
	template<typename AppImplTemplate>
	static int Run(const ApplicationConfig &config)
	{
		Application engineApp;
		if (!IsErrorCriticalExit() && engineApp.init(config))
		{
			AppImplTemplate userApp;			
			if (!IsErrorCriticalExit() && userApp.Init())
			{
				bool isExit = false;

				while (!isExit)
				{					
					// Render Draw
					isExit = (isExit || !engineApp.beginFrame());
					isExit = (isExit || !userApp.Frame());
					isExit = (isExit || !engineApp.endFrame());

					// Event Update
					isExit = (isExit || !engineApp.update());
					isExit = (isExit || !userApp.Update());
				}

				userApp.Close();
			}
		}
		engineApp.close();

		return 0;
	}

private:
	Application();
	~Application();

	bool init(const ApplicationConfig &config);
	bool beginFrame();
	bool endFrame();
	bool update();
	void close();

	struct AppPimpl *m_impl = nullptr;
};