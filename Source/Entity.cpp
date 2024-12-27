
#include "Globals.h"
#include "Module.h"
#include "Entity.h"
#include <box2d/b2_body.h>




Entity::Entity(Application* app) : Module(app) {

}

update_status Entity::Update() {

	update_status ret = UPDATE_CONTINUE;

	return ret;

}
//float Entity::TurnBody(bool isGoingForward, bool isGoingRight, float torque, float speed) const {
//	float FinalTorque = 0.f;
//	if (isGoingRight) {
//		if (isGoingForward) FinalTorque = torque / abs(speed);
//		else FinalTorque = -torque / abs(speed);
//	}
//	else {
//		if (isGoingForward) FinalTorque = -torque / abs(speed);
//		else FinalTorque = torque / abs(speed);
//	}
//
//	return FinalTorque;
//}