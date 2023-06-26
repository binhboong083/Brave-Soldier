


#ifndef BOSS_OBJECT_H_
#define BOSS_OBJECT_H_


#include "BaseObject.h"
#include "CommonFunc.h"
#include "BulletObject.h"

#define GRAVITY_SPEED 0.8 // Tốc độ rơi của boss
#define MAX_FALL_SPEED 10 // Độ rơi max của boss

// Tốc độ di chuyển của boss
#define PLAYER_SPEED 2
#define PLAYER_HIGHT_VAL 18;
// Số khung hình trong 1 ảnh trạng thái của boss
#define FRAME_NUM_32 32

class BossObject : public BaseObject
{
public:
    BossObject(); // Hàm khởi tạo boss
    ~BossObject(); // Hàm hủy boss

    // set tốc độ di chuyển của boss
    void set_x_val(int xp) { x_val_ = xp; }
    void set_y_val(int yp) { y_val_ = yp; }
    // set, get vị trí thực tế của boss
    void set_xpos(const int& xps) { x_pos_ = xps; }
    void set_ypos(const int& yps) { y_pos_ = yps; }
    int get_x_pos() { return x_pos_; }
    int get_y_pos() { return y_pos_; }
    // Hàm hiển thị boss ra màn hình
    void Show(SDL_Renderer* des);
    // Hàm load ảnh boss
    bool LoadImg(std::string path, SDL_Renderer* screen);
    // Xử lý các hiệu ứng animation
    void set_clips();
    // get width, height frame của boss
    int get_width_frame() const { return width_frame_; }
    int get_height_frame() const { return height_frame_; }
    // Hàm trả về tọa độ frame hiện tại của boss
    SDL_Rect GetRectFrame();
    // Đặt tọa độ map theo sự di chuyển của boss
    void SetMapXY(const int map_x, const int map_y) { map_x_ = map_x, map_y_ = map_y; };
    // Hàm kiểm tra boss với map
    void CheckToMap(Map& g_map);
    // Hàm tính toán ra thông số của map khi boss di chuyển
    void CenterEntityOnMap(Map& g_map);
    // Cập nhật di chuyển cho boss
    void DoPlayer(Map& g_map);
    // Hàm hồi sinh lại boss
    void InitPlayer();
    // get, set băng đạn của boss
    std::vector<BulletObject*> get_bullet_list() const { return bullet_list_; }
    void set_bullet_list(const std::vector<BulletObject*>& am_list) { bullet_list_ = am_list; }
    // Khởi tọa các thông số viên đạn của boss
    void InitBullet(SDL_Renderer* screen);
    // Xử lý viên đạn khi hiển thị ra màn hình
    void MakeBullet(SDL_Renderer* des, const int& x_limit, const int& y_limit);
    // Hàm xóa viên đạn bất kỳ
    void RemoveBullet(const int& idx);
    // Đặt lại tọa độ cho viên đạn của boss
    void ResetBullet(BulletObject* p_bullet);
private:
    // Kiểm tra giới hạn của bản đồ
    int map_x_;
    int map_y_;
    // Boss có di chuyển trên mặt đất
    int on_ground_;
    int think_time_;
    // Quản lý việc di chuyển
    Input input_type_;
    int frame_; // Chỉ số frame của boss
    SDL_Rect frame_clip_[FRAME_NUM_32];// số khung hình cho 1 trạng thái động của boss
    // Tọa độ thực tế của boss
    int x_pos_;
    int y_pos_;
    // Tốc độ di chuyển của boss theo chiều x, y
    float x_val_;
    float y_val_;
    // width, height frame
    int width_frame_;
    int height_frame_;
    // Băng đạn của boss
    std::vector<BulletObject*> bullet_list_;
};
#endif