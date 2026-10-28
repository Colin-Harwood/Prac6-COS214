#include "SimpleTask.h"
#include <iostream>

SimpleTask::SimpleTask(const std::string& name, const std::string& deadline, int duration):Task(name, deadline),duration(duration){}
int SimpleTask::totalTime() const{return duration;}
void SimpleTask::execute() {
    std::cout << "Executing task: " << getName()<< " (" << duration << " min)" << std::endl;
}
bool SimpleTask::isLeaf() const {
    return true;
}
//Prototype
Task* SimpleTask::clone() const{return nullptr;}
//Iterator
TaskIterator* SimpleTask::createIterator(int){return nullptr;}