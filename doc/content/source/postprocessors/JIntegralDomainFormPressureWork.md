# JIntegralDomainFormPressureWork

!syntax description /UserObjects/JIntegralDomainFormPressureWork

## Overview

The standard forms of J-integral, e.g. [`JIntegralContourForm`](JIntegralContourForm.md) and [`JIntegralDomainForm`](JIntegralDomainForm.md), assume crack surfaces (regularized or not) to be traction-free. When the crack surfaces are not traction-free, the J-integral need to be corrected.

This classes concerns with the special case where pressure is acting on phase-field regularized crack surfaces. The correction term can be written as

\begin{equation}
  J_p = - \int\limits_{\body} p \bft \cdot \bfH \grad d I_{,d} q \diff{V},
\begin{equation}

where $p$ is the pressure, $\bft$ is the direction of crack propagation, $\bfH$ is the displacement gradient tnesor, $q$ is the domain indicator function, and $I$ is the phase-field indicator function.

## Example Input File Syntax

!syntax parameters /UserObjects/JIntegralDomainFormPressureWork

!syntax inputs /UserObjects/JIntegralDomainFormPressureWork

!syntax children /UserObjects/JIntegralDomainFormPressureWork
