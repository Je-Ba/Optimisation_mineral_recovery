#pragma once

class CUnit {
 public:
  
  // Constructor
  // Default
  CUnit() {}
  CUnit(int* circuit_vector, int size, int i)
  {
    set_num_units(size);
    // The unit number of each unit (This is not strictly needed as an attribute but makes the variable naming neater below).
    set_unit_num(i);
    // feed_num is the first int in the circuit_vector array, and is common to all of the units.
    set_feed_num(circuit_vector[0]);
    // The conc_num of each unit, conc_num = (unit_num * 2) + 1;
    set_conc_num(circuit_vector[(unit_num * 2) + 1]);
    // The tails_num of each unit, tails_num = (unit_num * 2) + 2;
    set_tails_num(circuit_vector[(unit_num * 2)+ 2]);
  }

  // Destructor
  ~CUnit()
  {

  }

  // Attributes:

  // index of the unit to which this unit’s concentrate stream is connected 
  int conc_num;
  //index of the unit to which this unit’s tailing stream is connected 
  int tails_num;
  // A Boolean that is changed to true if the unit has been seen 
  bool mark;
  // index of the unit to which the feed is connected
  int feed_num;
  // index of this unit
  int unit_num;
  // number of units in the circuit to which the unit belongs
  int num_units;
  // A Boolean that is changed to true if there is a final concentration unit on the traversal path of a circuit unit
  bool is_set_conc = false;
  // A Boolean that is changed to true if there is a final tailings unit on the traversal path of a circuit unit
  bool is_set_tail = false;

  // Methods:

  // Set the conc_num
  void set_conc_num(int conc_num) {
  this->conc_num = conc_num;
  }
  // Set the tails_num
  void set_tails_num(int tails_num) {
  this->tails_num = tails_num;
  }
  // Set the feed_num
  void set_feed_num(int feed_num) {
  this->feed_num = feed_num;
  }
  // Set the unit_num
  void set_unit_num(int unit_num) {
  this->unit_num = unit_num;
 
  }
  // Set the num_units
  void set_num_units(int num_units) {
  this->num_units = num_units;
  }

};

