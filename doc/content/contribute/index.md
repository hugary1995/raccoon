# Contribute

Want to contribute? Thanks, *ANY* contribution is more than welcome!

## Bug Report / Feature Request

Contribution isn't only about writing code. If you identify a bug, or feel an important feature
is missing, tell us and it is a great way to contribute. Go to [RACCOON issue page](https://github.com/hugary1995/raccoon/issues) for a list of currently open and closed issues.

Click on the +New issue+ button to submit a ticket. Note that two templates are available, one for Bug Report and the other one for Feature Request.

## Code Development

It is *completely* fine if you want to make changes to the source code of RACCOON, but +DO NOT+ make changes to your cloned version of RACCOON as you won't be able to merge those changes into the upstream RACCOON. To propose changes properly, you need to fork RACCOON under your own GitHub account, make modifications in your fork, and submit a pull request in the upstream RACCOON.

#### Fork RACCOON

1. Log into your GitHub account, navigate to the [hugary1995/raccoon](https://github.com/hugary1995/raccoon) repository.
2. In the top-right corner of the page, click +Fork+

   !media media/fork_button.jpg
          style=width:300px;padding:auto;

3. After the forking process is done, you should be able to find your form under your GitHub account, i.e. `https://github.com/YourGitHubAccount/raccoon`.

#### Create a local clone of your fork

Right now, you have a fork of the RACCOON repository, but you don't have the files in that repository on your computer. Let's create a clone of your fork locally on your computer. First, open a terminal, navigate to the RACCOON repository, and verify your current remotes:

```bash
cd ~/projects/raccoon
git remote -v
```

Your remote should look like the following:

```bash
origin	https://github.com/hugary1995/raccoon.git (fetch)
origin	https://github.com/hugary1995/raccoon.git (push)
```

Next, let's add your fork of RACCOON to the remote:

```bash
git remote add myfork git@github.com:YourGitHubAccount/raccoon.git
git remote -v
```

and the output should look like the following:

```bash
origin	https://github.com/hugary1995/raccoon.git (fetch)
origin	https://github.com/hugary1995/raccoon.git (push)
myfork	git@github.com:YourGitHubAccount/raccoon.git (fetch)
myfork	git@github.com:YourGitHubAccount/raccoon.git (push)
```

Next, you may checkout the origin/master branch of RACCOON to your own development branch and let it track your own fork of RACCOON, i.e.

```bash
git checkout -b YourBranchName
git push -u myfork YourBranchName
```

Now, you may begin your code development. Just remember to add and commit often:

```bash
git add -A .
git commit -m 'some commit message'
git push
```

Often times, during your development, the origin/master branch of RACCOON may have new commits since your last checkout. To integrate new commits into your development branch, do:

```bash
git rebase -i origin master
```

## Pull Request

After you add changes to your fork of RACCOON, you can open a pull request to ask us to review your changes before merging them into the project. Follow [these instructions](https://help.github.com/en/github/collaborating-with-issues-and-pull-requests/creating-a-pull-request-from-a-fork) to create a pull request. Once you opened a pull request, you will notice that a continuous-integration build automatically starts and the merge button is blocked:

!media media/ci_in_progress.png
       style=width:500px;padding:auto;

Once all the regression tests in the continuous-integration build passed, and someone (not yourself) reviewed and approved your pull request, your proposed pull request will be merged into the upstream RACCOON.
