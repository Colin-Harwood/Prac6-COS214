# TIVIDY
## Unit tests
Unit Tests are made using DocTest
Making and running locally:
 - Unit tests are created with the `make unit_tests` command
 - This creates an xml file with the results of the test
 - Then running `./unit_tests` gets the output/results of the tests 

## GUI (Dear ImGui + GLFW)
The GUI is built using Dear ImGui, GLFW and OpenGL:
 - Install dependencies once: `make install-gui-deps`
 - Build the GUI executable: `make gui`
 - Run the GUI: `./tividy_gui` (or `make run-gui`)

## Running with Docker (WSL2 / WSLg)
The project is containerized using a multi-stage Ubuntu 22.04 Docker image compiled with C++11.
Our system runs on WSL2 with WSLg for native GUI forwarding.

### 1. Build the Docker image
docker build -t tividy .


### 2. Run the GUI in Docker (WSLg)
- WSLg forwards the display straight into Windows through the `/mnt/wslg` and `/tmp/.X11-unix` sockets.
- Mount your `.env` file with `-v "$(pwd)/.env:/app/.env:ro"` so the application can read your SMTP credentials to send notification emails through port 587.

docker run -it --rm \
  -e DISPLAY="$DISPLAY" \
  -e WAYLAND_DISPLAY="$WAYLAND_DISPLAY" \
  -v /tmp/.X11-unix:/tmp/.X11-unix \
  -v /mnt/wslg:/mnt/wslg \
  -v "$(pwd)/.env:/app/.env:ro" \
  tividy

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
- At each valid node returned by currentItem(), the handler executes the task via its Command object, transitioning it from Pending to In Progress and then to Complete.
- Once isDone() indicates that all eligible tasks for that department have been processed, we signal the Handler to pass the project to the next Handler in the chain, then we check if every node in the project is either Complete or Cancelled to decide whether the Workflow is finished or if the CoR must repeat

## What does a task look like:
In our C++ implementation, tasks form a **Composite pattern** hierarchy rooted at `Task`:
- **Core fields in `Task` (Component base class):**
  - `name`: string representing the task title. It starts with the category number (e.g. `"1.Category"`).
  - `deadline`: string indicating the deadline or time allocation (e.g. `"10 min"`).
  - `deptLevel`: parsed integer from the name prefix (`parseDeptLevel()`), defining which department/category level (1–9) handles it.
  - `state`: pointer to the current `TaskState` (starts as `PendingState`).
  - `isComplete`: boolean completion flag.
  - `observers`: vector of `Observer*` (e.g. `TimeoutMonitor`) notified on `"update"` and `"finished"` events.
  - `restoreFrom(snapshot)`: restores task attributes and deep-copies the state from a Prototype clone for rollback.
- **`SimpleTask` (Leaf):**
  - An individual unit of work that contains a `duration` (in minutes).
  - `isLeaf()` returns true and it is directly executed by commands.
- **`TaskGroup` (Composite):**
  - A container holding a `std::vector<Task*>` of children (can nest other `TaskGroup` or `SimpleTask` nodes).
  - `totalTime()` calculates the sum of all nested children's durations.
  - `getStateName()` and `isFinished()` dynamically compute the group state based on the states of its children (Pending, InProgress, Completed, or Cancelled).

## How do we move from task to another task (system progress):
System progression is driven by the interaction between the **Chain of Responsibility**, the **DepartmentTaskIterator**, and the **State pattern**:
- **Department/Category Routing via Chain of Responsibility:**
  - `ConcreteHandlerA` handles categories 1–3 (Workforce).
  - `ConcreteHandlerB` handles categories 4–6 (Middle Management).
  - `ConcreteHandlerC` handles categories 7–9 (Top Management / Executive).
  - When a task category number is outside a handler's range, `canHandle()` returns false and the request is passed down the chain.
- **Traversing with `DepartmentTaskIterator`:**
  - The handler requests an iterator from the root: `t->createIterator(number)`.
  - The iterator flattens the task tree and filters nodes with `isEligible()`, selecting only leaf tasks that match the target category level and are not finished: `t->isLeaf() && (t->getDepartmentLevel() == targetDepartment) && (!t->isFinished())`.
  - It automatically steps through eligible tasks while skipping already finished (`Completed` or `Cancelled`) nodes.
- **Task Lifecycle and Execution:**
  - Each task begins in `PendingState`.
  - When picked up by a handler, a `TimeoutMonitor` observer is attached to track execution time.
  - The task transitions to `InProgressState` via `start()`.
  - The handler executes the task using `ExecuteCommand` (`command->execute(task)`).
  - On successful execution, the task transitions to `CompletedState` via `complete()`.
  - If a timeout occurs, the monitor triggers `timeout()` -> `TimedOutState`, or a task may transition to `CancelledState` / `RejectedState`.
  - State changes trigger `notify("update")` or `notify("finished")` to update registered observers.
- **Project Completion & Logging:**
  - The workflow manager continues processing handlers until all nodes in the composite tree are finished (`isFinished()` returns true for the entire root).
  - Each completed task state is saved in the `TaskSnapshotManager` memento/prototype queue.
  - Once all tasks for an iterator are finished, `LogCommand` flushes audit logs and `VMimeEmailAdapter` dispatches a completion email via SMTP.

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
Kai: Command
Rynhardt: Composite + Chain of Responsibility 
Bheki: The GUI and Prototype
Colin: Mediator

# Third-Party Libraries and Credits:
- **Dear ImGui** ([GitHub](https://github.com/ocornut/imgui)):
  - Created by Omar Cornut. Immediate mode GUI library for C++ paired with GLFW and OpenGL3 to build the interactive desktop application (`tividy_gui`).
- **VMime** ([GitHub](https://github.com/kisli/vmime)):
  - An open-source C++ class library for MIME and internet messaging protocols. Integrated through the Adapter pattern (`VMimeMailer` and `VMimeEmailAdapter`) for SMTP connection, TLS support, and dispatching workflow completion emails over port 587.
- **doctest** ([GitHub](https://github.com/doctest/doctest)):
  - Lightweight, header-only C++11 unit testing framework used for our automated test suite (`unit_tests`).

