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
