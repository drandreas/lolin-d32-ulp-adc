# Lolin D32 Battery Voltage Measurement

This sample demonstrates how the ULP ADC can also be used to measure the
battery voltage.

## Dependencies
ULPCC: https://github.com/jasonful/lcc
ULP-Tool: https://github.com/duff2013/ulptool

## Input Calibration
I assume that the Main CPUs ADC and the ULPs ADC are different parts of the Silicon, since the Voltage measurements on my unit do not match.

| Batt   | uC ADC | ULP ADC |
| ------ | ------ | ------- |
| 4300mV |   2415 |    2480 |
| 3600mV |   1990 |    2058 |
| 3200mV |   1744 |    1801 |
| 2600mV |   1354 |    1399 |

For the single cell Li-Ion Battery the voltage-range of 3.2V to 4.3V is the most relevant, I've therefore decided to optimize my conversion for this range.

I’ve formulated the following two linear equations to calculate the steepness (a) and offset (b) of the line.
```
a * 2480 + b = 4300mV
a * 1801 + b = 3200mV
```

Solving the equation-system results in
```
a = 1100/679   = ~1659/1024
b = 191700/679 = ~282
```

## Implementing Multiplication and Division using shift operations
Multiplications of arbitrary values can be implemented as a combination of left shifts and addtions.
Divisions can only be implemented for powers of two (1, 2, 4, 16, 32, ...).

Multiplying the ADC input by 1100/679 can be approximated by 1659/1024 and implemented on the ULP as:
```
raw = adc(0, 8);

new_voltage  = raw;
new_voltage += raw >> 1;
new_voltage += raw >> 4;
new_voltage += raw >> 5;
new_voltage += raw >> 6;
new_voltage += raw >> 7;
new_voltage += raw >> 9;
new_voltage += raw >> 10;
```

## Validation

I've validated the ADC output for 0.1V increments:

| Input | Output |
| ----- | ------ |
| 4.3V  | 4.301  |
| 4.2V  | 4.199  |
| 4.1V  | 4.100  |
| 4.0V  | 4.005  |
| 3.9V  | 3.905  |
| 3.8V  | 3.810  |
| 3.7V  | 3.710  |
| 3.6V  | 3.618  |
| 3.5V  | 3.504  |
| 3.4V  | 3.406  |
| 3.3V  | 3.305  |
| 3.2V  | 3.194  |
| 3.1V  | 3.087  |
| 3.0V  | 2.984  |
