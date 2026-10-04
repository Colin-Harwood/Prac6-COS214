# Prac6-COS214
Practical 6 for COS214

# Composite + Chain of Responsibility
## Classes
- 'Workflow' (Client): builds the chain and runs it.
- 'Task' (Handler and Component): abstract base class with the next link, a name.
- 'SubTask' (Concrete Handler and Leaf): one unit of work with a duration.
- 'TaskGroup' (Concrete Handler and Composite): holds child tasks and runs them in order.

# Command
## Explanation 
  Each element as part of the tree and/or linked list owns a command, this command object works as the thing that actually does the work of each task

## Classes
- 'Command', acts as the interface
- 'taskCommand' does work of the class
- 'ConditionalTaskCommand' can be skipped over and the request be passed to the next element in the chain of responsiblity

# State (status)
## Explanation 
every task has a status which can be pending, inProgress, Failed, Cancelled, TimedOut or Completed.

## Classes
- 'Status' (State:, is the status interface
- 'PendingStatus' (concreteState): the task has been made, but not started to do anything yet
- 'inProgressStatus' (concreteState): task is executing/running/doing something and progressing.
- 'TimedOutStatus' (concreteState): the time limit allocated for the task has been reached.
- 'CancelledStatus' (concreteState): the task was cancelled.
- 'FailedStatus' (concreteState): an error occurred while the task was running, and failed.
- 'CompletedStatus' (concreteState): the task has successfully completed, or successfully been cancelled.

# Builder
## Explanation 
  The Builder class is an interface that the director uses to actually construct the task objects.

## Classes
- 'Director' (Director): constructs a Task using the Builder interface.
- 'Builder' (Builder): interface for creating parts of a task.
- 'TaskBuilder' (ConcreteBuilder): constructs and puts together parts of the Task and provides a way to get the Task object.



