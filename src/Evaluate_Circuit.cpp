// 2021 Team Curpite
#include <stdio.h>
#include <cmath>
#include <iostream>

#include "Evaluate_Circuit.h"

Eval_Circut_Unit::Eval_Circut_Unit() {
        this->input_flow_conc = 10;
        this->input_flow_tail = 140;
        this->input_flow_gormanium = 10;
        this->input_flow_waste = 140;
}

Eval_Circut_Unit::~Eval_Circut_Unit() {
}

double Eval_Circut_Unit::Evaluate_Circuit(int* circuit_vector, int num_units,
                         double tolerance, int max_iterations) {
    /*
    This function evaluates the fitness of the circuit_vector.
    The System input of Gormanium and waste can be changed just below.
    The economics (reward for Gormanium and penalization of waste in system
    conc stream) can be changed just below;
    
    */

    // this is the user-defined input to the system
    // i.e. 10 kg/s Gormanium and 100 ks/s of waste material
    double system_gormanium_input = 10;
    double system_waste_input = 100;

    // Circuit Performance Metric in pound/kd
    // will be paid for your valuable Gormanium and
    // enalised for waste in the system conc stream
    double paid_gormanium = 100;
    double penalised_waste = 500;

    num_units += 2;  // two extra units for system tail/system output

    // create an array of objects
    Eval_Circut_Unit* Units_array = new Eval_Circut_Unit[num_units];

    int _ = 1;  // dummy varible to index the correct value in circuit_vector

    // This loops converts the circuit_vector input into a array of our class
    for (int t = 0; t < num_units; t++) {
        // for every object set the correct conc and tails num
        Units_array[t].conc_num = circuit_vector[_];
        Units_array[t].tails_num = circuit_vector[_ + 1];
        _ += 2;  // increase index by 2 to point at next unit in circuit_vector

        // do an inital calculation of the output flow for every unit
        Units_array[t].Calc_flow();
    }

    // initialize varible to store how many units have reached the tolerance
    int tol_reached;
    int n = 0;  // varible to store number of iterations completed

    // do the mass balancer for max iterations
    for (int j = 0; j < max_iterations; j++) {
        n++;  // increment counter

        // Loop over all units to sum up the concentrate and tail streams
        // form the other units and save them as unit input
        for (int i = 0; i < num_units - 2; i++) {
            Units_array[Units_array[i].conc_num].input_flow_conc +=
                Units_array[i].conc_flow;
            Units_array[Units_array[i].tails_num].input_flow_tail +=
                Units_array[i].tails_flow;

            Units_array[Units_array[i].conc_num].input_flow_waste +=
                Units_array[i].conc_flow_waste;
            Units_array[Units_array[i].conc_num].input_flow_gormanium +=
                Units_array[i].conc_flow_gormanium;

            Units_array[Units_array[i].tails_num].input_flow_gormanium +=
                Units_array[i].tails_flow_gormanium;
            Units_array[Units_array[i].tails_num].input_flow_waste +=
                Units_array[i].tails_flow_waste;
        }

        // loop over all units
        for (int t = 0; t < num_units; t++) {
            // save the current input flow for every unit in the _old varible
            // for the tolerance calculation
            Units_array[t].conc_flow_old = Units_array[t].conc_flow;
            Units_array[t].tails_flow_old = Units_array[t].tails_flow;
            // calculate new flow output the unit
            Units_array[t].Calc_flow();

            // set the input to 0 so that it can be added to in the loop above
            // we do not need these for the rest of the itteration
            Units_array[t].input_flow_conc = 0;
            Units_array[t].input_flow_tail = 0;
            Units_array[t].input_flow_gormanium = 0;
            Units_array[t].input_flow_waste = 0;
        }

        // set the systen input at the first unit as incoming flow
        Units_array[circuit_vector[0]].input_flow_conc =
            system_gormanium_input;
        Units_array[circuit_vector[0]].input_flow_tail =
            system_waste_input;
        Units_array[circuit_vector[0]].input_flow_gormanium =
            system_gormanium_input;
        Units_array[circuit_vector[0]].input_flow_waste =
            system_waste_input;

        // set tol_reached counter to zero
        tol_reached = 0;

        // loop over all units to check if the unit have reached the tolerance
        // if tolerance for both, the concentrace flow and tails
        // flow is reached, increment the counter
        for (int t = 0; t < num_units - 2; t++) {
            if (std::abs(Units_array[t].conc_flow -
                Units_array[t].conc_flow_old)
                < tolerance &&
                std::abs(Units_array[t].tails_flow -
                Units_array[t].tails_flow_old)
                < tolerance) {
                tol_reached += 1;
            }
        }

        // if the counter is equal to the numer of units (without exit untis)
        // every unit has reached the tolerance and we break the loop
        if (tol_reached == num_units - 2) {
            break;
        }
    }

    // store number of unit that outputs to the system concentrace out
    int unit_conc;
    // stores the system output which should be equal to the system input
    double sys_out_flow = 0;

    for (int k = 0; k < num_units * 2 - 3; k++) {
        // all units to connect to the system conc output need to incude
        // their output in sys_out_flow
        if (circuit_vector[k] == num_units - 2) {
            // k is the index so we need to find the unit which outputs it
            unit_conc = (k / 2);
            // add the contribution of this unit conc output to the sys output
            sys_out_flow += Units_array[unit_conc].conc_flow;
        }
        // all units to connect to the system tails output need to incude
        // their output in sys_out_flow
        if (circuit_vector[k] == num_units - 1) {
            // k is the index so we need to find the unit which outputs it
            unit_conc = (k / 2) - 1;
            // add the contribution of this units tail output to the sys output
            sys_out_flow += Units_array[unit_conc].tails_flow;
        }
    }

    // check if the system output is equal the system input,
    // if the difference is more than 2% it is not converged
    // so return large negtive value
    if (std::fabs(sys_out_flow - (system_gormanium_input + system_waste_input))
    > (system_gormanium_input + system_waste_input)*0.02) {
        return  -system_waste_input * penalised_waste;
    }

    double Performance = 0;

    // if we reached the maximum number of iterations we know that it did not
    // converge so we set it to the max negative value
    if (n == max_iterations) {
        Performance = -system_waste_input * penalised_waste;
    } else {
        // we need to find all units that output to the system conc output,
        // so we loop over the circuit_vector
        for (int k = 0; k < num_units * 2 - 3; k++) {
            // if a value in this circuit_vector is equal to our system output
            // unit we need to count the output
            if (circuit_vector[k] == num_units - 2) {
                // k is the index so we need to find the unit which outputs it
                unit_conc = (k / 2);
                // now that we have the unit, we can add its contribution to
                // performance value using the economic factors
                Performance += Units_array[unit_conc].conc_flow_gormanium *
                    paid_gormanium - Units_array[unit_conc].conc_flow_waste *
                    penalised_waste;
            }
        }
    }

    // delete the array of untis
    delete[] Units_array;

    return Performance;
}
