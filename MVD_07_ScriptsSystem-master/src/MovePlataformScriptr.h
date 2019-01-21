#pragma once
#include "includes.h"
#include "ScriptSystem.h"


class MovePlatformScript : public Script {

public:

	void init();

	MovePlatformScript(int owner) : Script(owner) {};

	void update(float dt);

	bool shouldPlataformMove = false;
	Transform* transform;
};
