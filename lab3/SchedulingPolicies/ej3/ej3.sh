#!/bin/sh

chrt -p -f 12 $$
renice -n -10 -p $$

./ej2 $$ &
./ej2 $(pidof ej2) 
