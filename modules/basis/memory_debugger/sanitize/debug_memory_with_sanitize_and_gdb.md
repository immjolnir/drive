# [Debug memory errors with Valgrind and GDB](https://developers.redhat.com/articles/2021/11/01/debug-memory-errors-valgrind-and-gdb)

Buffer overflows, memory leaks, and similar memory issues plague many C and C++ programs. Valgrind is a sophisticated utility for finding low-level programming errors, particularly involving memory use. The GNU Project Debugger (GDB), is a popular tool for use with C/C++ and other languages.

This article explains how to use __Valgrind__ and __GDB__ together along with __vgdb__, a small program that serves as an intermediary between Valgrind and GDB. We'll debug a simple program containing various intentional bugs to demonstrate the combined usage of Valgrind, GDB, and vgdb. This buggy program writes to invalid memory, uses an uninitialized variable, and leaks memory. We will see how to combine Valgrind, GDB, and vgdb to find each of these bugs.

Note: It is also possible to use vgdb as an intermediary with the shell, although this usage will not be discussed.

## An example program with memory errors

We will use the following error-filled program, called bad_prog.c, as the basis for this article: bad_proc.c

- Compile the program with the following gcc command, which includes the necessary -g option to enable debugging information:

```
$ gcc -g -o bad bad_prog.c
```

- When you run the program, it seems to behave nicely:
```
./bad
buf[20]: 0
hey
```

- But when you run it under Valgrind, the code's problematic behaviors are uncovered. Valgrind produces output such as:
```

valgrind -q ./bad
==1696253== Invalid write of size 8
==1696253==    at 0x4011E8: setup_foo (bad_prog.c:23)
==1696253==    by 0x401210: main (bad_prog.c:30)
==1696253==  Address 0x4e40090 is 0 bytes after a block of size 80 alloc'd
==1696253==    at 0x4A36EA7: malloc (vg_replace_malloc.c:307)
==1696253==    by 0x4011B4: setup_foo (bad_prog.c:21)
==1696253==    by 0x401210: main (bad_prog.c:30)
==1696253== 
buf[0][0]: 82053872
==1696253== Conditional jump or move depends on uninitialised value(s)
==1696253==    at 0x401222: main (bad_prog.c:33)
==1696253== 
hey
```


Now let's combine Valgrind with GDB and take a closer look.

## Using Valgrind and GDB together
Start up two terminal windows so that you can interact with Valgrind and GDB simultaneously.

- In one terminal, run Valgrind with the `--vgdb-error=0 `option. 

When running with `--vgdb-error=n`, Valgrind waits for `n` errors to occur before pausing and waiting for a connection from GDB. However, we want to be able to issue GDB commands before Valgrind finds any errors, so we use `--vgdb-error=0`. The `--vgdb-error=0` option causes Valgrind to pause before executing any instruction. Specifying `--vgdb-error` with any value also enables `--vgdb=yes`, which is required for using GDB with Valgrind.

Valgrind starts up with these messages:

```
$ valgrind -q --vgdb-error=0 ./bad
==3781640== (action at startup) vgdb me ...
==3781640==
==3781640== TO DEBUG THIS PROCESS USING GDB: start GDB like this
==3781640== /path/to/gdb ./bad
==3781640== and then give GDB the following command
==3781640== target remote | /usr/local/lib/valgrind/../../bin/vgdb --pid=3781640
==3781640== --pid is optional if only one valgrind process is running
```

Note that Valgrind provides instructions for starting GDB as well as the command to use in order to connect GDB with Valgrind.

- In the other terminal, run GBD as the Valgrind output instructed:
```
$ gdb ./bad

...

Reading symbols from ./bad...
```

- To connect to __vgdb__, follow the instructions written in the Valgrind output:
```
(gdb) target remote | vgdb --pid=3781640
Remote debugging using | vgdb --pid=3781640
relaying data between gdb and process 3781640
warning: remote target does not support file transfer, attempting to access files from local filesystem.
Reading symbols from /lib64/ld-linux-x86-64.so.2...
Reading symbols from /usr/lib/debug/usr/lib64/ld-2.31.so.debug...
0x0000000004002110 in _start () from /lib64/ld-linux-x86-64.so.2
```
Note: The `--pid` option is necessary only if you use vgdb with multiple Valgrind sessions.

Normally, when it starts, Valgrind places you in your program's main function. Because the `--vgdb-error=0` option causes Valgrind to pause before executing any instruction, we start in ld.so, where the program is being set up to run.

- Now you can use normal GDB commands, such as setting a breakpoint and running the program from the current point:
```
(gdb) break main
Breakpoint 1 at at 0x40120a: file bad_prog.c, line 30.
(gdb) continue
Continuing.

Breakpoint 1, main () at bad_prog.c:30
30 setup_foo(&s);
(gdb) c
Continuing.

Program received signal SIGTRAP, Trace/breakpoint trap.
0x00000000004011ed in setup_foo (s=0x1ffefff420) at bad_prog.c:23
23 s->buf[i] = malloc(20 * sizeof(int));
```

The __SIGTRAP__ signal informs us that something is wrong. Valgrind emulates a "trapping instruction" by generating a __SIGTRAP__ signal.
This __SIGTRAP__ is sent by Valgrind only when attached to GDB, so that GDB can intercept the signal. Otherwise, Valgrind would simply print the error.

## Investigate an invalid write to memory

Let's see whether there's something new at the terminal where __vgdb__ is running:

```
==875162== Invalid write of size 8
==875162== at 0x4011ED: setup_foo (bad_prog.c:23)
==875162== by 0x401215: main (bad_prog.c:30)
==875162== Address 0x4e39090 is 0 bytes after a block of size 80 alloc'd
==875162== at 0x4A36EA7: malloc (vg_replace_malloc.c:307)
==875162== by 0x4011B9: setup_foo (bad_prog.c:21)
==875162== by 0x401215: main (bad_prog.c:30)
==875162==
==875162== (action on error) vgdb me ...
```

__Valgrind__ tells us we're writing beyond the end of a block of memory allocated with `malloc()`. This example illustrates how can Valgrind and GDB work interactively through __vgdb__.

To find out what's wrong, we return to the GDB terminal. GDB already told us that the problem is happening during memory allocation:
```
23 s->buf[i] = malloc(20 * sizeof(int));
```

Let's print the value of i and the addresses of our array members:
```
print i
$1 = 10
(gdb) p &s->buf[i]
$2 = (int **) 0x4e40090
(gdb) print sizeof (int)
$3 = 4
```

Here we can see that the address of the `&s->buf[10]` array member is `0x4e40090`. Valgrind told us that this address comes after the allocated block. Using this information, we see that the allocated memory block is too small.

In fact, this bug was caused by specifying the wrong type to the `sizeof` operator. We requested `sizeof(int)`, which on this particular architecture is 4 bytes, but the correct type is actually `int **`, whose size is 8 bytes:
```
print sizeof(int **)
$4 = 8
```


## 2. Discover uninitialized values
Lets type `c` to continue until we get back to our main function:

```
Program received signal SIGTRAP, Trace/breakpoint trap.
0x0000000000401227 in main () at bad_prog.c:33
33 if (s.flag1 || s.flag2)
```
If you look at the Valgrind terminal now, you see that Valgrind complains about uninitialized values:
```

==566986== Conditional jump or move depends on uninitialised value(s)
==566986==    at 0x401222: main (bad_prog.c:33)
==566986== 
==566986== (action on error) vgdb me ... 
```

GDB has a [monitor](https://valgrind.org/docs/manual/manual-core-adv.html#manual-core-adv.valgrind-monitor-commands) command that sends special requests to a remote target, in this case Valgrind. These requests include an extra set of commands that are specific to the target. Therefore, the commands shown in this article are specific to using GDB with Valgrind. The command won't be available under other circumstances.

To display the help for the monitor command, type:
```
(gdb) monitor help
general valgrind monitor commands:
  help [debug]            : monitor command help. With debug: + debugging commands
  v.wait [<ms>]           : sleep <ms> (default 0) then continue
  v.info all_errors       : show all errors found so far
  v.info last_error       : show last error found
  v.info location <addr>  : show information about location <addr>
  v.info n_errs_found [msg] : show the nr of errors found so far and the given msg
  v.info open_fds         : show open file descriptors (only if --track-fds=[yes|all])
  v.kill                  : kill the Valgrind process
  v.clo <clo_option>...   : changes one or more dynamic command line options
     with no clo_option, show the dynamically changeable options.
  v.set gdb_output        : set valgrind output to gdb
  v.set log_output        : set valgrind output to log
  v.set mixed_output      : set valgrind output to log, interactive output to gdb
  v.set merge-recursive-frames <num> : merge recursive calls in max <num> frames
  v.set vgdb-error <errornr> : debug me at error >= <errornr> 

memcheck monitor commands:
  xb <addr> [<len>]
        prints validity bits for <len> (or 1) bytes at <addr>
            bit values 0 = valid, 1 = invalid, __ = unaddressable byte
        Then prints the bytes values below the corresponding validity bits
        in a layout similar to the gdb command 'x /<len>xb <addr>'
        Example: xb 0x8049c78 10
  get_vbits <addr> [<len>]
        Similar to xb, but only prints the validity bytes by group of 4.
  make_memory [noaccess|undefined
                     |defined|Definedifaddressable] <addr> [<len>]
        mark <len> (or 1) bytes at <addr> with the given accessibility
  check_memory [addressable|defined] <addr> [<len>]
        check that <len> (or 1) bytes at <addr> have the given accessibility
            and outputs a description of <addr>
  leak_check [full*|summary|xtleak]
                [kinds kind1,kind2,...|reachable|possibleleak*|definiteleak]
                [heuristics heur1,heur2,...]
                [increased*|changed|any]
                [unlimited*|limited <max_loss_records_output>]
            * = defaults
         xtleak produces an xtree full leak result in xtleak.kcg.%p.%n
       where kind is one of:
         definite indirect possible reachable all none
       where heur is one of:
         stdstring length64 newarray multipleinheritance all none*
       Examples: leak_check
                 leak_check summary any
                 leak_check full kinds indirect,possible
                 leak_check full reachable any limited 100
  block_list <loss_record_nr>|<loss_record_nr_from>..<loss_record_nr_to>
                [unlimited*|limited <max_blocks>]
                [heuristics heur1,heur2,...]
        after a leak search, shows the list of blocks of <loss_record_nr>
        (or of the range <loss_record_nr_from>..<loss_record_nr_to>).
        With heuristics, only shows the blocks found via heur1,heur2,...
            * = defaults
  who_points_at <addr> [<len>]
        shows places pointing inside <len> (default 1) bytes at <addr>
        (with len 1, only shows "start pointers" pointing exactly to <addr>,
         with len > 1, will also show "interior pointers")
  xtmemory [<filename>]
        dump xtree memory profile in <filename> (default xtmemory.kcg.%p.%n)
```

One useful monitor command is xb <addr> [<len>], which prints information about the validity of <len> bytes (the default is 1) at <addr>. On the first line of output, the command prints a character that indicates whether the four bits at that position are defined. A defined bit is one that the program has written, whereas an undefined bit hasn't been written to yet, or was overwritten with another undefined bit.

In the output from monitor xb, a 0 indicates that the bits are defined, a 1 indicates that the bits are undefined, and a double underscore (__) indicates an unaddressable byte. On the second line of output, the command prints the addresses followed by the values at those addresses, in a layout similar to the GDB command x/<len> <addr>.

The output from xb is in hexadecimal, so ff equals 1111 1111, indicating an undefined value. Let's use the monitor xb command to check the flag fields in bad_prog. First, print the addresses of the flags and their sizes, then use this information in the xb command:
```

(gdb) print &s.flag1
$4 = (int *) 0x1ffefff400
(gdb) print &s.flag2
$5 = (int *) 0x1ffefff404
(gdb) print sizeof (s.flag1)
$6 = 4
(gdb) monitor xb 0x1ffefff400 4
                ff   ff   ff   ff
0x1FFEFFF400: 0x50 0x12 0x40 0x00
(gdb) monitor xb 0x1ffefff404 4
                00 00 00 00
0x1FFEFFF404: 0x02 0x00 0x00 0x00
```
`flag1` is all __f__'s, which means that it is undefined.

If we check the code, we see that only `flag2` is assigned a value. The first line of output from `monitor xb` for the address of `flag2` consists of all zeros (`00 00 00 00`), showing that the flag is defined.

## 3. Scan for memory leaks
The monitor leak_check command triggers a scan for memory leaks:
```
(gdb) monitor leak_check
==18002== 1,600 (+1,600) (1,440 (+1,440) direct, 160 (+160) indirect)
bytes in 18 (+18) blocks are definitely lost in loss record 3 of 3
==18002== at 0x4A36EA7: malloc (vg_replace_malloc.c:307)
==18002== by 0x4011EC: setup_foo (bad_prog.c:23)
==18002== by 0x401215: main (bad_prog.c:30)
==18002== 
```

Valgrind tells us here that the function `setup_foo`, which is called by `main`, leaks memory allocated by `malloc()`:

```
(gdb) monitor block_list 3
==18002== 1,600 (+1,600) (1,440 (+1,440) direct, 160 (+160) indirect)
bytes in 18 (+18) blocks are definitely lost in loss record 3 of 3
==18002== at 0x4A36EA7: malloc (vg_replace_malloc.c:307)
==18002== by 0x4011EC: setup_foo (bad_prog.c:23)
==18002== by 0x401215: main (bad_prog.c:30)
==18002== 0x4E390D0[80]
==18002== 0x4E39AF0[80] indirect loss record 1
==18002== 0x4E39B80[80] indirect loss record 1

...

==18002== 0x4E39160[80]
```

You can also ask for additional information about the pointers to memory that you think could be leaked. As an example, the variable s is allocated in main, so you would expect it to be on the stack. And indeed, the _montitor who_points_at_ command shows that the stack pointer __RSP__ points to that variable:
```

(gdb) print &s
$1 = (struct foo *) 0x1ffefff400
(gdb) monitor who_points_at 0x1ffefff400
==18002== Searching for pointers to 0x1ffefff400
==18002== tid 1 register RSP pointing at 0x1ffefff400
```

In this case, the information is not very interesting or useful, but when you're somewhere deep in the program and you suspect something has leaked, __who_points_at__ could be very helpful. It prints the list of references to a variable, which can help you find what variable you need to free.

It is possible to do the same with registers. __Memcheck__ keeps a whole "shadow" memory copy of your program. This shadow copy keeps track of which bits are defined.

Valgrind also maintains [shadow registers](https://valgrind.org/docs/manual/manual-core-adv.html#manual-core-adv.gdbserver-shadowregisters), which are "fake" registers that Valgrind provides to GDB to represent shadow bits. If you run Valgrind with the `--vgdb-shadow-registers=yes` option, you can view Valgrind's shadow registers in GDB.

`monitor xb` and `monitor get_vbits` get information on undefined memory for any memory address in the program. The same can be done for everything held in a register.

## 4. Command-line options
You can set some Valgrind [command-line options](https://valgrind.org/docs/manual/manual-core.html#manual-core.dynopts) through monitor commands. For example, if you wish to start tracing system calls from this point on, enter:
```
(gdb) monitor v.clo --trace-syscalls=yes
```

## Ending the debugging session
When you want to terminate the combined Valgrind and GDB debugging session, terminate the Valgrind process first via the GDB kill command. After that, use quit in GDB to exit the debugger:
```
(gdb) kill
Kill the program being debugged? (y or n) y
[Inferior 1 (Remote target) killed]
(gdb) q
```

## Conclusion
We've used Valgrind with GDB to find several programming errors related to memory use. First, we saw how Valgrind identified an invalid memory write and how GDB could be used to further diagnose this problem. Next, we looked for uninitialized data, and finally used GDB to ask Valgrind for a memory leak scan.

A pristine use of memory is crucial to providing secure, robust programs that don't break suddenly during use. The example in this article showed how a program could seem to perform properly in some situations while harboring serious memory flaws. Use Valgrind and GDB to protect your C and C++ code.