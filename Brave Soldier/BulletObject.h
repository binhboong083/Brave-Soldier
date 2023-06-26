
#ifndef BULLET_OBJECT_H_
#define BULLET_OBJECT_H_

#include "BaseObject.h"
#include "CommonFunc.h"
#include "BaseObject.h"

class BulletObject :public BaseObject {
public:
	BulletObject(); // Hàm tạo viên đạn
	~BulletObject(); // Hàm hủy viên đạn

	// Các kiểu trạng thái của viên đạn
	enum BulletDir {
		DIR_RIGHT = 20,
		DIR_LEFT = 21,
		DIR_UP = 22,
		DIR_UP_LEFT = 23,
		DIR_UP_RIGHT = 24,
		DIR_DOWN_LEFT = 25,
		DIR_DOWN_RIGHT = 26,
		DIR_DOWN = 27,
	};
	// Các loại đạn được sử dụng
	enum BulletType {
		SPHERE_SIMPLE_BULLET = 50, // Đạn cầu thường
		SPHERE_MAX_BULLET = 51, // Đạn cầu max
		THREAT_SPHERE_BULLET = 52, // Đạn của quái
		LASER_BULLET = 53 // Tia laze
	};

	// set get, tốc độ của viên đạn 
	void set_x_val(const int& xVal) { x_val_ = xVal; }
	void set_y_val(const int& yVal) { y_val_ = yVal; }
	int get_x_val()const { return x_val_; }
	int get_y_val()const { return y_val_; }

	// Trạng thái viên đạn có di chuyển không?
	void set_is_move(const bool& isMove) { is_move_ = isMove; }
	bool get_is_move()const { return is_move_; }

	// set, get trạng thái của viên đạn được bắn ra
	void set_bullet_dir(const unsigned int& bulletDir) { bullet_dir_ = bulletDir; }
	unsigned int get_bullet_dir() const { return bullet_dir_; }
	// set, get loại đạn nào được bắn ra
	void set_bullet_type(const unsigned int& bulletType) { bullet_type_ = bulletType; }
	unsigned int get_bullet_type()const { return bullet_type_; }

	// Hàm xử lý di chuyển của viên đạn
	void HandleMove(const int& x_border, const int& y_border);
	// Test
	void HandleMovePlayer(Map& map_data, const int& x_border, const int& y_border);
	void set_xy_pos_(int xpos, int ypos) { x_pos_ = xpos; y_pos_ = ypos; }
	// Hàm load hình ảnh viên đạn
	bool LoadImgBullet(SDL_Renderer* des);
	// Hàm kiểm tra viên đạn của nhân vật va chạm với map
	void CheckToMap(Map& map_data/*, Mix_Chunk* money_sound[2]*/);
private:
	// Tốc độ di chuyển của viên đạn theo chiều x, y
	int x_val_;
	int y_val_;
	// Biến kiểm tra viên đạn có di chuyển hay không?
	bool is_move_;
	unsigned int bullet_dir_; // direction: chiều hướng -> chiều hướng của viên đạn
	unsigned int bullet_type_; // Quản lý loại đặn bắn ra

	// Vị trí thực tế của viên đạn
	float x_pos_;
	float y_pos_;

};
#endif // BULLET_OBJECT_H_
