
#include <iostream>
#include "PlayerPower.h"

PlayerPower::PlayerPower()
{
	number_ = 0;
}

PlayerPower::~PlayerPower()
{
}

void PlayerPower::AddPos(const int& xPos)
{
	pos_list_.push_back(xPos);
}

void PlayerPower::Show(SDL_Renderer* screen)
{
	for (int i = 0; i < pos_list_.size(); i++) 
	{
		rect_.x = pos_list_.at(i);
		rect_.y = 0;
		Render(screen);
	}
}

void PlayerPower::Init(SDL_Renderer* screen)
{
	LoadImg("img\\player_pw.png", screen);
	number_ = POWER_NUMBER; // số mạng
	if (pos_list_.size() > 0)
	{
		pos_list_.clear();
	}

	for (int i = 0; i < number_; i++) 
	{
		AddPos(20 + 40 * i);
	}
}

void PlayerPower::Increase()
{
	number_++;
	int last_pos = pos_list_.back();
	last_pos += 40;
	pos_list_.push_back(last_pos);
	// Có thể chèn thêm âm thanh khi tăng mạng
}

void PlayerPower::Decrease()
{
	number_--;
	pos_list_.pop_back();
}

PlayerMoney::PlayerMoney()
{
	x_pos_ = 0;
	y_pos_ = 0;
}

PlayerMoney::~PlayerMoney()
{
}

void PlayerMoney::Init(SDL_Renderer* screen)
{
	bool ret = LoadImg("img\\money.png", screen);
	SetPos(SCREEN_WIDTH * 0.5 - 300, 8);
}

void PlayerMoney::Show(SDL_Renderer* screen)
{
	rect_.x = x_pos_;
	rect_.y = y_pos_;
	Render(screen);
}
