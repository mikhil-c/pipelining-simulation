#include<fstream>
#include<vector>
#include<string>
#include<sstream>
int convert_to_decimal(std::string& data){
    // length is 2
    int lower = (data[0] - '0' < 10 && data[0] - '0' >=0) ? data[0]-'0':data[0] - 'a' + 10;
    int upper = (data[1] - '0' < 10 && data[1] - '0' >=0) ? data[1]-'0':data[1] - 'a' + 10;

    return upper*16 + lower;
}
void get_data(const bool instructions, std::ifstream& input_file, int arr []){
    int j = 0;
    std::string data;
    while(input_file >> data){
        if(instructions){
            arr[j] = convert_to_decimal(data)*256;
            input_file >> data;
            arr[j] += convert_to_decimal(data);
        }
        else{
            arr[j] = convert_to_decimal(data);
        }
        j++;
    }
}
void fill_data_cache(std::ofstream& dcache_ouptut, int dcache[] ){
    std::stringstream ss;
    for (int j = 0 ; j < 256 ; j++){
        ss << std::hex << dcache[j];
        dcache_ouptut << ss.str() << std::endl; // convert to hexa decimal and output 
    }
}
void fill_output(std::ofstream& output, int output_metrics[]){
    std::vector<std::string> text = {
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

    output << text[0] << output_metrics[0] <<std::endl<< text[1] <<std::endl;
    int j = 1 ;
    for(; j < text.size() - 1; j++ ){
        output << text[j+1] << output_metrics[j] << std::endl; // to maintain offset 
    }
}
// convert to numeral & fill data
// decoding -- given a number decode the kind of instruction 
// convert back to hexadecimal
void simulate(std::string directory){
    int ICache[256], DCache[256];
    int RF[16];
    int output[13];
    // taking input 
    std::ifstream dcache("./input/"+ directory + "/DCache.txt");
    std::ifstream icache("./input/"+ directory + "/ICache.txt");
    std::ifstream rfile("./input"+ directory + "RF.txt");

    // simulation 







    std::ofstream dcache_output("./output" + directory + "DCache.txt");
    std::ofstream output("./output" + directory + "Output.txt");
    // 5 element queue to store output registers of perv 

    // 5 element array 

    // stall, control_hazard, raw_hazard





}
int main(){

}