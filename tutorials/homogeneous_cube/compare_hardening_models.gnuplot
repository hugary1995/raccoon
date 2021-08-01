set multiplot layout 1,2
set datafile separator ','
set xrange [1:]
set yrange [0:1.8]
set key left top
set xlabel "F_{yy}"

plot 'Hencky_J2_powerlawhardening/stress_deformation.csv' using 2:($5/320) with lines lc rgb "black" lw 2 title "sigma / sigma_y", \
     'Hencky_J2_powerlawhardening/stress_deformation.csv' using 2:($4/5/0.01) with lines lc rgb "green" lw 2 title "ep / (n ep_0)", \
     'Hencky_J2_powerlawhardening/stress_deformation.csv' using 2:3 with lines lc rgb "red" lw 2 title "d"

plot 'Hencky_J2_linearhardening/stress_deformation.csv' using 2:($5/320) with lines lc rgb "black" lw 2 title "sigma / sigma_y", \
     'Hencky_J2_linearhardening/stress_deformation.csv' using 2:($4*6.88e4/12/320) with lines lc rgb "green" lw 2 title "ep E / 12 sigma_y", \
     'Hencky_J2_linearhardening/stress_deformation.csv' using 2:3 with lines lc rgb "red" lw 2 title "d"

pause -1
