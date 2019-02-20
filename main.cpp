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

int max(int first, int second, int third){

    int max = first;

    if(second > max)
        max = second;

    if(third > max)
        max = third;

    return max;
}

int diff(std::string* geneArray, int xCord, int yCord){

    if(geneArray[0].at(xCord - 1) == geneArray[1].at(yCord - 1))
        return 0;

    return 1;
}

//this will only be single edit test, first with only +1 cost
int editCost(std::string* DNA){

    int answer;
    int xLength = DNA[0].length() + 1;
    int yLength = DNA[1].length() + 1;

    int** costMatrix = new int*[yLength];
    for(int i = 0; i < yLength; i++)
        costMatrix[i] = new int[xLength];

    for(int i = 0; i < xLength; i++)
        costMatrix[0][i] = i;

    for(int i = 0; i < yLength; i++)
        costMatrix[i][0] = i;

    for(int i = 1; i < yLength; i++){

        for(int j = 1; j < xLength; j++){

           costMatrix[i][j] = max(costMatrix[i][j - 1] + 1, costMatrix[i-1][j] + 1, costMatrix[i-1][j-1] + diff(DNA, j, i));
        }
    }

    return costMatrix[yLength - 1][xLength - 1];
}

int main(){

    int length;
    std::string** DNA = loadDNA(length);
    int** constArray = loadConst();
    printConst(constArray);
    printDNA(DNA,length);
    std::cout << "COST: " << editCost(DNA[0]) << std::endl;

    for(int i = 0; i < 6; i++)
        delete[] constArray[i];

    delete[] constArray;

    for(int i = 0; i < length; i++)
        delete[] DNA[i];

    delete[] DNA;

    return 0;
}