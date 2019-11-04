#include "wackgame.h"

wackGame game;

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  game.tick();
}
