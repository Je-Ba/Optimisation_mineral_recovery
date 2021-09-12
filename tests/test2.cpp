#include <cmath>
#include <iostream>

#include "CUnit.h"
#include "CCircuit.h"
#include "Genetic_Algorithm.h"

int main(int argc, char * argv[])
{   /*
    Test Evaluate_Circuit function for 4 example input circuit_vectors with known solutions
    The tests require an input of 100 waste and 10 gormanium to the system otherwise they fail
    */

    Eval_Circut_Unit evalcircut_test; //initialized class
	
    int circuit_vector1[11] = { 0, 4, 3, 2, 0, 5, 4, 4, 6, 2, 1 };


    std::cout << "Evaluate_Circuit(circuit_vector1, 5, 1.e-8, 200) close to 24.82:\n";
    if (std::fabs(evalcircut_test.Evaluate_Circuit(circuit_vector1, 5, 1.e-8, 200) - 24.82) < 1.0e-2)
        std::cout << "pass\n";
    else
        std::cout << "fail\n";


    int circuit_vector2[31] = { 9, 7, 12, 0, 12, 3, 9, 10, 1 ,8, 2, 14, 6, 13,5 ,10 ,4 ,3 ,11 ,12 ,8 ,15 ,7 ,4 ,5 ,5 ,2 ,14 ,16 ,7 ,8 };


    std::cout << "Evaluate_Circuit(circuit_vector2, 15, 1.e-4, 400) close to 45.89:\n";
    if (std::fabs(evalcircut_test.Evaluate_Circuit(circuit_vector2, 15, 1.e-4, 800) - 45.89) < 1.0e-2)
        std::cout << "pass\n";
    else
        std::cout << "fail";


    int circuit_vector3[31] = { 12,7,12,7,0,7,14,10,1,10,3,14,6,13,5,10,4,1,11,1,8,15,7,1,5,7,2,0,16,1,9 };

    std::cout << "Evaluate_Circuit(circuit_vector3, 15, 1.e-4, 600) close to 408.55:\n";
    if (std::fabs(evalcircut_test.Evaluate_Circuit(circuit_vector3, 15, 1.e-4, 800) - 408.55) < 1.0e-2)
        std::cout << "pass\n";
    else
        std::cout << "fail";


    int circuit_vector4[44] = { 18,14,2,14,16,14,9,8,19,20,15,8,18,18,12,14,0,4,13,14,10,3,11,19,17,18,21,4,14,8,3,20,8,14,7,5,6,14,1,8,5 };


    std::cout << "Evaluate_Circuit(circuit_vector4, 15, 1.e-8, 600) close to 630.77:\n";
    if (std::fabs(evalcircut_test.Evaluate_Circuit(circuit_vector4, 20, 1.e-12, 600) - 630.7) < 1.0e-1)
        std::cout << "pass\n";
    else
        std::cout << "fail";
	
}
