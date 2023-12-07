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

## How can I change the author name / email of a commit?

- Changing Your Committer Name & Email Globally
You can run the "git config" command with the --global flag; this will make sure all of your future commits use the given information:
```
$ git config --global user.name "John Doe"
$ git config --global user.email "john@doe.org"
```

- Changing Your Committer Name & Email per Repository
If you want to use special settings only when working in a certain repository, you can simply omit the `--global` flag. This makes the configuration valid only in that repository:
```
$ git config user.name "John Doe"
$ git config user.email "john@doe.org"
```

- Changing the Author Information Just for the Next Commit
Finally, with the `--author` flag, you can also overwrite the author information for just the next commit:
```
git commit --author="John Doe <john@doe.org>"
```

- Using `--amend` for the Very Last Commit
In case you want to change just the very last commit, Git offers a very easy way to do this:
```
git commit --amend --author="John Doe <john@doe.org>"
```

This effectively replaces the last commit with your "edited" version, correcting the wrong author information.

- Using Interactive Rebase
Interactive Rebase is the Swiss Army Knife of tools in Git: it allows you to do and change almost anything. However, being as powerful as it is, this also means you can very easily shoot yourself in the foot. Use it with care (and possibly read up on it)!

The first step is to identify the last "good" commit and provide its hash to the rebase command:
```
$ git rebase -i -p 0ad14fa5
```

Your editor will open, requesting you to mark all the commits you want to change with the "edit" keyword.

Git will now walk you through each commit, giving you the chance to mold it as you desire:
```
Stopped at 5772b4bf2... Add images to about page
You can amend the commit now, with

    git commit --amend
```

Once you are satisfied with your changes, run
```
    git rebase --continue
```
Your job, now, is to correct the author information and then continue to the next concerned commit object until you've edited all the commits you just marked:
```
$ git commit --amend --author="John Doe <john@doe.org>" --no-edit
$ git rebase --continue
```


- Using git filter-branch
Another way is to use Git's "filter-branch" command. It allows you to batch-process a (potentially large) number of commits with a script.
You can run the below sample script in your repository (filling in real values for the old and new email and name):
```
$ git filter-branch --env-filter '
WRONG_EMAIL="wrong@example.com"
NEW_NAME="New Name Value"
NEW_EMAIL="correct@example.com"

if [ "$GIT_COMMITTER_EMAIL" = "$WRONG_EMAIL" ]
then
    export GIT_COMMITTER_NAME="$NEW_NAME"
    export GIT_COMMITTER_EMAIL="$NEW_EMAIL"
fi
if [ "$GIT_AUTHOR_EMAIL" = "$WRONG_EMAIL" ]
then
    export GIT_AUTHOR_NAME="$NEW_NAME"
    export GIT_AUTHOR_EMAIL="$NEW_EMAIL"
fi
' --tag-name-filter cat -- --branches --tags
```

The same warning applies to this method as to the others mentioned: you are rewriting history with this command, creating new commit objects along the way!

Preferably, you should only do this in repositories that haven't been published / shared, yet. In any other case you should use it with extreme care - and only if you're aware of the side effects!

## Git Tagging

- git fetch --tags: get the all tags from `origin` remote.

- git fetch upstream --tags: get the all tags from `upstream` remote.

- git fetch tag_name: get the specific `tag_name`

- git fetch -all

- `git ls-remote --tags upstream`: list the tags on remote repository. Supposing the remote reference name is `upstream`.
- `git tag`: list the tags on local repository

```
$ git pull upstream 1.6.5608

$ git checkout -t -b upstream/1.6.5608
Branch 'upstream/1.6.5608' set up to track local branch 'master'.
Switched to a new branch 'upstream/1.6.5608'

# or
$ git co -t upstream/acc-test-20231113
Branch 'acc-test-20231113' set up to track remote branch 'acc-test-20231113' from 'upstream'.
Switched to a new branch 'acc-test-20231113'
```
- A function add the tag
```
add_tag_for_repo() {
    repo=${1}
    tag_name=${2}

    echo "add tag for ${repo}"
    git tag ${tag_name}
    upstream=`git remote |grep upstream`
    if [ "${upstream}" != "upstream" ]; then
        echo "add plusai upstream"
        git remote add upstream your_repo/${repo}
    fi
    git push upstream ${tag_name}
}
```
这个 tag name总是会给当前commit 打上tag. 如下示意图

```
user2:              push new commit
                            |
                            v
repo main/time line -----t0-t1-t2----------> time
                         ^\
                         | \           
user1:            c lone |  \          
                             \
                              tag
```
即 tag命令没有指定commit id时，默认使用当前的local 的latest commit id. 
所以，即便 user1 是它t2时刻执行 push tag 的命令在 user2提交新commit之后，这个tag仍然指向t0时刻的commit id. 即这个tag里不会包含user2的commit.

## Git lfs

- Enable lfs feature:

Add `.gitattributes` next to large files:
```
   dir
    |_ .gitattributes
    |_ bigfile1
    |_ bigfile2
```

- Content of `.gitattributes`:
```
bigfile1 filter=lfs diff=lfs merge=lfs -text
bigfile2 filter=lfs diff=lfs merge=lfs -text
```


