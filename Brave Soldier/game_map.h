
#ifndef GAME_MAP_H_
#define GAME_MAP_H_

#include "CommonFunc.h"
#include "BaseObject.h"

//#define MAX_TILES 20

class TileMap :public BaseObject { // Object lưu trữ image tile map
	// Image tĩnh (chỉ tải hình ảnh lên ô tile)
public:
	TileMap() { ; }
	~TileMap() { ; }
};

class GameMap {
	// Control tile
public:
	GameMap() { ; }
	~GameMap() { ; }

	void LoadMap(std::string name);
	void LoadTiles(SDL_Renderer* screen); // Load từng tile 
	void DrawMap(SDL_Renderer* screen);
	Map getMap() const { return game_map_; }
	// Xử lý để Map di chuyển theo nhân vật
	void SetMap(const Map& map_data) { game_map_ = map_data; }
private:
	// Tên map
	Map game_map_;
	// Mảng các tile map
	TileMap tile_map[MAX_TILES];
};

#endif // GAME_MAP_H_
