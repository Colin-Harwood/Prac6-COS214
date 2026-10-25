#ifndef DIRECTOR_H
#define DIRECTOR_h

class Director{
    private:
        Builder* builder;
        TaskInput input;

    public:
        Director();
        ~Director();
        void constructTask();
        void setBuilder(Builder* b);

};