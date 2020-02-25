# LegoSim
 A UNIX-based Simulator for LegOS/BrickOS with an Applet-GUI

[Official Website](https://arcb.csc.ncsu.edu/~mueller/legosim/)

Usage Info
----------
Thomas on <host2>:
    ../../tcp2stdout 4712 | simple-rover | ../../stdin2tcp <host1> 4711 5

 + Output:
    ../../tcp2stdout 4712 | simple-grover | ../../doublestdout.pl | ../../stdin2tcp <host1> 4711 5


Oliver on <host1>:
    tcp2stdout 4711 | appletviewer legosim.html | stdin2tcp <host2> 4712 5

 + Output:
    tcp2stdout 4711  | appletviewer legosim.html | doublestdout.pl| stdin2tcp <host2> 4712 5

