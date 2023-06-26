
#include <iostream>
#include "BulletObject.h"

BulletObject::BulletObject()
{
	x_pos_ = 0;
	y_pos_ = 0;
	x_val_ = 0;
	y_val_ = 0;
	is_move_ = false;
	bullet_type_ = SPHERE_SIMPLE_BULLET;
}

BulletObject::~BulletObject()
{

}

void BulletObject::HandleMove(const int& x_border, const int& y_border)
{
	if (bullet_dir_ == DIR_RIGHT) {
		rect_.x += x_val_;
		if (rect_.x > x_border) {
			is_move_ = false;
		}
	}
	else if (bullet_dir_ == DIR_LEFT) {
		rect_.x -= x_val_;
		if (rect_.x < 0) {
			is_move_ = false;
		}
	}
	else if (bullet_dir_ == DIR_UP)
	{
		rect_.y -= y_val_;
		if (rect_.y < 0) {
			is_move_ = false;
		}
	}
	else if (bullet_dir_ == DIR_UP_LEFT) {
		rect_.x -= x_val_;
		if (rect_.x < 0) {
			is_move_ = false;
		}
		rect_.y -= y_val_;
		if (rect_.y < 0) {
			is_move_ = false;
		}
	}
	else if (bullet_dir_ == DIR_UP_RIGHT) {
		rect_.x += x_val_;
		if (rect_.x > x_border) {
			is_move_ = false;
		}
		rect_.y -= y_val_;
		if (rect_.y < 0) {
			is_move_ = false;
		}
	}
	else if (bullet_dir_ == DIR_DOWN)
	{
		rect_.y += y_val_;
		if (rect_.y < 0) {
			is_move_ = false;
		}
	}
	else if (bullet_dir_ == DIR_DOWN_LEFT)
	{
		rect_.x -= x_val_;
		if (rect_.x < 0) {
			is_move_ = false;
		}
		rect_.y += y_val_;
		if (rect_.y > y_border) {
			is_move_ = false;
		}
	}
	else if (bullet_dir_ == DIR_DOWN_RIGHT)
	{
		rect_.x += x_val_;
		if (rect_.x < 0) {
			is_move_ = false;
		}
		rect_.y += y_val_;
		if (rect_.y > y_border) {
			is_move_ = false;
		}
	}
}

void BulletObject::HandleMovePlayer(Map& map_data, const int& x_border, const int& y_border)
{
	if (bullet_dir_ == DIR_RIGHT) {
		rect_.x += x_val_;
		x_pos_ += x_val_;
		if (rect_.x > x_border) {
			is_move_ = false;
		}
	}
	else if (bullet_dir_ == DIR_LEFT) {
		rect_.x -= x_val_;
		x_pos_ -= x_val_;
		if (rect_.x < 0) {
			is_move_ = false;
		}
	}
	else if (bullet_dir_ == DIR_UP)
	{
		rect_.y -= y_val_;
		if (rect_.y < 0) {
			is_move_ = false;
		}
	}
	else if (bullet_dir_ == DIR_UP_LEFT) {
		rect_.x -= x_val_;
		if (rect_.x < 0) {
			is_move_ = false;
		}
		rect_.y -= y_val_;
		if (rect_.y < 0) {
			is_move_ = false;
		}
	}
	else if (bullet_dir_ == DIR_UP_RIGHT) {
		rect_.x += x_val_;
		if (rect_.x > x_border) {
			is_move_ = false;
		}
		rect_.y -= y_val_;
		if (rect_.y < 0) {
			is_move_ = false;
		}
	}
	else if (bullet_dir_ == DIR_DOWN)
	{
		rect_.y += y_val_;
		if (rect_.y < 0) {
			is_move_ = false;
		}
	}
	else if (bullet_dir_ == DIR_DOWN_LEFT)
	{
		rect_.x -= x_val_;
		if (rect_.x < 0) {
			is_move_ = false;
		}
		rect_.y += y_val_;
		if (rect_.y > y_border) {
			is_move_ = false;
		}
	}
	else if (bullet_dir_ == DIR_DOWN_RIGHT)
	{
		rect_.x += x_val_;
		if (rect_.x < 0) {
			is_move_ = false;
		}
		rect_.y += y_val_;
		if (rect_.y > y_border) {
			is_move_ = false;
		}
	}
	//CheckToMap(map_data);
}

bool BulletObject::LoadImgBullet(SDL_Renderer* des)
{
	bool ret = false;
	if (bullet_type_ == LASER_BULLET) {
		ret = LoadImg("img\\laser_bullet.png", des);
	}
	else if (bullet_type_ == SPHERE_SIMPLE_BULLET)
	{
		ret = LoadImg("img\\sphere_bullet.png", des);
	}
	else if (bullet_type_ == SPHERE_MAX_BULLET)
	{
		ret = LoadImg("img\\sphere_max_bullet.png", des);
	}
	else if (bullet_type_ == THREAT_SPHERE_BULLET) {
		ret = LoadImg("img\\bullet_threat.png", des);
	}
	return ret;
}

void BulletObject::CheckToMap(Map& map_data)
{
	// Trong (x1;x2)
	int x1 = 0;
	int x2 = 0;
	// Trong (y1;y2)
	int y1 = 0;
	int y2 = 0;

	// Kiểm tra theo chiều ngang
	int height_min = rect_.h < TILE_SIZE ? rect_.h : TILE_SIZE;
	if (get_bullet_dir() == DIR_RIGHT) {
		x1 = (x_pos_ + x_val_) / TILE_SIZE;
		x2 = (x_pos_ + x_val_ + rect_.w - 1) / TILE_SIZE;
	}
	else if (get_bullet_dir() == DIR_LEFT) {
		x1 = (x_pos_ - x_val_) / TILE_SIZE;
		x2 = (x_pos_ - x_val_ + rect_.w - 1) / TILE_SIZE;
	}
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
		if (get_bullet_dir() == DIR_RIGHT) { // main object is moving to right: Tiến
			// Xử lý va chạm vào coin: tile map số 4.png
			int val1 = map_data.tile[y1][x2];
			int val2 = map_data.tile[y2][x2];
			if ((val1 != BLANK_TILE && val1 != STATE_MONEY) || (val2 != BLANK_TILE && val2 != STATE_MONEY)) {
				is_move_ = false;
				x_val_ = 0;
			}
		}
		else if (get_bullet_dir() == DIR_LEFT) { // Lùi
			int val1 = map_data.tile[y1][x1];
			int val2 = map_data.tile[y2][x1];

			if ((val1 != BLANK_TILE && val1 != STATE_MONEY) || (val2 != BLANK_TILE && val2 != STATE_MONEY)) {
				is_move_ = false;
				x_val_ = 0;
			}
		}
	}

	if (x_pos_ < 0) {
		x_pos_ = 0;
	}
	if (x_pos_ + rect_.w > map_data.max_x_)
	{
		x_pos_ = map_data.max_x_ - rect_.w - 1;
	}
}
