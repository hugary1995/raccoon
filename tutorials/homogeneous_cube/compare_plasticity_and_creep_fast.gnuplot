set datafile separator ','
set xrange [1:]
set key right bottom
set xlabel "F_{yy}"

plot 'Hencky_J2_arrheniuslawhardening/stress_deformation.csv' using 2:5 with lines lc rgb "black" lw 2 title "reference n = infinity", \
     'Hencky_J2_arrheniuslawhardening_creep/stress_deformation_n_5_rate_10000.csv' using 2:7 with lines lw 2 title "n = 5 rate = 10000", \
     'Hencky_J2_arrheniuslawhardening_creep/stress_deformation_n_10_rate_10000.csv' using 2:7 with lines lw 2 title "n = 10 rate = 10000", \
     'Hencky_J2_arrheniuslawhardening_creep/stress_deformation_n_20_rate_10000.csv' using 2:7 with lines lw 2 title "n = 20 rate = 10000", \
     'Hencky_J2_arrheniuslawhardening_creep/stress_deformation_n_40_rate_10000.csv' using 2:7 with lines lw 2 title "n = 40 rate = 10000", \
     'Hencky_J2_arrheniuslawhardening_creep/stress_deformation_n_80_rate_10000.csv' using 2:7 with lines lw 2 title "n = 80 rate = 10000", \
     'Hencky_J2_arrheniuslawhardening_creep/stress_deformation_n_160_rate_10000.csv' using 2:7 with lines lw 2 title "n = 160 rate = 10000"

pause -1
