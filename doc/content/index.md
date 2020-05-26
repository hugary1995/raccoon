!config navigation breadcrumbs=False scrollspy=False

# HOME style=visibility:hidden;

!media media/raccoon_logo.png style=display:block;margin-left:auto;margin-right:auto;width:60%;

# A parallel finite-element code specialized in phase-field for fracture class=center style=font-weight:200;font-size:200%

!row! style=margin-top:5em;
!col! small=12 medium=12 large=12 icon=group
### Developed By class=center style=font-weight:200;

### [Dolbow Research Group](https://dolbow.pratt.duke.edu) class=center style=font-weight:3200;

#### Primary Contributors class=center style=margin-top:3em;font-size:150%;text-decoration:underline;

#### Tianchen (Gary) Hu, Duke University class=center style=font-size:100%;

#### Wen Jiang, Idaho National Laboratory class=center style=font-size:100%;

#### Yingjie Liu, Duke University class=center style=font-size:100%;

#### Rudy Geelen, Duke University class=center style=font-size:100%;

#### Casper Versteeg, Duke University class=center style=font-size:100%;

#### Christian Peco, Pennsylvania State University class=center style=font-size:100%;

#### Carter Rhea, University of Montreal class=center style=font-size:100%;
!col-end!
!row-end!

!gallery! large=6

!card! media/mode1.gif title=[Mode I crack propagation [!icon!link]](benchmarks/mode1.md)
The standard benchmark problem of a notched plate in tension. The notch is either "meshed-in" or represented using an initial damage field. A Mode-I type of crack emanates from the notch.
!card-end!

!card! media/mode2.gif title=[Mode II crack propagation [!icon!link]](benchmarks/mode2.md)
The standard benchmark problem of a notched plate under shear load. The notch is either "meshed-in" or represented using an initial damage field. A Mode-II type of crack emanates from the notch.
!card-end!

!card! media/soil.gif title=[Soil desiccation [!icon!link]](benchmarks/mud.md)
Row 1: Three dimensional soil desiccation simulations in a film-substrate system.

Row 2: Two dimensional crack morphologies obtained with different constructions of stochastic fracture properties.
!card-end!

!card! media/kalthoff.gif title=[The Kalthoff-Winkler experiment [!icon!link]](benchmarks/kalthoff.md)
The Kalthoff-Winkler experiment simulated using the phase-field cohesive fracture model. The elastodynamics problem is solved using an explicit central-difference operator. The phase-field is shown on the left, and the field of maximum principal stress is shown on the right.
!card-end!
!gallery-end!
