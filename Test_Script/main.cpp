#include <fstream>
#include <stdlib.h>
#include <ctime>

char randomBase(){

    int num = rand() % 4;

    switch(num){

        case 0:
            return 'A';
        case 1:
            return 'T';
        case 2:
            return 'G';
        case 3:
            return 'C';
    }

    return  'A';
}

int main(){

    srand(time(NULL));
    int number = 4000;
    std::ofstream outputFile;
    outputFile.open("imp2input.txt");

    for(int i = 0; i < 10; i++) {

        for(int j = 0; j < number; j++){
						char temp = randomBase();
            outputFile.write(&temp, 1);

				}

        outputFile << ',';

        for(int j = 0; j < number; j++)
            outputFile << randomBase();
        
        outputFile << std::endl;
    }

    outputFile.close();
    return 0;
}
