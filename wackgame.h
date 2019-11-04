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
    , players({
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

      players[0].reset(0);

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

            players[0].update();

            if(buttonMgr.buttonDown(0))
            {
              players[0].buttonPress();
            }
            
            break;
          }
  
          case S_INGAME:
          {
            //Player 1
            //Select random LED, set on timer
            //When on timer is up light up LED and set off timer
            //Until off timer is up accept input
            //When input received turn off led, reset on timer, sound buzzer
            //When input received after off timer is up increase on timer by 100
    
            //Player 2 (if applicable)
            //......
    
            //Player 3 (if applicable)
            //......
            
            //Compare scores, update servo
            //If score reaches 10 set gameState S_GAMEOVER
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

    wackPlayer players[3];

    unsigned long int nextTick;

    Servo leadServo;
    int buzzerPin;
  
    int playerCount = 0;
    int difficulty = 0;
    int gameState = 0;
};

#endif // WACKGAME_H
