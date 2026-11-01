#include "TaskGroup.h"
#include "DepartmentTaskIterator.h"
#include <algorithm>
/**
 * @file TaskGroup.cpp
 * @brief Composite node that holds child tasks.
 *
 * Treats groups and leaves uniformly, supports deep-copy cloning (Prototype)
 * and creates a DepartmentTaskIterator (Iterator).
 */
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
// Worked out from the children each time it is asked for
std::string TaskGroup::getStateName() const{
    if (children.empty()){
        return "Pending";
    }
    bool allCancelled = true;
    bool anyStarted = false;
    for (const Task* child : children){
        const std::string childState = child->getStateName();
        if (childState != "Cancelled"){
            allCancelled = false;
        }
        if (childState != "Pending"){
            anyStarted = true;
        }
    }
    if (allCancelled){
        return "Cancelled";
    }
    if (isFinished()){
        return "Completed";
    }
    return anyStarted ? "InProgress" : "Pending";
}
// An empty group has no finished work
bool TaskGroup::isFinished() const{
    if (children.empty()){
        return false;
    }
    for (const Task* child : children){
        if (!child->isFinished()){
            return false;
        }
    }
    return true;
}
TaskGroup::TaskGroup(const TaskGroup& other)
    : Task(other) {
    // Deep copy children
    for (Task* child : other.children) {
        this->children.push_back(child->clone());
    }
}
//Prototype
Task* TaskGroup::clone() const{
    return new TaskGroup(*this);
}
//Iterator edited by Motheo
TaskIterator* TaskGroup::createIterator(int level){
    return new DepartmentTaskIterator(this, level);
}
