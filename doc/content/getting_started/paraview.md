# Install ParaView for visualization

The standard output file of RACCOON is Exodus, a file format developed by the Sandia National Laboratory. [ParaView](https://www.paraview.org/) is a powerful visualization toolkit and complies with the Exodus file format.

Suppose there is an output file called `elasticity_out.e` in the current directory, you can visualize it using Paraview using the following command:

```bash
paraview elasticity_out.e
```

Below is a screenshot of what it looks like:

!media media/paraview.png 
       alt="paraview logo" style=display:block;margin-left:auto;margin-right:auto;width:100%;
