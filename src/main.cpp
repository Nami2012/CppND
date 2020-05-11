#include "ncurses_display.h"
#include "system.h"
#include<iostream>
int main() {
  System system;
  std::cout<<system.OperatingSystem();
  //NCursesDisplay::Display(system);
}