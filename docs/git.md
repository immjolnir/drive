# git

- `.gitignore`: exclude secret or unwanted ones.

- `.gitkeep` is just a placeholder. A dummy file, so git keeps empty directories in the repository.

The .gitkeep feature is not a part of Git, but it is a trick we use to add empty directories to a Git repository.

Generally, Git doesn't add empty directories and sometimes we may need to add them to a Git repository. So, we create a .gitkeep file under the empty directory so that the directories are added to the repository.

```
|_ .gitignore
|_ keep_tracking_empty_dir
     |_ .gitkeep
```

## clone a repo including its submodules
- Way 1:
```
$ git clone https://github.com/cameronmcnz/surface.git
$ git submodule init
$ git submodule update
```

- Way 2:
  - `git clone --recurse-submodules -j8 git://github.com/foo/bar.git`
    - `--recurse-submodules` can be used instead of `--recursive`.
    - `-j8` is an optional performance optimization that became available in version 2.8, and fetches up to 8 submodules at a time in parallel — see `man git-clone`.