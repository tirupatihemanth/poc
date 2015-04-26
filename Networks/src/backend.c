#include <stdio.h>
#include <math.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "string.h"
#include "stdlib.h"


typedef struct node_ Node;

struct node_
{
	char ch;
	int freq;
	Node *left;
	Node *right;
};

Node *root;


int getCodeLength(char *code)
{

  int i,codeLength=0;
  for (i=0; i<8; i++)
  {
    if (code[i]=='1')
      codeLength+= pow(2,i);
  }
  return codeLength;
}

Node* constructHuffmanTree(Node* node, char ch)
{
  if (ch=='0')
  {
    if (node->left==NULL)
    {
      Node *temp= (Node*)malloc(sizeof(Node));
      node->left= temp;
    }
    return node->left;
  }
  
  else if (ch=='1')
  {
    if (node->right==NULL)
    {
      Node *temp= (Node*)malloc(sizeof(Node));
      node->right= temp;
    }
    return node->right;
  }
} 

void decoder()
{
    root= (Node*)malloc(sizeof(Node));
    
    FILE *input= fopen("../bin/echoedMessage.txt", "r");
    FILE *output= fopen("../bin/crcFreeMessage.txt", "w");
    
    // Removing the crc bit form each packet in the received message
    char ch; 
    int i=1,j;
    while((ch=fgetc(input))!=EOF)
    {
        if(i%128!=0)
          fputc(ch,output);
        i++;
    }
    
    fclose(input);
    fclose(output);
    
    //Generating the huffman tree from the packets in the start of the message
    
    FILE *fileReader= fopen("../bin/crcFreeMessage.txt", "r");
    FILE *fileWriter= fopen("../bin/decodedFile.txt", "w");  
    for (i=0; i<128; i++)
    {
      char code[8];
      for (j=0; j<8; j++)     
        code[j]=fgetc(fileReader);
      printf("code: %s\n", code);
      int codeLength= getCodeLength(code);
      printf("codeLength: %d\n", codeLength);
      Node* iteratorNode= root;
      for (j=0; j<codeLength; j++)
      {
        char ch=fgetc(fileReader);
        iteratorNode= constructHuffmanTree(iteratorNode, ch); 
      }
      iteratorNode->ch= (char)i;
    }    

    while(1){
    ch = fgetc(fileReader);
      if(ch == '1'){
        break;
      }
    }
    
    //Decoding the encoded message
    
    Node *tempRoot= root;
  	while((ch = fgetc(fileReader)) != EOF && ch!= '~')
  	{
		  if(tempRoot == NULL)
	  	{
		  	printf("errorInTransmission");
		  	exit(0);
		  }
		  
		  if(tempRoot->left == NULL && tempRoot->right == NULL)
		  { 
			  fputc(tempRoot->ch, fileWriter);	  
			  tempRoot = root;
		  }
		  
		  if(ch == '0')
		  	tempRoot = tempRoot->left;
  		else if(ch == '1')
			  tempRoot = tempRoot->right;
	}

	fclose(fileReader);
	fclose(fileWriter);

}

int main(){
  decoder();
}

