
#ifndef TEXT_OBJECT_H_
#define TEXT_OBJECT_H_

#include "CommonFunc.h"

class TextObject
{
public:
	TextObject();
	~TextObject();

	// Các màu cơ bản
	enum TextColor {
		RED_TEXT = 0,
		WHITE_TEXT = 1,
		BLACK_TEXT = 2
	};

	//bool LoadFromFile(std::string path);
	bool LoadFromRenderText(TTF_Font* font, SDL_Renderer* screen);
	void Free();
	void SetColor(Uint8 red, Uint8 green, Uint8 blue); // Truyền trực tiếp màu vào
	void SetColor(int type);
	//Show text lên screen
	void RenderText(SDL_Renderer* screen, int xp, int yp, 
		SDL_Rect* clip = NULL, double angle = 0.0/*góc*/,
		SDL_Point* center = NULL, 
		SDL_RendererFlip flip = SDL_FLIP_NONE /*Soi gương*/);
		
	int GetWidth() const { return width_; }
	int GetHeight() const { return height_; }

	void SetText(const std::string& text) { str_val_ = text; }
	std::string GetText() const { return str_val_; }

	// Hàm set kích thước image rect_
	void SetRect(const int& x, const int& y) { rect_.x = x; rect_.y = y; }
	SDL_Rect GetRect() const { return rect_; }
private:
	std::string str_val_;
	SDL_Color text_color_;
	SDL_Texture* texture_; // Quản lý text
	int width_;
	int height_;

	SDL_Rect rect_; // Lưu kích thước image
};


#endif // !TEXT_OBJECT_H_
