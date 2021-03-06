/*
signals are a simple way for processes to communicate

signals are limited to passing a single byte between two processes

what almost each signal does is all predefined by ansi c
or specific operating systems

# example: control c

an example most linux people have used directly already:
hit ctrl+c in a terminal and this will send an INT signal
to the running process.

Unless the process implements a handler for this process,
this has the effect of destroying the process,
which is what happens by default for most, but not all signals.

# ansi c

there is a small C ANSI signal interface,
and individual operating systems may implement many more
which is the case for linux

6 signals defined:

- SIGABRT: (Abort) Abnormal termination, such as is initiated by the abort function.

- SIGFPE:  (Floating-Point Exception) Erroneous arithmetic operation.

    Such as zero divide or an operation resulting in overflow

    Not necessarily generated by a floating-point operation, may be an integer operation.

    floating point division by 0 `1.0 / 0.0` does not generate exceptoins since it is defined by IEEE:
    <http://stackoverflow.com/questions/7267838/division-by-zero-does-not-throw-sigfpe>

    integer division by 1 / 0 may generate a SIGFPE

    TODO is 1 / 0 guaranteed to generate a `SIGFPE`?

    TODO how to deal with it? it just keeps coming back time after time.

    very similar question, but his behaviour is not exactly the same as mine:
        <http://stackoverflow.com/questions/14905947/what-does-c-c-handler-sigfpe>


- SIGILL:  (Illegal Instruction) Things that can generate this on Linux x86:

    - making an assembly operation that is only legal from kernel mode on user mode,
        such as modify cs, ds or ss registers from a user program.

- SIGSEGV: (Segmentation Violation)

    Invalid access to storage: When a program tries to read or write outside the memory it has allocated.

- SIGINT:  (Interrupt) Ask the process to terminate, possibly nicely. Sample cause: linux terminal ctrl+c.

- SIGTERM: (Terminate) Termination request sent to program. Major cause: shutdown, window close.

    The only difference between `SIGINT` and `SIGTERM` is that `SIGINT` is meant to be generated manually by users
    from the terminal, while `SIGTERM` is meant to be generated by other applications.

Functions defined (handlers):

- SIG_DFL Default handling: The signal is handled by the default action for that particular signal.
- SIG_IGN Ignore Signal: The signal is ignored.
- SIG_ERR Special return value indicating failure.

#raise

TODO

#abort

TODO

#allows opeartions on signal handlers

TODO can any operation be done in a signal handler (print to stdout for example?)

#sig_atomic_t

TODO

# sources

- <http://www.alexonlinux.com/signal-handling-in-linux>

    good intro, covers lots of POSIX / linux specific stuff,
    but also has nice examples that help understand the ANSI C model.
*/

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

void signal_handler( int sig )
{
    //sig arg allows us to use a single function for several different signals:
    //just look at it and decide which action to take based on the signal number

        //printf( "sig: %d\n", sig );

    //after the signal is dealt with, the handler is then changed to its default action
    //if you want to continue using this handler for future signals, you have to reregister
    //it here: TODO confirm. If I remove this it does not work.

        signal( sig, signal_handler );

    //you can change the action handler at any time
    //for example, if you uncomment this line, only the first signal will be ignored
    //and but the second will be dealt with the default action:

        //(void) signal( sig, SIG_DFL );
}

int main()
{
    /*
        registers signal_handler as handler for SIGINT and SIGTERM:

        you can get a SIGINT on terminals via C-C on linux or C-Z on windows
        while the program runs on the foreground:

        you can get a SIGTERM by

        - opening a new termianal (ex: `xterm` on linux)
        - running this program on that terminal

        from the current terminal, and then closing the first terminal.
    */

        signal( SIGINT, signal_handler );
        signal( SIGTERM, signal_handler );
        signal( SIGFPE, signal_handler );

    //#floating point exception

        //{
        //    int i = 0;
        //    int j = 0;
        //    //cannot do 1 / 0 or the compiler will give a warning. Lets dupe him:
        //    j = 1 / i;
        //    //you need this `printf` or the compiler may optimize your division away
        //    printf( "%d", j );
        //}

    //int i = 0;
    //while ( i < 10 )
    //{
    //    printf( "%d\n", i );
    //    i++;

    //    /*
    //    TODO how to wait here for say 1 sec, so that user can try signals out?
    //    there is no simple ANSI way of doing that...

    //    on posix we would:
    //    */

    //        //sleep( 1 );
    //}

    //TODO why does this not work:

        //puts( "press any key to exit" );
        //getchar();

    //nor does this:

        //puts( "press any key to exit" );
        //const int bufsiz = 16;
        //char buf[bufsiz];
        //fgets( buf, bufsiz, stdin );

    //if the user enters a C-C, the program exits

    //TODO is it because C-C also outputs chars to the stream, including a newline?

    return EXIT_SUCCESS;
}
