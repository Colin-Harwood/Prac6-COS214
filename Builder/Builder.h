#ifndef BUILDER_H
#define BUILDER_H

class Builder{

    Builder();
    ~Builder();

    void setName(String name);
    void setDeadline(String deadline);
    void setDuration(int duration);
    void setChildren(vector<Task*> children);
};