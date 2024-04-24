//
// Created by Christopher Eichert on 4/9/2024.
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

struct WormNode{
    SDL_Rect wormBody;
};

struct Worm{
    Worm(int segmentSize, int posX, int posY, int wormLength, ImVec4 wormColor);

    std::vector<WormNode*> nodes;

    int wormLength, segmentSize, posX, posY;

    ImVec4 wormColor;
};

struct WormManager{
    std::vector<Worm> worms;
    void Split(int wormId, int position, ImVec4 wormSplitColor);
    void CreateWorm(int segmentSize, int posX, int posY, int wormLength, ImVec4 wormColor);
};


#endif //MYPROJECT_WORM_H
