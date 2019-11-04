#ifndef WACKBUTTONMANAGER_H
#define WACKBUTTONMANAGER_H

class wackButtonManager
{
  public:
    //Constructor takes two int values to determine range of pins to poll for input
    wackButtonManager(int _startPin, int _endPin)

    //Initialize button state arrays with appropriate length
    : currentState(new bool[_endPin - _startPin + 1])
    , previousState(new bool[_endPin - _startPin + 1])

    //Initialize startPin and endPin attributes
    , startPin(_startPin)
    , endPin(_endPin)

    //Functional part of constructor sets pinMode and retrieves initial state
    {
      //Setting pin modes...
      for(int i = startPin; i < endPin + 1; i++)
      {
        pinMode(i, INPUT_PULLUP);
      }

      //Make sure previousState and currentState are set
      pollButtons();
      pollButtons();
    }

    //Update button states, to be called every loop()
    void pollButtons()
    {
      for(int i = 0; i < endPin - startPin; i++)
      {
        previousState[i] = currentState[i];
        currentState[i] = digitalRead(i + startPin);
      }
    }

    //Checks if a button has been released (previous high, current low)
    //Note: button is the pin number offset by -startPin
    bool buttonUp(int button)
    {
      return previousState[button] == HIGH && currentState[button] == LOW;
    }

    //Checks if a button has been pressed (previous low, current high)
    bool buttonDown(int button)
    {
      return previousState[button] == LOW && currentState[button] == HIGH;
    }

    //Checks if a button is currently HIGH
    bool buttonHigh(int button)
    {
      return currentState[button] == HIGH;
    }

    //Destructor deallocates array memory
    virtual ~wackButtonManager()
    {
      delete(currentState);
      delete(previousState);
    }

  private:
    //Arrays that will store the state of each button pin
    bool* currentState;
    bool* previousState;

    //Integers that indicate the range of pins used for buttons
    int startPin;
    int endPin;
};

#endif // WACKBUTTONMANAGER_H
