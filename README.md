## Introduction
This is a small script that simulates and animates a N-body problem.


### Dependencies
Apart from standard libraries, the following libraries are used:
- raylib: Animation of moving bodies
- cJSON: Parse a JSON-file which includes the complete system setup

It is assumed, that `cJSON.h` is situated in `/usr/local/include/cjson.`
`raylib.h` must be in `/usr/include`


### Integrator
The time stepping is done by employing Runge-Kutta method of order 4.