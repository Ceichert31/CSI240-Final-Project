//
// Created by newus on 4/9/2024.
//

#include "worm.h"

//Constructor
Worm::Worm(int segmentSize, int posX, int posY, int wormLength){
    this->segmentSize = segmentSize;
    this->posX = posX;
    this->posY = posY;
    this->wormLength = wormLength;

    head = new WormNode();

    WormNode* current = head;
    //Create new nodes for worm length
    for (int i = 0; i < wormLength; i++){
        //Construct segment
        current->wormBody.w = segmentSize;
        current->wormBody.h = segmentSize;

        current->wormBody.x = posX;
        current->wormBody.y = posY;

        //Create node
        current->nextNode = new WormNode();

        //Get next node
        WormNode* next = current->nextNode;
        current = next;

        //Move worm segments
        posX += segmentSize + 1;
        //posY += segmentSize;
    }
}

Worm::~Worm() {
    //Delete all nodes
    WormNode* current = head;
    while (current != nullptr){
        WormNode* next = current->nextNode;
        delete current;
        current = next;
    }
    current = nullptr;
    head = nullptr;
}

///Splits a worm into two at a given node
Worm* Worm::SplitAt(Worm::WormNode* node) {
    Worm* newWorm = new Worm(segmentSize, posX, posY, 0);
    newWorm->head = nullptr;

    if (node->nextNode == nullptr)
        return nullptr;

    //Start at split point
    WormNode* current = node;

    //Everytime we want to add a new node, we have to remove the reference to it
    // and its reference to the next node first

    //Iterate through the rest of the old worm
    //Add the old data to the new worm
    while (current != nullptr){
        WormNode* next = current->nextNode;

        current->nextNode = nullptr;

        newWorm->AddNode(current);

        current = next;
    }
    return newWorm;
}

//Worm segment number, ie: 0, 1, 2
//Create a new worm
//Add all nodes after the split point to new worm
//Delete node that was split
//Delete all nodes after split node in original worm
//

void Worm::AddNode(Worm::WormNode *node) {

    //bootstrap case
    if (head == nullptr){
        head = node;
        return;
    }



    WormNode* current = head;

    //Iterate to end of new worm
    while (current->nextNode != nullptr) {
        current = current->nextNode;
    }

    //Add desired node to end of linked list
    current->nextNode = node;

    //Increment worm length
    wormLength++;
}

///Creates a new worm given certain parameters
Worm *CreateWorm(int segmentSize, int posX, int posY, int wormLength, ImVec4 wormColor) {
    //Create new worm
    Worm* wormInstance = new Worm(segmentSize, posX, posY, wormLength);
    wormInstance->wormColor = wormColor;
    return wormInstance;
}
