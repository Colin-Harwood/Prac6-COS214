#include "SimpleTask.h"
#include "DepartmentTaskIterator.h"
#include <iostream>

SimpleTask::SimpleTask(const std::string& name, const std::string& deadline, int duration):Task(name, deadline),duration(duration){}
int SimpleTask::totalTime() const{return duration;}
void SimpleTask::execute() {
    std::cout << "Executing task: " << getName()<< " (" << duration << " min)" << std::endl;
}
bool SimpleTask::isLeaf() const {
    return true;
}

//added by KAI
SimpleTask::SimpleTask(const SimpleTask& other) : Task(other), duration(other.duration) {}

//Prototype edited by KAI
Task* SimpleTask::clone() const{
    return new SimpleTask(*this);
}

//Iterator edited by Motheo
TaskIterator* SimpleTask::createIterator(int level){
    return new DepartmentTaskIterator(this,level);
}
