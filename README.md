# Prac6-COS214
Practical 6 for COS214

# Composite + Chain of Responsibility
## Classes
- 'Workflow' (Client): builds the chain and runs it.
- 'Task' (Handler and Component): abstract base class with the next link, a name.
- 'SubTask' (Concrete Handler and Leaf): one unit of work with a duration.
- 'TaskGroup' (Concrete Handler and Composite): holds child tasks and runs them in order.
