#include <iostream>
#include "game_managing/GameManager.h"


int main(int arc, char* argv[]){
    GameManager* gameManager = new GameManager("DA GAME",1024,768);
    std::cout<<"==Initilizing Game engine\n";
    gameManager->EngineInit();
    gameManager->EngnieStart();
}