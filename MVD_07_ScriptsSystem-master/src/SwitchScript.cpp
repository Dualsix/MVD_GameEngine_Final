#include "extern.h"
#include "SwitchScript.h"
#include "CollisionSystem.h"

void SwitchScript::init()
{
	transform = &ECS.getComponentFromEntity<Transform>(owner_);
}

void SwitchScript::update(float dt)
{
	int col_id = ECS.getEntity("floor_go");
	col = ECS.getComponentFromEntity<Collider>(col_id);

	if (col.colliding) {
		move = true;
	}
	else {
		move = false;
	}
	if (move) {
		transform->translate(0, 1 * dt, 0);
	}
}

