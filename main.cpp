/************************************************
* Andrew Peterkort
* CS_325 DNA Edit Cost 
************************************************/
#include <fstream>
#include <iostream>
#include <string>
#include <cstdlib>
#include <sstream>

void printArray(int** constArray, int xLength, int yLength){

    std::cout << "START------------------------------------" << std::endl;

    for(int i = 0; i < yLength; i++){

        for(int j = 0; j < xLength; j++){
            
            std::cout << constArray[i][j] << " ";
        }
        std::cout << "|" << std::endl;
    }

    std::cout << "END------------------------------------" << std::endl;
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

int min(int first, int second, int third){

    int min = first;

    if(second < min)
        min = second;

    if(third < min )
        min = third;

    return min;
}

int translateDNA(char& base){

    switch(base){

        case 'A':
            return 2;
        case 'T':
            return 3;
        case 'G':
            return 4;
        case 'C':
            return 5;
        default:
            break;
    }

    return 1;
}

int getInsertXCost(std::string* geneArray, int xCord, int** constArray){

    char changedXBase = geneArray[0].at(xCord - 1);
    return constArray[1][translateDNA(changedXBase)];
}

int getInsertYCost(std::string* geneArray, int yCord, int** constArray){

    char changedYBase = geneArray[1].at(yCord - 1);

    return constArray[1][translateDNA(changedYBase)];
}

int getDiagonalCost(std::string* geneArray, int xCord, int yCord, int** constArray){

    char baseX = geneArray[0].at(xCord - 1);
    char baseY = geneArray[1].at(yCord - 1);

    return constArray[translateDNA(baseX)][translateDNA(baseY)];
}

int editCost(std::string* DNA, int** constArray){

    int answer, cost;
    int xLength = DNA[0].length() + 1;
    int yLength = DNA[1].length() + 1;

    int** costMatrix = new int*[yLength];
    for(int i = 0; i < yLength; i++)
        costMatrix[i] = new int[xLength];

    costMatrix[0][0] = 0;

    for(int i = 1; i < xLength; i++)
        costMatrix[0][i] = costMatrix[0][i - 1] + getInsertXCost(DNA, i, constArray);

    for(int i = 1; i < yLength; i++)
        costMatrix[i][0] = costMatrix[i - 1][0] + getInsertYCost(DNA, i, constArray);

    for(int i = 1; i < yLength; i++){

        for(int j = 1; j < xLength; j++){

           costMatrix[i][j] = min(costMatrix[i][j - 1] + getInsertXCost(DNA, j, constArray), costMatrix[i-1][j] + getInsertYCost(DNA, i, constArray), costMatrix[i-1][j-1] + getDiagonalCost(DNA, j, i, constArray));
        }
    }

    cost = costMatrix[yLength - 1][xLength - 1];

    for(int i = 0; i < yLength; i++)
        delete[] costMatrix[i];

    delete[] costMatrix;

    return cost;
}

int main(){

    int length;
    std::string** DNA = loadDNA(length);
    int** constArray = loadConst();
    printArray(constArray, 6, 6);
    printDNA(DNA,length);
    std::cout << "Cost: " << editCost(DNA[0], constArray) << std::endl;

    for(int i = 0; i < 6; i++)
        delete[] constArray[i];

    delete[] constArray;

    for(int i = 0; i < length; i++)
        delete[] DNA[i];

    delete[] DNA;
    
    return 0;
}