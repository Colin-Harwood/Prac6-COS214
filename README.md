[[COS 214]]
9/10/2026


# Making the files
## Unit tests
Unit Tests are made using DocTest
Making and running:
 - Unit tests are created with the make unit_tests command
 - This creates an xml file with the results of the test
 - Then running ./unit_tests gets the output/results of the tests 

 An interactive generalized workflow system used to define projects for automatic or manual approval by user input. Tasks are user defined with deadlines, states and sub task dependencies with a rollback system supported by a consistent snapshot history and a workflow log report.
## Define how tasks are built?
Builder builds the composite. There will be concreteBuilders for a leaf node and composite node.
The `construct()` function will pass in a text file in which the end user defines the structure of the tree and will call the correct concreteBuilder functions.

## Interaction of the Composite Task Nodes and the Chain of Responsibility Handlers
Composite builds the task structure, and the chain decides who processes each task based on its size.

## Command
Will sit between the Chain of Responsibility handler and the Composite tasks.
The command will invoke the tasks functions and be called by the handler.
While doing this the command will clone itself to save itself for undo and save itself for logging.

## Composite
The compsite is used to build each task with the help of other design patterns.
Each task is represented as a compsite so each task can have subtasks. 
Simpletask is the leaf node and a subtask which will be handled by the concrete handlers.
TaskGroup as a nested task with smaller subtasks.
The client treats one task and the subtasks in the same way.

## Chain of Responsibility
Each handeler holds a pointer to the next task and a range minleaf maxleaf.
Each concrete handler has its set task groups it can handle if it cant then it sends it down the chain.
The sender does not know which concrete handler ends up handeling the task.

## Iterator
- The iterator pattern allows the CoR Handler classes to traverse the Task composite, without seeing the tree structure. 
- When a department Handler processes a workflow, it calls the createIterator() on the root to instantiate an external DepartmentTaskIterator 
- Then we step through tasks matching them by their Department while skipping nodes whose status is Complete, Cancelled, Timed out, or failed.
- At each valid node returned by currentItem(), the system checks checkAutomatic() to see if the task can be completed automatically via its Command object or needs to be approved manually, then we transition from Pending to In Progress and then to Complete.
- Once isDone() indicates that all eligible tasks for that department have been processed, we signal the Handler to pass the project to the next Handler in the chain, then we check if every node in the project is either Complete or Cancelled to decide whether the Workflow is finished or if the CoR must repeat


This textfile is going to require a parser for tree construction.
## What does a tasks look like:  
- an id (produced by builder)
- name (comes from file)
- priority can be called departmentLevel (comes from file)
- timer (global of 30seconds defined by the builder on creation)
- Mediator_pointer for checking if a task has timed out
- status (all start from pending so builder defines it)
- next_task_pointer (Defined by builder when reading the construction file)
- command (builder creates the pointer to a command)
- log (string builds the state of the task. creates a memento snapshot)
- department level (executive/common/managerial)

## How do we move from task to another task (system progress):
- System is based on Department/priority based
- Each task node (within the composite) has its own flag that states whether the task itself needs user input or can be automatically done by the command object ( when the iterator loops over the nodes it will check whether or not it can be done automatically by using teh nodes API e.g `checkAutomatic() : bool `
- )
- The tasks get approved/completed depending on what department you are in, so the iterators can go over for example executive level tasks aka "high priority" tasks.
- You can set the names of the departments/groups
- The iterator will go through the tasks and once complete it returns a flag, which signals the COR (Chain of responsibility)  to move the project onto the next handler.
	- The status(aka state) changes as we work on the task
		- everyone starts on pending, once loaded into the handler set to in progress and if it completes within the allocated time it is set to complete. 
		- **BEFORE DECLARING A PROJECT DONE EVERY NODE MUST EITHER BE CANCELLED OR COMPLETE IF NOT REPEAT THE THE COR HANDLER LOOP.**
		- If a task times out it (which the observer detect) the status is set to timed out and then set to failed.
		- **the iterator skips failed/timed out and completed**:
			- if a node times out it is attempted again

# Audit Logging:
- create snapshots once a task is complete and adds it to a queue. the audit log command is called at the end once the project is defined as completed.

## How do the status changes occur and Where do they occur?
- Pending:
	- On creation
- In Progress
	- Once we set it to the stage we change it from pending to in progress.
- Timed out:
	- Mediator watches the tasks and if it times out sets it to the timed out set.
- Cancelled
	- requires user input to set a task to cancelled (gets skipped over in the workflow system)
- Complete
	- Once a task is executed it is defined as complete
- Failed
	- From time out to fail

## Defining what type of concreteHandlers we have and what they do?
- there will be a handler for each department, where that specified department completes a task and moves it onto the next department.
- It could also be priority based

# Task/Node Behavior
Each node has a certain list of responsibilities/jobs (just strings) and each task in the composite has a type associated with. If 
## Behavior of Commands:
- The commands are general but are only executed when the task handled/traversed by the correct handler or iterator.
- A log command that prints out a history of the workflow
- A snapshot retrieval/undo command that restores the task if it is in a timed out state.

## list of Patterns and Core Functions:
**Please list the participants and core functions related to the pattern, please have function description **

1. **Builder:** used to create the project:
	- Builder: Builder
	- ConcreteBuilder: SimpleTaskBuilder, TaskGroupBuilder
	- Director: Director
2. **Iterator:** used to traverse the task and execute functions. different types of iterators based on what the department the task is in (external)
	- Aggregate: Task
	- ConcreteAggregate: SimpleTask, TaskGroup
	- Iterator: TaskIterator
	- ConcreteIterator: DepartmentTaskIterator
3. **Adapter:** email/notification service (VMime library used for email correspondence and is adapted in our logging system and sends email on when a task or project is declared as complete)
	- Target: Target
	- Adapter: VMimeEmailAdapter
	- Adaptee: VMimeMailer
	- Client: Command (ExecuteCommand & LogCommand)
4. **Composite:** represents the structure of the project-
	- Component: Task
	- Composite: TaskGroup
	- Leaf: SimpleTask
5. **Prototype:** Creates snapshots of a task so they can be used in the rollback system and are integrated with the task execution subsystem for log reports and undo functionality
	- Prototype: Task
	- ConcretePrototype: SimpleTask and TaskGroup
	- PrototypeManager: TaskSnapshotManager
6. **Command:** logging and retrieval
	- Invoker: Handler
	- Receiver: SimpleTask & TaskGroup
	- Command: Command
	- ConcreteCommand: LogCommand & ExecuteCommand
7. **Chain of Responsibility:** progresses the workflow
	- Handler: Handler
	- ConcreteHandler: ConcreteHandlerA, ConcreteHandlerB, ConcreteHandlerC
8. **Mediator:** looks at task if they time out and changes the state to timed out:
	- Mediator:
	- ConcreteMediator:
	- Colleague:
	- ConcreteColleague:
9. **State:** the status of the task
	- Context:
	- State:
	- ConcreteState:
10. **Facade:** high level abstraction of the system implementation, GUI implementation (Configurable)
	- Subsystems Used:
		- Chain of Responsibility
		- Iterator
		- Builder
		- Command
		- Mediator
		- Adapter

# Team Roles:
Shreya: Builder
Motheo: Iterator + Adapter
Maeryn: State + Facade
Kai: Command + Memento
Rynhardt: Composite + Chain of Responsibility 
Bheki: The GUI
Colin: Mediator
