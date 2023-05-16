#!/bin/bash

printf '\n -------------- First Fit -------------- \n'
env LD_PRELOAD=lib/libmalloc-ff.so ps

printf '\n -------------- Worst Fit -------------- \n' 
env LD_PRELOAD=lib/libmalloc-wf.so ps

printf '\n -------------- Best Fit -------------- \n' 
env LD_PRELOAD=lib/libmalloc-bf.so ps