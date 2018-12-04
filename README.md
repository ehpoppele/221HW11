# 221HW11
Eli Poppele CS 221 Homework 11

# README
(please)

## Contents
1. Compilation and File Notes
2. Results

## Compilation and File Notes
Compile using the makefile.
The only file that really matters here is the tsp.cc file; that's where the main part of my work is. For the rest of the files, I used the solution Eitan provided, with some minor changes to accomodate for my HgrexChromosome class (I used my Hgrex algorithm from last week's extra credit here for fun). In the tsp.cc file itself, I added a new threaded_ga_search function, based off the current one, rather than replacing it entirely.

## Results
For my best results, I found that 4,000,000 iterations, a population of 2,000, a mutation rate of 0.5 (using climb chromosomes) and 16 threads worked pretty well, without taking an excessive amount of time. 
The best tour I found was of length 5517.48, currently appearing in shortest.csv. I don't know it's a csv, but the assignment said so.
Below are a few runtime values:
### Table

|nthreads|time(min:sec.ms)|
|--------|----------------|
|16      |1:32.161        |
|8       |1:31.170        |
|4       |1:42.503        |
|2       |2:32.062        |
|1       |5:7.270         |
