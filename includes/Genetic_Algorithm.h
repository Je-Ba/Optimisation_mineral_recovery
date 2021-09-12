//
//  Genetic_Algorithm.h
//  Implements methods for genetic algorithm
//
//  Created by Wendy Xu on 3/22/21.
//
#pragma once
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <cmath>
#include <algorithm>
#include <vector>
#include <fstream>
#include <sstream>

#include "Evaluate_Circuit.h"

class GeneticAlgorithm
{
public:
    // Hyper-parameters
    double mutate_rate; // rate of mutation, < 0.01
    double crossover_rate; // rate of crossover, 0.8-1.0
    int n; // regular and initial population sizes
    int num_units; // number of units in a single circuit
    int lookback; // number of past iterations to check for convergence
    
    // Population
    // each should have a length of n x (2*num_units + 1),
    // except for the initial round, where we generate >> n number of random parents
    int *parents, *children; // flattened to 1D
    
    // Maximum and minimum fitness function values of a generation
    double* fitness_data;

    // IDs of best circuits
    int *best_ids;

    // Performance variables
    double max_score;
    int* best_circuit;

    // Other variables
    int iter; // current iteration
    int max_iters, fitness_max_iters; // max number of iterations
    int proc_id; // processor rank for MPI implementations
    
    // Constructor & Destructor
    GeneticAlgorithm(double mutate_rate, double crossover_rate, int n, int num_units, int max_iters, int lookback, int fitness_max_iters, int proc_id);
    ~GeneticAlgorithm();
    
    // Methods
    // main method: run the entire algorithm
    void runIterations();
    void runOneIteration();
    
    // helper functions
    // crossover function
    void crossover(int* parent1, int* parent2);
    
    // mutate function
    void mutate(int* child);
    
    // validity check
    bool checkValid(int *circuit_vector);
    
    // compute performance metric for a single circuit
    //double evalCircuit(int *circuit_vector, double tolerance, int max_iterations);
    //double Evaluate_Circuit(int *circuit_vector, int num_units, double tolerance, int max_iterations);

    // get probability distribution
    void getFitness(double &min_score, double &max_score, double &sum_score, int &best_index);
    int getParentID(double &sum_score, double &min_score, double &max_score, int method = 0);
    // int ChooseParent(double &min_score, double &sum_score); archive for now as we are going back to a previous version

    // check if algorithm has converged
    bool checkConvergent();
    
    // initialize population
    void initializePop();

    // print parents
    void printParents();

    // output the array to a file, including the iteration number and the performance
    void array_to_file(int* best_array, int iteration, double performance, int tag);

private:
    // use to track the best parents
    int best_id_cnt;
};


// double Evaluate_Circuit(int *circuit_vector, double tolerance, int max_iterations);
