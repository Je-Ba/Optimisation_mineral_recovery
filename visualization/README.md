These tools provide basic visualization methods for the the vector output of the genetic algorithm code. They both use the graph
visualization package Graphviz [https://graphviz.gitlab.io](https://graphviz.gitlab.io).

Python version
==============

The example python script requires a working python installation of Graphviz. For conda users a working environment can be 

```
conda env create -f environment.yml
conda activate min_recov
pip install -r requirements.txt
```

or you can (conda) install the packages yourself
```
conda install graphviz python-graphviz
```

C++ version - Instructions kept but this method has NOT been implemented!
===========

A C++ stub is also provided, linking against the Graphviz C library. A Dockerfile provides an example ubuntu setup, a similar strategy should work on the DUG system.

On a Windows build, it will be necessary to download and build graphviz, and then modify the included `plot.vcxproj` appropriately.

Brief graphviz build instructions are

 1. Download the source code with  ` git clone --recurse-submodules https://gitlab.com/graphviz/graphviz.git`
 2. Add `%HomePath%\graphviz\windows\dependencies\graphviz-build-utilities\` to your path
 3. Start Visual Studio, open `graphviz\graphviz.sln` and build the package.

Post_Processing - Serial Code
=======

In order to use the post_processing.py file, you MUST create two folders within the visualization folder.

1. Create a folder called `/visualization/out`
    1. This will hold the `solution_[int].txt` files containing the iteration number, performance value, and the solution array created by the array_to_file(...) function in Genetic_Algorithm.cpp. This file will only contain the iteration number, best solution vector from the current generation and its associated performance. This should be out put with the following format:
    ``` 
        0 (iteration number)
        16.0182 (performance value)
        0 8 1 9 2 8 3 9 4 5 11 9 4 10 9 0 8 2 1 6 7 (solution vector)
    ```
    
    2. This folder will also hold an `iteration_list.txt` file which will contain a list of the iterations that were output as solution files, in the following format:
    ```
        0   50  100 150 ...
    ```

    3. If your solution vectors are NOT saved in this file in the specified format then no diagram will be produced.
2. Create a folder called `/visualization/diagrams`
    1. This will hold the outputs from the post_processing.py file, named using the format: `Solutions_[int].png`, where [int] is the
       integer representing the iteration from with the solution array was picked.
    2. To view the solution diagram simply open the file.

## Running post_processing.py

In order to produce viewable diagrams in the visualization folder, with the conda environment ACTIVATED run:

    python3 post_processing.py
After which you will encounter a string of command line prompts, if no command line prompts appear that means there was only one `solution_[int].txt` file available and it the diagram is already available for viewing in `/visualization/diagrams`.

