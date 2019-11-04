#ifndef WACKGAME_H
#define WACKGAME_H

#include <Servo.h>
#include "wackbuttonmanager.h"
#include "wackplayer.h"

//12 Pins starting with this pin will be allocated for LEDs
#define LEDPIN 2

//3 Pins starting with this pin will be allocated for Buttons
#define BUTTONPIN 14

//This pin will be allocated for the buzzer
#define BUZZERPIN 17

//This pin will be allocated for the servo
#define SERVOPIN 18

//Game states used in tick() function
#define S_SETUP 0
#define S_INGAME 1
#define S_GAMEOVER 2

class wackGame
{
  public:
    wackGame()
    : buttonMgr(BUTTONPIN, BUTTONPIN + 2)
    , playerArray({
        wackPlayer(LEDPIN, BUZZERPIN), 
        wackPlayer(LEDPIN + 4, BUZZERPIN),
        wackPlayer(LEDPIN + 8, BUZZERPIN)
      })
    {
      for(int i = LEDPIN; i < LEDPIN + 11; i++)
      {
        pinMode(i, OUTPUT);
      }

      pinMode(BUZZERPIN, OUTPUT);
      pinMode(SERVOPIN, OUTPUT);

      buzzerPin = BUZZERPIN;
      leadServo.attach(SERVOPIN);

      srand(millis());

      nextTick = millis() + 10;
    }

    void tick()
    {
      if(millis() >= nextTick)
      {
        buttonMgr.pollButtons();
        
        switch(gameState)
        {
          case S_SETUP:
          {
            //Retrieve player count using buttons
            //Retrieve difficulty using buttons
            //Countdown before game begins
            //Play some silly buzzer melody
            //set gameState S_INGAME
            
            break;
          }
  
          case S_INGAME:
          {
            playerArray[0].update();

            if(buttonMgr.buttonDown(0))
            {
              playerArray[0].buttonPress();
            }

            playerArray[1].update();

            if(buttonMgr.buttonDown(1))
            {
              playerArray[1].buttonPress();
            }

            playerArray[2].update();

            if(buttonMgr.buttonDown(2))
            {
              playerArray[2].buttonPress();
            }

            if(playerArray[0].getScore() > playerArray[1].getScore() && playerArray[0].getScore() > playerArray[2].getScore())
            {
              leadServo.write(0);

              if(playerArray[0].getScore() >= 10)
              {
                gameState = S_GAMEOVER;
              }
            }

            if(playerArray[1].getScore() > playerArray[0].getScore() && playerArray[1].getScore() > playerArray[2].getScore())
            {
              leadServo.write(80);

              if(playerArray[1].getScore() >= 10)
              {
                gameState = S_GAMEOVER;
              }
            }

            if(playerArray[2].getScore() > playerArray[0].getScore() && playerArray[2].getScore() > playerArray[1].getScore())
            {
              leadServo.write(160);

              if(playerArray[2].getScore() >= 10)
              {
                gameState = S_GAMEOVER;
              }
            }

            break;
          }
  
          case S_GAMEOVER:
          {
            //Wiggle servo over winner
            //Flash winners lights
            //Animate other lights
            //Any input will reset game by resetting scores and setting gameState S_SETUP
            break;
          }
  
          default:
          {
            gameState = 0;
            break;
          }
        }

        nextTick = millis() + 10;
      }
    }

    virtual ~wackGame()
    {}

  private:
    wackButtonManager buttonMgr;
    wackPlayer playerArray[3];

    unsigned long int nextTick;
    int gameState = 0;

    Servo leadServo;
    int buzzerPin;

    //SETUP VARIABLES
    int playerCount = 0;
    int difficulty = 0;

    //GAMEOVER VARIABLES
};

#endif // WACKGAME_H
