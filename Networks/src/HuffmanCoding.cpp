#include <iostream>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

/*
 * counts the frequency of occurence of every character in the file and returns an array
 */
int* freqCounter(string pathOfFile){
	int *freq = (int *) malloc(sizeof(int)*128);
	int i=0;
	/*
	 * Initilisation of the input array
	 */
	for(i=0;i<127;i++){
		freq[i] = 0;	
	}

	FILE *inputFile;
	inputFile = fopen("pathOfFile", "r");
	char ch;
	while((ch = getchar())!= EOF){
		freq[(int)ch]++;
	}
	fclose(inputFile);
	return freq;
}

	int main(){
		cout << "hello";
		return 0;
	}
