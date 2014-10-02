#!/bin/bash
# 1 256 512 768 1024 1280 1536 1792 2048 ; 
for s in 1792 2048  ; 
do 
#  echo run pth 1024x$s... ; 
#  bin/pth/test $((1024*$s)) > ex2/pth_$s.csv ; 
#  cat ex2/pth_$s.csv
#  echo run cuda 1024x$s... ; 
#  bin/cuda/test $((1024*$s)) > ex2/cuda_$s.csv ; 
#  cat ex2/cuda_$s.csv
   echo show cuda 1024x$s... ;
   cat ex2/cuda_$s.csv 
   echo run cuda2 1024x$s... ;
   bin/cuda2/test $((1024*$s)) > ex2/badcuda_$s.csv;
   cat ex2/badcuda_$s.csv
done ;
