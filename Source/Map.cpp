#include "ModuleRender.h"
#include "Map.h"
#include "ModuleTextures.h"
#include "ModulePhysics.h"
#include "Module.h"
#include "Globals.h"



#include <math.h>


Map::Map(Application* app, bool start_enabled) : Module(app, start_enabled), mapLoaded(false)
{
	
}

// Destructor
Map::~Map()
{}

// Called before render is available
bool Map::Awake()
{
	/*name = "map";*/
	LOG("Loading Map Parser");

	return true;
}

bool Map::Start() {

	return true;
}

update_status Map::Update()
{
	update_status ret = UPDATE_CONTINUE;

	if (mapLoaded) {

		// L07 TODO 5: Prepare the loop to draw all tiles in a layer + DrawTexture()
		// iterate all tiles in a layer
		for (const auto& mapLayer : mapData.layers) {
			//Check if the property Draw exist get the value, if it's true draw the lawyer
			if (mapLayer->properties.GetProperty("Draw") != NULL && mapLayer->properties.GetProperty("Draw")->value == true) {
				for (int i = 0; i < mapData.width; i++) {
					for (int j = 0; j < mapData.height; j++) {

						//Get the gid from tile
						int gid = mapLayer->Get(i, j);
						//Check if the gid is different from 0 - some tiles are empty
						if (gid != 0) {
							TileSet* tileSet = GetTilesetFromTileId(gid);
							if (tileSet != nullptr) {
								//Get the Rect from the tileSetTexture;
								Rectangle tileRect = tileSet->GetRect(gid);
								//Get the screen coordinates from the tile coordinates
								Vector2 mapCoord = MapToWorld(i, j);
								//Draw the texture
								
								App->renderer->Draw(*tileSet->texture, mapCoord.x , mapCoord.y, &tileRect);
							}
						}
					}
				}
			}
		}
	}

	return ret;
}

// L09: TODO 2: Implement function to the Tileset based on a tile id
TileSet* Map::GetTilesetFromTileId(int gid) const
{
	TileSet* set = nullptr;

	for (const auto& tileset : mapData.tilesets) {
		if (gid >= tileset->firstGid && gid < (tileset->firstGid + tileset->tileCount)) {
			set = tileset;
			break;
		}
	}

	return set;
}

// Called before quitting
bool Map::CleanUp()
{
	LOG("Unloading map");

	// L06: TODO 2: Make sure you clean up any memory allocated from tilesets/map
	for (const auto& tileset : mapData.tilesets) {
		delete tileset;
	}
	mapData.tilesets.clear();

	// L07 TODO 2: clean up all layer data
	for (const auto& layer : mapData.layers)
	{
		delete layer;
	}
	mapData.layers.clear();

	return true;
}

// Load new map
bool Map::Load(std::string path, std::string fileName)
{
	bool ret = false;

	// Assigns the name of the map file and the path
	mapFileName = fileName;
	mapPath = path;
	std::string mapPathName = mapPath + mapFileName;

	pugi::xml_document mapFileXML;
	pugi::xml_parse_result result = mapFileXML.load_file(mapPathName.c_str());

	if (result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", mapPathName.c_str(), result.description());
		ret = false;
	}
	/*else {*/

	mapData.width = mapFileXML.child("map").attribute("width").as_int();
	mapData.height = mapFileXML.child("map").attribute("height").as_int();
	mapData.tileWidth = mapFileXML.child("map").attribute("tilewidth").as_int() * SCALE;
	mapData.tileHeight = mapFileXML.child("map").attribute("tileheight").as_int() * SCALE;

	//Iterate the Tileset
	for (pugi::xml_node tilesetNode = mapFileXML.child("map").child("tileset"); tilesetNode != NULL; tilesetNode = tilesetNode.next_sibling("tileset"))
	{
		//Load Tileset attributes
		TileSet* tileSet = new TileSet();
		tileSet->firstGid = tilesetNode.attribute("firstgid").as_int();
		tileSet->name = tilesetNode.attribute("name").as_string();
		tileSet->tileWidth = tilesetNode.attribute("tilewidth").as_int() * SCALE;
		tileSet->tileHeight = tilesetNode.attribute("tileheight").as_int() * SCALE;
		tileSet->spacing = tilesetNode.attribute("spacing").as_int();
		tileSet->margin = tilesetNode.attribute("margin").as_int();
		tileSet->tileCount = tilesetNode.attribute("tilecount").as_int();
		tileSet->columns = tilesetNode.attribute("columns").as_int();

		//Load the tileset image
		std::string imgName = tilesetNode.child("image").attribute("source").as_string();
		tileSet->texture = App->textures->Load((mapPath + imgName).c_str());


		mapData.tilesets.push_back(tileSet);
	}

	// L07: TODO 3: Iterate all layers in the TMX and load each of them
	for (pugi::xml_node layerNode = mapFileXML.child("map").child("layer"); layerNode != NULL; layerNode = layerNode.next_sibling("layer")) {

		// L07: TODO 4: Implement the load of a single layer 
		//Load the attributes and saved in a new MapLayer
		MapLayer* mapLayer = new MapLayer();
		mapLayer->id = layerNode.attribute("id").as_int();
		mapLayer->name = layerNode.attribute("name").as_string();
		mapLayer->width = layerNode.attribute("width").as_int();
		mapLayer->height = layerNode.attribute("height").as_int();

		//L09: TODO 6 Call Load Layer Properties
		LoadProperties(layerNode, mapLayer->properties);

		//Iterate over all the tiles and assign the values in the data array
		for (pugi::xml_node tileNode = layerNode.child("data").child("tile"); tileNode != NULL; tileNode = tileNode.next_sibling("tile")) {
			mapLayer->tiles.push_back(tileNode.attribute("gid").as_int());
		}

		//add the layer to the map
		mapData.layers.push_back(mapLayer);
	}

	//Collisions
	for (pugi::xml_node tileNode = mapFileXML.child("map").child("objectgroup").child("object"); tileNode != NULL; tileNode = tileNode.next_sibling("object")) {
		int h;
		std::string p = tileNode.child("properties").child("property").attribute("value").as_string();
		if (p == "PLATFORM")
			h = 1;
		else
			h = tileNode.attribute("height").as_int();

		PhysBody* pb = App->physics->CreateRectangle(tileNode.attribute("x").as_int() + (tileNode.attribute("width").as_int() / 2), tileNode.attribute("y").as_int() + (tileNode.attribute("height").as_int() / 2), tileNode.attribute("width").as_int(), h, b2BodyType::b2_staticBody);
		if(p == "CAR")
		{
			pb->ctype = ColliderType::CAR;
		}
		if (p == "WALL")
		{
			pb->ctype = ColliderType::WALL;
		}
		//if (p == "WALL")
		//	pb->ctype = ColliderType::WALL;
		//else if (p == "PLATFORM")
		//	pb->ctype = ColliderType::GROUND;
		//else if (p == "DIE")
		//	pb->ctype = ColliderType::DIE;
		//else if (p == "NEW")
		//	pb->ctype = ColliderType::NEW;
		//else if (p == "LOAD")
		//	pb->ctype = ColliderType::LOAD;
		//else if (p == "LEV2")
		//	pb->ctype = ColliderType::LEV2;
		//else if (p == "LEV2")
		//	pb->ctype = ColliderType::WIN;
		//else
		//	pb->ctype = ColliderType::GROUND;

		collisions.push_back(pb);
	}


	for (const auto& mapLayer : mapData.layers) {
		//Check if the property Draw exist get the value, if it's true draw the lawyer
		for (int i = 0; i < mapData.width; i++) {
			for (int j = 0; j < mapData.height; j++) {

				//Get the gid from tile
				int gid = mapLayer->Get(i, j);
				//Check if the gid is different from 0 - some tiles are empty
				if (gid != 0) {

					if (gid == 1) {
						Vector2 mapCoord = MapToWorld(i, j);
						/*PhysBody* c1 = App->physics->CreateRectangle(mapCoord.x + 16, mapCoord.y + 16, 32, 32, b2BodyType::b2_staticBody);
						c1->ctype = ColliderType::WALL;*/
					}
				
				}
			}
		}
	}

	ret = true;

	// LOG all the data loaded iterate all tilesetsand LOG everything
	if (ret == true)
	{
		LOG("Successfully parsed map XML file :%s", fileName.c_str());
		LOG("width : %d height : %d", mapData.width, mapData.height);
		LOG("tile_width : %d tile_height : %d", mapData.tileWidth, mapData.tileHeight);

		LOG("Tilesets----");

		//iterate the tilesets
		for (const auto& tileset : mapData.tilesets) {
			LOG("name : %s firstgid : %d", tileset->name.c_str(), tileset->firstGid);
			LOG("tile width : %d tile height : %d", tileset->tileWidth, tileset->tileHeight);
			LOG("spacing : %d margin : %d", tileset->spacing, tileset->margin);
		}

		LOG("Layers----");

		for (const auto& layer : mapData.layers) {
			LOG("id : %d name : %s", layer->id, layer->name.c_str());
			LOG("Layer width : %d Layer height : %d", layer->width, layer->height);
		}
	}
	else {
		LOG("Error while parsing map file: %s", mapPathName.c_str());
	}

	if (mapFileXML) mapFileXML.reset();

	/*}*/

	mapLoaded = ret;
	return ret;
}

//std::list<Vector2> Map::GetBonfireList() {
//	return posBonfire;
//}
//
//std::list<Vector2> Map::GetPoisonList() {
//	return posPoison;
//}

// L07: TODO 8: Create a method that translates x,y coordinates from map positions to world positions
Vector2 Map::MapToWorld(int x, int y) const
{
	Vector2 ret;

	ret.x = (x * mapData.tileWidth);
	ret.y = (y * mapData.tileHeight);

	return ret;
}

Vector2 Map::WorldToMap(int x, int y) {

	Vector2 ret = { 0, 0 };

	ret.x = (x / mapData.tileWidth);
	ret.y = (y / mapData.tileHeight);

	return ret;
}

 //L09: TODO 6: Load a group of properties from a node and fill a list with it
bool Map::LoadProperties(pugi::xml_node& node, Properties& properties)
{
	bool ret = false;

	for (pugi::xml_node propertieNode = node.child("properties").child("property"); propertieNode; propertieNode = propertieNode.next_sibling("property"))
	{
		Properties::Property* p = new Properties::Property();
		p->name = propertieNode.attribute("name").as_string();
		p->value = propertieNode.attribute("value").as_bool(); // (!!) I'm assuming that all values are bool !!

		properties.propertyList.push_back(p);
	}

	return ret;
}

MapLayer* Map::GetNavigationLayer() {
	for (const auto& layer : mapData.layers) {
		if (layer->properties.GetProperty("Navigation") != NULL &&
			layer->properties.GetProperty("Navigation")->value) {
			return layer;
		}
	}

	return nullptr;
}

// L09: TODO 7: Implement a method to get the value of a custom property
Properties::Property* Properties::GetProperty(const char* name)
{
	for (const auto& property : propertyList) {
		if (property->name == name) {
			return property;
		}
	}

	return nullptr;
}

