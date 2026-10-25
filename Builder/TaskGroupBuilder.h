#ifndef TASKGROUPBUILDER_H
#define TASKGROUPBUILDER_H

class TaskGroupBuilder : Builder{

    private:
    Task* task;

    public:
    TaskGroupBuilder();
    void setName(String name);
    void setDeadline(String deadline);
    void setDuration(int duration);
    void setChildren(vector<Task*> children);


};