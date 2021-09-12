//
//  Genetic_Algorithm.cpp
//  Implements methods for genetic algorithm
//
//  Created by Wendy Xu on 3/22/21.
//

#include "Genetic_Algorithm.h"
#include "Evaluate_Circuit.h"

// int answer_vector[5] = {0, 1, 2, 3, 4}  ;

GeneticAlgorithm::GeneticAlgorithm(double mutate_rate, double crossover_rate, int n, int num_units, int max_iters, int lookback, int fitness_max_iters, int proc_id) : mutate_rate(mutate_rate), crossover_rate(crossover_rate), n(n), num_units(num_units), max_iters(max_iters), lookback(lookback), fitness_max_iters(fitness_max_iters), proc_id(proc_id)
{
    this->parents = new int[n*(2*num_units + 1)];
    this->children = new int[n*(2*num_units + 1)];
    this->fitness_data = new double[n];
    this->best_ids = new int[lookback*(2*num_units + 1)];
    this->iter = 0;
    this->best_id_cnt = 0;
    this->best_circuit = new int[2*num_units + 1];
}

GeneticAlgorithm::~GeneticAlgorithm()
{
    delete [] this->parents;
    delete [] this->children;
    delete [] this->fitness_data;
    delete [] this->best_ids;
    delete [] this->best_circuit;
}

void GeneticAlgorithm::runIterations()
{
    while (this->iter <= this->max_iters && !checkConvergent())
    {
        // Run a single iteration to get n new circuits
        runOneIteration();
        this->iter++;
        // Swap parents and children
        std::swap(this->children, this->parents);
    }
}

void GeneticAlgorithm::runOneIteration()
{
    // don't remove invalid parents here! do it in the initialization part
    // where random parents are generated and output only valid ones
    
    // Preparation steps
    // Compute a fitness score for each valid parent and create a probability distribution
    
    // Compute fitness scores
    int best_index;
    double min_score, sum_score;
    getFitness(min_score, this->max_score, sum_score, best_index);
    // Append the best circuit to best_ids
    if (this->best_id_cnt >= this->lookback)
    {
        // shift to left
        for (int j = 0; j < this->lookback*(2*this->num_units + 1); j++)
        {
            if (j < (this->lookback - 1)*(2*this->num_units + 1))
                this->best_ids[j] = this->best_ids[j + (2*this->num_units + 1)];
            else
                this->best_ids[j] = this->parents[best_index*(2*this->num_units + 1) + j - (this->lookback - 1)*(2*this->num_units + 1)];
        }
    } else
    {
        for (int j = 0; j < 2*this->num_units + 1; j++)
        {
            this->best_ids[this->best_id_cnt*(2*this->num_units + 1) + j] = this->parents[best_index*(2*this->num_units + 1) + j];
        }
        this->best_id_cnt++;
    }
    
    // Add the parent with highest score to next generation
    for (int i = 0; i < 2*this->num_units + 1; i++)
        this->children[i] = this->parents[best_index*(2*this->num_units + 1) + i];

    if(this->iter == 0 || this->iter == this->max_iters || this->iter%50 == 0)
    {
        int* best_array = new int[2*this->num_units + 1];

        for (int i = 0; i < 2*this->num_units + 1; i++)
            best_array[i] = this->children[i];
            
        array_to_file(best_array, this->iter, this->max_score, this->proc_id);
        delete [] best_array;
    }
    int cnt = 1; // stop when cnt = n, i.e. we have enough children for the next generation
    
    // Then go over the steps below
    int *parent1 = new int[2*this->num_units + 1];
    int *parent2 = new int[2*this->num_units + 1];
    while (cnt < this->n)
    {
        // 1. select 2 parents, each an array of length (2*num_units + 1)
        int id1 = getParentID(sum_score, min_score, this->max_score);
        int id2 = getParentID(sum_score, min_score, this->max_score);
        while (id2 == id1)
            id2 = getParentID(sum_score, min_score, this->max_score);
        //std::cout << "  selecting parents " << id1 << " and " << id2 << std::endl;
        for (int i = 0; i < 2*this->num_units + 1; i++)
        {
            parent1[i] = this->parents[id1*(2*this->num_units + 1) + i];
            parent2[i] = this->parents[id2*(2*this->num_units + 1) + i];
        }
        
        // 2. decide whether to crossover
        // generate a random number
        bool p = (rand() % 100) < this->crossover_rate*100;
        if (p)
            crossover(parent1, parent2);
        
        // 3. decide whether to mutate
        // generate a random number
        bool p1 = (rand() % 100) < this->mutate_rate*100;
        if (p1)
            mutate(parent1);
        bool p2 = (rand() % 100) < this->mutate_rate*100;
        if (p2)
            mutate(parent2);
        
        // 4. check if both children are valid
        // if valid, add them to children array and increment cnt
        if (Check_Validity(parent1, this->num_units) && Check_Validity(parent2, this->num_units))
        {
            for (int i = 0; i < 2*this->num_units + 1; i++)
                this->children[cnt*(2*this->num_units + 1) + i] = parent1[i];
            cnt++;
            if (cnt < this->n)
            {
                for (int i = 0; i < 2*this->num_units + 1; i++)
                {
                    this->children[cnt*(2*this->num_units + 1) + i] = parent2[i];
                }
                cnt++;
            }       
        }// otherwise go back to the beginning and repeat steps
    }
    delete [] parent1;
    delete [] parent2;
    
}

// crossover function, takes two parent vectors, generates a random integer between 0 and num_units,
// swaps all the values of each parent vector up to this integer to create two children vectors.
void GeneticAlgorithm::crossover(int* parent1, int* parent2)
{
    // find our random integer to index into 
    int random_n = rand() % (2 * this->num_units + 1);

    for(int i=0; i<random_n; i++)
    {
      std::swap(parent1[i], parent2[i]);
      // in this std::swap() swap all the values of the parents up to the random integer generated
      // above
    }
}

// mutate function, takes in a child vector and randomly selects an  integer between 0 and num_units, 
// at this position in the child vector it will generate another random integer that will replace the
// value currently in that position.th
void GeneticAlgorithm::mutate(int* child)
{
    int random_n = rand()%(2 * num_units + 1);

    // generate random unit integer that will take its place, this is done in the range of units
    int random_unit = rand()%(num_units + 1);
    
    // swap the value at the index location with the random unit number
    std::swap(child[random_n], random_unit);
}

// get a random parent id
// the probability is normalised such that the maximum is favoured
// the normalisation method works for negative fitnesses and fitness values that are very similar
int GeneticAlgorithm::getParentID(double &sum_score, double &min_score, double &max_score, int method)
{
    if (method == 0) // default method
    {
        // Generate a random number
        double random_number = double(rand())/double(RAND_MAX) * (sum_score - this->n * min_score); 
        // std::cout << "random number is " << random_number << std::endl;
        double s = 0;
        int i = this->n / 2;
        bool selected = false;
        
        while (!selected)
        {
            i = i % this->n;
            s += (this->fitness_data[i] - min_score + 0.001);
            if (random_number < s)
                return i;
            i++;
        }
        return -1;
    } else
    {
        std::cerr << "\n Undefined selection method! " << std::endl;
        return -1;
    }
}

// Get fitness data of parents in the generation it is called
void GeneticAlgorithm::getFitness(double &min_score, double &max_score, double &sum_score, int &best_index)
{
    max_score = 0;
    sum_score = 0;
    
    for (int i = 0; i < this->n; i++)
    {
        int* circuit_vector = new int[2*this->num_units + 1]; 
        for (int j = 0; j < 2*this->num_units + 1; j++)
        {
            circuit_vector[j] = this->parents[i*(2*this->num_units + 1) + j]; 
        }
        Eval_Circut_Unit evalcircut;
        double score;
        score = evalcircut.Evaluate_Circuit(circuit_vector, this->num_units, 0.0000001, this->max_iters); // random values for tolerance and max iterations for now. fix later!!!
        delete [] circuit_vector;
        this->fitness_data[i] = score; // store fitness score in array
        sum_score += score;
        if (i != 0)
        {
            min_score = std::min(score, min_score);
            if (score > max_score)
            {
                max_score = score;
                best_index = i;
            }
        } else
        {
            min_score = score;
            max_score = score;
            best_index = i;
        }
    }
}

// check if algorithm has converged
bool GeneticAlgorithm::checkConvergent()
{
    // If the best circuits in previous rounds are all equal
    if (this->best_id_cnt >= this->lookback)
    {
        // loop through columns
        for (int j = 0; j < 2*this->num_units + 1; j++)
        {
            // use first row as reference
            int tmp = this->best_ids[j];
            // loop through other rows
            for (int i = 1; i < this->lookback; i++)
            {
                if (this->best_ids[i*(2*this->num_units + 1) + j] != tmp)
                    return false;
            }
        }
        array_to_file(this->best_ids, this->iter, this->fitness_data[0], this->proc_id);
        std::cout << "!!!converged!!!" << std::endl;
        for (int i = 0; i < 2*this->num_units + 1; i++)
        {
            this->best_circuit[i] = this->best_ids[i];
            std::cout << this->best_ids[i] << " ";
        }
        return true;
    } else
        return false;
}

// initialize population
void GeneticAlgorithm::initializePop()
{
    int cnt = 0;
    while (cnt < this->n)
    {
        // create a random array of length (2*num_units + 1)
        int candidate[2*this->num_units + 1];
        candidate[0] = 0;
        for (int i = 0; i < this->num_units; i++)
        {
            do {
                candidate[2*i + 1] = rand() % (this->num_units + 2);
            } while (candidate[2*i + 1] == i || (i == 0 && candidate[2*i + 1] == this->num_units + 1) || (i == 0 && candidate[2*i + 1] == this->num_units));
            do {
                candidate[2*i + 2] = rand() % (this->num_units + 2);
            } while (candidate[2*i + 2] == i || candidate[2*i + 1] == candidate[2*i + 2] || (i == 0 && candidate[2*i + 2] == this->num_units + 1) || (i == 0 && candidate[2*i + 2] == this->num_units));
        }
        // check if valid
        if (Check_Validity(candidate, this->num_units))
        {
            // if yes, add it to parents and increment cnt
            for (int i = 0; i < 2*this->num_units + 1; i++)
            {
                this->parents[cnt*(2*this->num_units + 1) + i] = candidate[i];
            }
            cnt++;
        }
    }
}

void GeneticAlgorithm::printParents()
{
    for (int i = 0; i < this->n; i++)
    {
        for (int j = 0; j < 2*this->num_units + 1; j++)
        {
            std::cout << this->parents[i*(2*this->num_units + 1) + j] << " ";
        }
        std::cout << std::endl;
    }
}

// outputs a file in the /visualization/output folder for post_processing
void GeneticAlgorithm::array_to_file(int* best_array, int iteration, double performance, int tag)
{
    std::stringstream fname;
    std::fstream f1;

    // creating our list of out put iterations if it does not already exist
    // if it does exist we are appending to it.
    
    std::ofstream iter_file;
    iter_file.open("../visualization/out/iteration_list.txt", std::ios_base::app);
    iter_file << iteration << " ";
    iter_file.close();
    

    // creating the solution .txt file in the required format for post processing
    fname << "../visualization/out/solution_" << iteration << "_" << tag <<".txt";
    f1.open(fname.str().c_str(), std::ios_base::out);
    // our first line contains the iteration number
    f1 << iteration << std::endl;
    // second line has the performance value
    f1 << performance << std::endl;

    // now we add our solution array
    for(int i=0; i<(2 * this->num_units + 1); i++)
    {
        f1 << best_array[i] << " ";
    }
    f1.close();
}
