# Python

“The nice thing about standards is that you have so many to choose from; furthermore, if you do not like any of them, you can just wait for next year’s model.”  --- Andrew S. Tanenbaum

- Conda
  - Use different python versions
    - `conda create --name env_name python=3.10`

  - Disable Base Env activation
    - `conda config --set auto_activate_base false`

- venv
  - Use the system python version. 
    - `python3 -m venv`

# venv vs conda

```
$ python -m venv --system-site-packages --symlinks .venv
$ conda create -n venv python=3.8.3
```
venv cannot specify the python version but the conda can.


# built-in functions
- bin
  Return the binary representation of an integer.
  ```
  >>> bin(2796202)
  '0b1010101010101010101010'
  ```
- hex
  Return the hexadecimal representation of an integer.
  ```
  >>> hex(12648430)
  '0xc0ffee'
  ```
- oct
  Return the octal representation of an integer.
  ```
  >>> oct(342391)
  '0o1234567'
  ```

- ord(c, /)
  Return the Unicode code point for a one-character string.
  ```
  >>> ord('A')
  65
  ```

- chr
  `chr ()` is a built-in function in Python that is used to convert the ASCII code into its corresponding character.
  The parameter passed in the function is a numeric, integer type value. The function returns a character for which the parameter is the ASCII code.
  ```
  >>> chr(0x48)
  'H'
  ```

# Sleep
- `sleep` function works with seconds.
```python

# Import time
import time

# sleep for 3 seconds
print('Sleep time:', str(3), 'seconds')
time.sleep(3)
print('Woke up after:', str(3), 'seconds')
```

- `sleep` function works with milliseconds
```python
# Sleep 300 milliseconds
time.sleep(300/1000)
print('300 milliseconds passed')   
```

- Using the Timer() to Slepp Milliseconds
Uses the timer function from the Python threading module to schedule a function to be executed after sleeping for milliseconds. The Timer function takes two arguments: the first is the time delay in seconds, and the second is the function that should be executed.

```python
from threading import Timer
# Using the Timer() function
def Sparkbyexamples():
  print("After 0.08 seconds, Sparkbyexamples will be printed")
result = Timer(0.08, Sparkbyexamples)
result.start() 

# Output
# After 0.08 seconds, Sparkbyexamples will be printed

# Use the Timer() function
from threading import Timer
def time_func():
    print("This text is printed after 800 milliseconds.")
result = Timer(0.8 , time_func)
result.start()  

# Output
# This text is printed after 800 milliseconds.
```

## Frequently Asked Questions On Python Sleep Milliseconds
- Can I use time.sleep directly with milliseconds?
The time.sleep function in Python only accepts arguments in seconds, not milliseconds. If you want to sleep for a specific number of milliseconds, you need to convert the milliseconds to seconds before using time.sleep.

- How precise is the sleep duration using time.sleep?
The actual sleep duration may not be precise due to factors like the operating system’s scheduling granularity. If precise timing is crucial, consider using more advanced timing mechanisms, such as the threading.Timer class or the asyncio module.

- Can I use the threading module for sleeping in milliseconds?
The threading.Timer class allows you to create a timer that runs a function after a specified number of seconds, making it suitable for precise timing.

- Are there any alternatives to time.sleep for asynchronous programs?
If you are working with asynchronous code, the asyncio.sleep function is more suitable. It allows sleeping in an asynchronous program without blocking the event loop.

