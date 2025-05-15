#include<fstream>
#include<vector>
#include<string>

void simulate(std::string directory){
    int ICache[256], DCache[256];
    // taking input 
    std::ifstream dcache("./input/"+directory + "/DCache.txt");
    std::ifstream icache("./input/"+directory + "/ICache.txt");
}

int bin_to_dec(int bin_num) {
    int dec_num = 0;
    int base = 1;
    while (bin_num) {
        if (bin_num & 1) dec_num += base;
        base <<= 1;
        bin_num >>= 1;
    }
    return dec_num;
}

void decode_instruction(int instruction, std::stirng &opcode, int& rd, int& rs1, int& rs2) {
    // extracting rs2
    int rs2_encoding = ((instruction >> 4) << 4) ^ instruction;
    rs2 = bin_to_dec(rs2_encoding);
    instruction >>= 4;

    //extracting rs1
    int rs1_encoding = ((instruction >> 4) << 4) ^ instruction;
    rs1 = bin_to_dec(rs1_encoding);
    instruction >>= 4;
    
    //extracting rd
    int rd_encoding = ((instruction >> 4) << 4) ^ instruction;
    rd = bin_to_dec(rd_encoding);
    instruction >>= 4;

    // extracting opcode
    int opcode_encoding = ((instruction >> 4) << 4) ^ instruction;
    opcode = bin_to_dec(opcode_encoding);
}

int main(){

}
