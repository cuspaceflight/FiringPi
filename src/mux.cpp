#include "Mux.hpp"

Mux::Mux(std::vector<int> select_pins)
{
  pins = select_pins;
}

void Mux::select(int addr)
{
  int a = addr;
  for (int i=0; i<(int)pins.size(); i++)
  {
    std::cout<<(a&1);
    a = a>>1;
  }
}
