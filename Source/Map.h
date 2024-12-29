#pragma once

#include "Module.h"
#include "ModulePhysics.h"
#include <list>
#include <vector>


// L09: TODO 5: Add attributes to the property structure
struct Properties
{
	struct Property
	{
		std::string name;
		bool value; //We assume that we are going to work only with bool for the moment
	};

	std::list<Property*> propertyList;

	~Properties()
	{
		for (const auto& property : propertyList)
		{
			delete property;
		}

		propertyList.clear();
	}

	// L09: DONE 7: Method to ask for the value of a custom property
	Property* GetProperty(const char* name);

};

struct MapLayer
{
	// L07: TODO 1: Add the info to the MapLayer Struct
	int id;
	std::string name;
	int width;
	int height;
	std::vector<int> tiles;
	Properties properties;

	// L07: TODO 6: Short function to get the gid value of i,j
	int Get(int i, int j) const
	{
		return tiles[(j * width) + i];
	}
};


struct Object
{
	int id;
	int x;
	int y;
	int width;
	int height;
	const char* pointString;
	int* vertices;
	int vertNum;
	bool finishLine;
	int direction;
	/*
	1: UP
	2: RIGHT
	3: DOWN
	4: LEFT
	*/
};

struct ObjectGroup
{
	int id;
	std::string name;
	std::vector<Object*> object;
};


// L06: TODO 2: Create a struct to hold information for a TileSet
// Ignore Terrain Types and Tile Types for now, but we want the image!



struct TileSet
{
	int firstGid;
	std::string name;
	int tileWidth;
	int tileHeight;
	int spacing;
	int margin;
	int tileCount;
	int columns;
	Texture* texture;

	// L07: TODO 7: Implement the method that receives the gid and returns a Rect
	Rectangle GetRect(unsigned int gid) {
		Rectangle rect = { 0 };

		int relativeIndex = gid - firstGid;
		rect.width = tileWidth;
		rect.height = tileHeight;
		rect.x = margin + (tileWidth + spacing) * (relativeIndex % columns);
		rect.y = margin + (tileHeight + spacing) * (relativeIndex / columns);

		return rect;
	}

};

// L06: TODO 1: Create a struct needed to hold the information to Map node
struct MapData
{
	int width;
	int height;
	int tileWidth;
	int tileHeight;
	std::list<TileSet*> tilesets;
	std::list<MapLayer*> layers;
	std::list<ObjectGroup*> objectsGroups;
};

class Map : public Module
{
public:

	Map(Application* parent, bool start_enabled = true);

	// Destructor
	virtual ~Map();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	update_status Update();

	// Called before quitting
	bool CleanUp();

	// Load new map
	bool Load(std::string path, std::string mapFileName);

	// L07: TODO 8: Create a method that translates x,y coordinates from map positions to world positions
	Vector2 MapToWorld(int x, int y) const;

	Vector2 WorldToMap(int x, int y);

	// L09: TODO 2: Implement function to the Tileset based on a tile id
	TileSet* GetTilesetFromTileId(int gid) const;

	// L09: TODO 6: Load a group of properties 
	bool LoadProperties(pugi::xml_node& node, Properties& properties);

	MapLayer* GetNavigationLayer();

	int GetWidth() {
		return mapData.width;
	}

	int GetHeight() {
		return mapData.height;
	}

	int GetTileWidth() {
		return mapData.tileWidth;
	}

	int GetTileHeight() {
		return mapData.tileHeight;
	}

	int* ConvertToIntArray(const std::vector<b2Vec2>& vertices);

	std::vector<Object*> GetSensors();
	
public:
	std::string mapFileName;
	std::string mapPath;
	std::vector<b2Vec2> vertices;
	int* intVertices;
	int vertNum;

private:
	bool mapLoaded;

	MapData mapData;
	std::list<PhysBody*> collisions;
	std::list<PhysBody*> sensors;
	std::vector<b2Vec2> initialPos;
	std::vector<b2Vec2> sensorsInitialPos;
	std::vector<b2Vec2> posTurn;
	pugi::xml_node mapParameters;
};