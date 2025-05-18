#include <fstream>
#include <cstdint>
#include <string>
#include <sstream>
#include <algorithm>
#include <cctype>

std::string convert_to_hex(uint8_t num) {
    std::string result;
    int upper = num >> 4;
    int lower = num % 16;
    result += (upper < 10) ? '0' + upper : 'a' + (upper - 10);
    result += (lower < 10) ? '0' + lower : 'a' + (lower - 10);
    return result;
}

void fill_cache(std::ofstream& cache_ouptut, uint8_t Cache[]) {
    for (int j = 0; j < 256; j++) {
        cache_ouptut << convert_to_hex(Cache[j]) << "\n";
    }
}

uint8_t opcode_to_int(std::string& opcode) {
    if (opcode == "add") {
        return 0;
    }
    else if (opcode == "sub") {
        return 1;
    }
    else if (opcode == "mul") {
        return 2;
    }
    else if (opcode == "inc") {
        return 3;
    }
    else if (opcode == "and") {
        return 4;
    }
    else if (opcode == "or") {
        return 5;
    }
    else if (opcode == "xor") {
        return 6;
    }
    else if (opcode == "not") {
        return 7;
    }
    else if (opcode == "slli") {
        return 8;
    }
    else if (opcode == "srli") {
        return 9;
    }
    else if (opcode == "li") {
        return 10;
    }
    else if (opcode == "ld") {
        return 11;
    }
    else if (opcode == "st") {
        return 12;
    }
    else if (opcode == "jmp") {
        return 13;
    }
    else if (opcode == "beqz") {
        return 14;
    }
    else { // opcode == "hlt"
        return 15;
    }
}

uint8_t reg_to_int(std::string reg) {
    reg.erase(0, 1); // removes letter r
    return (uint8_t)std::stoi(reg);
}

uint8_t imm4_to_int(std::string imm) {
    int num = std::stoi(imm);
    return num >= 0 ? (uint8_t)num : (uint8_t)(16 + num);
}

uint8_t imm8_to_int(std::string imm) {
    int num = std::stoi(imm);
    return num >= 0 ? (uint8_t)num : (uint8_t)(256 + num);
}

int main() {
    // input file
    std::ifstream program("./program.s");
    
    uint8_t ICache[256] = {0};

    int j = 0;
    std::string instruction;
    while (std::getline(program, instruction)) {
        // skip empty or whitespace-only lines
        if (instruction.empty() || std::all_of(instruction.begin(), instruction.end(), ::isspace)) {
            continue;
        }       
        std::transform(instruction.begin(), instruction.end(), instruction.begin(), [] (unsigned char c) {
            return std::tolower(c);
        });
        std::istringstream instrStream(instruction);
        uint8_t data;
        std::string opcode, rd, rs1, rs2;
        instrStream >> opcode;
        if (opcode == "#") {
            continue;
        }
        if (opcode == "add" || opcode == "sub" || opcode == "mul" || opcode == "and" || opcode == "or" || opcode == "xor") {
            instrStream >> rd;
            data = (opcode_to_int(opcode) << 4) + reg_to_int(rd);
            ICache[j] = data;
            j++;

            instrStream >> rs1 >> rs2;
            data = (reg_to_int(rs1) << 4) + reg_to_int(rs2);
            ICache[j] = data;
            j++;
        }
        else if (opcode == "inc") {
            instrStream >> rd;
            data = (opcode_to_int(opcode) << 4) + reg_to_int(rd);
            ICache[j] = data;
            j++;

            j++;
        }
        else if (opcode == "not") {
            instrStream >> rd;
            data = (opcode_to_int(opcode) << 4) + reg_to_int(rd);
            ICache[j] = data;
            j++;

            instrStream >> rs1;
            data = (reg_to_int(rs1) << 4);
            ICache[j] = data;
            j++;
        }
        else if (opcode == "slli" || opcode == "srli" || opcode == "ld" || opcode == "st") {
            instrStream >> rd;
            data = (opcode_to_int(opcode) << 4) + reg_to_int(rd);
            ICache[j] = data;
            j++;

            instrStream >> rs1 >> rs2;
            data = (reg_to_int(rs1) << 4) + imm4_to_int(rs2);
            ICache[j] = data;
            j++;
        }
        else if (opcode == "li" || opcode == "beqz") {
            instrStream >> rd;
            data = (opcode_to_int(opcode) << 4) + reg_to_int(rd);
            ICache[j] = data;
            j++;

            instrStream >> rs1;
            data = imm8_to_int(rs1);
            ICache[j] = data;
            j++;
        }
        else if (opcode == "jmp") {
            instrStream >> rd;
            uint8_t imm = imm8_to_int(rd);
            data = (opcode_to_int(opcode) << 4) + (imm >> 4);
            ICache[j] = data;
            j++;

            data = (imm << 4);
            ICache[j] = data;
            j++;
        }
        else { // opcode == "hlt"
            data = (opcode_to_int(opcode) << 4);
            ICache[j] = data;
            j++;

            j++;
        }
    }

    // setup output file
    std::ofstream icache_output("./input/Program/ICache.txt");

    // write data
    fill_cache(icache_output, ICache);
}
