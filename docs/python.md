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