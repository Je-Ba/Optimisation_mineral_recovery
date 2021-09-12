#include <iostream>

//#include "CUnit.h"
#include "CCircuit.h"

int main(int argc, char * argv[]){

    int valid[3] = {0, 1, 2};
    int invalid[3] = {0, 2, 2};
    int TestCase1[9] = {0, 1, 2, 4, 0, 5, 0, 2, 1};
    int TestCase2[7] = {0, 1, 2, 1, 3, 4, 0};
    int TestCase3[9] = {0, 1, 2, 0, 0, 3, 5, 4, 0};
    int TestCase4[15] = {0, 8, 1, 3, 5, 0, 1, 5, 6, 5, 2, 4, 7, 0, 2};
    int TestCase5[15] = {0, 1, 5, 3, 8, 0, 1, 5, 6, 5, 2, 4, 7, 0, 2};
    int TestCase6[9] = {0, 2, 3, 0, 3, 1, 3, 4, 5};

	std::cout << "Check_Validity({0,1,2}):\n";
    if (Check_Validity(valid, 1))
	    std::cout  << "\nTEST PASSED\n- - - - - - - - - -\n";
	else
	    std::cout << "\nTEST FAILED\n- - - - - - - - - -\n";

      
	std::cout << "Check_Validity({0,2,2}):\n";
    if (Check_Validity(invalid, 1))
        std::cout << "\nTEST FAILED\n- - - - - - - - - -\n";
    else
        std::cout << "\nTEST PASSED\n- - - - - - - - - -\n";

    std::cout << "Check_Validity(Test Case 1):\n";
    if (Check_Validity(TestCase1, 4))
        std::cout << "\nTEST FAILED\n- - - - - - - - - -\n";
    else
        std::cout << "\nTEST PASSED\n- - - - - - - - - -\n";

    std::cout << "Check_Validity(Test Case 2):\n";
    if (Check_Validity(TestCase2, 3))
        std::cout << "\nTEST FAILED\n- - - - - - - - - -\n";
    else
        std::cout << "\nTEST PASSED\n- - - - - - - - - -\n";
    
    std::cout << "Check_Validity(Test Case 3):\n";
    if (Check_Validity(TestCase3, 4))
        std::cout << "\nTEST FAILED\n- - - - - - - - - -\n";
    else
        std::cout << "\nTEST PASSED\n- - - - - - - - - -\n";


    std::cout << "Check_Validity(Test Case 4):\n";
    if (Check_Validity(TestCase4, 7))
        std::cout << "\nTEST FAILED\n- - - - - - - - - -\n";
    else
        std::cout << "\nTEST PASSED\n- - - - - - - - - -\n";

    std::cout << "Check_Validity(Test Case 5):\n";
    if (Check_Validity(TestCase5, 7))
        std::cout << "\nTEST FAILED\n- - - - - - - - - -\n";
    else
        std::cout << "\nTEST PASSED\n- - - - - - - - - -\n";


    std::cout << "Check_Validity(Test Case 6):\n";
    if (Check_Validity(TestCase6, 4))
        std::cout << "\nTEST FAILED\n- - - - - - - - - -\n";
    else
        std::cout << "\nTEST PASSED\n- - - - - - - - - -\n";

    return 0;
}
