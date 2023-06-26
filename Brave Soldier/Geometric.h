
#ifndef GEOMETRIC_H_
#define GEOMETRIC_H_

#include "CommonFunc.h"

typedef struct GeometricFormat
{
public:
	// Kích thước của hình chữ nhật
	GeometricFormat(int left, int top, int width, int height)
	{
		left_ = left;
		top_ = top;
		width_ = width;
		height_ = height;
	}

	int left_;
	int top_;
	int width_;
	int height_;
};

	// Màu sắc của hình chữ nhật
typedef struct ColorData
{
public:
	ColorData(Uint8 r, Uint8 g, Uint8 b)
	{
		red_ = r;
		green_ = g;
		blue_ = b;
	}

	Uint8 red_;
	Uint8 green_;
	Uint8 blue_;

};
class Geometric
{
public:
	// Nền
	static void RenderRectangle(const GeometricFormat& geo_size, const ColorData& color_data, SDL_Renderer* screen);
	// Đường viền
	static void RenderOutLine(const GeometricFormat& geo_size, const ColorData& color_data, SDL_Renderer* screen);

};
#endif // GEOMETRIC_H_
