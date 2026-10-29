#include "Director.h"

Director::Director() : builder(nullptr), input(nullptr) {}

Director::Director(TaskInput* input, Builder* builder) 
    : input(input), builder(builder) {}

Director::~Director() {
}

void Director::constructTask() {
    if (!builder || !input) {
        return;
    }
    // Implement task construction using input data and builder steps
}

void Director::setBuilder(Builder* b) {
    this->builder = b;
}
