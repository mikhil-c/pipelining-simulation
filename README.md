This project simulates a 5 stage pipelined CPU and was jointly done my [Shivanshu](https://github.com/123shivanshukumar) and [Mikhil](https://github.com/Mikhil-C). 

`main.cpp` runs simulator to simulate pipelined processing on a binary with the following features:

1. 256 Byte Instruction Cache and 256 Byte Data Cache, bare metal operation without any misses. Register file contains 16 - 1 byte registers. Byte addressible memory.
2. Instructions encoded as `opcode + destination_register + source_register1 + source_resgister2` each component being 4 bits. 
16 different insructions encoded as follows are supported:

| Opcode | Encoding | Opcode | Encoding |
|--------|----------|--------|----------|
| ADD    | 0000     | SLLI   | 1000     |
| SUB    | 0001     | SRLI   | 1001     |
| MUL    | 0010     | LI     | 1010     |
| INC    | 0011     | LD     | 1011     |
| AND    | 0100     | ST     | 1100     |
| OR     | 0101     | JMP    | 1101     |
| XOR    | 0110     | BEQZ   | 1110     |
| NOT    | 0111     | HLT    | 1111     |

**Note**: 
-  For `ADD, SUB, MUL, AND, OR, XOR` format is `rd = op(rs2, rs1)`, where op is one of the operations.
- For `INC`, `rd = rd + 1` and the source regsiters are ignored
- For `NOT`, only one source `rs` is required and `rd = !rs` (logical not)
- For `SLLI, SRLI`, `rs2` represents 4 bit immediate and `rd  = op(rs, imm)`, op is left or right shift
- For `LI`, `rs1` and `rs2` combined represent an 8 bit immediate and `rd = imm`
- For `LD, ST` `rd` is loaded from/ stored at (resp.) at an address `rs1 + imm(rs2)` in the Data Cache
- For `JMP` `rd` and `rs1` represent an 8-bit number, which indicates jump from current `PC`. Last four bits are discarded.
- For `BEQZ`, `rd` is compared with `0` and control is transferred to `rs1,rs2` combined as an 8-bit offset
- For `HLT`, remaining 12 bits are discarded, and the program halts.

----
To run the project:
1. Create an `input` and an `output`directory in the same directory as `main.cpp` 
2. In `input` create 3 Files - `ICache.txt`, `DCache.txt` and `RF.txt` contains 256, 256 and 16 1 byte entries (represented as hexadecimal strings). These represent the contents of the Instruction Cache, Data Cache and register file respectively, before running the program. Note that PC starts at the first instruction in `ICache.txt`
3. In `output` create 2 Files - `DCache.txt`and `Output.txt`. `DCache` represents the Data Cache after running the program and `Output.txt` gives the performance metrics of the program. 
4. Compile using `g++ main.cpp` and execute `./a.out` (on linux) or `.\a.exe` (on Windows)
4. Sample files have been provided with this repository.