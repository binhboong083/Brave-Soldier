﻿
#ifndef BASE_OBJECT_H_
#define BASE_OBJECT_H_

#include "CommonFunc.h"

class BaseObject {
public:
	BaseObject(); // Hàm khởi tạo
	~BaseObject(); // Hàm hủy
	// Hàm set kích thước image rect_
	void SetRect(const int& x, const int& y) { rect_.x = x; rect_.y = y; }
	// Hàm get image vs Object
	SDL_Rect GetRect() const { return rect_; }
	SDL_Texture* GetObject() const { return p_object_; }

	// Hàm loading vs Render image
	virtual bool LoadImg(std::string path, SDL_Renderer* screen); // Hàm thêm virtual để phục vụ đa hình
	void Render(SDL_Renderer* des, const SDL_Rect* clip = NULL);
	void Free(); // Giải phóng
protected:
	SDL_Texture* p_object_; // Lưu trữ các image
	SDL_Rect rect_; // Lưu kích thước image
};

#endif // !BASE_OBJECT_H_

