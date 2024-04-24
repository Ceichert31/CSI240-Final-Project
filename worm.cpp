//
// Created by Christopher Eichert on 4/9/2024.
//

#include "worm.h"

//Constructor
Worm::Worm(int segmentSize, int posX, int posY, int wormLength, ImVec4 wormColor){
    //Initialize
    this->segmentSize = segmentSize;
    this->posX = posX;
    this->posY = posY;
    this->wormLength = wormLength;
    this->wormColor = wormColor;


    //Create new nodes for worm length
    for (int i = 0; i < wormLength; i++){
        auto* current = new WormNode();

        //Construct segment
        current->wormBody.w = segmentSize;
        current->wormBody.h = segmentSize;

        current->wormBody.x = posX;
        current->wormBody.y = posY;

        //TODO: Add options to change direction
        //Move worm segments
        posX += segmentSize + 1;
        //posY += segmentSize;

        nodes.push_back(current);
    }
}

void WormManager::Split(int wormId, int position, ImVec4 wormSplitColor) {
//Copy all elements after position to new worm and then
    // resize the old worm to the position

    //Store old worm
    Worm oldWorm = worms[wormId];

    //Create new worm
    Worm newWorm(oldWorm.segmentSize, oldWorm.posX, oldWorm.posY, 0, wormSplitColor);

    //Populate new worm
    for (int i = position; i < oldWorm.wormLength; i++){
        auto* current = oldWorm.nodes[i];

        newWorm.nodes.push_back(current);
        newWorm.wormLength++;
    }

    //Resize old worm to end at selected position
    oldWorm.nodes.resize(position);
    oldWorm.wormLength = position;

    //Add new worm
    worms.push_back(newWorm);
}

void WormManager::CreateWorm(int segmentSize, int posX, int posY, int wormLength, ImVec4 wormColor) {
    Worm instance(segmentSize, posX, posY, wormLength, wormColor);
    worms.push_back(instance);
}
