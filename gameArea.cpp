//
// Created by aiden on 11/13/2025.
//
#include "gameArea.h"
#include "gameBlock.h"

gameArea::gameArea() {
    Ready=true;
    for (int j=0;j<24;j++) {
        for (int k=0;k<10;k++) {
            playArea[j][k]=false;
            trueArea[j][k]=false;
        }
    }
}
bool gameArea::addBlock(gameBlock &activeBlock) {
    int posX=activeBlock.getXPos(), posY=activeBlock.getYPos();
    bool (*block)[2][4] = activeBlock.block;
    if (!Ready) {
        SDL_Log("Block Is Already Falling! \n");
        return false;
    }
    Ready = false;
    while (playArea[posY][posX]==true && posX<10) {
        SDL_Log("PosX incremented: %d \n",posX);
        posX+=1;
    }
    if (posX>=10) {
        SDL_Log("Game Over! \n");
        return false;
    }
    int i=0; //iterator for block along x axis
    int j=0; // iterator for block along y axis
    bool placed; //flag for finding a correct placement
    do{
        placed=true; // for every block location assume it can be placed until proven wrong
        i=0;
        j=0;
        while (j<2) {
            i=0;
            //go through every cell of the block being added and compare it with the playarea
            while (i<4) {

                if (*block[i][j] == true) { //if the cell of the active blcok is 1 that means there is a filled cell there
                    if (posX+i>=10) { // if the block reaches off the edge of the row then there are no other places to place the block
                        SDL_Log("posX: %d i: %d No room for block GameOver!\n", posX, i);
                        return false;
                    }
                    if (playArea[posY+j][posX+i]==1) { //if the overlapping spot is occupied then move to the next xPos
                        placed = false;
                        posX+=1;
                    }
                }
                i++;
            }
            j++;
        }

    }while (placed==false);
    activeBlock.setXPos(posX);
    activeBlock.setYPos(posY);
    return true;
}
bool gameArea::moveBlock(gameBlock &activeBlock, int dir) {
    int posX=activeBlock.getXPos(), posY=activeBlock.getYPos();
    bool (*block)[2][4] = activeBlock.block;
    if (dir==0) {posX-=1;}
    else if (dir==1) {posX+=1;}
    else {
        SDL_Log("Invalid Direction! \n");
        return false;
    }
    int i=0; //iterator for block along x axis
    int j=0; // iterator for block along y axis
    bool valid; //flag for finding a correct placement
        valid=true; // for every block location assume it can be placed until proven wrong
        i=0;
        j=0;
    if (posX<0 || posX>=10) {
        valid=false;
    }
        while (j<2&&valid==true) {
            //go through every cell of the block being added and compare it with the playarea
            i=0;
            while (i<4&&valid==true) {

                if (*block[j][i] == true) { //if the cell of the active blcok is 1 that means there is a filled cell there
                    if (posX+i>=10) { // if the block reaches off the edge of the row then there are no other places to place the block
                        valid=false;

                    }
                    else if (playArea[posY+j][posX+i]==1) { //else if the overlapping spot is occupied then keep initial xPos
                        valid = false;
                    }
                }
                i++;
            }
            j++;
        }
    if (valid) {
        activeBlock.setXPos(posX);
        activeBlock.setYPos(posY);
    }
return valid;
}
bool gameArea::fallBlock(gameBlock &activeBlock) {
    int posX=activeBlock.getXPos(), posY=activeBlock.getYPos();
    posY=posY+1;
    bool (*block)[2][4] = activeBlock.block;
    bool valid=true; //is true if there is room for block to fall one unit
    int i=0;
    int j=0;
    while (j<2) {
        i=0;
        while (i<4) {
            if (*block[j][i] == true) {
                if (posY+j>=24) {
                    valid=false;
                }
                else if (playArea[posY+j][posX+i]==1) {
                    valid = false;
                }
            }
            i++;
        }
        j++;
    }
    if (valid) {
        activeBlock.setYPos(posY);
        activeBlock.setXPos(posX);
    }
    return valid;
}
void gameArea::placeBlock(gameBlock &activeBlock) {
    int posX=activeBlock.getXPos(), posY=activeBlock.getYPos();
    bool (*block)[2][4] = activeBlock.block;
    for (int j=0;j<2;j++) {
        for (int i=0;i<4;i++) {
            if (*block[j][i] == true) {
                trueArea[posY+j][posX+i]=true;

            }
        }
    }

}
bool gameArea::updateGame() {
    bool filledRow=true;
    int filledRowCount=0;
    for (int j=0; j<24; j++) {
        filledRow=true;
        for (int i=0; i<10; i++) {
            if (trueArea[j][i]==0) {
                filledRow=false;
            }
        }
        if (filledRow) { // If the row is filled set entire row to zero increment score and set row to zero
            filledRowCount++;
            for (int i=0; i<10; i++) {
                trueArea[j][i]=false;
            }
        }
    }
    bool emptyRow=true;

    for (int j=23; j>=0; j--) {
        emptyRow=true;
        for (int i=0; i<10; i++) {
            if (trueArea[j][i]==1) {
                emptyRow=false;
            }
        }
        if (emptyRow) {
            for (int i=0; i<10; i++) {
                for (int k=j-1; k>=0; k--) {
                    if (trueArea[k][i]==true) {
                        trueArea[j][i]=true;
                        trueArea[k][j]=false;
                    }
                }
            }

        }
    }
    for (int j=0; j<24; j++) {
        for (int i=0; i<10; i++) {
            playArea[j][i]=trueArea[j][i];
        }
    }
    Ready=true;
    return Ready;
}
bool gameArea::isReady() {
    return Ready;
}
