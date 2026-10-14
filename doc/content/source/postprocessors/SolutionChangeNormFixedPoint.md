# SolutionChangeNormFixedPoint

Computes the maximum nodal change in a variable between consecutive fixed-point (Picard) iterations:

\begin{equation}
  \max_i \left| d_i^{(k)} - d_i^{(k-1)} \right|
\end{equation}

An L-infinity measure is used rather than an integral L2 norm so that the value is independent of domain size and of how far the crack has advanced. For a phase field bounded in $[0,1]$, a tolerance of $10^{-3}$ means no node's damage moved by more than 0.001 over the iteration.

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
