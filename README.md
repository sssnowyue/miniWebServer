# miniWebServer
A basic web server from scratch in C++

## Architecture
<img src="docs/Architecture Diagram.jpg"/>

[UML Class Diagram](docs/UML.png)

## Features
### Multi-[Reactor](https://en.wikipedia.org/wiki/Reactor_pattern) Multi-threaded Model

- `MainReactor`: 
    - Monitors connection establishment events through the IO multiplexing interface (Select/Poll/Epoll); 
    - After receiving an event:
        1. Accepts the connection through the `Acceptor`'s accept method;
        2. Then allocates the new connection to a `SubReactor`.
- `SubReactor`: 
    - Adds the connection allocated by the `MainReactor` to the IO multiplexing interface (Select/Poll/Epoll) to continue monitoring; 
    - After receiving an event, handles the connection's response event:
        1. Reads the data sent by the client;
        2. Assign business logic processing to a `Worker` in the thread pool;
        3. Writes the processed data back to the client.
- `Worker`:
    - Processes the data received from the client;

> Why Not Proactor Model:
> 
> Proactor model requires asynchronous IO support from the operating system, BUT Linux's asynchronous IO support is incomplete. Unix-like systems lack widespread support for asynchronous IO.  

### Epoll
#### APIs
1. `epoll_create()`: 
- To Create:
    - **Red-Black Tree**: store Added fd
    - **Ready Linked List**: store ready fd
    - **Wait Queue**: Blocked Thread and callback function
2. `epoll_ctl()`: 
- Add/Delete/Modify fd in the `Red-Black Tree` (Add with `callback function A`)
3. `epoll_wait()`: 
- Checking if there are any ready fd in the `Ready Linked List`,
    - if yes, current thread won't be blocked, return `Ready Linked` Listimmediately.
    - if not, current thread will be blocked, and link the thread into the `Wait Queue`.


- When fd is ready, kernel will insert the fd into the `Ready Linked List` through the `backcall function A`.
- Kernel will check the `Wait Queue` to find the blocked thread, and wake up the thread.

#### Trigger Modes
- Level Trigger Mode `LT`: Only notify on file descriptor state changes
    > Read events, as long as there is unfinished data on the socket, the POLLIN event will keep being generated;
    >
    > Write events: if the TCP window of the socket keeps unsaturated, the POLLOUT event will keep being triggered.

- Edge Trigger Mode `ET`: Repeatedly notify unhandled events
    > Read events: the first time data is available on the socket will trigger it once, subsequent data existence on the socket will not trigger it again, unless the data has been read off completely, and new data becomes available again that it will continue triggering;
    >
    > Write events: only trigger once, unless the TCP window goes from saturated to unsaturated then saturated again, will the POLLOUT event be triggered again.

### Thread Pool
To manage a group of worker threads for asynchronously executing tasks.
#### Task Submission and Execution
- The `enqueue` method is used to submit tasks to the thread pool, supporting tasks of any type (function + parameters).
- Each task is wrapped in a `std::packaged_task` and placed in the task queue for execution.
- Each worker thread in the thread pool continuously retrieves tasks from the queue and executes them in a loop.
#### Thread Safety
- Mutex `queueMutex` is used to ensure thread-safe access to the task queue.
- Condition variable `condition` is used to implement thread waiting and waking, avoiding busy-waiting.

### Buffer
#### 1. Interact with fd
- Read data from fd
> use `iovec` as extra buffer to guarantee read data from fd in one time
- Write data to fd
#### 2. Serve for logic processing
- Retrieve String from inputBuffer
- Append String to outputBuffer
#### 3. Expand Buffer Automatically


## TODO LIST
- Move the Worker from the Connector to the thread pool
- Add Support for Database

## Reference
- [muduo](https://github.com/chenshuo/muduo)
- [ThreadPool](https://github.com/progschj/ThreadPool)