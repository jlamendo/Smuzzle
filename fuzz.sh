#!/bin/bash
for i in $(seq 0 40);
do 
 ./smuzzle $((245 * $i))
done
