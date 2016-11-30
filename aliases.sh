alias codejam="python `pwd`/codejamlib/codejam.py"
alias c++="g++ --std=c++11 -o /tmp/a -Wall -pedantic -O3"

function app_test { time /tmp/a < $1.in > /tmp/out && diff /tmp/out $1.out; }