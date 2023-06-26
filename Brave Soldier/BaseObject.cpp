
#include <iostream>
#include "BaseObject.h"

BaseObject::BaseObject()
{
	p_object_ = NULL;
	rect_.x = 0;
	rect_.y = 0;
	rect_.w = 0;
	rect_.h = 0;
}

BaseObject::~BaseObject()
{
	Free();
}

bool BaseObject::LoadImg(std::string path, SDL_Renderer* screen)
{
	Free();
	SDL_Texture* new_texture = NULL;

	SDL_Surface* Load_surface = IMG_Load(path.c_str());
	if (Load_surface != NULL) {

		// Xóa nền của ảnh (VD ảnh của một nhân vật)
		SDL_SetColorKey(Load_surface, SDL_TRUE, SDL_MapRGB(Load_surface->format, COLOR_KEY_R, COLOR_KEY_G, COLOR_KEY_B));
		new_texture = SDL_CreateTextureFromSurface(screen, Load_surface);
		if (new_texture != NULL) {
			rect_.w = Load_surface->w;
			rect_.h = Load_surface->h;
		}
		SDL_FreeSurface(Load_surface); // Xóa Load_surface
	}
	p_object_ = new_texture;
	return p_object_ != NULL;

}
//				load ảnh lên đối tượng (nền) ?
void BaseObject::Render(SDL_Renderer* des, const SDL_Rect* clip/* = NULL*/)
{
	SDL_Rect renderQuad = { rect_.x ,rect_.y ,rect_.w ,rect_.h }; // Kích thước và vị trí của image
	if (clip != NULL) {
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}
	SDL_RenderCopy(des, p_object_, clip, &renderQuad); // Đẩy các thông số của p_object_ lên des
}
void BaseObject::Free() {
	if (p_object_ != NULL) {
		SDL_DestroyTexture(p_object_);
		p_object_ = NULL;
		//rect_.x = 0;
		//rect_.y = 0;
		rect_.w = 0;
		rect_.h = 0;
	}

}




