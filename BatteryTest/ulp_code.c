#ifdef _ULPCC_
#include <ulp_c.h>

// global variable that the main processor can see
unsigned voltage  = 0;

void entry() {
  // define local variables
  unsigned raw          = 0; // used for raw measurements
  unsigned new_voltage  = 0;

  // mesure battery voltage
  raw = adc(0, 8);

  // converting float to int operations (we want to end up with mV):
  //   1100/679 = ~1659/1024
  // converting multiplication and division to rs and ls operations
  //   1659 = 2^10 + 2^9 + 2^6 + 2^5 + 2^4 + 2^3 + 2^1 + 2^0
  // the code below uses the above values devided by 1024 = 2^10
  new_voltage  = raw;
  new_voltage += raw >> 1;
  new_voltage += raw >> 4;
  new_voltage += raw >> 5;
  new_voltage += raw >> 6;
  new_voltage += raw >> 7;
  new_voltage += raw >> 9;
  new_voltage += raw >> 10;

  // publish new voltage including offset of 282mV
  voltage = new_voltage + 282;
}

#endif
