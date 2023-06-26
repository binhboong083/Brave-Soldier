

#include <iostream>
#include "ThreatsObject.h"


ThreatsObject::ThreatsObject()
{
	width_frame_ = 0;
	height_frame_ = 0;
	x_val_ = 0.0f;
	y_val_ = 0.0f;
	x_pos_ = 0.0f;
	y_pos_ = 0.0f;
	on_ground_ = false;
	on_sky_ = false;
	come_back_time_ = 0;
	frame_ = 0;

	status_ = 0;

	animation_a_ = 0;
	animation_b_ = 0;
	input_type_.left_ = 0;
	input_type_.right_ = 0;
	input_type_.stop_ = 0;
	type_move_ = STATIC_THREAT;

	countLeft = 0;
	countRight = 0;
}

ThreatsObject::~ThreatsObject()
{
}

void ThreatsObject::set_clips()
{
	if (width_frame_ > 0 && height_frame_ > 0) {
		for (int i = 0; i < THREAT_FRAME_NUM; i++)
		{
			frame_clip_[i].x = width_frame_ * i;
			frame_clip_[i].y = 0;
			frame_clip_[i].w = width_frame_;
			frame_clip_[i].h = height_frame_;
		}
	}
}

void ThreatsObject::set_clips13()
{
	if (width_frame_ > 0 && height_frame_ > 0) {
		for (int i = 0; i < THREAT_FRAME_NUM1; i++)
		{
			frame_clip13_[i].x = width_frame_ * i;
			frame_clip13_[i].y = 0;
			frame_clip13_[i].w = width_frame_;
			frame_clip13_[i].h = height_frame_;
		}
	}
}

bool ThreatsObject::LoadImg(std::string path, SDL_Renderer* screen)
{
	bool ret = BaseObject::LoadImg(path, screen);
	if (ret) {
		width_frame_ = rect_.w / THREAT_FRAME_NUM;
		height_frame_ = rect_.h;
	}
	return ret;
}

bool ThreatsObject::LoadImg13(std::string path, SDL_Renderer* screen)
{
	bool ret = BaseObject::LoadImg(path, screen);
	if (ret) {
		width_frame_ = rect_.w / THREAT_FRAME_NUM1;
		height_frame_ = rect_.h;
	}
	return ret;
}

void ThreatsObject::Show(SDL_Renderer* des)
{
	if (come_back_time_ == 0) {
		rect_.x = x_pos_ - map_x_;
		rect_.y = y_pos_ - map_y_;
		frame_++;
		if (frame_ >= THREAT_FRAME_NUM) {
			frame_ = 0;
		}
		SDL_Rect* currentClip = &frame_clip_[frame_];
		SDL_Rect rendQuad = { rect_.x,rect_.y,width_frame_,height_frame_ };
		SDL_RenderCopy(des, p_object_, currentClip, &rendQuad);
	}
}


void ThreatsObject::DoPlayer(Map& gMap)
{
	if (come_back_time_ == 0)
	{
		x_val_ = 0;
		y_val_ += THREAT_GRAVITY_SPEED;
		if (y_val_ >= THREAT_MAX_FALL_SPEED)
		{
			y_val_ = THREAT_MAX_FALL_SPEED;
		}

		if (type_move_ == MOVE_IN_SKY_THREAT)
		{
			y_val_ = 0;
		}

		// Threat động
		if (input_type_.left_ == 1) {
			x_val_ -= THREAT_SPEED;
			status_ = 0;
		}
		else if (input_type_.right_ == 1) {
			x_val_ += THREAT_SPEED;
			status_ = 1;
		}

		CheckToMap(gMap);
	}
	else if (come_back_time_ > 0)
	{
		come_back_time_--;
		if (come_back_time_ == 0) {
			InitThreats();
		}
	}
}

void ThreatsObject::CheckToMap(Map& gMap)
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
			//input_type_.right_ = 1;
			//input_type_.left_ = 0;
			// Xử lý va chạm với tile map
			int val1 = gMap.tile[y1][x2];
			int val2 = gMap.tile[y2][x2];
			if ((val1 != BLANK_TILE && val1 != STATE_MONEY) || (val2 != BLANK_TILE && val2 != STATE_MONEY))
			{
				input_type_.stop_ = 1;
				x_pos_ = x2 * TILE_SIZE;
				x_pos_ -= width_frame_ + 1;
				x_val_ = 0;
				// status_ = 0;
			}
		}
		else if (x_val_ < 0) { // Lùi
			//input_type_.right_ = 0;
			//input_type_.left_ = 1;
			int val1 = gMap.tile[y1][x1];
			int val2 = gMap.tile[y2][x1];
			if ((val1 != BLANK_TILE && val1 != STATE_MONEY) || (val2 != BLANK_TILE && val2 != STATE_MONEY))
			{
				input_type_.stop_ = 1;
				x_pos_ = (x1 + 1) * TILE_SIZE;
				x_val_ = 0;
				// status_ = 1;
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
			int val1 = gMap.tile[y2][x1];
			int val2 = gMap.tile[y2][x2];
			if ((val1 != BLANK_TILE && val1 != STATE_MONEY && val1 != ABYSS_TILE) || (val2 != BLANK_TILE && val2 != STATE_MONEY && val2 != ABYSS_TILE))
			{
				y_pos_ = y2 * TILE_SIZE;
				y_pos_ -= (height_frame_ + 1);
				//y_pos_ -= height_frame_ ;
				y_val_ = 0; // Không vượt quá map

				on_ground_ = true;
			}
		}
		else if (y_val_ < 0) {
			int val1 = gMap.tile[y1][x1];
			int val2 = gMap.tile[y1][x2];
			if ((val1 != BLANK_TILE && val1 != STATE_MONEY) || (val2 != BLANK_TILE && val2 != STATE_MONEY))
			{
				y_pos_ = (y1 + 1) * TILE_SIZE;
				y_val_ = 0; // Không vượt quá map
			}
		}
	}

	on_sky_ = true;

	x_pos_ += x_val_;
	y_pos_ += y_val_;


	if (x_pos_ < 0) {
		x_pos_ = 0;
	}
	else if (x_pos_ + width_frame_ > gMap.max_x_)
	{
		x_pos_ = gMap.max_x_ - width_frame_ - 1;
	}
	if (y_pos_ > gMap.max_y_) {
		come_back_time_ = 60; // Độ trễ quay trở lại khi rơi xuống vực
	}
}

void ThreatsObject::InitThreats()
{
	x_val_ = 0;
	y_val_ = 0;
	if (x_pos_ > 256) {
		x_pos_ -= 256;
		animation_a_ -= 256;
		animation_b_ -= 256;
	}
	else {
		x_pos_ = 0;
	}
	y_pos_ = 0;
	come_back_time_ = 0;
	input_type_.left_ = 1;
}

void ThreatsObject::ImpMoveType(SDL_Renderer* screen)
{
	if (type_move_ == STATIC_THREAT) {
		;// Đứng nguyên một chỗ không làm gì cả
	}

	else if (type_move_ == MOVE_IN_SKY_THREAT)
	{
		if (on_sky_ == true) {
			if (x_pos_ > animation_b_ || (input_type_.stop_ == 1 && input_type_.right_ == 1)) {
				input_type_.left_ = 1;
				input_type_.right_ = 0;
				input_type_.stop_ = 0;
				status_ = 0; // left
				LoadImg("img\\threat2_left.png", screen);
			}
			else if (x_pos_ < animation_a_ || (input_type_.stop_ == 1 && input_type_.left_ == 1)) {
				input_type_.left_ = 0;
				input_type_.right_ = 1;
				input_type_.stop_ = 0;
				status_ = 1; // right
				LoadImg("img\\threat2_right.png", screen);
			}
		}
		else
		{
			if (input_type_.left_ == 1) {
				LoadImg("img\\threat2_left.png", screen);
			}
		}
	}

	else if (type_move_ == MOVE_IN_SPACE_THREAT)
	{
		if (on_ground_ == true) {
			if (x_pos_ > animation_b_ || (input_type_.stop_ == 1 && input_type_.right_ == 1)) {
				input_type_.left_ = 1;
				input_type_.right_ = 0;
				input_type_.stop_ = 0;
				status_ = 0; // left
				LoadImg("img\\threat_left.png", screen);
			}
			else if (x_pos_ < animation_a_ || (input_type_.stop_ == 1 && input_type_.left_ == 1)) {
				input_type_.left_ = 0;
				input_type_.right_ = 1;
				input_type_.stop_ = 0;
				status_ = 1; // right
				LoadImg("img\\threat_right.png", screen);
			}
		}
		else
		{
			if (input_type_.left_ == 1) {
				LoadImg("img\\threat_left.png", screen);
			}
		}
	}
}

SDL_Rect ThreatsObject::GetRectFrame()
{
	SDL_Rect rect;
	rect.x = rect_.x;
	rect.y = rect_.y;
	rect.w = rect_.w / THREAT_FRAME_NUM;
	rect.h = rect_.h;

	return rect;
}

void ThreatsObject::InitBullet(BulletObject* p_bullet, SDL_Renderer* screen)
{
	if (p_bullet != NULL) {
		if (type_move_ == STATIC_THREAT) {
			p_bullet->set_bullet_type(BulletObject::THREAT_SPHERE_BULLET);
		}
		else if (type_move_ == MOVE_IN_SPACE_THREAT) {
			p_bullet->set_bullet_type(BulletObject::SPHERE_SIMPLE_BULLET);
		}
		bool ret = p_bullet->LoadImgBullet(screen);
		if (ret) {
			p_bullet->set_is_move(true);
			p_bullet->set_bullet_dir(BulletObject::DIR_LEFT);
			if (getTypeMove() == MOVE_IN_SPACE_THREAT) {
				p_bullet->SetRect(rect_.x + 5, y_pos_ + 18);
			}
			else if (getTypeMove() == STATIC_THREAT) {
				p_bullet->SetRect(rect_.x + 5, y_pos_ + 10);
			}
			//p_bullet->SetRect(rect_.x + 5, this->rect_.y + 70);
			p_bullet->set_x_val(15);
			bullet_list_.push_back(p_bullet);
		}
	}
}

void ThreatsObject::MakeBullet(SDL_Renderer* screen, const int& x_limit, const int& y_limit)
{
	for (int i = 0; i < bullet_list_.size(); i++)
	{
		BulletObject* p_bullet = bullet_list_.at(i);
		if (p_bullet != NULL) {
			if (p_bullet->get_is_move()) {
				// Khoảng cách threat vs nhân vật để bắn
				int bullet_distance = rect_.x + width_frame_ - p_bullet->GetRect().x;
				if ((bullet_distance < 400 && bullet_distance > 0) || (bullet_distance < 0 && bullet_distance > -400)) {
					if (type_move_ == MOVE_IN_SPACE_THREAT && status_ == 0) {
						this->countLeft++;
						this->countRight = 0;
						p_bullet->set_bullet_dir(BulletObject::DIR_LEFT);
						if (this->countLeft == 1) {
							p_bullet->SetRect(rect_.x + 5, y_pos_ + 18);
						}
						//p_bullet->set_x_val(15);
					}
					else if (type_move_ == MOVE_IN_SPACE_THREAT && status_ == 1) {
						this->countRight++;
						this->countLeft = 0;
						p_bullet->set_bullet_dir(BulletObject::DIR_RIGHT);
						if (this->countRight == 1) {
							p_bullet->SetRect(rect_.x + width_frame_ - 18, y_pos_ + 18);
						}
						//p_bullet->set_x_val(15);
					}
					p_bullet->HandleMove(x_limit, y_limit);
					p_bullet->Render(screen);
				}
				else
				{
					p_bullet->set_is_move(false);
				}
			}
			else
			{
				p_bullet->set_is_move(true);

				if (type_move_ == MOVE_IN_SPACE_THREAT && status_ == 0) {
					p_bullet->SetRect(rect_.x + 5, y_pos_ + 18);
				}
				else if (type_move_ == MOVE_IN_SPACE_THREAT && status_ == 1) {
					p_bullet->SetRect(rect_.x + width_frame_ - 18, y_pos_ + 18);
				}
				else if (getTypeMove() == STATIC_THREAT) {
					p_bullet->SetRect(rect_.x + 5, y_pos_ + 10);
				}
				//p_bullet->SetRect(this->rect_.x + 5, this->rect_.y + 10);
			}
		}
	}

}

void ThreatsObject::RemoveBullet(const int& idx)
{
	int size = bullet_list_.size();
	if (size > 0 && idx < size) {
		BulletObject* p_bullet = bullet_list_.at(idx);
		bullet_list_.erase(bullet_list_.begin() + idx);

		if (p_bullet) {
			delete p_bullet;
			p_bullet = NULL;
		}
	}
}

void ThreatsObject::ResetBullet(BulletObject* p_bullet)
{
	//p_bullet->SetRect(rect_.x + 20, y_pos_ + 10);
	if (getTypeMove() == MOVE_IN_SPACE_THREAT) {
		p_bullet->SetRect(rect_.x + 20, y_pos_ + 18);
	}
	else if (getTypeMove() == STATIC_THREAT) {
		p_bullet->SetRect(rect_.x + 20, y_pos_ + 10);
	}
	p_bullet->set_x_val(15);
}
