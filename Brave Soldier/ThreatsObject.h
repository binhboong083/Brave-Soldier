
#ifndef THREATS_OBJECT_H_
#define THREATS_OBJECT_H_
#include "BaseObject.h"
#include "CommonFunc.h"
#include "BulletObject.h"

#define THREAT_MAX_FALL_SPEED 10 // Độ rơi max cho phép
#define THREAT_GRAVITY_SPEED 0.8 // Tốc độ rơi của threats
#define THREAT_FRAME_NUM 8		// Số khung hình trong 1 hành động của threat
#define THREAT_FRAME_NUM1 13
#define THREAT_SPEED 3			// Tốc độ di chuyển của threat

class ThreatsObject :public BaseObject {
public:
	ThreatsObject(); // Hàm tạo 1 threat
	~ThreatsObject(); // Hàm hủy threat
	// Các loại threat (tĩnh, động trên mặt đất, bay trên trời)
	enum TypeMove
	{
		STATIC_THREAT = 0,
		MOVE_IN_SPACE_THREAT = 1,
		MOVE_IN_SKY_THREAT = 2
	};
	// set tốc độ di chuyển của threat
	void set_x_val(const float& xVal) { x_val_ = xVal; }
	void set_y_val(const float& yVal) { y_val_ = yVal; }
	// set, get vị trí thực tế của threat
	void set_x_pos(const float& xp) { x_pos_ = xp; }
	void set_y_pos(const float& yp) { y_pos_ = yp; }

	float get_x_pos() const { return x_pos_; }
	float get_y_pos() const { return y_pos_; }
	// Xử lý map di chuyển theo threat
	void SetMapXY(const int& mp_x, const int& mp_y) { map_x_ = mp_x; map_y_ = mp_y; }
	// Xử lý các hiệu ứng animation
	void set_clips();
	void set_clips13();
	// Hàm load hình ảnh threat
	bool LoadImg(std::string path, SDL_Renderer* screen);
	bool LoadImg13(std::string path, SDL_Renderer* screen);
	// Hàm hiển thị threat
	void Show(SDL_Renderer* des);
	// Hàm get width, height frame
	int get_width_frame() const { return width_frame_; }
	int get_height_frame() const { return height_frame_; }
	// Hàm trả về tọa độ frame hiện tại của threat
	SDL_Rect GetRectFrame();
	// Cập nhật di chuyển cho threat
	void DoPlayer(Map& gMap);
	// Hàm kiểm tra threat với map
	void CheckToMap(Map& gMap);
	// Hàm khởi hồi sinh threat
	void InitThreats();
	// set, get loại threat
	void set_type_move(const int& typeMove) { type_move_ = typeMove; }
	int getTypeMove() { return type_move_; }
	// Giới hạn di chuyển
	void SetAnimationPos(const int& pos_a, const int& pos_b) { animation_a_ = pos_a; animation_b_ = pos_b; }
	void set_input_left(const int& ipleft) { input_type_.left_ = ipleft; }
	//void set_input_right(const int& ipright) { input_type_.right_ = ipright; }
	// Hàm xử lý di chuyển trong (a;b)
	void ImpMoveType(SDL_Renderer* screen);

	// Đạn bán cho threats
	void set_bullet_list(const std::vector<BulletObject*>& bl_list) { bullet_list_ = bl_list; }
	std::vector<BulletObject*> get_bullet_list()const { return bullet_list_; }
	// Khởi tạo các thông số của Bullet
	void InitBullet(BulletObject* p_bullet, SDL_Renderer* screen);
	// Xử lý viện đạn khi hiển thị ra màn hình
	void MakeBullet(SDL_Renderer* screen, const int& x_limit, const int& y_limit);
	// Hàm xóa viên đạn bất kỳ
	void RemoveBullet(const int& idx);
	// Đặt lại tọa độ cho viên đạn
	void ResetBullet(BulletObject* p_bullet);
private:
	// Kiểm tra giới hạn của bản đồ
	int map_x_;
	int map_y_;
	// Tăng tọa độ pos
	float x_val_;
	float y_val_;
	// Vị trí của Threats on map
	float x_pos_;
	float y_pos_;
	// Kiểm tra trên mặt đất
	bool on_ground_;
	// Kiểm tra trên trời
	bool on_sky_;
	// Thời gian quay lại khi Threat died
	int come_back_time_;
	SDL_Rect frame_clip_[THREAT_FRAME_NUM];// Mảng thể hiện số khung hình trong 1 ảnh trạng thái của threat
	SDL_Rect frame_clip13_[THREAT_FRAME_NUM1];
	int width_frame_; 
	int height_frame_;
	int frame_; // chỉ số frame

	int type_move_; // Kiểu di chuyển
	// Di chuyển giới hạn trong (a;b)
	int animation_a_;
	int animation_b_;
	// Quản lý việc di chuyển
	Input input_type_;
	int status_;
	// Đạn bắn cho threats
	std::vector<BulletObject*> bullet_list_;

	// Biến để fix trạng thái di chuyển trái, phải của viên đạn
	int countLeft;
	int countRight;

};

#endif // THREATS_OBJECT_H_

