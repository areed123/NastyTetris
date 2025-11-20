#include <iostream>
#include <gameBlock.h>
#include <gameArea.h>


// TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.
int main() {
    // TIP Press <shortcut actionId="RenameElement"/> when your caret is at the <b>lang</b> variable name to see how CLion can help you rename it.
    int input;
    bool gameOver = false;
    auto block = gameBlock(1);
    auto gameBoard = gameArea();
    do {
        if(gameBoard.isReady()) {
          gameOver=!(gameBoard.addBlock(block));
        }
        else {
            //std::cin >> input;
            gameBoard.moveBlock(block,-1);
            if (!(gameBoard.fallBlock(block))) {
               gameBoard.placeBlock(block);
                gameBoard.updateGame();
                for (int j=0; j<24; j++) {
                    for (int i=0; i<10; i++) {
                        if (gameBoard.playArea[j][i]==true)
                            SDL_Log("[X]");
                        else
                            SDL_Log("[O]");
                    }


                }
                SDL_Log("bottomOf");
            }

        }

    }while(!gameOver);
    // TIP See CLion help at <a href="https://www.jetbrains.com/help/clion/">jetbrains.com/help/clion/</a>. Also, you can try interactive lessons for CLion by selecting 'Help | Learn IDE Features' from the main menu.
}