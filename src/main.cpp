#include <stdio.h>
#include <iostream>
#include "Genetic_Algorithm.h"
#include "CCircuit.h"

int main(int argc, char * argv[])
{
    std::cout << "hello world! " << std::endl;

    // Set up a random seed
    srand(0);

    double mutate_rate = 0.01; //mutate rate
    double crossover_rate = 0.95; //crossover rate
    int    population_size = 500;  // n
    int    circuit_size = 5;   // num_units
    int    max_iters = 200;  // max iter
    int    converge_lookback = 50;   // how many 
    int    fitness_max_iters = 60; // fitness max iter

    // Setting the a test environment
    GeneticAlgorithm test(mutate_rate, crossover_rate, population_size, circuit_size, max_iters, converge_lookback, fitness_max_iters, 0);
    // Initialize parents
    test.initializePop();
    // Run iterations
    test.runIterations();

    return 0;
}

