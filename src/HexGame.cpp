#include "HexGameConfig.h"
#include "HexGameEngine.h"
#include "MainGameState.h"

int main(int argc, char *argv[]) {
  HexGameEngine game;
  game.Init(HexGame_VERSION_STRING, MainGameState::Instance());
  return 0;
}
