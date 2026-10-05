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

# State (priority)
## Explanation
Every task has a priority level that dictates how it is routed and escalated. This allows the task to shift its behavior based on urgency.

## Classes 
- 'PriorityState' (State): is the priority interface defining standard actions like checking priority and escalating.
- 'LowPriority' (concreteState): the task is of low urgency.
- 'MediumPriority' (concreteState): the task follows standard assignment and routing rules.
- 'HighPriority' (concreteState): the task bypasses standard assignment rules and instantly escalates.

# Builder
## Explanation 
  The Builder class is an interface that the director uses to actually construct the task objects.

## Classes
- 'Director' (Director): constructs a Task using the Builder interface.
- 'Builder' (Builder): interface for creating parts of a task.
- 'TaskBuilder' (ConcreteBuilder): constructs and puts together parts of the Task and provides a way to get the Task object.

# Memento
## Explanation:
- The Memento pattern and Command pattern are combined
- The memento pattern is used to create snapshots of a task for logging (to print what the current task looks like) and for undo functionality. its records the state of the a task.
- The ConcreteCommands ask as the caretakers to implement the undo using the memento in their own way
- The Originator/Task/Handler is a friend class of the snapshot class creating a wide interface between each other, however the snapshot provides a narrow interface with the caretaker ensuring encapsulation is not broken

## Classes/Participants:
- **Originator/Receiver:** this has the state that needs to be captured by the **Snapshot** class.
- **Snapshot(memento) :** It captures the state of the originator, has a wide interface with the originator via friend classing and a narrow interface with the **concreteCommand/caretaker.**
- **ConcreteCommands/caretaker**: It links an action to the **originator/receiver**. When it executes it gets a memento from the originator and stores it in its stack. If an undo action is needed it pops the stack, retrieves the snapshot and passes it back to the **originator/receiver**
## Functions:
- `createSnapshot():Snapshot*` creates the instance of a snapshot object
- `setSnapshot(snapshot : Snapshot *) : void` sets all the relevant attributes of originator to the attributes of the snapshot (useful for undo functionality)
- `getSnapshot() : Snapshot* ` returns the snapshot object
- `setSnapshot(taskName : string, id : string, status : Status, timer : int, user : string, priority : PriorityState) : void` sets the attributes/state of the originator to the state of the snapshot.
- `storeSnapshot(snapshot:Snapshot*) : void` pushes a snapshot object onto its stack
- `retreiveSnapshot() : Snapshot*` returns the top element of the command's stack and pops it from the stack.

# Observer
## Explanation
- The Observer and the Composite are combined.
- The observer is used to monitor the current task node (Composite) and ensure that it is still currently running.
- If it detects a hang it alerts the necessary classes to fix/move on from this.

## Participants
- **Subject:** This notifies the observer of the updated state.
- **ConcreteSubject:** Actually sends the updates of it's state to the observer. Sends everytime it's status changes.
- **Observer:** Just has the abstract update.
- **Subject:** Keeps the leaf's status and implements timer functionality to update if the leaf takes too long.

# Iterator
## Explanation
In order to make the traversing of the combined chain of responsibility and composite be easier the iterator is used.
Iterator also has 3 types to go over all priorities, medium and high only (medium), and high priority only.

## Participants
- **Iterator:** Just implements basic interface for the iterator.
- **ConcreteIterator:** Three kinds, to go over the different priorities.
- **Aggregate:** This is the Composite/Chain of Responsibility data structure that the iterator will traverse. **Will do dfs.**
- **ConcreteAgregate:** The leaves that will be iterated over.