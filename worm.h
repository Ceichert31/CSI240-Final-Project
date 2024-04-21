//
// Created by newus on 4/9/2024.
//

#ifndef MYPROJECT_WORM_H
#define MYPROJECT_WORM_H


#include "SDL_rect.h"
#include <iostream>
#include <algorithm>
#include <SDL.h>
#include <vector>
#include <SDL_image.h>
//#include <SDL_mixer.h>
//#include <SDL_ttf.h>
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_sdlrenderer.h"

class Worm{
    int segmentSize, posX, posY;

public:
    Worm(int segmentSize, int posX, int posY, int wormLength);
    ~Worm();

    bool isMoving = true;

    struct WormNode{
        WormNode* nextNode;

        SDL_Rect wormBody;
    };

    int wormLength;

    ImVec4 wormColor;

    WormNode* head;
};

Worm* CreateWorm(int segmentSize, int posX, int posY, int wormLength, ImVec4 wormColor);

#endif //MYPROJECT_WORM_H
