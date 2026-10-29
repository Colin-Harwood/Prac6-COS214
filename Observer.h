#ifndef OBSERVER_H
#define OBSERVER_H

// #include "TaskState.h"

class Observer {
    public:
        virtual void update() = 0;
        virtual void finished(){};
        virtual ~Observer(){};
};

#endif