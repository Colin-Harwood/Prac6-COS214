#ifndef SIMPLETASKBUILDER_H
#define SIMPLETASKBUILDER_H

class SimpleTaskBuilder : Builder{
    private:
    Task* task;

    public:
    SimpleTaskBuilder();
    void setName(String name);
    void setDeadline(String deadline);
    void setDuration(int duration);
   

};