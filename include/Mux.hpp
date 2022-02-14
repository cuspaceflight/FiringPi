#pragma once

#include <vector>
#include <iostream>

class Mux
{
public:
  std::vector<int> pins;
  Mux(std::vector<int> select_pins);
  void select(int addr);
  void clear();
 };
	
