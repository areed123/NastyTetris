#include <iostream>
#include<SDL3/SDL.h>
#include<SDL3_ttf/SDL_ttf.h>
#include "SDL3_image/SDL_image.h"
#include "vendored/SDL_ttf/include/SDL3_ttf/SDL_ttf.h"
SDL_Window* gWindow;
SDL_Renderer* gRenderer;
SDL_Texture* blockTexture;
SDL_Texture* gridTexture;
SDL_Texture nextTexture;
SDL_Texture scoreTexture;
TTF_Font* gFont{nullptr};
bool gArea[24][10];
bool gRows[24];
int score{0};
const int kWindowWidth{640};
const int kWindowHeight{720};
const int kFps{60};
 bool stick[4][4]={{1,1,1,1}, {0,0,0,0},{0,0,0,0},{0,0,0,0}}; //0
 bool skew[4][4] = {{1,1,0,0},{0,1,1,0},{0,0,0,0},{0,0,0,0}}; //1
 bool square[4][4] = {{1,1,0,0},{1,1,0,0},{0,0,0,0},{0,0,0,0}}; //2
 bool Tblock[4][4] = {{1,1,1},{0,1,0,0},{0,0,0,0},{0,0,0,0}}; //3
 bool LBlock[4][4] = {{1,1,1,0},{1,0,0,0},{0,0,0,0},{0,0,0,0}}; //4
 bool ReverseL[4][4] = {{1,1,1,0},{0,0,1,0},{0,0,0,0},{0,0,0,0}}; //5
 bool ReverseSkew[4][4] = {{0,1,1,0},{1,1,0,0},{0,0,0,0},{0,0,0,0}}; //6
constexpr SDL_FRect block{0,0,10,10};
bool init()
{
    for (int i=0; i<10; i++)
    {
        for (int j=0; j<24; j++)
        {
            gArea[j][i]=false;
        }
    }
    bool success{true};
    if (SDL_Init(SDL_INIT_VIDEO) == false)
    {
        SDL_Log("SDL INITIALIZATION FAILED! ERROR: %s\n",SDL_GetError());
        success=false;

    }
    else
    {
        if (SDL_CreateWindowAndRenderer("Nasty Tetris Game", kWindowWidth, kWindowHeight, 0, &gWindow, &gRenderer)==false)
        {
            SDL_Log("SDL WINDOW CREATION FAILED! ERROR: %s\n",SDL_GetError());
            success=false;
        }

    }
    return success;
}
/*bool loadFont(std::string path)
{
    bool success{true};
    if (gFont=TTF_OpenFont(path.c_str(),28); gFont==nullptr)
    {
        success=false;
    }
    else
    {
        SDL_Color textColor={0x00,0x00,0x00,0xff};
        if (scoreTexture.loadFromRenderedText("Score: ")==false)
        {
            success=false;
        }

    }
    return success;
}*/
bool loadMedia(std::string path)
{
    bool success{true};

    if (SDL_Surface* loadedSurface = IMG_Load(path.c_str()); loadedSurface == nullptr)
    {
        SDL_Log("IMAGE LOADING FAILED! ERROR: %s\n",SDL_GetError());
        success=false;
    }
    else
    {
        if (blockTexture=SDL_CreateTextureFromSurface(gRenderer,loadedSurface); blockTexture==nullptr)
        {
            SDL_Log("TEXTURE CREATION FAILED! ERROR %s \n",SDL_GetError());
            success=false;
        }

        if (SDL_SetSurfaceColorKey(loadedSurface, true, SDL_MapSurfaceRGB(loadedSurface, 0xFF,0xFF,0x00)) == false )
        {
            SDL_Log("UNABLE TO COLOR KEY! ERROR: %s\n",SDL_GetError());
            success=false;
        }
        else
        {
            if (gridTexture=SDL_CreateTextureFromSurface(gRenderer,loadedSurface); gridTexture==nullptr)
            {
                SDL_Log("TEXTURE CREATION FAILED! ERROR %s \n",SDL_GetError());
                success=false;
            }


        }
        SDL_DestroySurface(loadedSurface);
    }

    return(success);
}
int placeBlock(bool (*blk)[4][4])
{
    bool placed{true};
    int k=-1;
    do
    {
        k++;
        placed = true;
        for (int i=0; i<4; i++)
        {
            for (int j=0; j<4; j++)
            {
                if (*blk[i][j]==true)
                {
                    if (gArea[i][k+j]==true)
                    {
                        placed=false;
                    }
                }
            }
        }
    }while (placed == false && k<10);
    if (placed==false)
    {
        k=-1;
    }
    return(k);
}
bool checkCollisions(int xPos, int yPos, bool (*blk)[4][4])
{
    SDL_FRect gameBlock;
    bool collision{false};
    for (int i=0; i<4; i++)
    {
        for (int j=0; j<4; j++)
        {
            if ((*blk)[i][j]==true)
            {
                if (gArea[i+yPos][j+xPos]==true||j+xPos>=10||j+xPos<0||i+yPos>=24)
                {
                     collision = true;
                }
                /*else if (collision==false)
                {
                    gameBlock={100.f+24.f*static_cast<float>(j+xPos),100.f+24.f*static_cast<float>(i+yPos),24.f,24.f};
                    SDL_RenderTexture(gRenderer,blockTexture,nullptr,&gameBlock);
                }*/
            }
        }
    }
    return(collision);
}
bool renderBlock(int xPos, int yPos, bool (*blk)[4][4])
{
    SDL_FRect gameBlock;
    for (int i=0; i<4; i++)
    {
        for (int j=0; j<4; j++)
        {
            if ((*blk)[i][j]==true)
            {
                    gameBlock={100.f+24.f*static_cast<float>(j+xPos),100.f+24.f*static_cast<float>(i+yPos),24.f,24.f};
                    SDL_RenderTexture(gRenderer,blockTexture,nullptr,&gameBlock);

            }
        }
    }
    return true;
}
bool renderBlockOnScreen(int xPos, int yPos, bool (*blk)[4][4])
{
    SDL_FRect gameBlock;
    for (int i=0; i<4; i++)
    {
        for (int j=0; j<4; j++)
        {
            if ((*blk)[i][j]==true)
            {
                gameBlock={static_cast<float>(xPos)+24.f*static_cast<float>(j),static_cast<float>(yPos)+24.f*static_cast<float>(i),24.f,24.f};
                SDL_RenderTexture(gRenderer,blockTexture,nullptr,&gameBlock);

            }
        }
    }
    return true;
}
void copyBlock(bool (*blk)[4][4], bool (*copyBlock)[4][4])
{
    for (int i=0;i<4;i++)
    {
        for (int j=0; j<4; j++)
        {
            (*blk)[i][j]=(*copyBlock)[i][j];
        }
    }

}
void randBlock(bool (*blk)[4][4])
{
    Uint64 choice=SDL_GetTicks()%7;
    switch (choice){
    case 0:
        copyBlock(blk, &stick);
        break;
    case 1:
        copyBlock(blk, &skew);
        break;
    case 2:
        copyBlock(blk, &square);
        break;
    case 3:
        copyBlock(blk, &Tblock);
        break;
    case 4:
        copyBlock(blk,&LBlock );
        break;
    case 5:
        copyBlock(blk, &ReverseSkew);
        break;
    case 6:
        copyBlock(blk, &ReverseL);
        break;
    }
}
void setBlock(int xPos, int yPos, bool (*blk)[4][4])
{
    for (int i=0; i<4; i++)
    {
        for (int j=0; j<4; j++)
        {
            if ((*blk)[i][j]==true)
                gArea[yPos+i][xPos+j]=true;
        }
    }
}
int checkRows()
{
    int fullRows{0};
    bool full{true};
    for (int i=0;i<24;i++)
    {
        full=true;
        for (int j=0; j<10; j++)
        {
            if (gArea[i][j]==false)
                full=false;
        }
        if (full==true)
        {
            fullRows++;
            gRows[i]=true;
        }
    }
    return fullRows;
}
void clearFullRows()
{
    for (int i=0; i<24; i++)
    {
        if (gRows[i]==true)
        {
            for (int j=0; j<10; j++)
            {
                gArea[i][j]=false;
            }
        }
    }
}
void dropClearedRows()
{
    for (int i=1; i<24; i++)
    {
        if (gRows[i]==true)
        {
            for (int j=0; j<10; j++)
            {
                for (int i2=i; i2>0;i2--)
                {
                    gArea[i2][j]=gArea[i2-1][j];

                }
            }
            gRows[i]=false;
        }
    }
}
void rotateBlockRight(bool (*blk)[4][4], bool (*rblk)[4][4])
{
    for (int i=0; i<4;i++)
    {
        for (int j=0; j<4;j++)
        {
            (*rblk)[j][3-i]=(*blk)[i][j];
        }
    }
}
void rotateBlockLeft(bool (*blk)[4][4], bool (*rblk)[4][4])
{
    for (int i=0; i<4;i++)
    {
        for (int j=0; j<4;j++)
        {
            (*rblk)[3-j][i]=(*blk)[i][j];
        }
    }
}
Uint64 calculateScore(int rows)
{
    Uint64 calScore{0};
    return(calScore);

}
// TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.
int main() {
    // TIP Press <shortcut actionId="RenameElement"/> when your caret is at the <b>lang</b> variable name to see how CLion can help you rename it.
    int exitCode{0};
    Uint64 prevTicks{0};
    bool ready{true};
    Uint64 frameCount = 0;
    if (init() == false)
    {
        SDL_Log("Unable to initialize");
        exitCode = 1;
    }

    else
    {
        loadMedia("../Textures/TetrisBlock.png");
        bool quit{ false };
        bool collision{false};
        SDL_Event e;
        SDL_zero(e);
        Uint64 renderingNs;
        int posX{0};
        int posY{0};
        int preX{0};
        int preY{0};
        int rowsCleared{0};
        bool (currBlock)[4][4];
        bool (rotBlock)[4][4];
        bool (nextBlock)[4][4];
        randBlock(&currBlock);
        randBlock(&nextBlock);
        copyBlock(&rotBlock, &square);
        while (quit == false)
        {
            while (SDL_PollEvent(&e) == true)
            {
                if (e.type == SDL_EVENT_QUIT)
                {
                    quit = true;
                }
                else if (e.type== SDL_EVENT_KEY_DOWN)
                {
                    //Implement keyboard presses
                    switch (e.key.key)
                    {
                    case SDLK_LEFT:
                        if (!checkCollisions(posX-1,posY,&currBlock))
                        posX--;
                        break;
                    case SDLK_RIGHT:
                        if (!checkCollisions(posX+1,posY,&currBlock))
                            posX++;
                        break;
                    case SDLK_SPACE:
                        if (!checkCollisions(posX,posY+1,&currBlock))
                            posY++;
                        break;
                    case SDLK_E:
                        rotateBlockRight(&currBlock,&rotBlock);
                        if (!checkCollisions(posX,posY,&rotBlock))
                        {
                            copyBlock(&currBlock,&rotBlock);
                        }
                        break;
                    case SDLK_Q:
                        rotateBlockLeft(&currBlock,&rotBlock);
                        if (!checkCollisions(posX,posY,&rotBlock))
                        {
                                copyBlock(&currBlock,&rotBlock);
                        }
                        break;
                    default:
                        break;
                    }
                }

            }
            SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
            SDL_RenderClear( gRenderer );
            SDL_FRect gBlock={400,100,180,180};
            SDL_RenderTexture(gRenderer,gridTexture,nullptr,&gBlock);
            for (int i=0;i<24;i++)
            {
                for (int j=0; j<10; j++)
                {
                    gBlock={(100.f+24.f*static_cast<float>(j)),(100.f+24.f*static_cast<float>(i)),24,24};
                    //SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0xFF, 0x0F);
                    SDL_RenderTexture(gRenderer, gridTexture,nullptr,&gBlock);

                    if (gArea[i][j]==true)
                    {

                        //SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0xFF, 0x0F);
                        SDL_RenderTexture(gRenderer, blockTexture,nullptr,&gBlock);
                    }
                }
            }
            if ((rowsCleared=checkRows());rowsCleared>0)
            {
                calculateScore(rowsCleared);
                clearFullRows();
                dropClearedRows();
            }
            if (ready==true)
            {
                copyBlock(&currBlock,&nextBlock);
                randBlock(&nextBlock);
                posX=placeBlock(&currBlock);
                ready=false;

            }
            if (frameCount/720==1)
            {
                posY++;

                frameCount=0;
            }

            if (checkCollisions(posX,posY,&currBlock))
            {
                posY=preY;
                posX=preX;
                ready=true;
                setBlock(posX,posY,&currBlock);
                posX=posY=preX=preY=0;
            }
            else
            {
                renderBlock(posX,posY,&currBlock);
            }
            renderBlockOnScreen(466,166,&nextBlock);
            SDL_RenderPresent(gRenderer);
            renderingNs=SDL_GetTicksNS()-prevTicks;
            constexpr Uint64 nsPerFrame=1000000000/kFps;
            if (renderingNs<nsPerFrame)
            {
                SDL_DelayNS(nsPerFrame-renderingNs);
                prevTicks=SDL_GetTicksNS();

            }
            frameCount++;
            preX=posX;
            preY=posY;
        }
    }
    SDL_Quit();
    return exitCode;
    // TIP See CLion help at <a href="https://www.jetbrains.com/help/clion/">jetbrains.com/help/clion/</a>. Also, you can try interactive lessons for CLion by selecting 'Help | Learn IDE Features' from the main menu.
}
