

#include "CommonFunc.h"
#include "BaseObject.h"
#include "game_map.h"
#include "MainObject.h"
#include "ImpTimer.h"
#include "ThreatsObject.h"
#include "ExplosionObject.h" 
#include "ResultObject.h"
#include "TextObject.h"
#include "PlayerPower.h"
#include "Geometric.h"
#include "BossObject.h"

// #undef main

BaseObject g_background;
TTF_Font* font_time = NULL;
TTF_Font* font_menu = NULL;
TTF_Font* font_load = NULL;
int checkvictory = -1;
int checkdefeat = -1;
// Khi nhân vật chết -> quay trở lại sẽ bất tử khoảng 1 thời gian
bool come_back_ = false;
int check_come_back_ = -1;
int time_val_temp = 0;

int playtime;

bool InitData() {
	bool success = true;
	int ret = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO); // Thiết lập môi trường ban đầu cho SDL
	if (ret < 0)
		return false;
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
	g_window = SDL_CreateWindow("Brave Soldier",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		SCREEN_WIDTH, SCREEN_HEIGHT,
		SDL_WINDOW_SHOWN);

	if (g_window == NULL) {
		success = false;
	}
	else {
		g_screen = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		if (g_screen == NULL) {
			std::cout << "Renderer could not be created! SDL Error: \n" << SDL_GetError();
			success = false;
		}
		else
		{
			//set background color
			SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
			int imgFlags = IMG_INIT_PNG;
			if (!(IMG_Init(imgFlags) && imgFlags)) {
				std::cout << "SDL_image could not initialize! SDL_image Error: \n" << IMG_GetError();
				success = false;
			}
		}
		if (TTF_Init() == -1)
		{
			std::cout << "SDL_ttf could not initialize! SDL_ttf Error: \n" << TTF_GetError();
			success = false;
		}
		font_time = TTF_OpenFont("font\\dlxfont.ttf", 15/*Kích thước*/);
		if (font_time == NULL) {
			std::cout << "Failed to load dlxfont! SDL_ttf Error: \n" << TTF_GetError();
			success = false;
		}

		font_menu = TTF_OpenFont("font\\ARCADE.ttf", 80/*Kích thước*/);
		if (font_menu == NULL) {
			std::cout << "Failed to load ARCADE font! SDL_ttf Error: \n" << TTF_GetError();
			success = false;
		}

		font_load = TTF_OpenFont("font\\COOPBL.ttf", 60/*Kích thước*/);
		if (font_load == NULL) {
			std::cout << "Failed to load dlxfont! SDL_ttf Error: \n" << TTF_GetError();
			success = false;
		}

	}

	// Khởi tạo âm thanh: frequency: tấn số: 22050
	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
		return false;
	}
	// Đọc file wav audio
	g_sound_bullet[0] = Mix_LoadWAV("sound\\Fire1.wav");
	g_sound_bullet[1] = Mix_LoadWAV("sound\\Fire.wav");
	g_sound_exp[0] = Mix_LoadWAV("sound\\explosion.wav");
	g_sound_exp[1] = Mix_LoadWAV("sound\\Bomb1.wav");
	g_sound_effect[0] = Mix_LoadWAV("sound\\beep_.wav");
	g_sound_effect[1] = Mix_LoadWAV("sound\\fall.wav");

	g_sound_result[0] = Mix_LoadWAV("sound\\Victory.wav");
	g_sound_result[1] = Mix_LoadWAV("sound\\Defeat.wav");

	if (g_sound_exp[0] == NULL || g_sound_bullet[0] == NULL || g_sound_bullet[1] == NULL) {
		return false;
	}
	return success;
}

void close() {
	Mix_CloseAudio();

	g_background.Free();

	SDL_DestroyRenderer(g_screen);
	g_screen = NULL;

	SDL_DestroyWindow(g_window);
	g_window = NULL;

	//Free global font
	TTF_CloseFont(font_time);
	font_time = NULL;
	TTF_CloseFont(font_menu);
	font_menu = NULL;
	TTF_CloseFont(font_load);
	font_load = NULL;

	//Quit SDL subsystems
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

int LoadGame() {
	int x = 0;
	TextObject text_object[3];
	text_object[0].SetText("Game Loading...");
	text_object[0].SetColor(0, 255, 0);
	text_object[0].LoadFromRenderText(font_load, g_screen);

	text_object[2].SetText("%");
	text_object[2].SetColor(255, 0, 0);
	text_object[2].LoadFromRenderText(font_load, g_screen);

	SDL_Rect pos[3];
	pos[0].x = SCREEN_WIDTH / 4.5f;
	pos[0].y = SCREEN_HEIGHT / 3;

	pos[1].x = SCREEN_WIDTH / 1.6f;
	pos[1].y = SCREEN_HEIGHT / 3;

	pos[2].x = SCREEN_WIDTH / 1.4f;
	pos[2].y = SCREEN_HEIGHT / 3;

	while (x <= 100) {
		text_object[1].SetText(std::to_string(x));
		text_object[1].SetColor(255, 0, 0);
		text_object[1].LoadFromRenderText(font_load, g_screen);

		SDL_SetRenderDrawColor(g_screen, 0, 0, 0, 0);
		SDL_RenderClear(g_screen);

		for (int i = 0; i < 3; ++i)
		{
			text_object[i].RenderText(g_screen, pos[i].x, pos[i].y);
			pos[i].w = text_object[i].GetWidth();
			pos[i].h = text_object[i].GetHeight();
		}

		SDL_RenderPresent(g_screen);
		if (x == 75) SDL_Delay(1000);
		if (x == 100) SDL_Delay(500);
		SDL_Delay(20);
		x++;
	}
	return x;
}

bool LoadBackground() {
	bool ret = g_background.LoadImg("img\\background.png", g_screen);
	if (ret == false) {
		return false;
	}
	return true;

}
// Hàm danh sách các threats
std::vector<ThreatsObject*> MakeThreatList() {
	// Tĩnh
	std::vector<ThreatsObject*> list_threats;

	// Threat động mặt đất
	ThreatsObject* dynamic_threats = new ThreatsObject[10]; // 10 threats động
	for (int i = 0; i < 10; i++)
	{
		ThreatsObject* p_threat = (dynamic_threats + i);
		if (p_threat != NULL) {
			p_threat->LoadImg("img\\threat_left.png", g_screen);
			p_threat->set_clips();
			p_threat->set_type_move(ThreatsObject::MOVE_IN_SPACE_THREAT);
			p_threat->set_x_pos(500 + i * 500);
			p_threat->set_y_pos(200);

			int pos1 = p_threat->get_x_pos() - 100;
			int pos2 = p_threat->get_x_pos() + 100;
			p_threat->SetAnimationPos(pos1, pos2); // threat dynamic di chuyển trong (pos1;pos2)
			p_threat->set_input_left(1); // Động

			// Tạo đạn bắn
			BulletObject* p_bullet = new BulletObject();
			p_threat->InitBullet(p_bullet, g_screen);
			list_threats.push_back(p_threat);
		}
	}

	// Threat động trên trời
	ThreatsObject* dynamic_fly_threats = new ThreatsObject[20]; // 20 threats động
	for (int i = 0; i < 20; i++)
	{
		ThreatsObject* p_threat = (dynamic_fly_threats + i);
		if (p_threat != NULL) {
			p_threat->LoadImg("img\\threat2_left.png", g_screen);
			p_threat->set_clips();
			p_threat->set_type_move(ThreatsObject::MOVE_IN_SKY_THREAT);
			p_threat->set_x_pos(550 + i * 650);
			p_threat->set_y_pos(150);

			int pos1 = p_threat->get_x_pos() - 200;
			int pos2 = p_threat->get_x_pos() + 200;
			p_threat->SetAnimationPos(pos1, pos2); // threat dynamic di chuyển trong (pos1;pos2)
			p_threat->set_input_left(1); // Động

			// Tạo đạn bắn
			//BulletObject* p_bullet = new BulletObject();
			//p_threat->InitBullet(p_bullet, g_screen);
			list_threats.push_back(p_threat);
		}
	}


	ThreatsObject* threats_objs = new ThreatsObject[20]; // 10 threats
	for (int i = 0; i < 20; i++)
	{
		ThreatsObject* p_threat = (threats_objs + i);
		if (p_threat != NULL) {
			p_threat->LoadImg("img\\threat_level.png", g_screen);
			p_threat->set_clips();
			p_threat->set_x_pos(600 + i * 1200); // 24700 <= 25000
			p_threat->set_y_pos(250);
			p_threat->set_type_move(ThreatsObject::STATIC_THREAT);
			//p_threat->set_input_left(0); // Tĩnh

			// Tạo đạn bắn
			BulletObject* p_bullet = new BulletObject();
			p_threat->InitBullet(p_bullet, g_screen);
			list_threats.push_back(p_threat);
		}
	}
	return list_threats;
}
void Graphics(MainObject& p_player, TextObject& mark_game, TextObject& money_game, TextObject& time_game, PlayerPower& player_power, PlayerMoney& player_money, unsigned int& mark_value) {

	// Vẽ hình học: hình chữ nhật ở trên cùng
	GeometricFormat rectangle_size(0, 0, SCREEN_WIDTH, 40);
	ColorData color_data(36, 36, 36); // Nền đen
	Geometric::RenderRectangle(rectangle_size, color_data, g_screen);

	GeometricFormat outLineSize(1, 1, SCREEN_WIDTH - 1, 38);
	ColorData color_data2(255, 255, 255); // Viền trắng
	Geometric::RenderOutLine(outLineSize, color_data2, g_screen);

	std::string val_str_mark = std::to_string(mark_value);
	std::string strMark("Mark: ");
	strMark += val_str_mark;

	mark_game.SetText(strMark);
	mark_game.LoadFromRenderText(font_time, g_screen);
	mark_game.RenderText(g_screen, SCREEN_WIDTH * 0.5 - 50/*Góc phải*/, 15/*Độ cao*/);

	int money_count = p_player.GetMoneyCount();
	std::string money_str = std::to_string(money_count);
	std::string strMoney("Money: ");
	strMoney += money_str;

	money_game.SetText(strMoney);
	money_game.LoadFromRenderText(font_time, g_screen);
	money_game.RenderText(g_screen, SCREEN_WIDTH * 0.5 - 250/*Góc phải*/, 15/*Độ cao*/);

	// Show game_time
	std::string str_time = "Time: ";
	//Uint32 time_val = SDL_GetTicks() / 1000;
	unsigned int time_val = SDL_GetTicks() / 1000; // Time hiện tại
	//unsigned int val_time = 1000 - time_val;
	int val_time = MAX_PLAYER_TIME - (playtime / 1000);
	if (val_time <= 0) {
		checkdefeat = 0;
	}
	else // Show text
	{
		std::string str_val = std::to_string(val_time);
		str_time += str_val;

		time_game.SetText(str_time);
		time_game.LoadFromRenderText(font_time, g_screen);
		time_game.RenderText(g_screen, SCREEN_WIDTH - 200/*Góc phải*/, 15/*Độ cao*/);
	}
	// Hiển thị số sinh mạng
	player_power.Show(g_screen);
	// Hiển thị biểu tượng đồng tiền
	player_money.Show(g_screen);
}

int comeback(MainObject& player, bool& is_quit, const LPCTSTR& message, const LPCTSTR& title, UINT information) {
	if (MessageBox(NULL, message, title, information) == IDOK)
	{
		Sleep(500);
		int ret_menu = SDLCommonFunc::ShowMenuFinal(g_screen, font_menu, "Play Again", "Help", "Exit", "img//menu.png");
		//player.setdie(0);
		if (ret_menu == 2)
		{
			is_quit = true;
			return 2;
			//continue;
		}
		else if (ret_menu == 1) { // Help
			is_quit = false;
			return 1;
		}
		else // Continue play again game
		{
			is_quit = false;
			return 0;
		}
	}
}

int result(ResultObject& re, MainObject& player, bool& is_quit, const LPCTSTR& message, const LPCTSTR& title, UINT information) {
	for (int e = 0; e < NUM_FRAME_RESULT; e++) {
		re.set_frame(e);
		re.SetRect(SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 150);
		re.Show(g_screen);
		SDL_RenderPresent(g_screen);
		Sleep(200);
	}
	return comeback(player, is_quit, message, title, information);
}

int main(int argc, char* argv[])
{
	ImpTimer fps_timer;
	if (InitData() == false) { return -1; }

	bool is_quit = false;
	if (LoadGame() >= 100) {
	menu:
		//Make menu game 
		//int ret_menu = SDLCommonFunc::ShowMenu(g_screen, font_menu, "Play Game", "Exit", "img//menu.png");
		int ret_menu = SDLCommonFunc::ShowMenuFinal(g_screen, font_menu, "Play Game", "Help", "Exit", "img//menu.png");
		if (ret_menu == 2)
		{
			is_quit = true;
		}
		else if (ret_menu == 1) {
			int ret_menuHelp = SDLCommonFunc::ShowMenuHelp(g_screen, font_time, "<-- Go Back", "img//HelpMenu.png");
			if (ret_menuHelp == 0)
				goto menu;
		}
	}

	if (!LoadBackground())
	{
		return -1;
	}
	// Music
	g_music = Mix_LoadMUS("sound\\Action.mid");

	if (g_music == NULL) {
		return 0;
	}

	if (LoadBackground() == false) { return -1; }
again_label:
	// Hiển thị map
	GameMap game_map;
	game_map.LoadMap("map\\map01.dat");
	game_map.LoadTiles(g_screen);
	// Hiện thị nhân vật
	MainObject p_player;
	p_player.LoadImg("img\\player_right.png", g_screen);
	p_player.set_clips();
	//p_player.setdie(0);
	// Hiển thị số sinh mạng
	PlayerPower player_power;
	player_power.Init(g_screen);
	// Hiển thị hình ảnh biểu tượng đồng tiền
	PlayerMoney player_money;
	player_money.Init(g_screen);

	std::vector<ThreatsObject*> threats_list = MakeThreatList(); // Quái

	// Boss threat
	std::vector<BossObject*> boss;
	BossObject* bossObject = new BossObject();
	bossObject->LoadImg("img\\boss_object.png", g_screen);
	bossObject->set_clips();
	//int xPosBoss = 1900 - SCREEN_WIDTH * 0.6;
	int xPosBoss = (MAX_MAP_X * TILE_SIZE) - SCREEN_WIDTH * 0.6 - 11000; // Boss xuất hiện cuối map
	bossObject->set_xpos(xPosBoss);
	bossObject->set_ypos(10);
	bossObject->InitBullet(g_screen);

	boss.push_back(bossObject);
	int bossdied = 0;

	// Xử lý hiệu ứng nổ: threat nổ
	ExplosionObject exp_threat;
	bool tRet = exp_threat.LoadImg("img\\explosion1.png", g_screen);
	if (!tRet) return -1;
	exp_threat.set_clips();
	// Xử lý hiệu victory: boss nổ
	ResultObject vic;
	bool vicRet = vic.LoadImg("img\\Victory.png", g_screen);
	if (!vicRet) return -1;
	vic.set_clips();
	// Xử lý hiệu defeat: Nhân vật hết sinh mạng
	ResultObject def;
	bool defRet = def.LoadImg("img\\Defeat.png", g_screen);
	if (!defRet) return -1;
	def.set_clips();
	// Xử lý hiệu ứng nổ: nhân vật nổ

	ExplosionObject exp_main;
	//bool mRet = exp_main.LoadImg("img\\died_left.png", g_screen);
	//exp_main.set_clips();
	//if (!mRet) return -1;

	ExplosionObject exp_left;
	bool RetLeft = exp_left.LoadImg("img\\died_left.png", g_screen);
	exp_left.set_clips();
	if (!RetLeft) return -1;

	ExplosionObject exp_right;
	bool RetRight = exp_right.LoadImg("img\\died_right.png", g_screen);
	exp_right.set_clips();
	if (!RetRight) return -1;
	// Thêm chỉ số sinh mạng
	//int num_die = 0;
	p_player.setdie(0);
	// Hiển thị thời gian chơi còn lại
	TextObject time_game;
	time_game.SetColor(TextObject::WHITE_TEXT);
	// Hiển thị điểm số
	TextObject mark_game;
	mark_game.SetColor(TextObject::WHITE_TEXT);
	unsigned int mark_value = 0;
	// Hiên thị money thu được
	TextObject money_game;
	money_game.SetColor(TextObject::WHITE_TEXT);

	int checkmusic = 0;
	checkvictory = -1;
	checkdefeat = -1;
	ImpTimer timer;
	timer.start();
	//playtime = timer.get_ticks();
	while (!is_quit)
	{
		fps_timer.start();
		playtime = timer.get_ticks();
		while (SDL_PollEvent(&g_event) != 0) {
			//if (g_event.type == SDL_QUIT) {
			//	is_quit = true;
			//}
			switch (g_event.type)
			{
			case SDL_QUIT:
			{
				is_quit = true;
				break;
			}
			case SDL_KEYDOWN:
			{
				if (g_event.key.keysym.sym == SDLK_RETURN)
				{
					checkmusic++;
					if (!Mix_PlayingMusic() || checkmusic % 2 == 1) {
						if (!Mix_PlayingMusic()) Mix_PlayMusic(g_music, -1);
						else  Mix_ResumeMusic();
					}
					else if (Mix_PlayingMusic() && checkmusic % 2 == 0) {
						Mix_PauseMusic();
						//Mix_Pause(-1);
					}
				}
				// ? Thoát khỏi game
				else if (g_event.key.keysym.sym == SDLK_ESCAPE)
				{
					int check = 0;
					if (Mix_PausedMusic() == 0) {
						Mix_PauseMusic(); check = 1;
					}
					timer.paused();
					if (MessageBox(NULL, L"Do you want to resume game?", L"Message", MB_YESNO | MB_ICONINFORMATION) == IDNO) {
						Sleep(500);
						Mix_HaltMusic();
					menu1:
						timer.stop();
						int ret_menu = SDLCommonFunc::ShowMenuFinal(g_screen, font_menu, "Play Again", "Help", "Exit", "img//menu.png");
						if (ret_menu == 2)
						{
							is_quit = true;
						}
						else if (ret_menu == 1) {
							int ret_menuHelp = SDLCommonFunc::ShowMenuHelp(g_screen, font_time, "<-- Go Back", "img//HelpMenu.png");
							if (ret_menuHelp == 0)
								goto menu1;
						}
						else
						{
							is_quit = false;
							goto again_label;
						}
					}
					else {
						timer.unpause(); playtime = timer.get_ticks();
						if (check == 1) { Mix_ResumeMusic(); check = 0; }
					}
				}
				break;
			}

			}
			p_player.HandleInputAction(g_event, g_screen, g_sound_bullet); // Xử lý action nhân vật
		}

		//// Music
		//if (Mix_PlayingMusic() == 0) {
		//	// Play the music
		//	if (Mix_PlayMusic(g_music, -1) == -1) {
		//		return -1;
		//	}
		//}

		SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
		SDL_RenderClear(g_screen);

		g_background.Render(g_screen, NULL);

		Map map_data = game_map.getMap();

		p_player.HandleBullet(map_data, g_screen);
		p_player.SetMapXY(map_data.start_x_, map_data.start_y_);
		p_player.DoPlayer(map_data, g_sound_effect);

		if (p_player.getHeart() == true) {
			player_power.Increase();
			p_player.setHeart(false);
		}

		p_player.Show(g_screen);

		game_map.SetMap(map_data);
		game_map.DrawMap(g_screen); // Hiển thị map

		Graphics(p_player, mark_game, money_game, time_game, player_power, player_money, mark_value);
		if (p_player.player_left() == true) { exp_main = exp_right; }
		else if (p_player.player_left() == false) { exp_main = exp_left; }

		// Bất tử khi comeback
		int time_val = SDL_GetTicks() / 1000; // Time hiện tại
		if (come_back_ == true && check_come_back_ == -1)
		{
			time_val_temp = time_val;
			check_come_back_++;
		}
		if ((time_val - time_val_temp) >= IMMORTAL_TIME)// Vì đã delay 1s -> Nhân vật sẽ bất tử IMMORTAL_TIME-1s
		{
			come_back_ = false;
			check_come_back_ = -1;
		}

#pragma region Nhân vật rơi xuống vực
		if (player_power.getNum() > 0) {
			if (p_player.getComeBack() == true) {
				Mix_PlayChannel(-1, g_sound_effect[1], 0);
				come_back_ = true;
				check_come_back_ = -1;
				p_player.setComeBack(false);
				p_player.Countdie();
				// Hồi sinh lại nhân vật
				p_player.SetRect(0, 0);
				p_player.set_comeback_time(60);
				SDL_Delay(1000); // 1000ms = 1s
				// Khi chết cập nhật lại số hình ảnh sinh mạng 
				player_power.Decrease();
				player_power.Render(g_screen);
			}
		}
		else if (player_power.getNum() <= 0) { checkdefeat = 0; }
#pragma endregion

#pragma region Threats và kiểm tra va chạm threats vs nhân vật
		// Hiển thị quái
		for (int i = 0; i < threats_list.size(); i++)
		{
			ThreatsObject* p_threat = threats_list.at(i);
			if (p_threat != NULL) {
				p_threat->SetMapXY(map_data.start_x_, map_data.start_y_);
				p_threat->ImpMoveType(g_screen);
				p_threat->DoPlayer(map_data);
				p_threat->MakeBullet(g_screen, SCREEN_WIDTH, SCREEN_HEIGHT);
				p_threat->Show(g_screen);


				if (player_power.getNum() > 0 && come_back_ == false) {
					SDL_Rect rect_player = p_player.GetRectFrame();
					bool bCol1 = false;
					std::vector<BulletObject*> tBullet_list = p_threat->get_bullet_list();
					for (int j = 0; j < tBullet_list.size(); j++) {
						BulletObject* pt_bullet = tBullet_list.at(j);
						if (pt_bullet) {
							bCol1 = SDLCommonFunc::CheckCollision(pt_bullet->GetRect(), rect_player);
							if (bCol1) {
								p_threat->ResetBullet(pt_bullet);
								//p_threat->RemoveBullet(j);
								break;
							}
						}
					}
					// Kiểm tra va chạm nhân vật với threat
					SDL_Rect rect_threat = p_threat->GetRectFrame();
					bool bCol2 = SDLCommonFunc::CheckCollision(rect_player, rect_threat);
					if (bCol2 || bCol1) {
						come_back_ = true;
						check_come_back_ = -1;
						// Xử lý hiệu ứng nổ: nhân vật nổ
						int width_exp_frame = exp_main.get_frame_width();
						int height_exp_frame = exp_main.get_frame_height();

						for (int ex = 0; ex < NUM_FRAME_EXP; ex++) // 8 frame / 1s
						{
							int x_pos = (p_player.GetRect().x + p_player.get_frame_width() * 0.5) - width_exp_frame * 0.5;
							int y_pos = (p_player.GetRect().y + p_player.get_frame_height() * 0.5) - height_exp_frame * 0.5;

							exp_main.set_frame(ex);
							exp_main.SetRect(x_pos, y_pos);
							exp_main.Show(g_screen);
							SDL_RenderPresent(g_screen);
							//Sleep(500);
						}
						Mix_PlayChannel(-1, g_sound_exp[0], 0);

						//num_die++;
						p_player.Countdie();
						p_player.SetRect(0, 0);
						p_player.set_comeback_time(60);
						SDL_Delay(1000); // 1000ms = 1s
						// Khi chết cập nhật lại số hình ảnh sinh mạng 
						player_power.Decrease();
						player_power.Render(g_screen);
					}
				}
				else if (player_power.getNum() <= 0) {
					checkdefeat = 0;
				}
			}
		}
#pragma endregion

#pragma region Kiểm tra va chạm nhân vật với threats

		int frame_exp_width = exp_threat.get_frame_width();
		int frame_exp_height = exp_threat.get_frame_height();

		// Xử lý va chạm
		std::vector<BulletObject*> bullet_arr = p_player.get_bullet_list();
		for (int i = 0; i < bullet_arr.size(); i++)
		{
			BulletObject* p_bullet = bullet_arr.at(i);
			if (p_bullet != NULL) {
				for (int j = 0; j < threats_list.size(); j++)
				{
					ThreatsObject* obj_threat = threats_list.at(j);
					if (obj_threat != NULL) {
						SDL_Rect tRect;
						tRect.x = obj_threat->GetRect().x;
						tRect.y = obj_threat->GetRect().y;
						tRect.w = obj_threat->get_width_frame();
						tRect.h = obj_threat->get_height_frame();

						SDL_Rect bRect = p_bullet->GetRect();
						bool bCol = SDLCommonFunc::CheckCollision(bRect, tRect);

						// Xảy ra va chạm
						if (bCol) {
							mark_value++; // Tăng điểm số khi shoot threat
							// Xử lý hiệu ứng nổ: Threat nổ
							for (int ex = 0; ex < NUM_FRAME_EXP; ex++) {
								int x_pos = p_bullet->GetRect().x - frame_exp_width * 0.5;
								int y_pos = p_bullet->GetRect().y - frame_exp_height * 0.5;

								exp_threat.set_frame(ex);
								exp_threat.SetRect(x_pos, y_pos);
								exp_threat.Show(g_screen);
							}
							// Âm thanh
							Mix_PlayChannel(-1, g_sound_exp[1], 0);
							p_player.RemoveBullet(i);
							obj_threat->Free();
							threats_list.erase(threats_list.begin() + j);
						}
					}
				}
			}
		}
#pragma endregion

		if (checkvictory == 0) { checkvictory = 1; }

#pragma region Boss cuối và kiểm tra va chạm giữa boss vs nhân vật
		// Show boss
		int val = MAX_MAP_X * TILE_SIZE - (map_data.start_x_ + p_player.GetRect().x);
		if (val <= SCREEN_WIDTH + 11000) {
			if (bossdied < 10) {
				bossObject->SetMapXY(map_data.start_x_, map_data.start_y_);
				bossObject->DoPlayer(map_data);
				bossObject->MakeBullet(g_screen, 400, 400);
				bossObject->Show(g_screen);

				if (player_power.getNum() > 0 && come_back_ == false) {
					SDL_Rect rect_player = p_player.GetRectFrame();
					bool bCol1 = false;
					std::vector<BulletObject*> tBullet_list = bossObject->get_bullet_list();
					for (int j = 0; j < tBullet_list.size(); j++) {
						BulletObject* pt_bullet = tBullet_list.at(j);
						if (pt_bullet) {
							bCol1 = SDLCommonFunc::CheckCollision(pt_bullet->GetRect(), rect_player);
							if (bCol1) {
								//bossObject.ResetBullet(pt_bullet);
								bossObject->RemoveBullet(j);
								break;
							}
						}
					}
					// Kiểm tra va chạm nhân vật với boss
					SDL_Rect rect_boss = bossObject->GetRectFrame();
					bool bCol2 = SDLCommonFunc::CheckCollision(rect_player, rect_boss);
					if (bCol2 || bCol1) {
						come_back_ = true;
						check_come_back_ = -1;
						// Xử lý hiệu ứng nổ: nhân vật nổ
						int width_exp_frame = exp_main.get_frame_width();
						int height_exp_frame = exp_main.get_frame_height();

						for (int ex = 0; ex < NUM_FRAME_EXP; ex++) // 8 frame / 1s
						{
							int x_pos = (p_player.GetRect().x + p_player.get_frame_width() * 0.5) - width_exp_frame * 0.5;
							int y_pos = (p_player.GetRect().y + p_player.get_frame_height() * 0.5) - height_exp_frame * 0.5;

							exp_main.set_frame(ex);
							exp_main.SetRect(x_pos, y_pos);
							exp_main.Show(g_screen);
							SDL_RenderPresent(g_screen);
							//Sleep(500);
						}
						Mix_PlayChannel(-1, g_sound_exp[0], 0);

						p_player.Countdie();
						p_player.Countdie();
						// Hồi sinh lại nhân vật
						p_player.SetRect(0, 0);
						p_player.set_comeback_time(60);
						SDL_Delay(1000); // 1000ms = 1s
						// Khi chết cập nhật lại số hình ảnh sinh mạng 
						if (player_power.getNum() >= 2) {
							player_power.Decrease();
							player_power.Decrease();
						}
						else if (player_power.getNum() < 2) {
							player_power.Decrease();
						}
						player_power.Render(g_screen);
					}
				}
				else if (player_power.getNum() <= 0) { checkdefeat = 0; }

				// Xử lý va chạm boss nổ
				bullet_arr = p_player.get_bullet_list();
				for (int i = 0; i < bullet_arr.size(); i++)
				{
					BulletObject* p_bullet = bullet_arr.at(i);
					if (p_bullet != NULL) {

						SDL_Rect tRect;
						tRect.x = bossObject->GetRect().x;
						tRect.y = bossObject->GetRect().y;
						tRect.w = bossObject->get_width_frame();
						tRect.h = bossObject->get_height_frame();

						SDL_Rect bRect = p_bullet->GetRect();
						bool bCol = SDLCommonFunc::CheckCollision(bRect, tRect);

						int x_pos = p_bullet->GetRect().x - frame_exp_width * 0.5;
						int y_pos = p_bullet->GetRect().y - frame_exp_height * 0.5;
						// Xảy ra va chạm
						if (bCol) {
							// Xử lý hiệu ứng nổ: boss trúng đạn
							for (int ex = 0; ex < NUM_FRAME_EXP; ex++) {
								exp_threat.set_frame(ex);
								exp_threat.SetRect(x_pos, y_pos);
								exp_threat.Show(g_screen);
							}
							// Âm thanh
							Mix_PlayChannel(-1, g_sound_exp[1], 0);
							p_player.RemoveBullet(i);
							bossdied++;
							if (bossdied >= 10) {
								mark_value += 10;// Tăng điểm số khi shoot threat
								bossObject->Free();
								boss.pop_back();
								//SDL_RenderPresent(g_screen);
								checkvictory = 0;
							}
						}
					}
				}
			}
		}// Đóng điều kiện boss xuất hiện cuối map

#pragma endregion

		Graphics(p_player, mark_game, money_game, time_game, player_power, player_money, mark_value);

		if (checkdefeat == 0) {
			Mix_PlayChannel(-1, g_sound_result[1], 0);
			Mix_PlayChannel(-1, g_sound_result[1], 0);
			//Mix_PauseMusic();
			Mix_HaltMusic();
			int resultCheck = result(def, p_player, is_quit, L"GAME OVER", L"INFORMATION", MB_OK | MB_ICONINFORMATION);
			if (resultCheck == 0) {
				goto again_label;
				continue;
			}
		result1:
			if (resultCheck == 1)
			{
				int ret_menuHelp = SDLCommonFunc::ShowMenuHelp(g_screen, font_time, "<-- Go Back", "img//HelpMenu.png");
				if (ret_menuHelp == 0) {
					int ret_menu = SDLCommonFunc::ShowMenuFinal(g_screen, font_menu, "Play Again", "Help", "Exit", "img//menu.png");
					if (ret_menu == 2)
					{
						is_quit = true;
					}
					else if (ret_menu == 1) { goto result1; }
					else if (ret_menu == 0) { goto again_label; }
				}
			}
		}

		if (checkvictory == 1) {
			Mix_PlayChannel(-1, g_sound_result[0], 0);
			Mix_PlayChannel(-1, g_sound_result[0], 0);
			//Mix_PauseMusic();
			Mix_HaltMusic();
			int resultCheck = result(vic, p_player, is_quit, L"WINNER", L"INFORMATION", MB_OK | MB_ICONINFORMATION);
			if (resultCheck == 0) {
				goto again_label;
				continue;
			}
		result2:
			if (resultCheck == 1)
			{
				int ret_menuHelp = SDLCommonFunc::ShowMenuHelp(g_screen, font_time, "<-- Go Back", "img//HelpMenu.png");
				if (ret_menuHelp == 0) {
					int ret_menu = SDLCommonFunc::ShowMenuFinal(g_screen, font_menu, "Play Again", "Help", "Exit", "img//menu.png");
					if (ret_menu == 2)
					{
						is_quit = true;
					}
					else if (ret_menu == 1) { goto result2; }
					else if (ret_menu == 0) { goto again_label; }
				}
			}
		}
		SDL_RenderPresent(g_screen);

		int real_imp_time = fps_timer.get_ticks();
		int time_one_frame = 1000 / FRAME_PER_SECOND; // Tính theo ms

		if (real_imp_time < time_one_frame) { // Tạo độ trễ -> tạo sự cân bằng về time cho program
			int delay_time = time_one_frame - real_imp_time;
			if (delay_time >= 0) {
				SDL_Delay(delay_time); // Tạo độ trễ
			}
		}
	}
	// Giải phóng các Threats
	for (int i = 0; i < threats_list.size(); i++) {
		ThreatsObject* p_threat = threats_list.at(i);
		if (p_threat) {
			p_threat->Free();
			p_threat = NULL;
		}
	}
	threats_list.clear();

	bossObject->Free();
	bossObject = NULL;
	boss.clear();

	close();
	return 0;
}