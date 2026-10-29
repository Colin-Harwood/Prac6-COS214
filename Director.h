#ifndef DIRECTOR_H
#define DIRECTOR_H

#include "Builder.h"

class TaskInput;

class Director {
private:
    Builder* builder;
    TaskInput* input;

public:
    Director();
    Director(TaskInput* input, Builder* builder);
    ~Director();

    void constructTask();
    void setBuilder(Builder* b);
};

#endif // DIRECTOR_H