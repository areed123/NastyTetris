//
// Created by aiden on 11/13/2025.
//

#ifndef NASTYTETRIS_GAMEAREA_H
#define NASTYTETRIS_GAMEAREA_H
#include "gameBlock.h"
#include <SDL3/SDL.h>

class gameArea {
    public:
    static constexpr bool stick[2][4]={{1,1,1,1}, {0,0,0,0}}; //0
    static constexpr bool skew[2][4] = {{1,1,0,0},{0,1,1,0}}; //1
    static constexpr bool square[2][4] = {{1,1,0,0},{1,1,0,0}}; //2
    static constexpr bool Tblock[2][4] = {{1,1,1},{0,1,0,0}}; //3
    static constexpr bool LBlock[2][4] = {{1,1,1,0},{1,0,0,0}}; //4
    static constexpr bool ReverseL[2][4] = {{1,1,1,0},{0,0,1,0}}; //5
    static constexpr bool ReverseSkew[2][4] = {{0,1,1,0},{1,1,0,0}}; //6

    gameArea();
    bool moveBlock(gameBlock &activeBlock, int dir); //Move block a step down and in the direction inputted return false if block stops left=0,right=1
    bool addBlock(gameBlock &activeBlock); //Add a block to the gameArea return false if a block is already in the playArea
    bool fallBlock(gameBlock &activeBlock);
    void placeBlock(gameBlock &activeBlock);
    bool updateGame();
    bool isReady();
    bool playArea[24][10]={};
    private:

    bool trueArea[24][10]={};
    bool Ready=true;
};
#endif //NASTYTETRIS_GAMEAREA_H