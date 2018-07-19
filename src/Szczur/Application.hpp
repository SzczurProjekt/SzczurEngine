#pragma once

#include "Szczur/Utility/Time/Clock.hpp"
#include "Szczur/Utility/Modules/ModulesHolder.hpp"
#include "Szczur/Modules/Window/Window.hpp"
#include "Szczur/Modules/Input/Input.hpp"
#include "Szczur/Modules/AudioEditor/AudioEditor.hpp"
#include "Szczur/Modules/Music/Music.hpp"
#include "Szczur/Modules/Sound/Sound.hpp"
#include "Szczur/Modules/AudioEffects/AudioEffects.hpp"
#include "Szczur/Modules/GUI/GUI.hpp"
#ifdef GUI_TEST
#include "Szczur/Modules/GUITest/GUITest.hpp"
#endif
#include "Szczur/Modules/Script/Script.hpp"
#include "Szczur/Modules/DragonBones/DragonBones.hpp"
#include "Szczur/Modules/World/World.hpp"
#include "Szczur/Modules/GUI/GUI.hpp"
#include "Szczur/Modules/Dialog/Dialog.hpp"
#include "Szczur/Modules/DialogEditor/DialogEditor.hpp"
#include "Szczur/Modules/Cinematics/Cinematics.hpp"
#include "Szczur/Modules/Listener/Listener.hpp"
#include "Szczur/Modules/Player/Player.hpp"

#include "ImGuiStyler.hpp"

namespace rat
{

class Application
{
public:

	Application() = default;

	Application(const Application&) = delete;

	Application& operator = (const Application&) = delete;

	Application(Application&&) = delete;

	Application& operator = (Application&&) = delete;

	int run();

	void init();

	bool input();

	void update();

	void render();

	template <typename U, typename... Us>
	void initModule(Us&&... args);
	
	///
	template <typename U>
	U& getModule();

	///
	template <typename U>
	const U& getModule() const;

private:

	ModulesHolder<Window, Input, Script, GUI, Player
	#ifdef GUI_TEST
	,GUITest 
	#endif
	> _modules;
	Clock _mainClock;
	#ifdef EDITOR
	bool _isImGuiInitialized = false;
	#endif

};

}
#include "Application.tpp"