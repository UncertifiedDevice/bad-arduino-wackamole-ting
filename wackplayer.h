#ifndef WACKPLAYER_H
#define WACKPLAYER_H

#include <stdlib.h>

#define EASYMOD 400 + rand() % 50
#define MEDIUMMOD 350 + rand() % 50
#define HARDMOD 200 + rand() % 100

class wackPlayer
{
  public:
    wackPlayer(int _ledPin, int _buzzPin)
    : ledPin(_ledPin)
    , buzzPin(_buzzPin)
    {}

    void update()
    {
      if(digitalRead(ledPin + ledOffset) == LOW && millis() >= onTimer)
      {
        digitalWrite(ledPin + ledOffset, HIGH);
        offTimer = millis() + difficultyModifier();
      }

      if(digitalRead(ledPin + ledOffset) == HIGH && millis() >= offTimer)
      {
        digitalWrite(ledPin + ledOffset, LOW);
        onTimer = millis() + difficultyModifier();

        ledOffset = rand() % 3;
      }

      if(digitalRead(ledPin + 3) == HIGH && millis() >= blueTimer)
      {
        digitalWrite(ledPin + 3, LOW);
      }
    }

    void buttonPress()
    {
      if(digitalRead(ledPin + ledOffset) == HIGH)
      {
        score++;

        digitalWrite(ledPin + ledOffset, LOW);
        onTimer = millis() + difficultyModifier();
        
        ledOffset = rand() % 3;
        
        digitalWrite(ledPin + 3, HIGH);
        blueTimer = millis() + 250;

        tone(buzzPin, 2000, 75);
      }
      else
      {
        onTimer += 300;

        tone(buzzPin, 1000, 75);
      }
    }

    int getScore()
    {
      return score;
    }

    void reset(int _difficulty)
    {
      for(int i = ledPin; i <= ledPin + 3; i++)
      {
        digitalWrite(i, LOW);
      }

      score = 0;
      difficulty = _difficulty;

      ledOffset = rand() % 3;
      onTimer = millis() + difficultyModifier();
    }

    unsigned long int difficultyModifier()
    {
      switch(difficulty)
      {
        case 0:
        {
          return EASYMOD;
          break;
        }

        case 1:
        {
          return MEDIUMMOD;
          break;
        }

        case 2:
        {
          return HARDMOD;
          break;
        }

        default:
        {
          return MEDIUMMOD;
          break;
        }
      }
    }

    virtual ~wackPlayer() {}

  private:
    unsigned long int onTimer = 0;
    unsigned long int offTimer = 0;
    unsigned long int blueTimer = 0;

    int difficulty = 0;
    int buzzPin;
    int ledPin;

    int ledOffset = 0;
    int score = 0;
};

#endif // WACKPLAYER_H
