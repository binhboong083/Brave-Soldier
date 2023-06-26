
#ifndef COMMON_FUNCTION_H_
#define COMMON_FUNCTION_H_

// Khai báo các thư viện cần dùng
#include <iostream>
#include <Windows.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <math.h>
#include <SDL.h>
#include <SDL_image.h> // Hình ảnh
#include <SDL_mixer.h> // Âm thanh
#include <SDL_ttf.h> // Font: chữ
#include "TextObject.h"
#include "BaseObject.h"

// Định nghĩa các biến
	// Xây dựng form window cho game
static SDL_Window* g_window = NULL;
static SDL_Renderer* g_screen = NULL;

static SDL_Surface* g_img_menu = NULL;

static SDL_Event g_event;

// Âm thanh game
static Mix_Chunk* g_sound_bullet[2];
static Mix_Chunk* g_sound_exp[2];
static Mix_Chunk* g_sound_effect[2];
static Mix_Chunk* g_sound_result[2];
static Mix_Music* g_music;
// static Mix_Chunk* g_sound_bkr[2];
	// Screen
const int FRAME_PER_SECOND = 35; // fps: số frame trên 1s: 35 image/1s
const int SCREEN_WIDTH = 1280; // Chiều rộng của màn hình
const int SCREEN_HEIGHT = 640; // Chiều cao màn hình
const int SCREEN_BPP = 32; // 32px

// Màu xám: màu nền của nhân vật (khai báo để sau xóa nền nhân vật)
const int COLOR_KEY_R = 167; // Red
const int COLOR_KEY_G = 175; // Green
const int COLOR_KEY_B = 180; // Blue

const int RENDER_DRAW_COLOR = 0xff; //0xff = 255
const int IMMORTAL_TIME = 6; // Thời gian bất tử của nhân vật
#define BLANK_TILE 0  // Ô trống 
#define ABYSS_TILE 10// Vực thẳm

#define MAX_PLAYER_TIME 1000
// Số lượng tile map tối đa được dùng
#define MAX_TILES 20
// Định nghĩa kích thước của map
#define TILE_SIZE 64
// Tổng có 4000 tile x64
#define MAX_MAP_X 400
#define MAX_MAP_Y 10
#define STATE_MONEY 4
#define MAX_BULLET 18
#define HEART 19
#define FIGUREHEAD 15
// Cấu trúc của di chuyển của nhân vật
typedef struct Input {
	int left_;
	int right_;
	int up_;
	int down_;
	int jump_;
	int stop_;
	int change_bullet_;
}Input;

// Cấu trúc của 1 tile (ô vuông texture image)
typedef struct Map {
	// vị trí tile
	int start_x_;
	int start_y_;

	// Chỉ số tile
	int max_x_;
	int max_y_;

	int tile[MAX_MAP_Y][MAX_MAP_X];
	std::string file_name_;

}Map;

namespace SDLCommonFunc
{
	// Hàm kiểm tra va chạm
	bool CheckCollision(const SDL_Rect& object1, const SDL_Rect& object2);
	// Hàm hiển thị menu base
	int ShowMenu(SDL_Renderer* g_screen, TTF_Font* font,
		const std::string& menu1,
		const std::string& menu2,
		const std::string& img_name);
	// Hàm hiển thị Menu Help
	int ShowMenuHelp(SDL_Renderer* g_screen, TTF_Font* font,
		const std::string& menuHelp,
		const std::string& img_name);
	// Hàm hiển thị Menu Final
	int ShowMenuFinal(SDL_Renderer* g_screen, TTF_Font* font,
		const std::string& menu1,
		const std::string& menu2,
		const std::string& menu3,
		const std::string& img_name);

}

#endif // COMMON_FUNCTION_H_


