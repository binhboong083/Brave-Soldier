
#ifndef MAIN_OBJECT_H_
#define MAIN_OBJECT_H_

#include <vector>
#include "CommonFunc.h"
#include "BaseObject.h"
#include "BulletObject.h"

#define GRAVITY_SPEED 0.8 // Tốc độ rơi
#define MAX_FALL_SPEED 10 // Độ rơi max cho phép
#define PLAYER_SPEED 8	// tốc độ di chuyển nhân vật
#define PLAYER_JUMP_VAL 18 // tốc độ nhảy của nhân vật
#define NUM_FRAME 8		// Số khung hình trong 1 hành động của nhân vật

class MainObject :public BaseObject {
public:
	MainObject(); // Hàm khởi tạo
	~MainObject(); // Hàm hủy
	// Hàm đặt y_pos_
	void set_y_pos(float ypos) { y_pos_ = ypos; }

	// Danh sách các kiểu di chuyển của nhân vật (đứng yên, sang trái, sang phải, ...)
	enum WalkType { WALK_NONE = 0, WALK_RIGHT = 1, WALK_LEFT = 2, CHANGE_BULLET = 3 };
	// Hàm load ảnh nhân vật chính riêng (có các thông số riêng)
	bool LoadImg(std::string path, SDL_Renderer* screen);
	// Hàm hiển thị nhân vật lên màn hình
	void Show(SDL_Renderer* des);
	// Hàm xử lý sự kiện khi bật button trái, phải; di chuyển
	void HandleInputAction(SDL_Event events, SDL_Renderer* screen, Mix_Chunk* bullet_sound[2]);
	void set_clips(); // Xử lý các hiệu ứng animation
	// Hàm xử lý nhân vật chạy được trên màn hình và va chạm với map
	void DoPlayer(Map& map_data, Mix_Chunk* money_sound[2]); // Cập nhật di chuyển cho nhân vật
	void CheckToMap(Map& map_data, Mix_Chunk* money_sound[2]); // Hàm kiểm tra nhân vật với map
	// Xử lý map di chuyển theo nhân vật
	void SetMapXY(const int map_x, const int map_y) { map_x_ = map_x; map_y_ = map_y; }
	// Hàm tính toán ra thông số của map khi nhân vật di chuyển
	void CenterEntityOnMap(Map& map_data);
	// Cập nhật trangh thái di chuyển của nhân vật để hiện thị qua hình ảnh
	void UpdateImagePlayer(SDL_Renderer* des);
	// Hàm get width, height frame
	int get_frame_width() const { return width_frame_; }
	int get_frame_height() const { return height_frame_; }
	// Hàm trả về tọa độ frame hiện tại của nhân vật
	SDL_Rect GetRectFrame();
	// Hàm set, get đạn bắn
	void set_bullet_list(std::vector<BulletObject*> bullet_list) {
		p_bullet_list_ = bullet_list;
	}
	std::vector<BulletObject*> get_bullet_list() const { return p_bullet_list_; }
	// Hàm điều khiển đạn
	void HandleBullet(Map& map_data, SDL_Renderer* des);
	// Hàm xóa viên đạn bất kỳ
	void RemoveBullet(const int& idx);
	// Hàm tính số coin nhân vật ăn được
	void IncreaseMoney(Mix_Chunk* money_sound[2]); 
	int GetMoneyCount() const { return money_count; }
	// Hàm set thời gian nhân vật hồi sinh
	void set_comeback_time(const int& cb_time) { come_back_time_ = cb_time; }
	// Hàm đếm lượt tử
	void Countdie();
	void setdie(int ndie) { num_die_ = ndie; }
	// Hàm lấy số lượt tử
	int getdie() { return num_die_; }
	// set, get -> kiểm tra nhân vật có chết?
	int getComeBack() { return come_back_die_; }
	void setComeBack(bool cbd) { come_back_die_ = cbd; }
	// Hàm trả về trạng thái di chuyển của nhân vật (trái/phải)
	bool player_left() { return player_left_; }
	// Hàm set, get heart
	void setHeart(bool heart) { inHeart_ = heart; }
	bool getHeart() { return inHeart_; }
private:
	int money_count; // Đếm số coin nhân vật ăn được
	std::vector<BulletObject*> p_bullet_list_; // Băng đạn
	float x_val_; // Tốc độ di chuyển theo phương x của nhân vật
	float y_val_; // Tốc độ di chuyển theo phương y của nhân vật
	
	float x_pos_; // Tọa độ x thực tế của nhân vật
	float y_pos_; // Tọa độ y thực tế của nhân vật

	// Kích thước thật sự của 1 nhân vật trong 8 khung hình
	int width_frame_;
	int height_frame_;

	SDL_Rect frame_clip_[8]; // Mảng thể hiện số khung hình trong 1 ảnh trạng thái của nhân vật
	Input input_type_;// Trạng thái di chuyển
	int frame_; // chỉ số của frame
	int status_; // Trạng thái left-right
	bool on_ground_; // Trạng thái đứng trên mặt đất
	bool sit_on_ground_; // Trạng thái ngồi

	// Tọa độ map hiện tại nhân vật đi qua
	int map_x_;
	int map_y_;

	// kiểm tra, tính toán thời gian quay lại của nhân vật khi chết
	int come_back_time_;
	bool come_back_die_;

	// Số mạng của nhân vật
	int num_die_;
	// Kiểm ra thay đạn
	bool change_bullet_;
	// Kiểm tra chiều trái, phải của nhân vật
	bool player_left_;
	// Kiểm tra ăn mạng
	bool inHeart_;
};


#endif // MAIN_OBJECT_H_
