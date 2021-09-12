// Copyright 2021 Teddy Cheung and Chigozirim Ihebenachi

#include "CCircuit.h"
#include "CUnit.h"
#include <stdio.h>
#include <vector>
#include <iostream>



bool Check_Validity(int* circuit_vector, int size) {
  bool validity = true;

  // Converting from an array of ints to an array of CUnit's.

  // Number of units in the circuit, size = [(length of circuit vector) - 1] / 2
  /* Dynamically allocate memory for the CUnit representation of the circuit (array of CUnits)
  so that the memory can be deleted once the validity each sequence has been assessed. */
  CUnit* circuit_CUnit = new CUnit[size];

  /* Each element of the above array is of class CUnit.
  Now we have to set the attributes of each unit using the CUnit class methods: */
  for (int i = 0; i < size; i++) {
    circuit_CUnit[i] = CUnit(circuit_vector, size, i);
  }

  // Test case 1:
  /* To ensure that every unit is accessible by the feed,
  check that all the unit references exist in the genetic sequence at least once. */
  // Set all the cells to unseen
  for (int i=0; i< circuit_CUnit->num_units; i++) {
    circuit_CUnit[i].mark = false;
  }
  // This marks all the values accessible from the feed num
  mark_units(circuit_CUnit->feed_num, circuit_CUnit, circuit_CUnit->feed_num);
  /* Loop through the list of units to check if it traversed through all units,
  if not set the validity to false */
  for (int i = 0; i < circuit_CUnit->num_units; i++) {
    if (!circuit_CUnit[i].mark) {
      // std::cout << "\nCircuit is invalid. ";
      // std::cout << "[Case 1: Not all units are accessible ";
      // std::cout << "from the feed.]" << endl;
      return false;
    }
  }

  // Test case 2:
  /* To ensure that no units are not self-recyling,
  check that each unit's conc_num and tails_num are not equal to their unit_num. */
  for (int i = 0; i < circuit_CUnit->num_units; i++) {
    if (circuit_CUnit[i].conc_num == circuit_CUnit[i].unit_num ||
    circuit_CUnit[i].tails_num == circuit_CUnit[i].unit_num) {
      // std::cout << "\nCircuit is invalid. [Case 2: Self-recycling "";
      // std::cout << (Unit " << circuit_CUnit[i].unit_num << ")]" << endl;
      return false;
    }
  }

  // Test case 3:
  /* To ensure that the destination for both products from a unit are not the same unit,
  check that conc_num and tails_num are not equal. */
  for (int i = 0; i < circuit_CUnit->num_units; i++) {
    if (circuit_CUnit[i].conc_num == circuit_CUnit[i].tails_num) {
      // std::cout << "\nCircuit is invalid. [Case 3: Both output streams ";
      // std::cout << "to same destination (Unit ";
      // std::cout << circuit_CUnit[i].unit_num << ")]" << endl;
      return false;
    }
  }

  // Test case 4:
  /* This test case checks that the input feed does not feed directly into any of the final product streams unless size =1
  We have set the final concentration destination unit as size and the final tailings destination unit as size+1. */

  if (circuit_CUnit->num_units != 1) {
    if (circuit_CUnit[circuit_CUnit->feed_num].conc_num  >= size ||
    circuit_CUnit[circuit_CUnit->feed_num].tails_num >= size) {
      // std::cout << "\nCircuit is invalid. ";
      // std::cout << "[Case 4: Input feeds directly into ";
      // std::cout << "final product stream." << endl;
      return false;
    }
  }


  // Test case 5
  /* More than one unit cannot point to a final tailings unit or final concentrate unit.
  Only one unit must contain either of the destination units */
  int tailings_count = 0;
  int conc_count = 0;
  for (int i = 0; i < size; i++) {
    if (circuit_CUnit[i].conc_num >= size) conc_count++;
    if (circuit_CUnit[i].tails_num >= size) tailings_count++;
  }

  if (tailings_count > 1 || conc_count > 1) {
    // std::cout << "\nCircuit is invalid. [Case 5: More than one unit points ";
    // std::cout << "to final tailings unit or final ";
    // std::cout << "concentrate unit.]" << endl;
    return false;
  }

  // Test case 6:
  /* No single unit can have both a final tailing unit
  and a final concentrate unit as its destination unit */
  for (int i = 0; i < size; i++) {
    if (size != 1) {
      if (circuit_CUnit[i].conc_num >= size &&
      circuit_CUnit[i].tails_num >= size) {
        // std::cout << "\nCircuit is invalid. ";
        // std::cout << "[Case 6: Unit with both the final ";
        // std::cout << "tailings unit and final concentrate unit as ";
        // std::cout << "destination units (Unit ";
        // std::cout << circuit_CUnit[i].unit_num << ")]" << endl;
        return false;
      }
    }
  }


  // Test case 7:
  /* Check that the first item in the circuit_vector array, 
  which is the feed is less than n - 1. */
  if (circuit_vector[0] > size - 1) {
    // std::cout << "\nCircuit is invalid. [Case 7: ";
    // std::cout << "Feed stream does not lead to any existing unit.]" << endl;
    return false;
  }

  // Test case 8:
  /* The final streams (concentration and tails)
  must be included in the circuit */
  bool seen1 = false;
  bool seen2 = false;
  for (int i = 0 ; i < (2 * size) + 1 ; i++) {
    if (circuit_vector[i] == size && i % 2 == 1)
        seen1 = true;
    else if (circuit_vector[i] == size + 1  && i % 2 == 0)
        seen2 = true;
  }
  if (!(seen1 && seen2)) {
      // std::cout << "\nCircuit is invalid. [Case 8: Final ";
      // std::cout << "concentration streams are not in circuit.]" << endl;
      return false;
  }

  // Test case 9:
  /* Ensure that every unit has a route forward
  from the outlet streams (avoid accumulation). */
  // Set all the cells to unseen
  for (int j = 0; j < size; j++) {
      for (int i=0; i< circuit_CUnit->num_units; i++)
        circuit_CUnit[i].mark = false;
      // This marks all the values accessible from the feed num
      mark_units(circuit_CUnit[j].unit_num,
      circuit_CUnit, circuit_CUnit[j].unit_num);
      /* Loop through the list of units to check if it traversed through all units,
      if not set the validity to false */
      for (int i = 0; i < circuit_CUnit->num_units; i++) {
        if (!circuit_CUnit[i].mark) {
          // std::cout << "\nCircuit is invalid. ";
          // std::cout << "[Case 9: not every unit has a ";
          // std::cout << "route forward to the outlet streams ";
          // std::cout << "(accumulation).]" << endl;
          return false;
        }
      }
  }

  delete[] circuit_CUnit;

  return validity;
}


/* Traverse through the path of each unit,
checking for the final concentration and tailings unit */
void mark_units(int unit_num, CUnit* circuit, int original_unit) {
  if (circuit[unit_num].mark) return;

  circuit[unit_num].mark = true;

  // If we have seen this unit already exit
  // Mark that we have now seen the unit
  /* If conc_num does not point at a circuit outlet
  recursively call the function */

  if (circuit[unit_num].conc_num < circuit->num_units) {
    mark_units(circuit[unit_num].conc_num, circuit, original_unit);

  } else {
    circuit[original_unit].is_set_conc = true;
  }

  /* If tails_num does not point at a circuit
  outlet recursively call the function */
  if (circuit[unit_num].tails_num < circuit->num_units) {
    mark_units(circuit[unit_num].tails_num, circuit, original_unit);
  } else {
    circuit[original_unit].is_set_tail = true;
  }
}
