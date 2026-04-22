# SolutionChangeNormFixedPoint

Computes the L2 norm of the change in a variable between consecutive fixed-point (Picard) iterations:

\begin{equation}
  \| d^{(k)} - d^{(k-1)} \|_2 = \sqrt{\int_\Omega \left( d^{(k)} - d^{(k-1)} \right)^2 \, \mathrm{d}V}
\end{equation}

Intended for use as a staggered-scheme convergence criterion in conjunction with [PostprocessorConvergence.md] and the `multiapp_fixed_point_convergence` Executioner parameter.

## Usage

The postprocessor defaults to `execute_on = MULTIAPP_FIXED_POINT_CONVERGENCE`, which causes it to evaluate after each sub-app solve and transfer but before the convergence decision is made.

!alert note
When using this postprocessor with `PostprocessorConvergence`, the parameters `max_iterations` and `converge_at_max_iterations` belong to the `[Convergence]` block, not the `[Executioner]` block.

## Example Input File Syntax

```
[AuxVariables]
  [d]
  []
[]

[Postprocessors]
  [d_fp_change]
    type = SolutionChangeNormFixedPoint
    variable = d
  []
[]

[Convergence]
  [fp_conv]
    type = PostprocessorConvergence
    postprocessor = d_fp_change
    tolerance = 1e-4
    max_iterations = 20
    converge_at_max_iterations = false
  []
[]

[Executioner]
  type = Transient
  multiapp_fixed_point_convergence = fp_conv
[]
```

!syntax inputs /UserObjects/SolutionChangeNormFixedPoint

!syntax children /UserObjects/SolutionChangeNormFixedPoint
