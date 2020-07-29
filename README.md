# uPower ISA Assembler and Simulator

This repository consists of 2 applications:
1. An assembler for converting assembly code written in uPOWER(a subset of the IBM-POWER ISA) into object code. 

   Command to run the assembler:

        $ python assembler.py

    The program takes in the input and the output file names to read the assembly code and save the binary output respectively.
   
2. A simulator to run the object code and display the desired output.

    Command to run the simulator:

        $ g++ simulator.cpp
        $ ./a.out < binary_file

Sample assembly programs for reference are also present in the repo.

Refer [this](https://drive.google.com/file/d/1OOS8Qv906oqScWoYSKW1coyaCayzalFJ/view?usp=sharing) link for more details on uPower ISA.
