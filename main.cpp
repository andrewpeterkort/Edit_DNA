/************************************************
* Andrew Peterkort
* CS_325 DNA Edit Cost 
************************************************/
#include <fstream>
#include <iostream>
#include <string>
#include <cstdlib>
#include <sstream>

//prints out DNA cost array for testing 
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

//prints out DNA input for testin
void printDNA(std::string** dataArray, int length){

    for(int i = 0; i < length; i++){

        for(int j = 0; j < 2; j++){

            std::cout << dataArray[i][j] << " | ";
        }

        std::cout << std::endl;
    }
}

//Keeps track of possible add/delete/keep paths through the DNA mutation in the path string while
//also recording the cost to take that path
struct Node{

    Node(): cost(0){
        //path[0] = "";
        //path[1] = "";
    }
    std::string path[2];
    int cost;
};

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

//loads the mutation costs from the const file.
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

//Loads the DNA sequencies to be tested
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

//returns the minimume cost option at current DNA mutation path. This can return a bias number if multiple options
//cost the same, hence why you can get different mutation paths for the same cost
Node* min(Node& xNode, int xAddition, Node& yNode, int yAddition, Node& diagonalNode, int diagonalAddition){

    int first = xNode.cost + xAddition;
    int second = yNode.cost + yAddition;
    int third = diagonalNode.cost + diagonalAddition;
    int min = first;
    std::string minPath[2] = {xNode.path[0].c_str(), xNode.path[1].c_str()};
    Node* newNode = new Node;

    if(second < min){

        min = second;
        minPath[0] = yNode.path[0].c_str();
        minPath[1] = yNode.path[1].c_str();
    }

    if(third < min ){

        min = third;
        minPath[0] = diagonalNode.path[0].c_str();
        minPath[1] = diagonalNode.path[1].c_str();
    }

    newNode->cost = min;
    newNode->path[0] = minPath[0].c_str(); //it makes sure to save the path of the cheepest cost
    newNode->path[1] = minPath[1].c_str();

    return newNode;
}

//because the program loads the const data or mutation costs into an int array instead of a char array, the collumns of the
//const array need to be translated to their coresponding base pair according to the file format... probably should
//have just been loaded in as char and then converted dynamically....
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

//X and Y costs correspond to inserts and deletions for and Edit distance algorithm. See edit distance
//algo online
int getInsertXCost(std::string* geneArray, int xCord, int** constArray){

    char changedXBase = geneArray[0].at(xCord - 1);
    return constArray[1][translateDNA(changedXBase)];
}

int getInsertYCost(std::string* geneArray, int yCord, int** constArray){

    char changedYBase = geneArray[1].at(yCord - 1);

    return constArray[1][translateDNA(changedYBase)];
}

//diagonal edit distance keeps or replaces the base
int getDiagonalCost(std::string* geneArray, int xCord, int yCord, int** constArray){

    char baseX = geneArray[0].at(xCord - 1);
    char baseY = geneArray[1].at(yCord - 1);

    return constArray[translateDNA(baseX)][translateDNA(baseY)];
}

//this is what loops through the cost matrix and finds the cost for all possible paths. Note the constant saving of paths...
//code probably needs refactoring the most out of all the code here
Node* editCost(std::string* DNA, int** constArray){

    int answer, cost;
    int xLength = DNA[0].length() + 1;
    int yLength = DNA[1].length() + 1;
    Node* newNode = new Node;
    Node* tempNode;

    Node** costMatrix = new Node*[yLength];
    for(int i = 0; i < yLength; i++)
        costMatrix[i] = new Node[xLength];

    costMatrix[0][0].cost = 0;

    for(int i = 1; i < xLength; i++){

        costMatrix[0][i].cost = costMatrix[0][i - 1].cost + getInsertXCost(DNA, i, constArray);
        costMatrix[0][i].path[0] = costMatrix[0][i - 1].path[0] + DNA[0].at(i - 1); 
        costMatrix[0][i].path[1] = costMatrix[0][i - 1].path[1] + "-";
    }

    for(int i = 1; i < yLength; i++){

        costMatrix[i][0].cost = costMatrix[i - 1][0].cost + getInsertYCost(DNA, i, constArray);
        costMatrix[i][0].path[0] = costMatrix[i - 1][0].path[0] +  "-";
        costMatrix[i][0].path[1] = costMatrix[i - 1][0].path[1] + DNA[1].at(i - 1);
    }

    for(int i = 1; i < yLength; i++){

        for(int j = 1; j < xLength; j++){ //here is the double loop that loops through the entire Node cost array

            tempNode = min(costMatrix[i][j - 1], getInsertXCost(DNA, j, constArray), costMatrix[i-1][j], getInsertYCost(DNA, i, constArray), costMatrix[i-1][j-1],  getDiagonalCost(DNA, j, i, constArray));

            costMatrix[i][j].cost = tempNode->cost;

						//notice how the if statements check to see what Node is returned by checking their paths..... this probably needs
						//to be replaced with something better for building the edit path for the Nodes
            if((tempNode->path[0] == costMatrix[i][j - 1].path[0]) && (tempNode->path[1] == costMatrix[i][j - 1].path[1])){

                tempNode->path[0] += DNA[0].at(j - 1);//this would mean a delation if going from 0 to 1 and insertion if from 1 to 0
                tempNode->path[1] += "-";
            }

            if((tempNode->path[0] == costMatrix[i - 1][j].path[0]) && (tempNode->path[1] == costMatrix[i - 1][j].path[1])){

                tempNode->path[0] += "-"; //read above comment, vice versus
                tempNode->path[1] += DNA[1].at(i - 1);
            }

            if((tempNode->path[0] == costMatrix[i - 1][j - 1].path[0]) && (tempNode->path[1] == costMatrix[i - 1][j - 1].path[1])){

                tempNode->path[0] += DNA[0].at(j - 1); //this just adds the new path character to each path, replacement or skip
                tempNode->path[1] += DNA[1].at(i - 1);
            }


           costMatrix[i][j] = *tempNode;
           delete tempNode;
        }
    }

    newNode->path[0] = costMatrix[yLength - 1][xLength - 1].path[0]; //all the answers are stored in newNode
    newNode->path[1] = costMatrix[yLength - 1][xLength - 1].path[1];
    newNode->cost = costMatrix[yLength - 1][xLength - 1].cost;

    for(int i = 0; i < yLength; i++)
        delete[] costMatrix[i];

    delete[] costMatrix;

    return newNode;
}

int main(){

    int length;
    std::ofstream outputFile;
    outputFile.open("imp2output_our.txt");
    std::string** DNA = loadDNA(length);
    int** constArray = loadConst();
    Node* answer;
    //printArray(constArray, 6, 6);
    //printDNA(DNA,length);

    for(int i = 0; i < length; i++){

        answer = editCost(DNA[i], constArray);
        outputFile << answer->path[0].c_str() << "," <<  answer->path[1].c_str() << ":" <<  answer->cost << std::endl;
    }

    delete answer;

    for(int i = 0; i < 6; i++)
        delete[] constArray[i];

    delete[] constArray;

    for(int i = 0; i < length; i++)
        delete[] DNA[i];

    delete[] DNA;
    outputFile.close();
    
    return 0;
}
