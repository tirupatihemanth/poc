#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <queue>
#include <time.h>
#include <string.h>
#include <limits.h>
#include <math.h>

/*typedef struct node_ Node;

struct node_
{
	char ch;
	int freq;
	Node *left;
	Node *right;
};*/

class Node{

public:
	char ch;
	int freq;
	Node *left;
	Node *right;

	bool operator<(Node node) const{
		return this->freq > node.freq;
	}

};

using namespace std;

#define PACKETSIZE 128
#define CODELENGTH 127
#define CRCLENGTH 1

//code word length 127 and crc padding 1

char **codebook;

/*
 * counts the frequency of occurence of every character in the file and returns an array
 */

void updateFrequencies(char pathOfFile[], Node *nodes){

	FILE *input;
	input = fopen(pathOfFile, "r");
	char ch;
	while((ch = fgetc(input))!= EOF){
		nodes[(int)ch].freq++;
	}
	fclose(input);
	

}

/*
 * constructs all the codewords generated from the huffmann coded tree and puts the code word string  
 * in the array indexed with the ascii value of the character
 *
 */

void extractMessage(Node *root, char receivedFile[]){

	FILE *input = fopen(receivedFile, "r");
	FILE *output = fopen("../decodedFile.txt", "w");
	char ch;
	Node *node = root;
	while((ch = fgetc(input)) != EOF || ch != '\0'){

		if(node == NULL){
			printf("errorInTransmission");
			exit(0);
		}
		if(node->left == NULL && node->right == NULL){
			fputc(node->ch, output);
			node = root;
		}
		if(ch == '0'){
			node = node->left;
		}
		else if(ch == '1'){
			node = node->right;
		}
	}

	fclose(input);
	fclose(output);

}


void constructCodeBook(Node *node, char *str){
	int temp;
	//printf("entered\n");
	if(node == NULL){
		return;
	}
	else if(node->left == NULL && node->right == NULL){
		//printf("character: %c code: %s\n", node->ch, str);
		strcpy(codebook[(int)node->ch], str);
	}
	else{
		temp = strlen(str);
		//printf("length: %d\n", temp);
		str[temp] = '0';
		str[temp+1] = '\0';
		constructCodeBook(node->left, str);
		//printf("str1: %s\n", str);
		str[strlen(str)-1] = '\0';
		str[temp] = '1';
		str[temp+1] = '\0';
		//printf("str2: %s\n", str);
		constructCodeBook(node->right, str);
		str[strlen(str)-1] = '\0';
	}


}


void constructEncodedFile(char inputFile[]){

	FILE *input = fopen(inputFile, "r");
	FILE *output = fopen("../bin/encodedFile.txt", "w");

	char ch;
	while((ch = fgetc(input)) != EOF){
		fputs(codebook[(int)ch], output);
	}
	fclose(input);
	fclose(output);

}


/*
	Implementing an one bit crc
*/

char * computeCRC(char *str){

	int sum=0,i;
	for(i=0;i<strlen(str);i++){
		if(str[i] == '1'){
			sum++;
		}
	}

	if(sum%2 == 0){
		return "0";
	}
	else{
		return "1";
	}

}


void packetise(char encodedFile[]){
	
	FILE *input = fopen(encodedFile, "r");
	char ch;
	int i=0,j;
	char *str = (char *) malloc(sizeof(char)*PACKETSIZE);
	char *crc;

	while(1){
		ch = fgetc(input);
		if(ch == EOF){
			if(i==0){
				break;
			}
			for(;i<CODELENGTH;i++){
				str[i] = '\0';
			}
		}

		str[i] = ch;
		if(i == CODELENGTH){
			str[i] = '\0';
			crc = computeCRC(str);
			for(j=0;j<strlen(crc);j++){
				str[i] = crc[j];
				i++;
			}
			str[i] = '\0';
			printf("packet: %s\n",str);
			// packet ready for sending from here
			str = (char *) malloc(sizeof(char)*PACKETSIZE);
			i = -1;
		}
		//str[i] = ch;
		i++;
	}
}


char* induceError(char *str){

	int i=0;
	for(i=0;i<PACKETSIZE;i++){
		if(rand()*10000 == 5000){
			if(str[i] == '1'){
				str[i] = '0';
			}
			else if(str[i] == '0'){
				str[i] = '1';
			}
		}	
	}

}


char * getCodeEncodeSizeEncoding(int num){
	char *str  = (char *) malloc(sizeof(char) * 8);
	int i=0;
	while(num!=0){
		if(num%2 == 0){
			str[i++] = '0';
		}
		else{
			str[i++] = '1';	
		}
		num = num/2;
	}
	for(i;i<8;i++){
		str[i] = '0';
	}
	str[8]='\0';
	return str;

}

int main(){

	Node *nodes;
	nodes = (Node *) malloc(sizeof(Node)*128);

	srand(time(NULL));
	/*
		Initialising a node for each character and frequency to be zero
	*/

	int i,j;
	for(i=0;i<128;i++){
		nodes[i].ch = (char)i;
		nodes[i].freq = 0;
		nodes[i].left = NULL;
		nodes[i].right = NULL;
	}

	/*
	 * updating frequencies of the characters in the nodes
	 */

	updateFrequencies("../input.txt", nodes);

	priority_queue<Node> queue;
	for(i=0;i<128;i++){
			//printf("%d space %c space %d\n",i, nodes[i].ch, nodes[i].freq);
			queue.push(nodes[i]);
	}
	
	//printf("blah");
	/*
	 * constructing the huffmann tree by using the priority queue data structure and Huff Mann coding algorithm
	 */

	//printf("%c %d\n", queue.top().ch, queue.top().freq);

	Node *node1; 
	Node *node2;
	while(queue.size() > 1){
		node1  = (Node *) malloc(sizeof(Node));
		node2  = (Node *) malloc(sizeof(Node));
		//*node1 = queue.top();
		node1->freq = queue.top().freq;
		node1->ch = queue.top().ch;
		node1->left = queue.top().left;
		node1->right = queue.top().right;
		queue.pop();
		//*node2 = queue.top();
		node2->freq = queue.top().freq;
		node2->ch = queue.top().ch;
		node2->left = queue.top().left;
		node2->right = queue.top().right;
		queue.pop();
		Node node;
		node.freq = node1->freq + node2->freq;
		node.left = node1;
		node.right = node2;
		queue.push(node);
	}

	Node *root = (Node *) malloc(sizeof(Node));
	*root = queue.top();
	queue.pop();
	//printf("freq: %d %d %d %d\n", root->freq, root->left->freq, root->left->left->freq, root->right->freq);

	
	//	It's an one dimensional array of strings where index gives the ascii value of the character and the codeword is a string
	

	codebook = (char **) malloc(sizeof(char *)*128);
	for(i=0;i<128;i++){
		codebook[i] = (char *) malloc(sizeof(char)*50);
	}

	char *str = (char *) malloc(sizeof(char)*50);
	strcpy(str, "");
	constructCodeBook(root, str);
	//int maxLength = INT_MIN;
/*	for(i=0;i<128;i++){
		if(strlen(codebook[i])>maxLength){
			maxLength = strlen(codebook[i]);
		}
		printf("%s\n", codebook[i]);
	}*/

	int codeSize;
	char *codeSizeEncoding;
	//printf("codesize: %d\n", codeSize);

	FILE *codebookFile= fopen("../bin/codebook.txt","w");
	char *var;

	for(i=0;i<128;i++){
		codeSizeEncoding = getCodeEncodeSizeEncoding(strlen(codebook[i]));
		printf("codeSizeEncoding: %s\n", codeSizeEncoding);
		fputs(codeSizeEncoding, codebookFile);
		fputs(codebook[i], codebookFile);
		//fprintf(codebookFile, "%s%s",codeSizeEncoding, codebook[i]);
	}
	fclose(codebookFile);
	constructEncodedFile("../input.txt");
	packetise("../bin/codebook.txt");
	return 0;

}
