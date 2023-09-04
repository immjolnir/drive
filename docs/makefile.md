# What do $@ and $< in a makefile mean?

- https://stackoverflow.com/questions/3220277/what-do-the-makefile-symbols-and-mean
- https://www.gnu.org/software/make/manual/html_node/Automatic-Variables.html#Automatic-Variables

* `$@` is the name of the target being generated

* `$<` the first prerequisite (usually a source file)

* `$^` the names of all the prerequisites, with spaces between them

* `$*` The stem of the target filename. A stem is typically a filename without its suffix. Its use outside of pattern rules is discouraged.

For example, consider the following declaration:
```
all: library.cpp main.cpp
```
In this case:
* $@ evaluates to all
* $< evaluates to library.cpp
* $^ evaluates to library.cpp main.cpp

