
#ifndef PLAYER_POWER_H_
#define PLAYER_POWER_H_

#include "CommonFunc.h"
#include "BaseObject.h"

#define POWER_NUMBER 6
// Hiển thị số sinh mạng
class PlayerPower : public BaseObject
{
public:
	PlayerPower();
	~PlayerPower();

	void SetNum(const int& num) { number_ = num; }
	void AddPos(const int& xPos); 
	void Show(SDL_Renderer* screen);
	void Init(SDL_Renderer* screen);

	// Hàm tăng mạng
	void Increase();
	// Hàm giảm mạng
	void Decrease();
	// Hàm set, get số mạng
	void setNum(int num) { number_ = num; }
	int getNum() { return number_; }

private:
	int number_; // Số mạng
	std::vector<int> pos_list_; // Vị trí đặt mạng

};

// Hiển thị đồng tiền
class PlayerMoney :public BaseObject
{
public:
	PlayerMoney();
	~PlayerMoney();
	void Init(SDL_Renderer* screen);
	void Show(SDL_Renderer* screen);
	void SetPos(const int& x, const int& y) { x_pos_ = x; y_pos_ = y; }
private:
	int x_pos_;
	int y_pos_;
};

#endif // PLAYER_POWER_H_

