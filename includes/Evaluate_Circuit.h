#pragma once
#include <stdio.h>
#include <cmath>
#include <iostream>
#include <vector>
#include <algorithm>

#include "CUnit.h"
#include "CCircuit.h"

class Eval_Circut_Unit : public CUnit
{
public:

    //this class inherits the conc_num and tails_num from parent

    //varibles to store input flow to every cell
    double input_flow_conc; //input from conc streams
    double input_flow_waste; //waste input total per cell
    double input_flow_gormanium; //gormanium input total per cell
    double input_flow_tail; //input from tail streams

    //varibles to store conc output flows
    double conc_flow; //current itteration
    double conc_flow_old; //last itteration
    double conc_flow_gormanium; //gormanium in conc flow output
    double conc_flow_waste;  //wast in conc flow output

    //varibles to store tails output flows
    double tails_flow;  //current itteration
    double tails_flow_old;  //last itteration
    double tails_flow_gormanium;
    double tails_flow_waste;

    //Constructors, initialize varibles with a guess
    Eval_Circut_Unit();

    ~Eval_Circut_Unit();

    //Find the output for every cell for the given inputs
    void Calc_flow() {
        //find the gormanium and waste values for both tails and conc output flows
        this->conc_flow_gormanium = this->input_flow_gormanium * 0.2;
        this->conc_flow_waste = this->input_flow_waste * 0.05;
        this->tails_flow_gormanium = this->input_flow_gormanium* 0.8;
        this->tails_flow_waste = this->input_flow_waste * 0.95;

        //find combined output conc and tails flow
        this->conc_flow = this->input_flow_conc * 0.2 + this->input_flow_tail * 0.05;
        this->tails_flow = this->input_flow_conc * 0.8 + this->input_flow_tail * 0.95;

    }

    double Evaluate_Circuit(int* circuit_vector, int num_units, double tolerance, int max_iterations);
};


    