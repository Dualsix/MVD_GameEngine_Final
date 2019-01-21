#pragma once
#include "includes.h"
#include "ScriptSystem.h"

class waypoints : public Script {

public:

	waypoints(int owner) : Script(owner) {};
	void init();

	void update(float dt);

	bool changePos(lm::vec3 pos);


	std::vector<lm::vec3> Waypoints;
	int current_waypoint;
	Transform* transform;
	Transform* player;
	lm::vec3 playerPos;
	int player_id;
	bool girar;
	bool first;
	float anguloAux;
	float vel_rotate;
	float seguir;
	bool first_seguir;
	bool waypo;
	bool frontal;
};
