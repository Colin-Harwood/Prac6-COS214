/**
 * @file Observer.h
 * @brief Defines the Observer interface for watching task state changes.
 */

#ifndef OBSERVER_H
#define OBSERVER_H

#include "TaskState.h"

class Observer {
    public:

        /**
         * @brief Tells the observer that the task has changed state
         */ 

        virtual void update() = 0;

        /**
        * @brief Tells observer that the task is finished and to end watching it  
        */        
        virtual void finished(){};

        /**
         * @brief Deletes observer
         */
        virtual ~Observer(){};
};

#endif