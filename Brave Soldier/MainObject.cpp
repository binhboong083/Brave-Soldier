
#include <iostream>
#include "MainObject.h"

MainObject::MainObject()
{
	frame_ = 0;
	x_pos_ = 0;
	y_pos_ = 0;
	x_val_ = 0;
	y_val_ = 0;
	width_frame_ = 0;
	height_frame_ = 0;
	status_ = WALK_NONE;
	input_type_.left_ = 0;
	input_type_.right_ = 0;
	input_type_.jump_ = 0;
	input_type_.down_ = 0;
	input_type_.up_ = 0;
	input_type_.change_bullet_ = 0;
	on_ground_ = false;
	sit_on_ground_ = false;
	map_x_ = 0;
	map_y_ = 0;
	come_back_time_ = 0;
	money_count = 0;

	num_die_ = 0;
	come_back_die_ = false;
	change_bullet_ = false;
	player_left_ = false;
	inHeart_ = false;
}

MainObject::~MainObject()
{
}

bool MainObject::LoadImg(std::string path, SDL_Renderer* screen)
{
	bool ret = BaseObject::LoadImg(path, screen);
	if (ret == true) {
		width_frame_ = rect_.w / 8;
		height_frame_ = rect_.h;
	}
	return ret;
}

void MainObject::Show(SDL_Renderer* des)
{
	UpdateImagePlayer(des);

	// Khi nhân vật chuyển động
	if ((input_type_.left_ == 1 || input_type_.right_ == 1)) {
		frame_++;
	}
	else
	{
		frame_ = 0;
	}
	if (frame_ >= 8) {
		frame_ = 0;
	}
	if (come_back_time_ == 0) {
		rect_.x = x_pos_ - map_x_;
		rect_.y = y_pos_ - map_y_;

		SDL_Rect* current_clip = &frame_clip_[frame_];

		SDL_Rect renderQuad = { rect_.x, rect_.y, width_frame_,height_frame_ };

		if (current_clip != NULL)
		{
			renderQuad.w = current_clip->w;
			renderQuad.h = current_clip->h;
		}

		SDL_RenderCopy(des, p_object_, current_clip, &renderQuad);

	}
}

void MainObject::HandleInputAction(SDL_Event events, SDL_Renderer* screen, Mix_Chunk* bullet_sound[2])
{
	if (events.type == SDL_KEYDOWN) {
		switch (events.key.keysym.sym)
		{
		case SDLK_RIGHT:
		{
			status_ = WALK_RIGHT;
			input_type_.right_ = 1;
			input_type_.left_ = 0;
			player_left_ = false;
			sit_on_ground_ = false;
			UpdateImagePlayer(screen);
		}break;
		case SDLK_LEFT:
		{
			status_ = WALK_LEFT;
			input_type_.left_ = 1;
			input_type_.right_ = 0;
			player_left_ = true;
			sit_on_ground_ = false;
			UpdateImagePlayer(screen);
		}break;
		case SDLK_DOWN:
		{
			input_type_.down_ = 1;
			input_type_.jump_ = 0;
			sit_on_ground_ = true;
			UpdateImagePlayer(screen);
		}break;
		case SDLK_f:
		{
			if (input_type_.change_bullet_ == 1) {
				status_ = CHANGE_BULLET;
				input_type_.change_bullet_ = 0;
				if (input_type_.jump_ == 1) {
					sit_on_ground_ = false;
				}
				UpdateImagePlayer(screen);
			}
		}break;
		case SDLK_g:
		{
			if (change_bullet_ == true) {
				status_ = CHANGE_BULLET;
				input_type_.change_bullet_ = 1;
				if (input_type_.jump_ == 1) {
					sit_on_ground_ = false;
				}
				UpdateImagePlayer(screen);
			}
		}break;
		}
	}
	else if (events.type == SDL_KEYUP) {
		switch (events.key.keysym.sym)
		{
		case SDLK_RIGHT:
		{
			input_type_.right_ = 0;
		}break;
		case SDLK_LEFT:
		{
			input_type_.left_ = 0;
		}break;
		case SDLK_DOWN:
		{
			input_type_.down_ = 0;
			sit_on_ground_ = false;
		}break;
		}
	}
	if (events.type == SDL_MOUSEBUTTONDOWN) {
		if (events.button.button == SDL_BUTTON_RIGHT) {
			input_type_.jump_ = 1;
			input_type_.down_ = 0;
			sit_on_ground_ = false;
			//on_ground_ = false;
			UpdateImagePlayer(screen);
		}
		else if (events.button.button == SDL_BUTTON_LEFT) {
			// Tạo viên đạn
			BulletObject* p_bullet = new BulletObject();
			if (input_type_.change_bullet_ == 0) {
				p_bullet->set_bullet_type(BulletObject::SPHERE_SIMPLE_BULLET);
				// Âm thanh
				Mix_PlayChannel(-1, bullet_sound[0], 0);
			}
			else if (input_type_.change_bullet_ == 1) {
				p_bullet->set_bullet_type(BulletObject::SPHERE_MAX_BULLET);
				// Âm thanh
				Mix_PlayChannel(-1, bullet_sound[1], 0);
			}
			p_bullet->LoadImgBullet(screen);

			if (status_ == WALK_LEFT && sit_on_ground_ == false) {
				p_bullet->set_bullet_dir(BulletObject::DIR_LEFT);
				p_bullet->SetRect(this->rect_.x, rect_.y + height_frame_ * 0.25);
				p_bullet->set_xy_pos_(x_pos_, rect_.y + height_frame_ * 0.25);
			}

			else if (status_ == WALK_RIGHT && sit_on_ground_ == false) {
				p_bullet->set_bullet_dir(BulletObject::DIR_RIGHT);
				p_bullet->SetRect(this->rect_.x + width_frame_ - 20, rect_.y + height_frame_ * 0.25);
				p_bullet->set_xy_pos_(x_pos_ + width_frame_ - 20, rect_.y + height_frame_ * 0.25);
			}

			else if (status_ == WALK_LEFT && sit_on_ground_ == true) {
				p_bullet->set_bullet_dir(BulletObject::DIR_LEFT);
				p_bullet->SetRect(this->rect_.x, rect_.y + height_frame_ * 0.55);
				p_bullet->set_xy_pos_(x_pos_, rect_.y + height_frame_ * 0.55);
			}

			else if (status_ == WALK_RIGHT && sit_on_ground_ == true) {
				p_bullet->set_bullet_dir(BulletObject::DIR_RIGHT);
				p_bullet->SetRect(this->rect_.x + width_frame_ - 20, rect_.y + height_frame_ * 0.55);
				p_bullet->set_xy_pos_(x_pos_ + width_frame_ - 20, rect_.y + height_frame_ * 0.55);
			}

			else {
				p_bullet->set_bullet_dir(BulletObject::DIR_RIGHT);
				p_bullet->set_xy_pos_(x_pos_ + width_frame_ - 20, rect_.y + height_frame_ * 0.25);
			}

			p_bullet->set_x_val(20); // Tốc độ viên đạn
			p_bullet->set_y_val(20);
			p_bullet->set_is_move(true);
			p_bullet_list_.push_back(p_bullet); // nạp đạn vào băng đạn

		}
	}
}

void MainObject::set_clips()
{
	if (width_frame_ > 0 && height_frame_ > 0) {
		for (int i = 0; i < NUM_FRAME; i++)
		{
			frame_clip_[i].x = width_frame_ * i;
			frame_clip_[i].y = 0;
			frame_clip_[i].w = width_frame_;
			frame_clip_[i].h = height_frame_;
		}
	}
}

void MainObject::DoPlayer(Map& map_data, Mix_Chunk* g_sound_effect[2])
{
	// Xử lý độ rơi nhân vật
		// Khi nhân vật không rơi xuống vực
	if (come_back_time_ == 0) {
		x_val_ = 0;
		y_val_ += 0.8;

		if (y_val_ >= MAX_FALL_SPEED) {
			y_val_ = MAX_FALL_SPEED;
		}

		if (input_type_.left_ == 1) {
			x_val_ -= PLAYER_SPEED;
		}
		else if (input_type_.right_ == 1) {
			x_val_ += PLAYER_SPEED;
		}
		// Điều kiện nhảy
		if (input_type_.jump_ == 1) {
			if (on_ground_ == true) {
				y_val_ = -PLAYER_JUMP_VAL;
			}
			on_ground_ = false;
			input_type_.jump_ = 0;
		}
		CheckToMap(map_data, g_sound_effect);
		CenterEntityOnMap(map_data);
	}

	// Khi nhân vật rơi xuống vực -> xử lý cho nhân vật xuất hiện trở lại
	if (come_back_time_ > 0) {
		come_back_time_--;
		if (come_back_time_ == 0) { // reset again
			on_ground_ = false;
			if (x_pos_ > 256) {
				x_pos_ -= 256; // lùi lại 4 tile map
			}
			else {
				x_pos_ = 0;
			}

			y_pos_ = 0;
			x_val_ = 0;
			y_val_ = 0;
		}
	}
}

void MainObject::CheckToMap(Map& map_data, Mix_Chunk* g_sound_effect[2])
{
	// Trong (x1;x2)
	int x1 = 0;
	int x2 = 0;
	// Trong (y1;y2)
	int y1 = 0;
	int y2 = 0;

	// Kiểm tra theo chiều ngang
	int height_min = height_frame_ < TILE_SIZE ? height_frame_ : TILE_SIZE;

	x1 = (x_pos_ + x_val_) / TILE_SIZE;
	x2 = (x_pos_ + x_val_ + width_frame_ - 1) / TILE_SIZE;

	y1 = (y_pos_) / TILE_SIZE;
	y2 = (y_pos_ + height_min - 1) / TILE_SIZE;

	/*
			x1,y1********x2,y1
			*				*
			*				*
			*				*
			x1,y2********x2,y2
	*/
	if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y) {
		if (x_val_ > 0) { // main object is moving to right: Tiến
			// Xử lý va chạm vào coin: tile map số 4.png
			int val1 = map_data.tile[y1][x2];
			int val2 = map_data.tile[y2][x2];

			if (val1 == STATE_MONEY || val2 == STATE_MONEY || val1 == MAX_BULLET || val2 == MAX_BULLET || val1 == HEART || val2 == HEART) {
				// Khi nhân vật chạm vào tile map coin thì coin biến mất
				map_data.tile[y1][x2] = 0;
				map_data.tile[y2][x2] = 0;
				if (val1 == STATE_MONEY || val2 == STATE_MONEY) {
					IncreaseMoney(g_sound_effect);
				}
				if(val1 == MAX_BULLET || val2 == MAX_BULLET)
				{
					input_type_.change_bullet_ = 1;
					change_bullet_ = true;
					Mix_PlayChannel(-1, g_sound_effect[0], 0);
				}
				if (val1 == HEART || val2 == HEART) {
					Mix_PlayChannel(-1, g_sound_effect[0], 0);
					inHeart_ = true;
				}
			}
			else if (val1 == FIGUREHEAD || val2 == FIGUREHEAD) {
				x_pos_ = -500;
			}
			else
			{
				if (val1 != BLANK_TILE || val2 != BLANK_TILE) {
					x_pos_ = x2 * TILE_SIZE;
					x_pos_ -= width_frame_ + 1;
					x_val_ = 0;
				}
			}
		}
		else if (x_val_ < 0) { // Lùi
			int val1 = map_data.tile[y1][x1];
			int val2 = map_data.tile[y2][x1];
			if (val1 == STATE_MONEY || val2 == STATE_MONEY || val1 == MAX_BULLET || val2 == MAX_BULLET || val1 == HEART || val2 == HEART) {
				// Khi nhân vật chạm vào tile map coin thì coin biến mất
				map_data.tile[y1][x1] = 0;
				map_data.tile[y2][x1] = 0;
				if (val1 == STATE_MONEY || val2 == STATE_MONEY)
				{
					IncreaseMoney(g_sound_effect);
				}
				if (val1 == MAX_BULLET || val2 == MAX_BULLET)
				{
					input_type_.change_bullet_ = 1;
					change_bullet_ = true;
					Mix_PlayChannel(-1, g_sound_effect[0], 0);
				}
				if (val1 == HEART || val2 == HEART)
				{
					Mix_PlayChannel(-1, g_sound_effect[0], 0);
					inHeart_ = true;
				}
			}
			else if (val1 == FIGUREHEAD || val2 == FIGUREHEAD) {
				x_pos_ = -500;
			}
			else
			{
				if (val1 != BLANK_TILE || val2 != BLANK_TILE) {
					x_pos_ = (x1 + 1) * TILE_SIZE;
					x_val_ = 0;
				}
			}

		}
	}
	// Kiểm tra theo chiều dọc
	int width_min = width_frame_ < TILE_SIZE ? width_frame_ : TILE_SIZE;
	x1 = (x_pos_) / TILE_SIZE;
	x2 = (x_pos_ + width_min) / TILE_SIZE;

	y1 = (y_pos_ + y_val_) / TILE_SIZE;
	y2 = (y_pos_ + y_val_ + height_frame_ - 1) / TILE_SIZE;

	if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y) {
		if (y_val_ > 0) {
			int val1 = map_data.tile[y2][x1];
			int val2 = map_data.tile[y2][x2];
			if (val1 == STATE_MONEY || val2 == STATE_MONEY || val1 == MAX_BULLET || val2 == MAX_BULLET || val1 == HEART || val2 == HEART) {
				// Khi nhân vật chạm vào tile map coin thì coin biến mất
				map_data.tile[y2][x1] = 0;
				map_data.tile[y2][x2] = 0;
				if (val1 == STATE_MONEY || val2 == STATE_MONEY)
				{
					IncreaseMoney(g_sound_effect);
				}
				if (val1 == MAX_BULLET || val2 == MAX_BULLET)
				{
					input_type_.change_bullet_ = 1;
					change_bullet_ = true;
					Mix_PlayChannel(-1, g_sound_effect[0], 0);
				}
				if (val1 == HEART || val2 == HEART)
				{
					Mix_PlayChannel(-1, g_sound_effect[0], 0);
					inHeart_ = true;
				}
			}
			else if (val1 == FIGUREHEAD || val2 == FIGUREHEAD) {
				x_pos_ = -500;
			}
			else if (val1 == ABYSS_TILE || val2 == ABYSS_TILE) on_ground_ = false;
			else
			{
				if (val1 != BLANK_TILE || val2 != BLANK_TILE) {
					y_pos_ = y2 * TILE_SIZE;
					y_pos_ -= height_frame_ + 1;
					//y_pos_ -= height_frame_ ;
					y_val_ = 0; // Không vượt quá map

					on_ground_ = true;
					//sit_on_ground_ = false;

					if (status_ == WALK_NONE) {
						status_ = WALK_RIGHT;
					}
				}

			}

		}
		else if (y_val_ < 0) {
			int val1 = map_data.tile[y1][x1];
			int val2 = map_data.tile[y1][x2];
			if (val1 == STATE_MONEY || val2 == STATE_MONEY || val1 == MAX_BULLET || val2 == MAX_BULLET || val1 == HEART || val2 == HEART) {
				// Khi nhân vật chạm vào tile map coin thì coin biến mất
				map_data.tile[y1][x1] = 0;
				map_data.tile[y1][x2] = 0;
				if (val1 == STATE_MONEY || val2 == STATE_MONEY)
				{
					IncreaseMoney(g_sound_effect);
				}
				if (val1 == MAX_BULLET || val2 == MAX_BULLET)
				{
					input_type_.change_bullet_ = 1;
					change_bullet_ = true;
					Mix_PlayChannel(-1, g_sound_effect[0], 0);
				}
				if (val1 == HEART || val2 == HEART)
				{
					Mix_PlayChannel(-1, g_sound_effect[0], 0);
					inHeart_ = true;
				}
			}
			else if (val1 == FIGUREHEAD || val2 == FIGUREHEAD) {
				x_pos_ = -500;
			}
			else if (val1 == ABYSS_TILE || val2 == ABYSS_TILE) on_ground_ = false;
			else
			{
				if (val1 != BLANK_TILE || val2 != BLANK_TILE) {
					y_pos_ = (y1 + 1) * TILE_SIZE;
					y_val_ = 0; // Không vượt quá map

					on_ground_ = true;
					//sit_on_ground_ = false;
				}
			}

		}
	}

	x_pos_ += x_val_;
	y_pos_ += y_val_;

	if (x_pos_ < 0) {
		x_pos_ = 0;
	}
	else if (x_pos_ + width_frame_ > map_data.max_x_)
	{
		x_pos_ = map_data.max_x_ - width_frame_ - 1;
	}
	if (y_pos_ > map_data.max_y_) {
		come_back_time_ = 60; // Độ trễ quay trở lại khi rơi xuống vực
		come_back_die_ = true;
	}
}

void MainObject::CenterEntityOnMap(Map& map_data)
{
	map_data.start_x_ = x_pos_ - (SCREEN_WIDTH / 2); // Khi nhân vật đi quá 1/2 độ rộng màn hình thì map sẽ được cuốn theo
	if (map_data.start_x_ < 0) {
		map_data.start_x_ = 0;
	}
	else if (map_data.start_x_ + SCREEN_WIDTH >= map_data.max_x_) {
		map_data.start_x_ = map_data.max_x_ - SCREEN_WIDTH;
	}
	map_data.start_y_ = y_pos_ - (SCREEN_HEIGHT / 2);
	if (map_data.start_y_ < 0) {
		map_data.start_y_ = 0;
	}
	else if (map_data.start_y_ + SCREEN_HEIGHT >= map_data.max_y_) {
		map_data.start_y_ = map_data.max_y_ - SCREEN_HEIGHT;
	}
}

void MainObject::UpdateImagePlayer(SDL_Renderer* des)
{
	if (on_ground_ == true && sit_on_ground_ == false) {
		if (status_ == WALK_LEFT) {
			LoadImg("img\\player_left.png", des);
		}
		else if (status_ == WALK_RIGHT) {
			LoadImg("img\\player_right.png", des);
		}
	}

	else if (on_ground_ == true && sit_on_ground_ == true) {
		if (status_ == WALK_LEFT) {
			LoadImg("img\\SitL.png", des);
		}
		else  if (status_ == WALK_RIGHT) {
			LoadImg("img\\SitR.png", des);
		}
	}

	else if (on_ground_ == false && sit_on_ground_ == false) {
		if (status_ == WALK_LEFT) {
			LoadImg("img\\jum_left.png", des);
		}
		else {
			LoadImg("img\\jum_right.png", des);
		}
	}

}

SDL_Rect MainObject::GetRectFrame()
{
	SDL_Rect rect;
	rect.x = rect_.x;
	rect.y = rect_.y;
	rect.w = rect_.w / NUM_FRAME;
	rect.h = rect_.h;

	return rect;
}

void MainObject::HandleBullet(Map& map_data, SDL_Renderer* des)
{
	for (int i = 0; i < p_bullet_list_.size(); i++) {
		BulletObject* p_bullet = p_bullet_list_.at(i); // Lấy viên đạn thứ i từ băng
		if (p_bullet != NULL) {
			if (p_bullet->get_is_move() == true) {
				//p_bullet->HandleMove(SCREEN_WIDTH, SCREEN_HEIGHT);
				p_bullet->HandleMovePlayer(map_data, SCREEN_WIDTH, SCREEN_HEIGHT);
				if (input_type_.change_bullet_ != 1) {
					p_bullet->CheckToMap(map_data);
				}
				p_bullet->Render(des);
			}
			else {
				RemoveBullet(i);
			}
		}
	}
}

// Hàm xóa viên đạn bất kỳ
void MainObject::RemoveBullet(const int& idx)
{
	int size = p_bullet_list_.size();
	if (size > 0 && idx < size) {
		BulletObject* p_bullet = p_bullet_list_.at(idx);
		p_bullet_list_.erase(p_bullet_list_.begin() + idx);

		if (p_bullet) {
			delete p_bullet;
			p_bullet = NULL;
		}
	}
}

void MainObject::IncreaseMoney(Mix_Chunk* g_sound_effect[2])
{
	money_count++;
	// Âm thanh
	Mix_PlayChannel(-1, g_sound_effect[0], 0);
}

void MainObject::Countdie()
{
	num_die_++;
}
