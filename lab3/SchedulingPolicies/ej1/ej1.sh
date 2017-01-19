#!/bin/sh

# Change the current shell's priority to 12 and sched policy to FIFO
chrt -v -f -p 12 $$

# Change the current shell's nice value to -10
renice -n -10 -p $$
