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

void printDNA(std::string** dataArray, int length){

    for(int i = 0; i < length; i++){

        for(int j = 0; j < 2; j++){

            std::cout << dataArray[i][j] << " | ";
        }

        std::cout << std::endl;
    }
}

int countFileLines(std::fstream& input){

    int count = 0;
    std::string currentLine;
    input.clear();
    input.seekp(0, input.beg); 

    while(std::getline(input, currentLine))
        count++;

    input.clear();
    input.seekp(0, input.beg);
    return count;
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

std::string** loadDNA(int& length){

    std::string line;
    std::fstream inputFile;
    inputFile.open("imp2input.txt", std::ios::in);

    length = countFileLines(inputFile);
    std::string** DNA = new std::string*[length];
    for(int i = 0; i < length; i++)
        DNA[i] = new std::string[2];

    for(int i = 0; i < length; i++){

        std::getline(inputFile, line);
        std::stringstream stream(line);

        for(int j = 0; j < 2; j++){

            std::getline(stream, DNA[i][j], ',');
        }
    }

    inputFile.close();
    return DNA;
}

int main(){

    int length;
    std::string** DNA = loadDNA(length);
    int** constArray = loadConst();
    printConst(constArray);
    printDNA(DNA,length);

    for(int i = 0; i < 6; i++)
        delete[] constArray[i];

    delete[] constArray;

    for(int i = 0; i < length; i++)
        delete[] DNA[i];

    delete[] DNA;

    return 0;
}