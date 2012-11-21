# VendingMachine

Simple vending machine. GUI made in "command line" mode using Borland's graphics.h lib. Maybe someday I will make this work again :P.

*Watch out!* Comments and naming in Portuguese :) 

## 2012-11-21

Finally, able to compile it (with a mountain of warnings :[) and run it in Ubuntu!

### Helpfull links

http://blog.eternal-thinker.com/2010/09/how-to-use-graphicsh-in-ubuntu.html
I actually done a lot of googling, but the above links sums it up :P

### I had to declare and implement dummy calls to

```c++
	settextstyle(...); // from graphics.h
	clrscr(); // from conio.h
```

Both of those implemented as empty functions.

### Very ugly hacks

I had to do a very ugly hack on libgraph-1.0.2; to avoid this crash:
```
	../../src/xcb_io.c:221: poll_for_event: Assertion `(((long) (event_sequence) - (long) (dpy->request)) <= 0)' failed.
```

I had to comment lines 98 to 101 of `libgraph.c`:

```c++
  /*_pid = fork();	// Don't bother with this. 
  if 			//The process forks a child to check for Ctrl-C.
    (!_pid) ctrlbreak();
  else*/
    {
      signal(SIGUSR1, inthandler);
      signal (SIGUSR2, refresh_interrupt);
    }
```


