set datafile separator ','
set logscale x

plot 'a_0.0001.csv' using 1:4 with lines lw 2 title 'psie', \
     'a_0.0001.csv' using 1:5 with lines lw 2 title 'psin'

pause -1
