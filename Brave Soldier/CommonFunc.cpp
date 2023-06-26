
#include <istream>
#include "CommonFunc.h"
#include "TextObject.h"

bool SDLCommonFunc::CheckCollision(const SDL_Rect& object1, const SDL_Rect& object2)
{
    int left_a = object1.x;
    int right_a = object1.x + object1.w;
    int top_a = object1.y;
    int bottom_a = object1.y + object1.h;

    int left_b = object2.x;
    int right_b = object2.x + object2.w;
    int top_b = object2.y;
    int bottom_b = object2.y + object2.h;

    // Case 1: size object 1 < size object 2
    if (left_a > left_b && left_a < right_b)
    {
        if (top_a > top_b && top_a < bottom_b)
        {
            return true;
        }
    }

    if (left_a > left_b && left_a < right_b)
    {
        if (bottom_a > top_b && bottom_a < bottom_b)
        {
            return true;
        }
    }

    if (right_a > left_b && right_a < right_b)
    {
        if (top_a > top_b && top_a < bottom_b)
        {
            return true;
        }
    }

    if (right_a > left_b && right_a < right_b)
    {
        if (bottom_a > top_b && bottom_a < bottom_b)
        {
            return true;
        }
    }

    // Case 2: size object 1 < size object 2
    if (left_b > left_a && left_b < right_a)
    {
        if (top_b > top_a && top_b < bottom_a)
        {
            return true;
        }
    }

    if (left_b > left_a && left_b < right_a)
    {
        if (bottom_b > top_a && bottom_b < bottom_a)
        {
            return true;
        }
    }

    if (right_b > left_a && right_b < right_a)
    {
        if (top_b > top_a && top_b < bottom_a)
        {
            return true;
        }
    }

    if (right_b > left_a && right_b < right_a)
    {
        if (bottom_b > top_a && bottom_b < bottom_a)
        {
            return true;
        }
    }

    // Case 3: size object 1 = size object 2
    if (top_a == top_b && right_a == right_b && bottom_a == bottom_b)
    {
        return true;
    }

    return false;
}

int SDLCommonFunc::ShowMenu(SDL_Renderer* g_screen, TTF_Font* font,
    const std::string& menu1,
    const std::string& menu2,
    const std::string& img_name)
{
    char* ch1 = (char*)menu1.c_str();
    char* ch2 = (char*)menu2.c_str();
    char* img_file = (char*)img_name.c_str();

    int size1 = menu1.length();
    int size2 = menu2.length();

    int time = 0;
    int x = 0;
    int y = 0;
    const int kMenuNum = 2;
    char* labels[kMenuNum];

    labels[0] = new char[size1 + 1];
    labels[1] = new char[size2 + 1];

    strcpy_s(labels[0], size1 + 1, ch1);
    strcpy_s(labels[1], size2 + 1, ch2);

    SDL_Texture* menu[kMenuNum];
    bool selected[kMenuNum] = { 0, 0 };
    SDL_Color color[2] = { { 255, 255, 255 },{ 255, 0, 0 } };

    TextObject text_object[kMenuNum];
    text_object[0].SetText(labels[0]);
    text_object[0].SetColor(color[0].r, color[0].g, color[0].b);

    text_object[0].LoadFromRenderText(font, g_screen);

    text_object[1].SetText(labels[1]);
    text_object[1].SetColor(color[0].r, color[0].g, color[0].b);
    text_object[1].LoadFromRenderText(font, g_screen);


    SDL_Rect pos[kMenuNum];
    pos[0].x = SCREEN_WIDTH * 0.3 - 150;
    pos[0].y = SCREEN_HEIGHT * 0.8 - 40;
    pos[1].x = SCREEN_WIDTH * 0.3 - 150;
    pos[1].y = SCREEN_HEIGHT * 0.8 + 40;

    BaseObject gBackground;
    bool ret = gBackground.LoadImg(img_file, g_screen);

    SDL_Event event;
    while (1)
    {
        time = SDL_GetTicks();
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                text_object[0].Free();
                text_object[0].Free();
                gBackground.Free();
                return 1;
            case SDL_MOUSEMOTION:
                x = event.motion.x;
                y = event.motion.y;
                for (int i = 0; i < kMenuNum; ++i)
                {
                    if (x >= pos[i].x && x <= pos[i].x + pos[i].w &&
                        y >= pos[i].y && y <= pos[i].y + pos[i].h)
                    {
                        if (!selected[i])
                        {
                            selected[i] = 1;
                            text_object[i].SetText(labels[i]);
                            text_object[i].SetColor(color[1].r, color[1].g, color[1].b);
                            text_object[i].LoadFromRenderText(font, g_screen);
                        }
                    }
                    else
                    {
                        if (selected[i])
                        {
                            selected[i] = 0;
                            text_object[i].Free();
                            text_object[i].SetText(labels[i]);
                            text_object[i].SetColor(color[0].r, color[0].g, color[0].b);
                            text_object[i].LoadFromRenderText(font, g_screen);
                        }
                    }
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                x = event.button.x;
                y = event.button.y;
                for (int i = 0; i < kMenuNum; ++i) {
                    if (x >= pos[i].x && x <= pos[i].x + pos[i].w &&
                        y >= pos[i].y && y <= pos[i].y + pos[i].h)
                    {
                        text_object[0].Free();
                        text_object[1].Free();
                        gBackground.Free();
                        return i;
                    }
                }
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE)
                {
                    text_object[0].Free();
                    text_object[1].Free();
                    gBackground.Free();
                    return 1;
                }
            }
        }

        gBackground.Render(g_screen, NULL);

        for (int i = 0; i < kMenuNum; ++i)
        {
            text_object[i].RenderText(g_screen, pos[i].x, pos[i].y);
            pos[i].w = text_object[i].GetWidth();
            pos[i].h = text_object[i].GetHeight();
        }


        SDL_RenderPresent(g_screen);
        if (1000 / 30 > (SDL_GetTicks() - time))
            SDL_Delay(1000 / 30 - (SDL_GetTicks() - time));
    }

    return 0;
}

int SDLCommonFunc::ShowMenuHelp(SDL_Renderer* g_screen, TTF_Font* font, const std::string& menuHelp, const std::string& img_name)
{
    char* ch1 = (char*)menuHelp.c_str(); // Play game

    char* img_file = (char*)img_name.c_str();

    int size1 = menuHelp.length();

    int time = 0;
    int x = 0;
    int y = 0;
    //const int kMenuNum = 2;
    const int kMenuNum = 1;
    char* labels[kMenuNum];

    labels[0] = new char[size1 + 1];

    strcpy_s(labels[0], size1 + 1, ch1); // copy chuỗi ch1 vào labels[0]

    SDL_Texture* menu[kMenuNum];
    bool selected[kMenuNum] = { 0 };
    // Ban đầu chữ màu trắng -> click vào thì chuyển sang màu đỏ
    SDL_Color color[2] = { { 255, 255, 255 },{ 255, 0, 0 } };

    TextObject text_object[kMenuNum];
    text_object[0].SetText(labels[0]);
    text_object[0].SetColor(color[0].r, color[0].g, color[0].b);
    text_object[0].LoadFromRenderText(font, g_screen);
    
    SDL_Rect pos[kMenuNum];
    pos[0].x = 0;
    pos[0].y = 10;

    const int conNum = 9;
    // Nội dung hướng dẫn sử dụng
    std::string con[conNum];
    con[0] = "ENTER              ON/OFF MUSIC";
    con[1] = "ESC                PAUSE GAME ?";
    con[2] = "LEFT               LEFT  ARROW";
    con[3] = "RIGHT              RIGHT ARROW";
    con[4] = "SIT                DOWN  ARROW";
    con[5] = "JUMP               RIGHT CLICK";
    con[6] = "SHOOT              LEFT  CLICK";
    con[7] = "CHANGE BULLET      F OR G";
    con[8] = "(IF YOU HAVE MORE THAN 1 BULLET STYLE)";

    TextObject text_con[conNum];
    for (int i = 0; i < conNum; i++)
    {
        text_con[i].SetText(con[i]);
        text_con[i].SetColor(color[0].r, color[0].g, color[0].b);
        text_con[i].LoadFromRenderText(font, g_screen);
    }

    SDL_Rect posCon[conNum];
    for (int i = 0; i < conNum; i++)
    {
        posCon[i].x = SCREEN_WIDTH /3.4;
        posCon[i].y = SCREEN_HEIGHT/3 + 30*i;
    }

    BaseObject gBackground;
    bool ret = gBackground.LoadImg(img_file, g_screen);

    SDL_Event event;
    while (1)
    {
        time = SDL_GetTicks();
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                text_object[0].Free();
                for (int i = 0; i < conNum; i++)
                {
                    text_con[i].Free();
                }
                gBackground.Free();
                return 0;
            case SDL_MOUSEMOTION:
                x = event.motion.x;
                y = event.motion.y;
                for (int i = 0; i < kMenuNum; ++i)
                {
                    if (x >= pos[i].x && x <= pos[i].x + pos[i].w &&
                        y >= pos[i].y && y <= pos[i].y + pos[i].h)
                    {
                        if (!selected[i])
                        {
                            selected[i] = 1;
                            text_object[i].SetText(labels[i]);
                            text_object[i].SetColor(color[1].r, color[1].g, color[1].b);
                            text_object[i].LoadFromRenderText(font, g_screen);
                        }
                    }
                    else
                    {
                        if (selected[i])
                        {
                            selected[i] = 0;
                            text_object[i].Free();
                            text_object[i].SetText(labels[i]);
                            text_object[i].SetColor(color[0].r, color[0].g, color[0].b);
                            text_object[i].LoadFromRenderText(font, g_screen);
                        }
                    }
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                x = event.button.x;
                y = event.button.y;
                for (int i = 0; i < kMenuNum; ++i) {
                    if (x >= pos[i].x && x <= pos[i].x + pos[i].w &&
                        y >= pos[i].y && y <= pos[i].y + pos[i].h)
                    {
                        text_object[0].Free();
                        for (int i = 0; i < conNum; i++)
                        {
                            text_con[i].Free();
                        }
                        gBackground.Free();
                        return i;
                    }
                }
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE)
                {
                    text_object[0].Free();
                    for (int i = 0; i < conNum; i++)
                    {
                        text_con[i].Free();
                    }
                    gBackground.Free();
                    return 0;
                }
            }
        }

        gBackground.Render(g_screen, NULL);

        for (int i = 0; i < kMenuNum; ++i)
        {
            text_object[i].RenderText(g_screen, pos[i].x, pos[i].y);
            pos[i].w = text_object[i].GetWidth();
            pos[i].h = text_object[i].GetHeight();
        }

        for (int i = 0; i < conNum; ++i)
        {
            text_con[i].RenderText(g_screen, posCon[i].x, posCon[i].y);
            posCon[i].w = text_con[i].GetWidth();
            posCon[i].h = text_con[i].GetHeight();
        }

        SDL_RenderPresent(g_screen);
        if (1000 / 30 > (SDL_GetTicks() - time))
            SDL_Delay(1000 / 30 - (SDL_GetTicks() - time));
    }

    return 0;
}

int SDLCommonFunc::ShowMenuFinal(SDL_Renderer* g_screen, TTF_Font* font,
    const std::string& menu1,
    const std::string& menu2,
    const std::string& menu3,
    const std::string& img_name)
{
    char* ch1 = (char*)menu1.c_str(); // Play game
    char* ch2 = (char*)menu2.c_str(); // Help
    char* ch3 = (char*)menu3.c_str(); // Exit
    char* img_file = (char*)img_name.c_str();

    int size1 = menu1.length();
    int size2 = menu2.length();
    int size3 = menu3.length();

    int time = 0;
    int x = 0;
    int y = 0;
    //const int kMenuNum = 2;
    const int kMenuNum = 3;
    char* labels[kMenuNum];

    labels[0] = new char[size1 + 1];
    labels[1] = new char[size2 + 1];
    labels[2] = new char[size3 + 1];

    strcpy_s(labels[0], size1 + 1, ch1); // copy chuỗi ch1 vào labels[0]
    strcpy_s(labels[1], size2 + 1, ch2);
    strcpy_s(labels[2], size3 + 1, ch3);

    SDL_Texture* menu[kMenuNum];
    bool selected[kMenuNum] = { 0, 0, 0 };
    // Ban đầu chữ màu trắng -> click vào thì chuyển sang màu đỏ
    SDL_Color color[2] = { { 255, 255, 255 },{ 255, 0, 0 } };

    TextObject text_object[kMenuNum];
    text_object[0].SetText(labels[0]);
    text_object[0].SetColor(color[0].r, color[0].g, color[0].b);
    text_object[0].LoadFromRenderText(font, g_screen);

    text_object[1].SetText(labels[1]);
    text_object[1].SetColor(color[0].r, color[0].g, color[0].b);
    text_object[1].LoadFromRenderText(font, g_screen);

    text_object[2].SetText(labels[2]);
    text_object[2].SetColor(color[0].r, color[0].g, color[0].b);
    text_object[2].LoadFromRenderText(font, g_screen);

    SDL_Rect pos[kMenuNum];
    pos[0].x = SCREEN_WIDTH * 0.3 - 150;
    pos[0].y = SCREEN_HEIGHT * 0.8 - 150;

    pos[1].x = SCREEN_WIDTH * 0.3 - 150;
    pos[1].y = SCREEN_HEIGHT * 0.8 - 70;

    pos[2].x = SCREEN_WIDTH * 0.3 - 150;
    pos[2].y = SCREEN_HEIGHT * 0.8 + 10;

    BaseObject gBackground;
    bool ret = gBackground.LoadImg(img_file, g_screen);

    SDL_Event event;
    while (1)
    {
        time = SDL_GetTicks();
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                text_object[0].Free();
                text_object[0].Free();
                gBackground.Free();
                return 2;
            case SDL_MOUSEMOTION:
                x = event.motion.x;
                y = event.motion.y;
                for (int i = 0; i < kMenuNum; ++i)
                {
                    if (x >= pos[i].x && x <= pos[i].x + pos[i].w &&
                        y >= pos[i].y && y <= pos[i].y + pos[i].h)
                    {
                        if (!selected[i])
                        {
                            selected[i] = 1;
                            text_object[i].SetText(labels[i]);
                            text_object[i].SetColor(color[1].r, color[1].g, color[1].b);
                            text_object[i].LoadFromRenderText(font, g_screen);
                        }
                    }
                    else
                    {
                        if (selected[i])
                        {
                            selected[i] = 0;
                            text_object[i].Free();
                            text_object[i].SetText(labels[i]);
                            text_object[i].SetColor(color[0].r, color[0].g, color[0].b);
                            text_object[i].LoadFromRenderText(font, g_screen);
                        }
                    }
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                x = event.button.x;
                y = event.button.y;
                for (int i = 0; i < kMenuNum; ++i) {
                    if (x >= pos[i].x && x <= pos[i].x + pos[i].w &&
                        y >= pos[i].y && y <= pos[i].y + pos[i].h)
                    {
                        text_object[0].Free();
                        text_object[1].Free();
                        text_object[2].Free();
                        gBackground.Free();
                        return i;
                    }
                }
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE)
                {
                    text_object[0].Free();
                    text_object[1].Free();
                    text_object[2].Free();
                    gBackground.Free();
                    return 2;
                }
            }
        }

        gBackground.Render(g_screen, NULL);

        for (int i = 0; i < kMenuNum; ++i)
        {
            text_object[i].RenderText(g_screen, pos[i].x, pos[i].y);
            pos[i].w = text_object[i].GetWidth();
            pos[i].h = text_object[i].GetHeight();
        }


        SDL_RenderPresent(g_screen);
        if (1000 / 30 > (SDL_GetTicks() - time))
            SDL_Delay(1000 / 30 - (SDL_GetTicks() - time));
    }

    return 0;
}