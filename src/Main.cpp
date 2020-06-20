#include "Game.h"

int main(int argc, char* args[]){

  Game game;
  if (game.Initialize())
  {
    game.RunLoop();
  }

  return 0;
}
