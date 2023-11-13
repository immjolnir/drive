# Python Debugging With Pdb
Getting Started: Printing a Variable’s Value
In this first example, we’ll look at using pdb in its simplest form: checking the value of a variable.

Insert the following code at the location where you want to break into the debugger:

```
import pdb; pdb.set_trace()
```
When the line above is executed, Python stops and waits for you to tell it what to do next. You’ll see a (Pdb) prompt. This means that you’re now paused in the interactive debugger and can enter a command.

Starting in Python 3.7, PEP 553 describes the built-in function `breakpoint()`, which makes entering the debugger easy and consistent:
```
breakpoint()
```

By default, `breakpoint()` will `import pdb and call pdb.set_trace()`, as shown above. However, using `breakpoint()` is more flexible and allows you to control debugging behavior via its API and use of the environment variable `PYTHONBREAKPOINT`. For example, setting `PYTHONBREAKPOINT=0` in your environment will completely disable `breakpoint()`, thus disabling debugging. If you’re using Python 3.7 or later, I encourage you to use `breakpoint()` instead of pdb.set_trace().

But, a patch: 
> https://stackoverflow.com/questions/67813813/runtimewarning-ignoring-unimportable-pythonbreakpoint


You can also break into the debugger, without modifying the source and using `pdb.set_trace()` or `breakpoint()`, by running Python directly from the command-line and passing the option `-m pdb`. If your application accepts command-line arguments, pass them as you normally would after the filename. For example:
```
$ python3 -m pdb app.py arg1 arg2
```
There are a lot of pdb commands available. At the end of this tutorial, there is a list of Essential pdb Commands. For now, let’s use the p command to print a variable’s value. Enter `p variable_name` at the (Pdb) prompt to print its value.


## Stepping Through Code
There are two commands you can use to step through code when debugging:

```
s(tep)
        Execute the current line, stop at the first possible occasion
        (either in a function that is called or in the current
        function).

n(ext)
        Continue execution until the next line in the current function
        is reached or it returns.
```
The difference between `n (next)` and `s (step)` is where pdb stops.

Use `n (next)` to continue execution until the next line and stay within the current function, i.e. not stop in a foreign function if one is called. Think of next as “staying local” or “step over”.

Use `s (step)` to execute the current line and stop in a foreign function if one is called. Think of step as “step into”. If execution is stopped in another function, s will print --Call--.

Both n and s will stop execution when the end of the current function is reached and print --Return-- along with the return value at the end of the next line after ->.

## Listing Source Code
Don’t forget the command `ll` (longlist: list the whole source code for the current function or frame). It’s really helpful when you’re stepping through unfamiliar code or you just want to see the entire function for context.
```
longlist | ll
        List the whole source code for the current function or frame.
```

To see a shorter snippet of code, use the command `l (list)`.

## Using Breakpoints

Breakpoints are very convenient and can save you a lot of time. Instead of stepping through dozens of lines you’re not interested in, simply create a breakpoint where you want to investigate. Optionally, you can also tell pdb to break only when a certain condition is true.

Use the command `b (break)` to set a breakpoint. You can specify a line number or a function name where execution is stopped.
```
b(reak) [ ([filename:]lineno | function) [, condition] ]
        Without argument, list all breaks.

        With a line number argument, set a break at this line in the
        current file.  With a function name, set a break at the first
        executable line of that function.  If a second argument is
        present, it is a string specifying an expression which must
        evaluate to true before the breakpoint is honored.

        The line number may be prefixed with a filename and a colon,
        to specify a breakpoint in another file (probably one that
        hasn't been loaded yet).  The file is searched for on
        sys.path; the .py suffix may be omitted.
```
- The command `c (continue)` continues execution until a breakpoint is found.

- Enter `b` with no arguments to see a list of all breakpoints.

- You can disable and re-enable breakpoints using the command `disable bpnumber` and `enable bpnumber`. bpnumber is the breakpoint number from the breakpoints list’s 1st column Num. 

- To delete a breakpoint, use the command `cl (clear)`:
```
cl(ear) filename:lineno
cl(ear) [bpnumber [bpnumber...]]
```
In this example scenario, the get_path() function is failing when it receives a relative path, i.e. the file’s path doesn’t start with /. I’ll create an expression that evaluates to true in this case and pass it to b as the 2nd argument:
- Sample code
```python
# $ cat example4.py 
#!/usr/bin/env python3

import util

filename = __file__
#import pdb; pdb.set_trace()
filename_path = util.get_path(filename)
print(f'path = {filename_path}')

# $ cat util.py 
def get_path(filename):
    """Return file's path or empty string if no path."""
    import os
    head, tail = os.path.split(filename)
    return head
```
- Example
```
$ ./example4.py 
> /code/example4.py(7)<module>()
-> filename_path = util.get_path(filename)
(Pdb) b util.get_path, not filename.startswith('/')
Breakpoint 1 at /code/util.py:1
(Pdb) c
> /code/util.py(3)get_path()
-> import os
(Pdb) a
filename = './example4.py'
(Pdb) 
```

After you create the breakpoint above and enter `c` to continue execution, pdb stops when the expression evaluates to true. The command `a (args)` prints the argument list of the current function.

If you need to break using an expression with a variable name located inside a function, i.e. a variable name not in the function’s argument list, specify the line number:
```
$ ./example4.py 
> /code/example4.py(7)<module>()
-> filename_path = util.get_path(filename)
(Pdb) b util:5, not head.startswith('/')
Breakpoint 1 at /code/util.py:5
(Pdb) c
> /code/util.py(5)get_path()
-> return head
(Pdb) p head
'.'
(Pdb) a
filename = './example4.py'
(Pdb) 
```
You can also set a temporary breakpoint using the command tbreak. It’s removed automatically when it’s first hit. It uses the same arguments as b.


## Continuing Execution
So far, we’ve looked at stepping through code with n (next) and s (step) and using breakpoints with b (break) and c (continue).

There’s also a related command: unt (until).

Use unt to continue execution like c, but stop at the next line greater than the current line. Sometimes unt is more convenient and quicker to use and is exactly what you want. I’ll demonstrate this with an example below.

Let’s first look at the syntax and description for unt:
```
unt(il) [lineno]
        Without argument, continue execution until the line with a
        number greater than the current one is reached.  With a line
        number, continue execution until a line with a number greater
        or equal to that is reached.  In both cases, also stop when
        the current frame returns.
```

Depending on whether or not you pass the line number argument lineno, unt can behave in two ways:

- Without lineno, continue execution until the line with a number greater than the current one is reached. This is similar to n (next). It’s an alternate way to execute and “step over” code. The difference between n and unt is that unt stops only when a line with a number greater than the current one is reached. n will stop at the next logically executed line.
- With lineno, continue execution until a line with a number greater or equal to that is reached. This is like c (continue) with a line number argument.
In both cases, unt stops when the current frame (function) returns, just like `n (next)` and `s (step)`.
- Sample code
```python
$ cat example4unt.py 
#!/usr/bin/env python3

import os


def get_path(fname):
    """Return file's path or empty string if no path."""
    import pdb; pdb.set_trace()
    head, tail = os.path.split(fname)
    for char in tail:
        pass  # Check filename char
    return head


filename = __file__
filename_path = get_path(filename)
print(f'path = {filename_path}')

```
- Console Output
```
$ python3 example4unt.py 
> /home/zhishan/Downloads/example4unt.py(9)get_path()
-> head, tail = os.path.split(fname)
(Pdb) ll
  6  	def get_path(fname):
  7  	    """Return file's path or empty string if no path."""
  8  	    import pdb; pdb.set_trace()
  9  ->	    head, tail = os.path.split(fname)
 10  	    for char in tail:
 11  	        pass  # Check filename char
 12  	    return head
(Pdb) unt
> /home/zhishan/Downloads/example4unt.py(10)get_path()
-> for char in tail:
(Pdb) 
> /home/zhishan/Downloads/example4unt.py(11)get_path()
-> pass  # Check filename char
(Pdb) 
> /home/zhishan/Downloads/example4unt.py(12)get_path()
-> return head
(Pdb) p char, tail
('y', 'example4unt.py')
(Pdb) 
```
The ll command was used first to print the function’s source, followed by unt. pdb remembers the last command entered, so I just pressed Enter to repeat the unt command. This continued execution through the code until a source line greater than the current line was reached.

## Displaying Expressions
Similar to printing expressions with p and pp, you can use the command display [expression] to tell pdb to automatically display the value of an expression, if it changed, when execution stops. Use the command undisplay [expression] to clear a display expression.

- Sample Code
```python
$ cat example4unt.py 
#!/usr/bin/env python3

import os


def get_path(fname):
    """Return file's path or empty string if no path."""
    import pdb; pdb.set_trace()
    head, tail = os.path.split(fname)
    for char in tail:
        pass  # Check filename char
    return head


filename = __file__
filename_path = get_path(filename)
print(f'path = {filename_path}')
```
Here’s the syntax and description for both commands:

```
(Pdb) help display
display [expression]

        Display the value of the expression if it changed, each time execution
        stops in the current frame.

        Without expression, list all display expressions for the current frame.
(Pdb) help undisplay
undisplay [expression]

        Do not display the expression any more in the current frame.

        Without expression, clear all display expressions for the current frame.
```

Below is an example, example4display.py, demonstrating its use with a loop:

```
$ ./example4display.py 
> /code/example4display.py(9)get_path()
-> head, tail = os.path.split(fname)
(Pdb) ll
  6     def get_path(fname):
  7         """Return file's path or empty string if no path."""
  8         import pdb; pdb.set_trace()
  9  ->     head, tail = os.path.split(fname)
 10         for char in tail:
 11             pass  # Check filename char
 12         return head
(Pdb) b 11
Breakpoint 1 at /code/example4display.py:11
(Pdb) c
> /code/example4display.py(11)get_path()
-> pass  # Check filename char
(Pdb) display char
display char: 'e'
(Pdb) c
> /code/example4display.py(11)get_path()
-> pass  # Check filename char
display char: 'x'  [old: 'e']
(Pdb) 
> /code/example4display.py(11)get_path()
-> pass  # Check filename char
display char: 'a'  [old: 'x']
(Pdb) 
> /code/example4display.py(11)get_path()
-> pass  # Check filename char
display char: 'm'  [old: 'a']
```

In the output above, pdb automatically displayed the value of the char variable because each time the breakpoint was hit its value had changed. Sometimes this is helpful and exactly what you want, but there’s another way to use display.

You can enter display multiple times to build a watch list of expressions. This can be easier to use than p. After adding all of the expressions you’re interested in, simply enter display to see the current values:
```

$ ./example4display.py 
> /code/example4display.py(9)get_path()
-> head, tail = os.path.split(fname)
(Pdb) ll
  6     def get_path(fname):
  7         """Return file's path or empty string if no path."""
  8         import pdb; pdb.set_trace()
  9  ->     head, tail = os.path.split(fname)
 10         for char in tail:
 11             pass  # Check filename char
 12         return head
(Pdb) b 11
Breakpoint 1 at /code/example4display.py:11
(Pdb) c
> /code/example4display.py(11)get_path()
-> pass  # Check filename char
(Pdb) display char
display char: 'e'
(Pdb) display fname
display fname: './example4display.py'
(Pdb) display head
display head: '.'
(Pdb) display tail
display tail: 'example4display.py'
(Pdb) c
> /code/example4display.py(11)get_path()
-> pass  # Check filename char
display char: 'x'  [old: 'e']
(Pdb) display
Currently displaying:
char: 'x'
fname: './example4display.py'
head: '.'
tail: 'example4display.py'
```

## Python Caller ID
In this last section, we’ll build upon what we’ve learned so far and finish with a nice payoff. I use the name “caller ID” in reference to the phone system’s caller identification feature. That is exactly what this example demonstrates, except it’s applied to Python.

Here’s the source for the main script example5.py:

#!/usr/bin/env python3

import fileutil


def get_file_info(full_fname):
    file_path = fileutil.get_path(full_fname)
    return file_path


filename = __file__
filename_path = get_file_info(filename)
print(f'path = {filename_path}')
Here’s the utility module fileutil.py:

def get_path(fname):
    """Return file's path or empty string if no path."""
    import os
    import pdb; pdb.set_trace()
    head, tail = os.path.split(fname)
    return head
In this scenario, imagine there’s a large code base with a function in a utility module, get_path(), that’s being called with invalid input. However, it’s being called from many places in different packages.

How do you find who the caller is?

Use the command w (where) to print a stack trace, with the most recent frame at the bottom:

$ ./example5.py 
> /code/fileutil.py(5)get_path()
-> head, tail = os.path.split(fname)
(Pdb) w
  /code/example5.py(12)<module>()
-> filename_path = get_file_info(filename)
  /code/example5.py(7)get_file_info()
-> file_path = fileutil.get_path(full_fname)
> /code/fileutil.py(5)get_path()
-> head, tail = os.path.split(fname)
(Pdb) 
Don’t worry if this looks confusing or if you’re not sure what a stack trace or frame is. I’ll explain those terms below. It’s not as difficult as it might sound.

Since the most recent frame is at the bottom, start there and read from the bottom up. Look at the lines that start with ->, but skip the 1st instance since that’s where pdb.set_trace() was used to enter pdb in the function get_path(). In this example, the source line that called the function get_path() is:

-> file_path = fileutil.get_path(full_fname)
The line above each -> contains the filename, line number (in parentheses), and function name the source line is in. So the caller is:

  /code/example5.py(7)get_file_info()
-> file_path = fileutil.get_path(full_fname)
That’s no surprise in this small example for demonstration purposes, but imagine a large application where you’ve set a breakpoint with a condition to identify where a bad input value is originating.

Now we know how to find the caller.

But what about this stack trace and frame stuff?

A stack trace is just a list of all the frames that Python has created to keep track of function calls. A frame is a data structure Python creates when a function is called and deletes when it returns. The stack is simply an ordered list of frames or function calls at any point in time. The (function call) stack grows and shrinks throughout the life of an application as functions are called and then return.

When printed, this ordered list of frames, the stack, is called a stack trace. You can see it at any time by entering the command w, as we did above to find the caller.

See this call stack article on Wikipedia for details.

To understand better and get more out of pdb, let’s look more closely at the help for w:

(Pdb) h w
w(here)
        Print a stack trace, with the most recent frame at the bottom.
        An arrow indicates the "current frame", which determines the
        context of most commands. 'bt' is an alias for this command.
What does pdb mean by “current frame”?

Think of the current frame as the current function where pdb has stopped execution. In other words, the current frame is where your application is currently paused and is used as the “frame” of reference for pdb commands like p (print).

p and other commands will use the current frame for context when needed. In the case of p, the current frame will be used for looking up and printing variable references.

When pdb prints a stack trace, an arrow > indicates the current frame.

How is this useful?

You can use the two commands u (up) and d (down) to change the current frame. Combined with p, this allows you to inspect variables and state in your application at any point along the call stack in any frame.

Here’s the syntax and description for both commands:

Command	Syntax	Description
u	u(p) [count]	Move the current frame count (default one) levels up in the stack trace (to an older frame).
d	d(own) [count]	Move the current frame count (default one) levels down in the stack trace (to a newer frame).
Let’s look at an example using the u and d commands. In this scenario, we want to inspect the variable full_fname that’s local to the function get_file_info() in example5.py. In order to do this, we have to change the current frame up one level using the command u:

$ ./example5.py 
> /code/fileutil.py(5)get_path()
-> head, tail = os.path.split(fname)
(Pdb) w
  /code/example5.py(12)<module>()
-> filename_path = get_file_info(filename)
  /code/example5.py(7)get_file_info()
-> file_path = fileutil.get_path(full_fname)
> /code/fileutil.py(5)get_path()
-> head, tail = os.path.split(fname)
(Pdb) u
> /code/example5.py(7)get_file_info()
-> file_path = fileutil.get_path(full_fname)
(Pdb) p full_fname
'./example5.py'
(Pdb) d
> /code/fileutil.py(5)get_path()
-> head, tail = os.path.split(fname)
(Pdb) p fname
'./example5.py'
(Pdb) 
The call to pdb.set_trace() is in fileutil.py in the function get_path(), so the current frame is initially set there. You can see it in the 1st line of output above:

> /code/fileutil.py(5)get_path()
To access and print the local variable full_fname in the function get_file_info() in example5.py, the command u was used to move up one level:

(Pdb) u
> /code/example5.py(7)get_file_info()
-> file_path = fileutil.get_path(full_fname)
Note in the output of u above that pdb printed the arrow > at the beginning of the 1st line. This is pdb letting you know the frame was changed and this source location is now the current frame. The variable full_fname is accessible now. Also, it’s important to realize the source line starting with -> on the 2nd line has been executed. Since the frame was moved up the stack, fileutil.get_path() has been called. Using u, we moved up the stack (in a sense, back in time) to the function example5.get_file_info() where fileutil.get_path() was called.

Continuing with the example, after full_fname was printed, the current frame was moved to its original location using d, and the local variable fname in get_path() was printed.

If we wanted to, we could have moved multiple frames at once by passing the count argument to u or d. For example, we could have moved to module level in example5.py by entering u 2:

$ ./example5.py 
> /code/fileutil.py(5)get_path()
-> head, tail = os.path.split(fname)
(Pdb) u 2
> /code/example5.py(12)<module>()
-> filename_path = get_file_info(filename)
(Pdb) p filename
'./example5.py'
(Pdb) 
It’s easy to forget where you are when you’re debugging and thinking of many different things. Just remember you can always use the aptly named command w (where) to see where execution is paused and what the current frame is.

## Essentail Pdb Commands
- p
- pp
- n
- s
- c
- unt
- l
- ll
- b
- w
- u
- d
- h: See a list of available commands
- h <topic>: Show help for a command or topic
- h pdb: Show the full pdb documentation.
- q: Quit the debugger and exit

------------------

```
(Pdb) help

Documented commands (type help <topic>):
========================================
EOF    c          d        h         list      q        rv       undisplay
a      cl         debug    help      ll        quit     s        unt      
alias  clear      disable  ignore    longlist  r        source   until    
args   commands   display  interact  n         restart  step     up       
b      condition  down     j         next      return   tbreak   w        
break  cont       enable   jump      p         retval   u        whatis   
bt     continue   exit     l         pp        run      unalias  where    

Miscellaneous help topics:
==========================
exec  pdb
```
(Pdb) help b



run [args...]
        Restart the debugged python program. If a string is supplied
        it is split with "shlex", and the result is used as the new
        sys.argv.  History, breakpoints, actions and debugger options
        are preserved.  "restart" is an alias for "run".


r(eturn)
        Continue execution until the current function returns.



w(here)
        Print a stack trace, with the most recent frame at the bottom.
        An arrow indicates the "current frame", which determines the
        context of most commands.  'bt' is an alias for this command.


