reset
set ylabel 'time(ns)'
set title 'best S'
set key left top
set term png enhanced font 'Verdana,10'
set output 'time_diff.png'
set grid
plot [0:999][:] \
'diff_ns.txt' using 1:2 with linespoints linewidth 2 title "S size ",\
