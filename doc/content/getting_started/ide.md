# Setup VSCode Editor for MOOSE

[VSCode](https://code.visualstudio.com/) is a text editor developed by Microsoft with a flexible plugin structure. [It is recommended to set up VSCode as documented by the MOOSE team.](https://mooseframework.inl.gov/help/development/VSCode.html)

## Optional steps

If you plan to work with the source code of RACCOON, then it is recommended to use the [clangd](https://marketplace.visualstudio.com/items?itemName=llvm-vs-code-extensions.vscode-clangd) extension instead of Microsoft IntelliSense for code autocompletion. Clangd reads the project's compile commands and stores linking information for faster retrieval. If you go this route, make sure to disable IntelliSense for C++ in VSCode (a prompt will pop up instructing you to do so once you install Clangd).

Clangd needs to have access to a `compile_commands.json` file to work. To generate this, simply navigate to your RACCOON directory and run,

``` bash
make compile_commands.json
```

To configure Clangd so that it is able to find the file, it is recommended to set up a VSCode workspace as such,

```JSON
{
    "folders": [
        {
            "path": "."
        },
        {
            "path": "./moose"
        }
    ],
    "settings": {
        "clangd.arguments": [
            "--compile-commands-dir=/<path to RACCOON>/raccoon",
            "--background-index",
            "--query-driver=/<path to miniforge>/miniforge/envs/moose/bin/mpicxx"
        ],
        "[moose]": {
            "outline.showProperties": false,
            "outline.showStrings": false
        }
    }
}
```

Create a file called `raccoon.code-workspace` with the above content and place it into the `.vscode` folder (if this folder does not exist then create it). Remember to replace the `<path to ... >` placeholders with their actual respective paths.

Clangd will now be activated by opening a source file and saving it (this can typically be done by pressing Ctrl+S). If all is in order, then you will see the indexing process start in the status bar of VSCode:

!media media/indexing.png style=display:block;margin:auto;width:50%;

This may take a while depending on the speed of your processor; however, it only needs to be done once.

Once the indexing finishes you should be able to utilize the autocomplete features of Clangd in both RACCOON and MOOSE. I recommend reading the Clangd [documentation](https://marketplace.visualstudio.com/items?itemName=llvm-vs-code-extensions.vscode-clangd) to learn about the full capabilities.
