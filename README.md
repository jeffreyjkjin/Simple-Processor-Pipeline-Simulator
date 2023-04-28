# Simple Processor Pipeline Simulator
This program produces a cycle-accurate simulation of a W-wide superscalar in-order processor with five stages that models how each instruction moves through the pipeline. The five stages include instruction fetch (IF), instruction decode and read operands (ID), instruction issue and execute (EX), memory access (MEM), and writeback results and retire (WB). The simulated processor has a single integer ALU, floating point unit, branch execution unit, read port, and write port.

### Simplifications
The following assumptions are made to simplify the simulator's logic:
- No branch prediction
- Instructions fetches hit in the L1 data cache in one cycle 
- Load and store instructions hit in the L1 data cache in one cycle
- Integer and floating point operations execute in one cycle
- Branch instructions delay incoming IFs until it reaches the EX stage

## Getting Started

### Building
The simulator can be built using the following command:
```
make proj
```

### Trace Files
The simulator simulates programs from a trace file where each line is an instruction with the following format:
```
instrPC, instrType, dependency1, dependency2, ...
```
where 
- `instrPC` is a hexadecimal value representing the instruction's address
- `instrType` is a value from 1 to 5 where
    - `1` is an integer instruction that utilizes the integer ALU
    - `2` is a floating point instruction that utilizes the floating point unit
    - `3` is a branching instruction that transfers control to the next instruction in the trace
    - `4` is a load instruction that reads a value from memory
    - `5` is a store instruction that writes a value to memory
- `dependency1, dependency2, ...` is a comma-separated list of PC values of instructions that this instruction is dependent on

### Running Simulation
The simulator can be run by using the following command:
```
./proj fileName startInstr numberInstr pipelineWidth
```
where 
- `fileName` is the name of the trace file
- `startInstr` is the first instruction that will be read from the file
- `numberInstr` is the number of instructions that will be simulated
- `pipelineWidth` is the number of pipelines in the processor

The output of the program should look something like this:
```
-----Simple Processor Pipeline Simulator-----
Trace File                              srv_0
Starting Instruction                        1
Instruction Count                     1000000
Pipeline Width                              2
------------Simulation Statistics------------
Simulation Runtime (Seconds)           9.9740
Total Execution Time (Cycles)         1221642
Total Instructions Executed           1000000
------------Instruction Histogram------------
Instruction Type               Percentage (%)
(1) Integer                    47.5764       
(2) Float                      0.4487        
(3) Branch                     18.7246       
(4) Load                       22.9315       
(5) Store                      10.3188  
```

## Two-Factor Full Factorial Design with Replications
The following is a two-factor experimental design that I conducted which measures the impact of the pipeline width and workload trace on the processor. Pipeline levels include 1, 2, 3, and 4. The three trace files are simplified versions of traces from the first [Championship Value Prediction](https://www.microarch.org/cvp1/) competition and can be downloaded [here](https://drive.google.com/drive/folders/1sMLWkzluE0JoY5sOm20_RyGc7Nq_7J5H). `compute_fp_1` contains mostly floating point instructions, `compute_int_0` contains mostly integer instructions, and `srv_0` contains a server trace. Each trace has ~30 million instructions. Six replications were run on each of the $4 \times 3$ configurations. The complete results and analysis from this experiment are shown below.

### Two-Factor Table
| Trace File | 1 | 2 | 3 | 4 | Row Sum | Row Mean | Row Effect |
|---|---|---|---|---|---|---|---|
| compute_fp_1 | 1364949 | 1061593 | 1061593 | 1045459 | 4533594 | 1133399 | -17167 |
| compute_int_0 | 1208458 | 1034034 | 987729 | 987507 | 4217728 | 1054432 | -96134 |
| srv_0 | 1470354 | 1214375 | 1188267 | 1182479 | 5055475 | 1263869 | 113303 |
| **Column Sum** | 4043761 | 3310002 | 3237589 | 3215445 | 13806797 |  |  |
| **Column Mean** | 1347920 | 1103334 | 1079196 | 1071815 |  | 1150566 |  |
| **Column Effect** | 197354 | -47232 | -71370 | -78751 |  |  |  |

### Allocation of Variance
Let $A$ be the pipeline width and $B$ be the trace files. Thus $a = 4$, $b = 3$, and $r = 6$. Then,

$$
\begin{align*}
    SSY & = \sum_{i = 1, j = 1, k = 1}^{3, 4, 6} y_{i, j, k}^2 \\
    & = 9.86 \times 10^{13} \\
    \\
    SS0 & = abr\mu^2 \\
    & = 4 \cdot 3 \cdot 6 \cdot (1150566)^2 \\
    & = 9.53 \times 10^{13} \\
    \\
    SST & = SSY - SS0 \\
    & = 9.86 \times 10^{13} - 9.53 \times 10^{13}\\ 
    & = 3.26 \times 10^{12}
\end{align*}
$$

Next, 

$$
\begin{align*}
    SSA & = br\sum_{j = 1}^4 \alpha_j^2 \\
    & = 3 \cdot 6 \cdot \sum_{j = 1}^4 \alpha_j^2 \\
    & = 9.45 \times 10^{11} \\
    \\
    SSB & = ar\sum_{i = 1}^3 \beta_i^2 \\ 
    & = 4 \cdot 6 \cdot \sum_{i = 1}^3 \beta_i^2 \\
    & = 5.37 \times 10^{11} \\
    \\
    SSAB & = r\sum_{i = 1, j = 1}^{3, 4} \gamma_{i, j}^2 \\
    & = 6 \cdot \sum_{i = 1, j = 1}^{3, 4} \gamma_{i, j}^2 \\
    & = 2.84 \times 10^{10} \\
    \\
    SSE & = \sum_{i = 1, j = 1, k = 1}^{3, 4, 6} e_{i, j, k}^2 \\ 
    & = 1.75 \times 10^{12}
\end{align*}
$$

### Conclusion
From above, it can be concluded that 28.99% of the variation is explained by pipeline width, 16.47% from trace files, 0.87% from the interaction between pipeline width and trace files, and 53.68% from experimental error.

### Raw Data

compute_fp_1
| Starting Instruction | 1 | 2 | 3 | 4 | %int | %float | %branch | %load | %store |
|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
| 1 | 1666381 | 1317871 | 1317871 | 1317871 | 25.7445 | 28.8089 | 25.7444 | 19.7022 | 0 |
| 5,000,000 | 1665527 | 1316937 | 1316937 | 1316935 | 25.7059 | 28.8705 | 25.7056 | 19.718 | 0 |
| 10,000,000 | 1660201 | 1311128 | 1311128 | 1311128 | 25.4635 | 29.2584 | 25.4635 | 19.8146 | 0 |
| 15,000,000 | 1066210 | 808242 | 808242 | 775966 | 19.5213 | 25.7144 | 3.2817 | 38.6254 | 12.8572 |
| 20,000,000 | 1065798 | 807808 | 807808 | 775540 | 19.4822 | 25.736 | 3.2683 | 38.6455 | 12.868 |
| 25,000,00 | 1065576 | 807574 | 807574 | 775314 | 19.46 | 25.7477 | 3.2615 | 38.6569 | 12.8739 |
| Mean | 1364949 | 1061593 | 1061593 | 1045459 | 22.5629 | 27.3560 | 14.4542 | 29.1938 | 6.43312 |


compute_int_0
| Starting Instruction | 1 | 2 | 3 | 4 | %int | %float | %branch | %load | %store |
|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
| 1 | 1205685 | 1031801 | 984952 | 984798 | 53.2393 | 0.8778 | 3.3183 | 27.0201 | 15.5445 |
| 5,000,000 | 1222308 | 1045192 | 1001612 | 1001047 | 52.7243 | 2.8805 | 4.4473 | 25.5281 | 14.4198 |
| 10,000,000 | 1205687 | 1031803 | 984954 | 984800 | 53.2393 | 0.8778 | 3.3184 | 27.02 | 15.5445 |
| 15,000,000 | 1205688 | 1031806 | 984957 | 984803 | 53.2397 | 0.8778 | 3.3183 | 27.0196 | 15.5446 |
| 20,000,000 | 1205687 | 1031802 | 984953 | 984799 | 53.2393 | 0.8778 | 3.3184 | 27.0201 | 15.5444 |
| 25,000,00 | 1205690 | 1031801 | 984947 | 984794 | 53.244 | 0.8721 | 3.3174 | 27.0199 | 15.5466 |
| **Mean** | 1208458 | 1034034 | 987729 | 987507 | 53.1543 | 1.2106 | 3.5064 | 26.7713 | 15.3574 |

srv_0
| Starting Instruction | 1 | 2 | 3 | 4 | %int | %float | %branch | %load | %store |
|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
| 1 | 1476019 | 1221642 | 1195711 | 1190063 | 47.5764 | 0.4487 | 18.7246 | 22.9315 | 10.3188 |
| 5,000,000 | 1457170 | 1197133 | 1170819 | 1164766 | 47.9605 | 0.7024 | 18.2051 | 22.8653 | 10.2667 |
| 10,000,000 | 1467839 | 1211250 | 1185078 | 1179153 | 47.7866 | 0.5066 | 18.488 | 22.8909 | 10.3279 |
| 15,000,000 | 1463483 | 1204111 | 1177661 | 1171659 | 47.6417 | 0.6309 | 18.4237 | 22.928 | 10.3757 |
| 20,000,000 | 1479487 | 1227363 | 1201609 | 1196087 | 47.5779 | 0.4046 | 18.7975 | 22.9112 | 10.3088 |
| 25,000,00 | 1478126 | 1224751 | 1198725 | 1193147 | 47.5057 | 0.4098 | 18.7701 | 22.967 | 10.3474 |
| **Mean** | 1470354 | 1214375 | 1188267 | 1182479 | 47.6748 | 0.5172 | 18.5682 | 22.9157 | 10.3176 |

## Acknowledgments
Special thanks to [Dr. Alaa Alameldeen](https://www2.cs.sfu.ca/~alaa/alaa_home.shtml) for teaching me about computer simulation and modelling and being one of the GOAT professors at SFU.