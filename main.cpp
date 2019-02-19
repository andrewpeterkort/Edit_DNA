/************************************************
* Andrew Peterkort
* CS_325 DNA Edit Cost 
************************************************/
#include <fstream>
#include <iostream>
#include <string>

int** loadConst(){

    std::fstream constFile;
    constFile.open("imp2cost.txt", std::ios::in);

    int** costArray = new int*[5];
    for(int i = 0; i < 5; i++)
        costArray[i] = new int[5];
        
    std::string line;
    while(std::getline(constFile, line))
        std::cout << line << std::endl;

    return costArray;
}



int main(){

    loadConst();

    return 0;
}