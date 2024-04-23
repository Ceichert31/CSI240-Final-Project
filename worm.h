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
#include <cmath>
#include <SDL_image.h>
//#include <SDL_mixer.h>
//#include <SDL_ttf.h>
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_sdlrenderer.h"
#include "SDL_mouse.h"

class Worm{
    int segmentSize, posX, posY;

public:
    Worm(int segmentSize, int posX, int posY, int wormLength);
    ~Worm();

    struct WormNode{
        WormNode* nextNode;

        SDL_Rect wormBody;
    };

    int wormLength;

    ImVec4 wormColor;

    WormNode* head;

    Worm* SplitAt(WormNode* node);
    void AddNode(WormNode* node);
};

Worm* CreateWorm(int segmentSize, int posX, int posY, int wormLength, ImVec4 wormColor);

#endif //MYPROJECT_WORM_H
