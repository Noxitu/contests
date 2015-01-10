#!/bin/bash

"$1" < "$2" 2> /tmp/gnuplot.data

head -n 1 "$2" | read W H F

gnuplot <<EOF
set term png
set output "filary.png"
set size ratio -1
set xrange [0:$W]
set yrange [0:$H]

set xtics 1
set ytics 1
set grid
unset border
set format x ""
set format y ""

plot "/tmp/gnuplot.data" using (\$1-0.5):(\$2-0.5) with lines lw 3 linecolor rgb "black" notitle, \
	"< tail -n +2 \"$2\"" using 1:2:(1) with circles linecolor rgb "black" fill solid notitle

EOF
