# CrackOpeningDisplacementApproximation

!syntax description /Materials/CrackOpeningDisplacementApproximation

## Overview

Crack opening calculation is accomplished using the gradient of damage variable, $\grad d$, which is used to define the unit vector perpendicular to the crack plane as $\normal^d = \grad d/\norm{\grad d}$. The crack opening field is then calculated as

\begin{equation}
\varw_n = h^e \left( \normal^d \cdot \strain \cdot \normal^d \right),
\end{equation}
where $h^e$ is the edge length of the current element.

## Example Input File Syntax

!syntax parameters /Materials/CrackOpeningDisplacementApproximation

!syntax inputs /Materials/CrackOpeningDisplacementApproximation

!syntax children /Materials/CrackOpeningDisplacementApproximation
