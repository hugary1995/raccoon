## Code Development

It is *completely* fine if you want to make changes to the source code of RACCOON, but +DO NOT+ make changes to `hugary1995/raccoon` as you don't have write access. To propose changes properly, you need to fork RACCOON under your own GitHub account, make modifications in your fork, and submit a pull request `hugary1995/raccoon`.

#### Fork RACCOON

1. Log into your GitHub account, navigate to the [hugary1995/raccoon](https://github.com/hugary1995/raccoon) repository.
2. In the top-right corner of the page, click +Fork+

   !media media/fork_button.jpg
          style=width:300px;padding:auto;

3. After the forking process is done, you should be able to find your fork under your GitHub account, i.e. `https://github.com/YourGitHubUsername/raccoon`.

#### Create a local clone of your fork

Now that you have a fork of `hugary1995/raccoon` under `YourGithubUsername/raccoon`, you can setup the workflow for your fork. First, open a terminal, navigate to the RACCOON repository, and verify your current remotes:

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
git remote add YourRemoteName git@github.com:YourGitHubUsername/raccoon.git
git remote -v
```

and the output should look like the following:

```bash
origin	https://github.com/hugary1995/raccoon.git (fetch)
origin	https://github.com/hugary1995/raccoon.git (push)
YourRemoteName	git@github.com:YourGitHubUsername/raccoon.git (fetch)
YourRemoteName	git@github.com:YourGitHubUsername/raccoon.git (push)
```

Next, you may checkout the `devel` branch of RACCOON to your own development branch and let it track your own fork of RACCOON, i.e.

```bash
git checkout origin/devel
git switch -c YourBranchName
git push -u YourRemoteName YourBranchName
```

Now, you may begin your code development. Just remember to add and commit often, i.e.

```bash
git add -A .
git commit -m 'Some commit message'
git push
```

Often times, during your development, the `devel` branch of `hugary1995/raccoon` may have new commits since your last checkout. To integrate new commits into your development branch, do:

```bash
git rebase -i origin devel
```

If you would like to make changes to the documentation, follow [these instructions](getting_started/docs.md)
