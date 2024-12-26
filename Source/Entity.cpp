
#include "Globals.h"
#include "Module.h"
#include "Entity.h"

Entity::Entity(Application* app) : Module(app) {

}

update_status Entity::Update() {

	update_status ret = UPDATE_CONTINUE;

	return ret;

}