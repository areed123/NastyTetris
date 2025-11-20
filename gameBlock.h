//
// Created by aiden on 11/13/2025.
//

#ifndef NASTYTETRIS_GAMEBLOCK_H
#define NASTYTETRIS_GAMEBLOCK_H
class gameBlock {
    public:
    bool stick[2][4]={{1,1,1,1}, {0,0,0,0}}; //0
    bool skew[2][4] = {{1,1,0,},{0,1,1}}; //1
    bool square[2][4] = {{1,1},{1,1}}; //2
    bool Tblock[2][4] = {{1,1,1},{0,1,0}}; //3
    bool LBlock[2][4] = {{1,1,1},{1,0,0}}; //4
    bool ReverseL[2][4] = {{1,1,1},{0,0,1}}; //5
    bool ReverseSkew[2][4] = {{0,1,1},{1,1,0}}; //6
    gameBlock();
    gameBlock(int choice);
    int getXPos();
    int getYPos();
    void setXPos(int XPos);
    void setYPos(int YPos);
    bool (*block)[2][4];

    private:
    int posX,posY;

};
#endif //NASTYTETRIS_GAMEBLOCK_H