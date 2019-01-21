#include "MovePlataformScriptr.h"
#include "extern.h"

void MovePlatformScript::init() {
	transform = &ECS.getComponentFromEntity<Transform>(owner_);
}


void MovePlatformScript::update(float dt) {
	//get transform of entity
	//Transform& trans = ECS.getComponentFromEntity<Transform>(owner_);

	if (input_->GetKey(GLFW_KEY_J)) {
		shouldPlataformMove = true;
	}

	if (input_->GetKey(GLFW_KEY_K)) {
		shouldPlataformMove = false;
	}
	if (shouldPlataformMove) {
		transform->translate(0, 1*dt, 0);
	}

	std::cout << shouldPlataformMove << "\n";
}