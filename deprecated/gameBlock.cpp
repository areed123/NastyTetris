//
// Created by aiden on 11/13/2025.
//
#include "gameBlock.h"
gameBlock::gameBlock() {
    block=&stick;
    posX=0;
    posY=0;


}
gameBlock::gameBlock(int choice) {
    switch (choice){
        case 0:
            block = &stick;
            break;
        case 1:
            block = &skew;
            break;
        case 2:
            block = &square;
            break;
        case 3:
            block = &Tblock;
            break;
        case 4:
            block = &LBlock;
            break;
        case 5:
            block = &ReverseL;
            break;
        case 6:
            block = &ReverseSkew;
            break;
        default:
            block = &square;
            break;
    }
    posX=0;
    posY=0;
}
int gameBlock::getXPos() {
    return posX;
}
int gameBlock::getYPos() {
    return posY;
}
void gameBlock::setXPos(int Xpos) {
    posX=Xpos;

}
void gameBlock::setYPos(int YPos) {
    posY=YPos;
}
