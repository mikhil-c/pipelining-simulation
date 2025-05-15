#include <fstream>
#include <vector>
#include <string>
#include <cstdint>

void simulate(std::string directory){
    int ICache[256], DCache[256];
    // taking input 
    std::ifstream dcache("./input/"+directory + "/DCache.txt");
    std::ifstream icache("./input/"+directory + "/ICache.txt");
}

void decode_instruction(int instruction, std::vector<int8_t>& RF, int8_t& A, int8_t& B, int& opcode, int& rd, int& rs1, int& rs2) {
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

    // stores the output in the temporary registers
    A = RF[rs1];
    B = RF[rs2];
}

int8_t get_imm_4(int& num) {
    return num < 8 ? num : 16 - num; 
}

void execute_instruction(std::vector<int8_t>& RF, int8_t& A, int8_t& B, int8_t& ALUOuput, int& PC, bool& halt, int& opcode, int& rd, int& rs1, int&rs2) {
    switch (opcode) {
        case 0: ALUOuput = A + B; break;               // ADD
        case 1: ALUOuput = A - B; break;               // SUB
        case 2: ALUOuput = A * B; break;               // MUL
        case 3: ALUOuput = RF[rd] + 1; break;          // INC
        case 4: ALUOuput = A & B; break;               // AND
        case 5: ALUOuput = A | B; break;               // OR
        case 6: ALUOuput = A ^ B; break;               // XOR
        case 7: ALUOuput = ~A; break;                  // NOT
        case 8: ALUOuput = A << get_imm_4(rs2); break; // SLLI
        case 9: ALUOuput = A >> get_imm_4(rs2); break; // SRLI
        case 10: int8_t imm = (rs1 << 4) + rs2;        // LI
                 ALUOuput = imm;
                 break;                                
        case 11:                                       
        case 12: ALUOuput = A + get_imm_4(rs2); break; // LD & ST
        case 13: int8_t imm = (rd << 4) + rs1;         // JMP
                 PC += imm;
                 break;
        case 14: int8_t imm = (rs1 << 4) + rs2;        // BEQZ
                 if (RF[rd] == 0) {
                     PC += imm;
                 }
                 break;
        case 15: halt = true;                          // HLT
    }
}

int main() {

}
