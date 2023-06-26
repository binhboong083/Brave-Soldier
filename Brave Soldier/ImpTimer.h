
#ifndef IMP_TIMER_H_
#define IMP_TIMER_H_

class ImpTimer {
public:
	ImpTimer(); // Hàm khởi tạo
	~ImpTimer(); // Hàm hủy

	void start(); // Xác định thời gian bắt đầu chơi
	void stop(); // Dừng chạy thời gian 
	void paused(); // Tạm dừng thời gian chơi game
	void unpause(); // Tiếp tục thời gian chơi game

	int get_ticks(); // Trả về thời gian chơi game

	bool is_started(); // Kiểm tra game bắt đầu ?
	bool is_paused(); // Kiểm tra game tạm dừng

private:
	// Các tham số thời gian
	int start_tick_;
	int paused_tick_;

	bool is_paused_;
	bool is_started_;
};
#endif // IMP_TIMER_H_
