# Jupyter

# [Jupyter Docker Stacks](https://jupyter-docker-stacks.readthedocs.io/en/latest/)
Jupyter Docker Stacks are a set of ready-to-run Docker images containing Jupyter applications and interactive computing tools. You can use a stack image to do any of the following (and more):

- Start a personal Jupyter Server with the JupyterLab frontend (default)

- Run JupyterLab for a team using JupyterHub

- Start a personal Jupyter Server with the Jupyter Notebook frontend in a local Docker container

- Write your own project Dockerfile

> Since 2023-10-20 our images are only pushed to Quay.io registry. 

## Example 1
This command pulls the `jupyter/scipy-notebook` image tagged `2024-01-08` from Quay.io if it is not already present on the local host.
It then starts a container running a Jupyter Server with the JupyterLab frontend and exposes the container’s internal port `8888` to port `10000` of the host machine:
```
docker run -p 10000:8888 quay.io/jupyter/scipy-notebook:2024-01-08
```
You can modify the port on which the container’s port is exposed by changing the value of the -p option to -p 8888:8888.

Visiting `http://<hostname>:10000/?token=<token>` in a browser loads JupyterLab, where:

- The `hostname` is the name of the computer running Docker

- The `token` is the secret token printed in the console.

The container remains intact for restart after the Server exits.

## Example 2
This command pulls the `jupyter/datascience-notebook` image tagged `2024-01-08` from Quay.io if it is not already present on the local host.
It then starts an ephemeral container running a Jupyter Server with the JupyterLab frontend and exposes the server on host port 10000.
```
docker run -it --rm -p 10000:8888 -v "${PWD}":/home/jovyan/work quay.io/jupyter/datascience-notebook:2024-01-08
```
The use of the `-v` flag in the command mounts the current working directory on the host (`${PWD}` in the example command) as `/home/jovyan/work `in the container.
The server logs appear in the terminal.

Visiting `http://<hostname>:10000/?token=<token>` in a browser loads JupyterLab.

Due to the usage of the flag `--rm` Docker automatically cleans up the container and removes the file system when the container exits, but any changes made to the `~/work` directory and its files in the container will remain intact on the host. The `-it` flag allocates pseudo-TTY.

> By default, jupyter’s root_dir is `/home/jovyan`. So, new notebooks will be saved there, unless you change the directory in the file browser.

## Choosing Jupyter frontend
JupyterLab is the default for all the Jupyter Docker Stacks images.
It is still possible to switch back to Jupyter Notebook (or to launch a different startup command).
You can achieve this by passing the environment variable `DOCKER_STACKS_JUPYTER_CMD=notebook` (or any other valid jupyter subcommand) at container startup.
- https://jupyter-docker-stacks.readthedocs.io/en/latest/using/common.html#alternative-commands

```
# Run Jupyter Server with the Jupyter Notebook frontend
docker run -it --rm \
    -p 8888:8888 \
    -e DOCKER_STACKS_JUPYTER_CMD=notebook \
    quay.io/jupyter/base-notebook
# Executing the command: jupyter notebook ...

# Use Jupyter NBClassic frontend
docker run -it --rm \
    -p 8888:8888 \
    -e DOCKER_STACKS_JUPYTER_CMD=nbclassic \
    quay.io/jupyter/base-notebook
# Executing the command: jupyter nbclassic ...
```

## Conda Environments
The default Python 3.x Conda environment resides in `/opt/conda`. The `/opt/conda/bin` directory is part of the default jovyan user’s `${PATH}`.
That directory is also searched for binaries when run using sudo (sudo my_binary will search for my_binary in `/opt/conda/bin/`.

The jovyan user has full read/write access to the `/opt/conda` directory.
You can use either mamba, pip, or conda (mamba is recommended) to install new packages without any additional permissions.

```
# install a package into the default (python 3.x) environment and cleanup it after
# the installation
mamba install --yes some-package && \
    mamba clean --all -f -y && \
    fix-permissions "${CONDA_DIR}" && \
    fix-permissions "/home/${NB_USER}"

pip install --no-cache-dir some-package && \
    fix-permissions "${CONDA_DIR}" && \
    fix-permissions "/home/${NB_USER}"

conda install --yes some-package && \
    conda clean --all -f -y && \
    fix-permissions "${CONDA_DIR}" && \
    fix-permissions "/home/${NB_USER}"
```

- `mamba` is a CLI tool to manage conda s environments.
If you already know conda, great, you already know mamba ! 
If you're new to this world, don't panic you will find everything you need in this documentation.
We recommend to get familiar with [concepts first](https://mamba.readthedocs.io/en/latest/user_guide/mamba.html).

- Quickstart
The mamba create command creates a new environment.

You can create an environment with the name nameofmyenv by calling:
```
mamba create -n nameofmyenv <list of packages>
```

After this process has finished, you can _activate_ the virtual environment by calling mamba activate <nameofmyenv>. For example, to install JupyterLab from the conda-forge channel and then run it, you could use the following commands:
```
mamba create -n myjlabenv jupyterlab -c conda-forge
mamba activate myjlabenv  # activate our environment
jupyter lab               # this will start up jupyter lab and open a browser
```

Once an environment is activated, `mamba install` can be used to install further packages into the environment.
```
mamba activate myjlabenv
mamba install bqplot  # now you can use bqplot in myjlabenv
mamba install "matplotlib>=3.5.0" cartopy  # now you installed matplotlib with version>=3.5.0 and default version of cartopy
```
