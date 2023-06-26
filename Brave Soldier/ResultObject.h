
#ifndef RESULT_OBJECT_H_
#define RESULT_OBJECT_H_

#include "CommonFunc.h"
#include "BaseObject.h"

#define  NUM_FRAME_RESULT 32

class ResultObject : public BaseObject
{
public:
	ResultObject();
	~ResultObject();

	void set_clips();
	void set_frame(const int& fr) { frame_ = fr; }
	virtual bool LoadImg(std::string path, SDL_Renderer* screen);
	void Show(SDL_Renderer* screen);
	int get_frame_width() const { return frame_width_; }
	int get_frame_height() const { return frame_height_; }
private:
	int frame_width_;
	int frame_height_;
	unsigned int frame_;
	SDL_Rect frame_clip_[NUM_FRAME_RESULT];
};

#endif // RESULT_OBJECT_H_

