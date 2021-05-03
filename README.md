# DSA1Project1

Builds up an implementation of a typical Stack and Queue Data Structure from scratch. 

Commands should be loaded into an input file and the output of this program is the result of all of the commands.
The program does not validate commands, it assumes all commands inputted are valid.

Creating a stack or queue:
command: create inumbers queue
The data structres can be of integers, doubles or strings, desginated by an 'i' 'd' or 's' at the start of the name respectively. 
A stack and a queue cannot have the same name, nor can there be two stacks or two queues with the same name- if so, "ERROR: This name already exists!" will be printed. 
If the command is valid, PROCESSING COMMAND: create inumbers queue will be printed and in this case the queue of integers "inumbers" will be created. 

Pop:
command: pop inumbers
As a pop functions the same for a stack and a queue, the command 'pop inumbers' will return the first value in the data structure. 
If the structure is empty, it will return "ERROR: this list is empty!"
If the structure under that name does not exist, it will return "ERROR: this name does not exist!"
Otherwise, "Value popped: -50" will be returned- in this case the first value was -50.

Push: 
command: push inumbers 12
For a stack, this will push '12' to the top of the stack. for a queue, this will enqueue a '12' to the back (farthest from being popped) of the queue. 
If the structure under that name does not exist, it will return "ERROR: this name does not exist!"
Nothing is returned if there is no error.

Most of these operations are preformed in constant time unless linear time was unavoidable (searches)
