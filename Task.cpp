#include "Task.h"
#include "Observer.h"
#include <algorithm>

Task::Task(const std::string& name, const std::string& deadline): name(name), deadline(deadline), deptLevel(parseDeptLevel(name)),isComplete(false), state(nullptr) {}
Task::~Task() {}

// Reads the department number from the task this is just for the categorys and saves it in dept num
int Task::parseDeptLevel(const std::string& taskName) {
    int value = 0;
    for (char c : taskName){
        if (c<'0'||c>'9'){
            break;
        }
        value = value*10+(c -'0');
    }
    return value;
}
std::string Task::getName() const{
    return name;
}
std::string Task::getDeadline() const{return deadline;}
int Task::getDepartmentLevel() const{return deptLevel;}
void Task::add(Task*){}
void Task::remove(Task*){}
std::vector<Task*> Task::getChildren() const{
    return std::vector<Task*>();
}
void Task::setCompleteStatus(bool b){
    isComplete = b;
}
// State
void Task::setState(TaskState* s){state = s;}
std::string Task::getStateName() const {return "Unknown (check line 33 task.cpp)";}
void Task::start() {}
void Task::complete() {}
void Task::reject() {}
void Task::timeout() {}
void Task::cancel() {}

void Task::reopen() {} 

///ADDED BY KAI

void Task::restoreFrom(Task* snapshot) {
    if (!snapshot) return;

    // 1. Copy primitive/string fields back (NO pointer swapping)
    this->name = snapshot->name;
    this->deadline = snapshot->deadline;
    this->deptLevel = snapshot->deptLevel;
    this->isComplete = snapshot->isComplete;

    // 2. Deep copy the state back
    if (snapshot->state) {
        delete this->state;
        this->state = snapshot->state->clone();
    }
}

// Observer
void Task::addObserver(Observer* observer) {
    observers.push_back(observer);
}

void Task::removeObserver(Observer* observer) {
   auto it = observers.begin();
   while (it != observers.end()) {
    if (*it == observer) {
        delete *it;
        it = observers.erase(it);
    } else {
        ++it;
    }
   }
}

// ADDED BY COLIN   

void Task::notify(std::string type) {
    if (type == "finished") {
        for (size_t i = 0; i < observers.size(); i++) {
            observers[i]->finished();
        }
    } else if (type == "update") {
        for (size_t i = 0; i < observers.size(); i++) {
            observers[i]->update();
        }
    }
}

// Added by Motheo

bool Task::isFinished() const 
{
    if(state != nullptr)
    {
        return state->isFinished();
    }
    return isComplete;
}