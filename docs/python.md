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
