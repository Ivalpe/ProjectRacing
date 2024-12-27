
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