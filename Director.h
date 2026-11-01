#ifndef DIRECTOR_H
#define DIRECTOR_H

#include "Builder.h"

class TaskInput;

/**
 * @brief Orchestrates construction of a task from a TaskInput.
 *
 * The director uses the supplied builder for the root task and creates
 * appropriate builders recursively for child tasks.
 */
class Director {
private:
    Builder* builder;
    TaskInput* input;

public:
    /// Creates a director without an input or builder.
    Director();
    /**
     * @brief Creates a director for the given input and builder.
     * @param input Task description to construct.
     * @param builder Builder compatible with the root input type.
     */
    Director(TaskInput* input, Builder* builder);
    /// Destroys the director.
    ~Director();

    /**
     * @brief Constructs the task tree described by the input.
     * @throws std::logic_error if either the input or builder is unset.
     * @throws std::invalid_argument if a builder does not match an input type
     *         or a simple task input contains children.
     */
    void constructTask();
    /**
     * @brief Sets the builder used to construct the root task.
     * @param b Builder compatible with the root input type.
     */
    void setBuilder(Builder* b);
};

#endif // DIRECTOR_H