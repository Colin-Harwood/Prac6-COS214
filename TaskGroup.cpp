#include "TaskGroup.h"
#include <algorithm>

TaskGroup::TaskGroup(const std::string& name, const std::string& deadline):Task(name, deadline) {}
TaskGroup::~TaskGroup(){
    for (Task* child : children){
        delete child;
    }
}
// goes over all the childeren treat one group and a leaf the same
int TaskGroup::totalTime() const{
    int total = 0;
    for (const Task* child : children){
        total += child->totalTime();
    }
    return total;
}
// just calls execute on all the leaf nodes
void TaskGroup::execute(){
    for (Task* child : children){
        child->execute();
    }
}
void TaskGroup::add(Task* t){
    if (t){
        children.push_back(t);
    }
}

// Detaches the child the CALLER NEEDS TO DELETE 
void TaskGroup::remove(Task* t){
    children.erase(std::remove(children.begin(),children.end(),t),children.end());
}
bool TaskGroup::isLeaf() const{
    return false;
}
std::vector<Task*> TaskGroup::getChildren() const{
    return children;
}
//Prototype
Task* TaskGroup::clone() const{
    return nullptr;
}
//Iterator
TaskIterator* TaskGroup::createIterator(int){
    return nullptr;
}