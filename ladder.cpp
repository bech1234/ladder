#include "Arduino.h"
#include "ladder.h"

Ladder::Ladder() {
  for (int i = 0; i < 30; i++) {
    ladder[i] = NULL;
  }

  currentMicros = micros();

}

void Ladder::addAction(ActionInterface *action) {
  if (action->isNow()) {
    Serial.println("Not accepting actions past due");
    return;
  }

  for (int i = 0; i < 30; i++) {
    if (ladder[i] == NULL) {
      ladder[i] = action;
      return;
    }
  }

  Serial.println("Failed to add action");
}

void Ladder::execute() {
  unsigned long now = micros();

  if (now < currentMicros) {
    epoch = epoch ^ 1;
  }

  currentMicros = now;

  for (int i = 0; i < 30; i++) {
    if (ladder[i] != NULL) {
      if (ladder[i]->isNow()) {
        if (ladder[i]->execute() == ladder[i]->COMPLETE) {
          delete ladder[i];
          ladder[i] = NULL;
        }

        return;
      }
    }
  }
}

int Ladder::emptySlots() {
  int result = 0;

  for (int i = 0; i < 30; i++) {
    if (ladder[i] == NULL) {
      result++;
    }
  }

  return result;
}


ActionInterface::ActionInterface(Ladder *l, unsigned long base, unsigned long delay) {
  ladder = l;
  epoch = base + delay < base ? ladder->epoch ^ 1 : ladder->epoch;
  executionTime = base + delay;
}

unsigned char ActionInterface::isNow() {
  if (epoch != ladder->epoch) {
    return 0;
  }

  if (executionTime > ladder->currentMicros) {
    return 0;
  }

  return 1;
}

void ActionInterface::addDelay(unsigned long delay) {
  executionTime = micros();
  if (executionTime < ladder->currentMicros) {
    epoch = ladder->epoch ^ 1;
  } else {
    epoch = executionTime + delay < executionTime ?
            ladder->epoch ^ 1 : ladder->epoch;
  }
  executionTime = executionTime + delay;
}
