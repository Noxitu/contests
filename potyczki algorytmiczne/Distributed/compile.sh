g++ "solutions/$1.cpp" -c -g --std=c++11 -Wall -pedantic -Dmain=thread_main -Itasks/ -Ilib/ -o /tmp/main.o
g++ lib/{main,message,node,runner}.cpp /tmp/main.o "tasks/$2.cpp" -g --std=c++11 -Wall -pedantic -o /tmp/a
/tmp/a $3 $4 $5 $6