set datafile separator ','
set xrange [0:1]
set key left top
set xlabel "x"
set ylabel "w"

plot 'data/e_0.012_w_0001.csv' using ($3):($2*0.08) with lines lw 2 title "e = 0.012", \
     'data/e_0.006_w_0001.csv' using ($3):($2*0.08) with lines lw 2 title "e = 0.006", \
     'data/e_0.003_w_0001.csv' using ($3):($2*0.08) with lines lw 2 title "e = 0.003", \
     'data/e_0.0015_w_0001.csv' using ($3):($2*0.08) with lines lw 2 title "e = 0.0015"

pause -1
