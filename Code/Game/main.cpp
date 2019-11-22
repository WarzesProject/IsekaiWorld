#include "Scene.h"

class Game : public ouzel::Application
{
public:
	explicit Game(const std::vector<std::string>& args) :
		bundle(std::make_unique<assets::Bundle>(engine->getCache(), engine->getFileSystem()))
	{
		// disable screen saver
		engine->setScreenSaverEnabled(false);

		std::string sample;
		for (auto arg = args.begin(); arg != args.end(); ++arg)
		{
			if (arg == args.begin())
			{
				// skip the first parameter
				continue;
			}

			if (*arg == "-sample")
			{
				if (++arg != args.end())
					sample = *arg;
				else
					ouzel::engine->log(Log::Level::Warning) << "No sample specified";
			}
			else
				ouzel::engine->log(Log::Level::Warning) << "Invalid argument \"" << *arg << "\"";
		}

		engine->getFileSystem().addResourcePath("../Resources");

		ouzel::storage::Archive archive(engine->getFileSystem().getPath("gui.zip"));
		engine->getFileSystem().addArchive("gui.zip", std::move(archive));

		bundle->loadAssets("assets.json");

		std::unique_ptr<ouzel::scene::Scene> currentScene = std::make_unique<MyScene>();
		engine->getSceneManager().setScene(std::move(currentScene));
	}

private:
	std::unique_ptr<assets::Bundle> bundle;
};

std::unique_ptr<ouzel::Application> ouzel::main(const std::vector<std::string>& args)
{
	return std::make_unique<Game>(args);
}