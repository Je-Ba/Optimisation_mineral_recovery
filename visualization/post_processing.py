# This script reads in a .txt file containing solution arrays
import numpy as np
import graphviz


# We rebuild the CUnit class, to create objects allowing us to convert the
# solution array out put as a .txt
# file into a "tree"
class CUnit:
    def __init__(self, value):
        self.conc = None
        self.tail = None
        self.unit_num = value
        self.feed_num = None
        self.num_units = None


# function to read in a file structured in the manner specified in the README
def read_in_file(file_name):
    infile = open(file_name)
    row1 = infile.readlines(1)
    iteration = int(row1[0])
    row2 = infile.readlines(2)
    performance = float(row2[0])
    infile.close()
    sol_Array = np.loadtxt(file_name, skiprows=2)
    return iteration, performance, sol_Array


# function that takes in the solution_array and the number of units used,
# it then converts the array into a list
# of objects linked via their concentration and tail streams
def array2CUnit_array(solution_Array):
    size = int((len(solution_Array) - 1) / 2)
    array_CUnit = []

    for i in range(0, size):
        array_CUnit.append(CUnit(i))
        array_CUnit[i].conc = solution_Array[(array_CUnit[i].unit_num*2)+1]
        array_CUnit[i].tail = solution_Array[array_CUnit[i].unit_num*2+2]
        array_CUnit[i].feed_num = solution_Array[0]
        array_CUnit[i].num_units = size

    return array_CUnit


# Implementing the graphviz library in order to build a digraph of nodes
# representing units
def draw_solution(solution, sol_Array, iteration, performance):
    size = int(len(solution))
    # start with a graphviz object
    graph = graphviz.Digraph()

    graph.attr(rankdir='LR')
    graph.attr('node', shape='rectangle')
    graph.attr(label='Iteration: ' + str(iteration) + '  |  Performace: '
               + str(performance) + '  |  Solution Array: ' + str(sol_Array))

    # Create the graph. New nodes are created automatically
    # when first named.
    # here we create the visualisation for the stream
    graph.edge('Feed', str(solution[0].feed_num), color='blue',
               headport='w', tailport='e',
               arrowhead='normal', arrowtail='normal')

    # now we loop through the solution list and exploit the structure of the
    # class allowing us to simply create a linked diagram representative of
    # the circuit
    for i in range(0, size):
        graph.edge(str(solution[i].unit_num), str(solution[i].conc),
                   color='blue', headport='nw', tailport='n',
                   arrowhead='normal')
        graph.edge(str(solution[i].unit_num), str(solution[i].tail),
                   color='red',
                   headport='sw', tailport='se',
                   arrowhead='normal')

    # Write to disk
    graph.node('Feed', fillcolor='#CCFFE5', style='filled')
    graph.node(str(size+1), pos='nw!', label='concentrate',
               fillcolor='#FF9999', style='filled')
    graph.node(str(size), label='tailings', fontcolor='white',
               fillcolor='#99CCFF', style='filled')
    graph.render('./diagrams/Solution_' + str(iteration), cleanup=True,
                 format='png')
    return None


# This the driver function which will search inside the out folder for
# relevant files and provide useful error messages
def do_post_processing():
    # Here we load in the iteration_list.txt produced by Genetic_Algorithm.cpp
    array_of_iterations = np.loadtxt("./out/iteration_list.txt")
    try:
        number_of_files = len(array_of_iterations)
    except TypeError:
        # Catch for if only one number exists in iteration_list.txt
        number_of_files = 1

    # Now we provide the user the option to process ALL the files in ./out or
    # only the FINAL solution

    if(number_of_files > 1):
        process_char = 5
        print("Was MPI used? [y/n]: ")
        mpi_used = str(input())
        if(mpi_used == 'n'):
            while(process_char != 1 or process_char != 2):
                print("Would you like to process all solution files in the"
                      + "folder ./out or only the final solution?\n")
                print("Enter [1] All solutions | [2] FINAL solution: ")

                try:
                    process_char = int(input())
                except ValueError:
                    print("Value Error! Please enter an Integer of 1 or 2.")

                if(process_char == 1 or process_char == 2):
                    break
                else:
                    print("Invalid Input, try again.\n")
        if(mpi_used == 'y'):
            process_char = 2
        if(process_char == 2):
            # Only working on the final file
            if(mpi_used == 'n'):
                iteration, performance, sol_Array\
                    = read_in_file("./out/solution_{:d}_0.txt".
                                   format(int(array_of_iterations[-1])))
                solution = array2CUnit_array(sol_Array.astype(int))
                draw_solution(solution, sol_Array, iteration, performance)

                print("Done")
            elif(mpi_used == 'y'):
                iteration, performance, sol_Array = \
                    read_in_file("./out/solution_{:d}_-1.txt".
                                 format(int(array_of_iterations[-1])))
                solution = array2CUnit_array(sol_Array.astype(int))
                draw_solution(solution, sol_Array, iteration, performance)

        elif(process_char == 1):
            # Processing all the files in the ./out folder
            if(mpi_used == 'n'):
                for i in range(number_of_files):

                    print("working | file: solution_" +
                          str(int(array_of_iterations[i])) + "_0.txt")

                    iteration, performance, \
                        sol_Array = read_in_file("./out/solution_{:d}_0.txt".
                                                 format(int(
                                                    array_of_iterations[i])))

                    solution = array2CUnit_array(sol_Array.astype(int))
                    draw_solution(solution, sol_Array,
                                  int(array_of_iterations[i]), performance)

                print("Done.")

    elif(number_of_files == 1):
        # Catch for if only one file is made
        print("One processable file exists would you \
            like to process it? [y/n]:")

        process_char = str(input())
        if(process_char == "y"):

            iteration, performance, sol_Array = read_in_file("./out/solution_\
                {:d}_0.txt".format(int(array_of_iterations)))

            solution = array2CUnit_array(sol_Array.astype(int))
            draw_solution(solution, sol_Array, iteration, performance)
            print("Done.")
        else:
            print("Done.")
            return None

    return None


do_post_processing()
