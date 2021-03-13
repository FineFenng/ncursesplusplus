# Event Loop

An `EventLoop` is responsible for waiting on some event to happen, and then
posting that event to the global `EventQueue`. Each event loop should be run in
its own thread.

## User Input Event Loop

The main `UserInputEventLoop` waits on user input from the mouse, keyboard,
or a terminal resize, and then posts that event to the correct Widget. This is
run on the main thread.

## Creating New Event Loops

New Event Loop types can be created, these are useful if there is an async
operation in your program. The new loop can wait on whatever async activity and
post a (typically custom) event to the Event Queue to be processed safely in the
main thread. Posting events to the Event Queue is thread-safe. Events processed
from the Event Queue are handled one at a time and make it simple to have
thread-safe handling of each event, even if it is posted from another thread.

A new Event Loop can be created by publicly inheriting from the `EventLoop`
class and overriding the `virtual void LoopFunction()` method. This overridden
method should wait for an event to occur, once it does, it can post it with the
`System::post_event(...)` method.

The new Event Loop can be owned by a Widget, the `void EventLoop::run_async()`
method should be used to launch the loop in a new thread.

All Event Loops post their events to a single, global queue, the main thread is
the only thread that processes the Event Queue.

## See Also

- [Reference](https://a-n-t-h-o-n-y.github.io/TermOx/classox_1_1Event__loop.html)
- [System](system.md)
- [Events](events.md)
