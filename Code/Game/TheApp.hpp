#pragma once
#include "Engine\Core\Command.hpp"
#include "Game\GameCommon.hpp"
#include "Engine\Core\EngineCommon.hpp"
#include "Engine\Net\TCPSocket.hpp"
#include "Engine\Core\NamedProperties.hpp"
#include <thread>

class TheApp
{
public:
	TheApp();
	~TheApp();
	void Update();
	void PreRender();
	void Render();
	void PostRender();
	void Initialize();
	void RunFrame();
	float UpdateInput(float timeDelta);
};

void Quit(Command &cmd);

bool TestFunction(NamedProperties& args);
bool TestFunction2(NamedProperties& args);

extern TheApp* g_theApp;




