#pragma once
#include "includes.h"
#include "ScriptSystem.h"

class SwitchScript : public Script {
public:

	SwitchScript(int owner) : Script(owner) {};
	void init();
	void update(float dt);

	Collider col;
	bool move = false;
	Transform* transform;
};
