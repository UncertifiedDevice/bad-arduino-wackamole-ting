#ifndef WACKGAME_H
#define WACKGAME_H

#include <Servo.h>
#include "wackbuttonmanager.h"
#include "wackplayer.h"

//12 Pins starting with this pin will be allocated for LEDs
#define LEDPIN0 2
#define LEDPIN1 6
#define LEDPIN2 12

//3 Pins starting with this pin will be allocated for Buttons
#define BUTTONPIN 16

//This pin will be allocated for the buzzer
#define BUZZERPIN 10

//This pin will be allocated for the servo
#define SERVOPIN 11

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
        wackPlayer(LEDPIN0, BUZZERPIN), 
        wackPlayer(LEDPIN1, BUZZERPIN),
        wackPlayer(LEDPIN2, BUZZERPIN)
      })
    {
      for(int i = LEDPIN0; i < LEDPIN0
      + 3; i++)
      {
        pinMode(i, OUTPUT);
      }
      
      for(int i = LEDPIN1; i < LEDPIN1 + 3; i++)
      {
        pinMode(i, OUTPUT);
      }
      
      for(int i = LEDPIN2; i < LEDPIN2 + 3; i++)
      {
        pinMode(i, OUTPUT);
      }

      pinMode(BUZZERPIN, OUTPUT);

      buzzerPin = BUZZERPIN;

      srand(millis());

      nextTick = millis() + 10;
      servoDelay = 0;
    }

    void tick()
    {
      if(millis() > servoDetachTimer)
      {
        leadServo.detach();
        servoDetachTimer = -1;
      }
        
      if(millis() >= nextTick)
      {
        buttonMgr.pollButtons();
        
        switch(gameState)
        {
          case S_SETUP:
          {
            switch(setupStage)
            {
              case 0:
              {
                playerCount = 1;
                setupStage++;
                
                break;
              }

              case 1:
              {
                difficulty = 2;
                
                gameState = S_INGAME;
                setupStage = 0;
                
                break;
              }

              default:
              {
                setupStage = 0;
                break;
              }
            }

            playerArray[0].reset(difficulty);
            playerArray[1].reset(difficulty);
            playerArray[2].reset(difficulty);

            break;
          }
  
          case S_INGAME:
          {
            for(int i = 0; i < 3; i++)
            {
              playerArray[i].update();
  
              if(buttonMgr.buttonDown(i))
              {
                playerArray[i].buttonPress();
              }
            }

            if(playerArray[0].getScore() > playerArray[1].getScore() && playerArray[0].getScore() > playerArray[2].getScore())
            {
              if(millis() > servoDelay)
              {
                leadServo.attach(SERVOPIN);
                leadServo.write(0);
                servoDelay = millis() + 500;
                servoDetachTimer = millis() + 490;
              }

              if(playerArray[0].getScore() >= 10)
              {
                gameState = S_GAMEOVER;
                Serial.println("Player 0 wins");
              }
            }
            
            if(playerArray[1].getScore() > playerArray[0].getScore() && playerArray[1].getScore() > playerArray[2].getScore())
            {
              if(millis() > servoDelay)
              {
                leadServo.attach(SERVOPIN);
                leadServo.write(80);
                servoDelay = millis() + 500;
                servoDetachTimer = millis() + 490;
              }

              if(playerArray[1].getScore() >= 10)
              {
                gameState = S_GAMEOVER;
                Serial.println("Player 1 wins");
              }
            }
            
            if(playerArray[2].getScore() > playerArray[0].getScore() && playerArray[2].getScore() > playerArray[1].getScore())
            {
              if(millis() > servoDelay)
              {
                leadServo.attach(SERVOPIN);
                leadServo.write(160);
                servoDelay = millis() + 500;
                servoDetachTimer = millis() + 490;
              }

              if(playerArray[2].getScore() >= 10)
              {
                gameState = S_GAMEOVER;
                Serial.println("Player 2 wins");
              }
            }

            break;
          }
  
          case S_GAMEOVER:
          {
            gameState = S_SETUP;            

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
    unsigned long int servoDelay;
    unsigned long int servoDetachTimer;
    int gameState = 0;

    Servo leadServo;
    int buzzerPin;

    int playerCount = 0;
    int difficulty = 0;

    //SETUP VARIABLES
    int setupStage = 0;
    unsigned long int setupLEDTimer = 0;

    //GAMEOVER VARIABLES
};

#endif // WACKGAME_H
