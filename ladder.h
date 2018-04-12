#ifndef __LADDER_H__
#define __LADDER_H__

#include "Arduino.h"

class Ladder;

class ActionInterface {
  private:
    unsigned long executionTime = 0;
    unsigned short epoch = 0;
    Ladder *ladder = NULL;

  protected:
    void addDelay(unsigned long delay);

  public:
    const unsigned short COMPLETE = 1, REPEAT = 0;

    ActionInterface(Ladder *l, unsigned long base, unsigned long delay);
    virtual ~ActionInterface() {
    }
    virtual unsigned short execute() = 0;
    unsigned char isNow();

};


class Ladder {
  public:
    unsigned short epoch = 0;
    unsigned long currentMicros = 0;

    Ladder();
    ~Ladder() {};
    void addAction(ActionInterface *action);
    void execute();
    int emptySlots();

  protected:
    ActionInterface* ladder[30];
};



#endif