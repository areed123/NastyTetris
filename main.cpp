#include <iostream>
#include <gameBlock.h>
//#include <gameArea.h>
#include<SDL3/SDL.h>

#include "SDL3_image/SDL_image.h"
SDL_Window* gWindow;
SDL_Renderer* gRenderer;
SDL_Texture* blockTexture;
SDL_Texture* gridTexture;

bool gArea[24][10];
int score{0};
const int kWindowWidth{640};
const int kWindowHeight{720};
const int kFps{60};
 bool stick[2][4]={{1,1,1,1}, {0,0,0,0}}; //0
 bool skew[2][4] = {{1,1,0,0},{0,1,1,0}}; //1
 bool square[2][4] = {{1,1,0,0},{1,1,0,0}}; //2
 bool Tblock[2][4] = {{1,1,1},{0,1,0,0}}; //3
 bool LBlock[2][4] = {{1,1,1,0},{1,0,0,0}}; //4
 bool ReverseL[2][4] = {{1,1,1,0},{0,0,1,0}}; //5
 bool ReverseSkew[2][4] = {{0,1,1,0},{1,1,0,0}}; //6
constexpr SDL_FRect block{0,0,10,10};
bool init()
{
    for (int i=0; i<10; i++)
    {
        for (int j=0; j<24; j++)
        {
            gArea[i][j]=false;
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
int placeBlock(bool (*blk)[2][4])
{
    bool placed{true};
    int k=-1;
    do
    {
        k++;
        placed = true;
        for (int i=0; i<2; i++)
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
bool checkCollisions(int xPos, int yPos, bool (*blk)[2][4])
{
    SDL_FRect gameBlock;
    bool collision{false};
    for (int i=0; i<2; i++)
    {
        for (int j=0; j<4; j++)
        {
            if ((*blk)[i][j]==true)
            {
                if (gArea[i+yPos][j+xPos]==true||j+xPos>=10||i+yPos>=24)
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
bool renderBlock(int xPos, int yPos, bool (*blk)[2][4])
{
    SDL_FRect gameBlock;
    for (int i=0; i<2; i++)
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
}
void setBlock(int xPos, int yPos, bool (*blk)[2][4])
{
    for (int i=0; i<2; i++)
    {
        for (int j=0; j<4; j++)
        {
            if ((*blk)[i][j]==true)
                gArea[yPos+i][xPos+j]=true;
        }
    }
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
        loadMedia("../../Textures/TetrisBlock.png");
        bool quit{ false };
        bool collision{false};
        SDL_Event e;
        SDL_zero(e);
        Uint64 renderingNs;
        int posX{0};
        int posY{0};
        bool (*currBlock)[2][4]=&square;
        while (quit == false)
        {
            while (SDL_PollEvent(&e) == true)
            {
                if (e.type == SDL_EVENT_QUIT)
                {
                    quit = true;
                }

            }
            SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
            SDL_RenderClear( gRenderer );
            SDL_FRect gBlock={100,100,10,10};
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
            if (ready==true)
            {
                posX=placeBlock(currBlock);
                ready=false;

            }
            if (frameCount/720==1)
            {
                posY++;

                frameCount=0;
            }

            if (checkCollisions(posX,posY,currBlock))
            {
                posY--;
                ready=true;
                setBlock(posX,posY,currBlock);
                posX=posY=0;
            }
            else
            {
                renderBlock(posX,posY,currBlock);
            }

            SDL_RenderPresent(gRenderer);
            renderingNs=SDL_GetTicksNS()-prevTicks;
            constexpr Uint64 nsPerFrame=1000000000/kFps;
            if (renderingNs<nsPerFrame)
            {
                SDL_DelayNS(nsPerFrame-renderingNs);
                prevTicks=SDL_GetTicksNS();

            }
            frameCount++;

        }
    }
    return exitCode;
    // TIP See CLion help at <a href="https://www.jetbrains.com/help/clion/">jetbrains.com/help/clion/</a>. Also, you can try interactive lessons for CLion by selecting 'Help | Learn IDE Features' from the main menu.
}