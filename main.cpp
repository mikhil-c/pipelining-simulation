#include <fstream>
#include <string>
#include <sstream>
#include <cstdint>
#include <filesystem>

// file handling functions
template <typename T>
T convert_to_decimal(std::string& data) {
    // length is 2
    T lower = (data[0] - '0' < 10 && data[0] - '0' >= 0) ? data[0] - '0' : data[0] - 'a' + 10;
    T upper = (data[1] - '0' < 10 && data[1] - '0' >= 0) ? data[1] - '0' : data[1] - 'a' + 10;
    return (lower << 4) + upper;
}

template <typename T> // either int or int8_t
void get_data(int type, std::ifstream& input_file, T arr[]) {
    int j = 0;
    std::string data;
    while (input_file >> data) {
        if (type == 1) {
            arr[j] = convert_to_decimal<int>(data) << 8;
            input_file >> data;
            arr[j] += convert_to_decimal<int>(data);
        }
        else if (type == 0) {
            arr[j] = convert_to_decimal<int8_t>(data);
        }
        j++;
    }
}

void fill_data_cache(std::ofstream& dcache_ouptut, int8_t dcache[]) {
    std::stringstream ss;
    for (int j = 0; j < 256; j++) {
        dcache_ouptut << dcache[j] << "\n"; // convert to hexadecimal and output 
    }
}

void fill_output(std::ofstream& output, int output_metrics[]) {
    std::string text[] = {
        "Total number of instructions executed        : ",
        "Number of instructions in each class",     
        "Arithmetic instructions                      : ",
        "Logical instructions                         : ",
        "Shift instructions                           : ",
        "Memory instructions                          : ",
        "Load immediate instructions                  : ",
        "Control instructions                         : ",
        "Halt instructions                            : ",
        "Cycles Per Instruction                       : ",
        "Total number of stalls                       : ",
        "Data stalls (RAW)                            : ",
        "Control stalls                               : "
    };
    output << text[0] << output_metrics[0] << "\n" << text[1] << "\n";
    int j = 1;
    for (; j < 13 - 1; j++) { // 13 is the size of the text array
        output << text[j + 1] << output_metrics[j] << "\n"; // to maintain offset 
    }
}

// stages
void instruction_fetch(int ICache[], int& PC, int& IR, int& stall_count) {
    if (stall_count <= 0) {
        IR =  ICache[PC]; 
        PC++; // not doing +2
        int _opcode = IR >> 12;
        if (_opcode == 13 || _opcode == 14) {
            stall_count += 2;
        }
    }
}

void decode_instruction(int instruction, int8_t RF[], int8_t& A, int8_t& B, int& opcode, int& rd, int& rs1, int& rs2, int& stall_count, std::pair<int, int> instruction_metadata[]) {
    // extracting rs2
    rs2 = instruction % 16;
    instruction >>= 4;

    // extracting rs1   
    rs1 = instruction % 16;
    instruction >>= 4;
    
    // extracting rd
    rd = instruction % 16;
    instruction >>= 4;

    // extracting opcode
    opcode = instruction;

    int _rd,_opcode;
    // check if any instruction in 2 3 4 induces a dependency in ID, set metadata appropriately
    bool RAW = false;
    for(int j = 2 ; j < 5 ; j++){ // the earlier the better 
        _opcode = instruction_metadata[j].first;
        _rd = instruction_metadata[j].second;
        if(_opcode <= 11 && (_rd == rs1 || _rd == rs2) && _rd !=0 ){ // dont stall if _rd = 0
            RAW  = true;
            stall_count = 5 - j;
            break; 
        }
    }
    // storing the output in the temporary registers
    if (!RAW) {
        A = RF[rs1];
        B = RF[rs2];
        instruction_metadata[1] = instruction_metadata[0]; // pull the IF stage instruction into decode, else let it stay 
    }    
}

int8_t get_imm_4(int& num) {
    return num < 8 ? num : 16 - num; 
}

void execute_instruction(int8_t RF[], int8_t& A, int8_t& B, int8_t& ALUOuput, int& PC, bool& halt, int& opcode, int& rd, int& rs1, int&rs2) {
    int8_t imm = 0;
    switch (opcode) {
        case 0:                           // ADD
            ALUOuput = A + B;
            break;
        case 1:                           // SUB
            ALUOuput = A - B;
            break;
        case 2:                           // MUL
            ALUOuput = A * B;
            break;
        case 3:                           // INC
            ALUOuput = RF[rd] + 1;
            break;
        case 4:                           // AND
            ALUOuput = A & B;
            break;
        case 5:                           // OR
            ALUOuput = A | B;
            break;
        case 6:                           // XOR
            ALUOuput = A ^ B;
            break;
        case 7:                           // NOT
            ALUOuput = ~A;
            break;
        case 8:                           // SLLI
            ALUOuput = A << get_imm_4(rs2);
            break;
        case 9:                           // SRLI
            ALUOuput = A >> get_imm_4(rs2);
            break;
        case 10:                          // LI
            imm = (rs1 << 4) + rs2;
            ALUOuput = imm;
            break;
        case 11:
        case 12:                          // LD & ST
            ALUOuput = A + get_imm_4(rs2);
            break;
        case 13:                          // JMP
            imm = (rd << 4) + rs1;
            PC += imm;
            break;
        case 14:                          // BEQZ
            imm = (rs1 << 4) + rs2;
            if (RF[rd] == 0) {
                PC += imm;
            }
            break;
        case 15:                          // HLT
            halt = true;
    }
}

void memory(int8_t RF[], int8_t DCache[], int& opcode, int& rd, int8_t& ALUOutput, int8_t& LMD) {
    // ALUOutput is the address
    if (opcode == 11) {      // load instruction
        LMD = DCache[ALUOutput];
    }
    else if (opcode == 12) { // store instruction
        DCache[ALUOutput] = RF[rd];
    }
}


void write_back(int8_t RF[], int& rd, int& opcode, const int8_t& ALUOutput, const int8_t& LMD ){
    if(opcode == 11){ // load instruction
        RF[rd] = LMD; // load memory data
    }
    else if (opcode < 11) {
        RF[rd] = ALUOutput;
    }
}

std::pair<int, int> get_metadata(int instruction) {
    instruction >>= 8; // to get opcode and rd
    return {instruction >> 4, instruction % 16};
}

// main simulation function
void simulate(std::string directory) {
    int ICache[128];
    int8_t DCache[256], RF[16];
    int output_metrics[12] = {0};   // to be filled manually
    int PC = 0, IR;
    int8_t A, B, ALUOutput, LMD;
    int stall_count = 0, clock = 0; // stall_count to see if the processor is stalled, clock to count clock cycle 
    bool halt = false;
    
    // taking input 
    std::ifstream icache("./input/" + directory + "/ICache.txt");
    std::ifstream dcache("./input/" + directory + "/DCache.txt");
    std::ifstream rfile("./input/" + directory + "/RF.txt");

    // fetch data
    get_data(1, icache, ICache);
    get_data(0, dcache, DCache);
    get_data(0, rfile, RF);
    /*
     * type = 1 for instr cache
     * type = 0 for data cache and reg file
     */

    std::pair<int, int> instruction_metadata[5]; // to store the metadata of instruction 
    /* 
     * (opcode, rd)
     * 0: IF Stage
     * 1: ID stage
     * 2: EXE stage
     * 3: MEM stage 
     * 4: WB stage
     * in every clock cycle transfer the metadata to the next stage for the same instructions
     */

    // local variables
    int opcode, rd, rs2, rs1;

    while(!halt && clock < 25){
        // all instructions except IF run as usual

        // writeback stage
        if (clock > 3) {
            opcode = instruction_metadata[4].first;
            rd = instruction_metadata[4].second;

            write_back(RF, rd, opcode, ALUOutput, LMD);

            instruction_metadata[4] = instruction_metadata[3];
        }

        // memory stage
        if (clock > 2) {
            opcode = instruction_metadata[3].first;
            rd = instruction_metadata[3].second;

            memory(RF, DCache, opcode, rd, ALUOutput, LMD);

            instruction_metadata[3] = instruction_metadata[2];
        }

        // execute stage
        if (clock > 1) {
            opcode = instruction_metadata[2].first;
            rd = instruction_metadata[2].second;

            execute_instruction(RF, A, B, ALUOutput, PC, halt, opcode, rd, rs1, rs2); // rs1, rs2, A, B was updated in the previous iteration ... 

            instruction_metadata[2] = instruction_metadata[1];
        }

        // decode stage
        if(clock > 0){opcode = instruction_metadata[1].first;
        rd = instruction_metadata[1].second;

            decode_instruction(IR, RF, A, B, opcode, rd, rs1, rs2, stall_count, instruction_metadata);
        }

        // IF for this cycle
        instruction_fetch(ICache, PC, IR, stall_count); // IR = ICache[PC], PC++ 

        if (stall_count > 0) {
            stall_count--;
        }
        else {
            instruction_metadata[0] = get_metadata(ICache[PC]); // to ensure correctness
        }
        clock++;
    }

    // setup outputfiles 
    std::ofstream dcache_output("./output/" + directory + "/DCache.txt");
    std::ofstream output("./output/" + directory + "/Output.txt");

    // write data
    fill_data_cache(dcache_output, DCache);
    fill_output(output, output_metrics);
}

int main() {
    std::string path = "./input";
    for (auto& entry : std::filesystem::directory_iterator(path)) {
        if (entry.is_directory()) {
            std::string directory = entry.path().filename().string();
            simulate(directory);
        }
    }
}
