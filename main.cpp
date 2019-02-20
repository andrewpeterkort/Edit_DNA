/************************************************
* Andrew Peterkort
* CS_325 DNA Edit Cost 
************************************************/
#include <fstream>
#include <iostream>
#include <string>
#include <cstdlib>
#include <sstream>

void printConst(int** constArray){

    for(int i = 0; i < 6; i++){

        for(int j = 0; j < 6; j++){
            
            std::cout << constArray[i][j];
        }
        std::cout << std::endl;
    }
}

int** loadConst(){

    int lineNumber = 0;
    std::string line, token;
    std::fstream constFile;
    char** ptr = nullptr;
    constFile.open("imp2cost.txt", std::ios::in);

    int** constArray = new int*[6];
    for(int i = 0; i < 6; i++)
        constArray[i] = new int[6];

    for(int i = 0; i < 6; i++){

        std::getline(constFile, line);
        std::stringstream stream(line);

        for(int j = 0; j < 6; j++){

            std::getline(stream, token, ',');
            constArray[lineNumber][j] = strtol(token.c_str(), ptr, 10);
        }

        lineNumber++;
    }

    constFile.close();
    return constArray;
}

int main(){

    int** constArray = loadConst();
    printConst(constArray);

    for(int i = 0; i < 6; i++)
        delete[] constArray[i];

    delete[] constArray;
    return 0;
}