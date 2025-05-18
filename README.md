This project simulates a 5-stage pipelined CPU with hazard support and was jointly done by [Shivanshu](https://github.com/123shivanshukumar) and [Mikhil](https://github.com/Mikhil-C). 

# Overview
`simulate.cpp` runs a simulator to simulate pipelined processing on a binary with the following features:
1. 256-byte instruction cache and 256-byte data cache, direct-mapped caches, bare-metal operation without any misses. 
2. Register file contains 16 8-bit registers. 
3. Byte-addressable memory.
4. Instructions are encoded as `opcode + destination_register + source_register1 + source_register2`, each component being 4 bits. 

## Supported Instructions
This architecture supports 16 instructions, divided into the following categories:

### Arithmetic Instructions:
```
add rd rs1 rs2
sub rd rs1 rs2
mul rd rs1 rs2
inc r
```

### Logical Instructions:
```
and rd rs1 rs2
or rd rs1 rs2
xor rd rs1 rs2
not rd rs
```

### Shift Instructions:
```
slli rd rs1 imm(4)
srli rd rs1 imm(4)
```

### Load Immediate Instruction:
```
li rd imm(8)
```

### Memory Instructions:
```
ld rd rs1 imm(4)
st rd rs1 imm(4)
```

### Control Instructions:
```
jmp imm(8)
beqz rs imm(8)
```

### Halt Instruction:
```
hlt
```

## Instruction Encoding

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

### Instruction Formats
- For `ADD`, `SUB`, `MUL`, `AND`, `OR`, `XOR`, the format is `rd = op(rs1, rs2)`, where `op` is one of the operations.
- For `INC`, `rd = rd + 1` and the source registers are ignored.
- For `NOT`, only one source `rs` is required and `rd = ~rs` (logical not).
- For `SLLI, SRLI`, `rs2` represents a 4-bit immediate and `rd  = op(rs, imm)`, where `op` is left or right shift.
- For `LI`, `rs1` and `rs2` combined represent an 8-bit immediate, and `rd = imm`.
- For `LD` and `ST`, `rd` is loaded from/stored at (respectively) an address `rs1 + imm(rs2)` in the data cache.
- For `JMP`, `rd` and `rs1` represent an 8-bit immediate, which indicates jump the from current `PC`. The last four bits are discarded.
- For `BEQZ`, `rd` is compared with `0`; if the comparison is true, `PC` jumps by the 8-bit immediate (formed by combining `rs1` and `rs2`).
- For `HLT`, the remaining 12 bits are discarded, and the program halts.

> **Note**: All immediates are signed.

----
## To Run the Project:

1. Clone the repository:
```bash
git clone https://github.com/123shivanshukumar/Pipelining_simulation.git
cd Pipelining_simulation
```

2. Use the `input` and `output` directories to store the inputs and outputs of the simulation.
- To store the inputs, create a directory (for example, `Sample`) inside the `input` directory. Inside it, create 3 files: `ICache.txt`, `DCache.txt`, and `RF.txt`, containing 256, 256, and 16 8-bit entries respectively, represented as hexadecimal numbers. These represent the contents of the instruction cache, data cache, and register file before simulating the program. 

> **Note**: The `PC` starts at the first instruction in `ICache.txt`.

- Similarly, to store the outputs, create a directory (for example, `Sample`) with the same name as its corresponding input directory inside the `output` directory. Inside it, create 3 files: `DCache.txt`, `RF.txt`, and `Output.txt`. `DCache.txt` and `RF.txt` contain the contents of the data cache and register file, respectively, after simulation. `Output.txt` provides the program metrics. 

3. If the input data is not available, then write the assembly program in the file `program.s`. This will be used to populate the instruction cache in the `Program` directory.

4. Run the following command to populate the cache (if program `program.s` is used) and generate the binary for the simulation:
```bash
make
```

5. Run the following command to get the output of the simulation:
```bash
./simulate [Name of the directory]
```
For example: 
- To simulate the input in `Sample`:
```bash
./simulate Sample
```

- To simulate for the program written in `program.s`:
```bash
./simulate Program
```

> **Note**: Use the `Program` directory only if you want to simulate the program written in `program.s`.
