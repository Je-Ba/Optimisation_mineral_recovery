# Gormanian Rush
Contributors: Teddy Cheung, Jens Bauer, Wenjie Xu, Raha Moosavi, Benson Chang, Chigozirim Ihebenachi, Calvin Davies

---
## Contents
1. Includes/Src
    1. CUnit.h/CUnit.cpp
    2. CCircuit.h/CCircuit.cpßp
    3. Evaluate_Circuit.h/Evaluate_Circuit.cpp
    4. Genetic_Algorithm.h/Genetic_Algorithm.cpp
2. MPI
3. License

---

# Includes/Src

## 1. CUnit.h

 - CUnit.h is a class that creates a unit and assigns to it all the values mentioned in the constructor. Specifically allows the creation of a tree in which each unit has its associated feed, concentrate stream, and tailing stream.
 - Additionally it contains several assistant functions to help with this set up

 ## 2. CCirciut.h/.cpp

 - This actually builds the tree and contains in it the validity checking which ensure that a circuit_vector fed to it is a valid circuit to prevent cases of no-convergence.
    - This ist of validity checks is NOT exhaustive and as new cases are encountered they will need to be added.
    - It does this by running through the 'tree' after it's been created and marking whether or not the unit has been seen, before performing such validity checks.

## 3. Evaluate_Circuit.h/.cpp

- The Evaluate_Circuit function evaluates the performance of the circuit_vectors and returns the fitness value for the circuit. The monetary reward for Gormanium and penalisation for waste in the system concentrate stream can be changed in this function. Additionally, the system input of Gormanium and waste can be changed in this function as well. 
- Circuits that do not converge are given the maximum negative fitness value defined by the economic factors.

## 4. Genetic_Algorithm.h/.cpp
- Houses all the needed functions to perform the maximization in terms of Gormanium out put via genetic algorithms. 
- optimizations

    ##  Setting up a main

    A main should be set up as follows:
    First set all the variables required to build this includes initiating a random seed in order to generate random cross over points and mutations.

        // Set up a random seed
        srand(0);

        double mutate_rate = 0.01; //mutate rate
        double crossover_rate = 0.95; //crossover rate
        int    population_size = 500;  // n
        int    circuit_size = 5;   // num_units
        int    max_iters = 200;  // max iter
        int    converge_lookback = 50;   // how many 
        int    fitness_max_iters = 60; // fitness max iter
        int    proc_id = 0; // rank of processor (useful for MPI; default/serial = 0)

    Afterwards it is a simple matter of initiating the class, creating the initial population and running the iterations as follows:

        // Setting the a test environment
        GeneticAlgorithm test(mutate_rate, crossover_rate, population_size, circuit_size, max_iters, converge_lookback, fitness_max_iters, proc_id);
        // Initialize parents
        test.initializePop();
        // Run iterations
        test.runIterations();


    ### Building the System

    Before running the system please create the following folder:
    1. `/visualization/out` - key for storing the output from main.

    In order to build and execute the main.cpp and achieve an optimised circuit the system should be built via a CMakeLists.txt. This will create a makefile and it will also create a directory called 'executables' which will house all of the executable files including those of the tests. These should be built and compiled in the following manner:
    ```
    cmake -S . -B .
    make
    ```
    This will create all the executables (except mainP) which can be run by simply calling:
    ```
    cd executables
    ./'executable_name_here'
    ```
    To run mainP, replace the execution line with:
    ```
    mpiexec -n number_of_cpus mainP
    ```
    The mainP initialises and populate the class in each individual computer, iterate through and outputs files to `/visualization/out`, a master cpu will then process all the output files and choose the best result and copies that file with a identifiable tag.
    Ensure you are in the 'executables' directory before running your main, if you are not your processing files will not appear in the correct folder.

    ### Post Processing
    Please look at and follow the instructions of the README located in `/visualization`. Instructions on the creation of the required virtual environment and an example output file is provided there.

# 2. MPI

We offer a main.cpp which implements the Genetic Algorithm using MPI and is ready for running on a HPS system... unfortunately we did not get round to incorporating the compilation in the CMakeLists and workflows. Therefore the responsiblity of the compilation and use of this file falls to the user.

---

# 3. License
[MIT](https://choosealicense.com/licenses/mit/)
