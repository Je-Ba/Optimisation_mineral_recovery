#include <iostream>
#include <mpi.h>
#include "Genetic_Algorithm.h"
#include "CCircuit.h"
#include <time.h>

int    id; // Processor rank
int    p;  // Processor size

int main(int argc, char * argv[])
{
    double mutate_rate = 0.02;
    double crossover_rate = 0.80;
    int    n = 500;
    int    num_units = 10;
    int    max_iter = 400;
    int    lookback = 100;
    int    fitness_max_iter = 100;
    int    tag_num = 1;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    // Initialise a population of circuits in each cpu
    srand(time(NULL)+id*10);
    GeneticAlgorithm mycircuits = GeneticAlgorithm(mutate_rate, crossover_rate, n, num_units, max_iter, lookback, fitness_max_iter, id);
    mycircuits.initializePop();

    // Iterate through generations in each cpu
    mycircuits.runIterations();

    /* 
       By this point each cpu should have a population of circuits, iterated
       through generations and printed their best circuits (gen 0 and gen max_iter)
       into textfiles in the visualization/out directory.
    */

   // Reading files of the final generation
   if (id == 0)
   {
       int    ilist[2*p]; // list of circuit iterations
       double plist[2*p]; // list of performances of circuits
       int    circuitvecs[p * (2 * num_units + 1)]; // list of said best circuit vectors (array)
       int    recvdata; // the unique number of iteration each processor gone through

       for (int i = 0; i < p; i++)
       {
           if (i == 0)
           {
               recvdata = mycircuits.iter;
           }
           else
           {
               MPI_Recv(&recvdata, 1, MPI_INT, i, tag_num, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
           }

           std::stringstream name;
           name << "../visualization/out/solution_" << recvdata << "_" << i << ".txt";
           std::ifstream textfile(name.str().c_str());
           std::cout << "opening solution_" << recvdata << "_" << i << "\n";
           if (!textfile.is_open())
           {
               std::cout << "Error failed to open a file!" << '\n'; 
               break;
           }
           else
           {
                for (int j = 0; j < (2*num_units + 2); j++)
                {
                    if (j == 0)
                    {
                        textfile >> ilist[i];
                        textfile >> plist[i];
                    }
                    else
                    {
                        textfile >> circuitvecs[i * (2 * num_units + 1) + j - 1];
                    }
           }
           }
       }
        // Finding the best out of the best circuits received
       int index = 0;
       double value = 0;
       int bestcircuit[2*num_units + 1];
       //std::cout << std::endl;
       for (int i = 0; i < p; i++)
       {
           if (plist[i] > value)
           {
               value = plist[i];
               index = i;
           }
       }

       for (int i = 0; i < (2*num_units + 1); i++)
       {
           bestcircuit[i] = circuitvecs[index * (2 * num_units + 1) + i];
       }

       // Writing this information to file (with identifiable negative name tag)
       mycircuits.array_to_file(bestcircuit, ilist[index], plist[index], -1);

   }
   else
   {
       int senddata = mycircuits.iter;
       MPI_Send(&senddata, 1, MPI_INT, 0, tag_num, MPI_COMM_WORLD);
   }
    MPI_Finalize();
}