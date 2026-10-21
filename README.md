[[COS 214]]
9/10/2026

 An interactive generalized workflow system used to define projects for automatic or manual approval by user input. Tasks are user defined with deadlines, states and sub task dependencies with a rollback system supported by a consistent snapshot history and a workflow log report.
## Define how tasks are built?
Builder builds the composite. There will be concreteBuilders for a leaf node and composite node.
The `construct()` function will pass in a text file in which the end user defines the structure of the tree and will call the correct concreteBuilder functions.

**Textfile/Constructor input Format:**  
```
$TaskGroup | 0[
	(task | 0)
	(task | 1)
	(task | 2)
	$TaskGroup | 1[
		(task | 2)
		(task | 2)
		(task | 2)
	]
]$

//dollar sign $ to incase a project
//square brackets [] indicates a taskgroup
//brackets indicates a task
//the number after the '|' is the priority level, 0 for highest and 2 for lowest
```

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
	- Aggregate:
	- ConcreteAggregate:
	- Iterator:
	- ConcreteIterator:
3. **Adapter:** email/notification service (VMine library used for email correspondence and is adapted in our logging system and sends email on when a project is declared as complete)
	- Target:
	- Adapter:
	- Adaptee:
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
