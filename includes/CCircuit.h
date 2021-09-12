#pragma once
#include <vector>
#include <algorithm>

#include "CUnit.h"

bool Check_Validity(int *circuit_vector, int size);

void mark_units(int unit_num, CUnit* circuit, int original_unit);

