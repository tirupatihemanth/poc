/*-------------------------------------------------------------------------
 *  Client.c - Client Program for sending messages to the server
 *  Version:	$Name:  $
 *  Module:	
 *
 *  Purpose:	
 *  See:	
 *
 *  Author:	Hema Murthy (hema@bhairavi.iitm.ernet.in)
 *
 *  Created:        Sat 17-Mar-2007 14:09:41
 *  Last modified:  Mon 26-Mar-2007 09:54:20 by hema
 *  $Id: Client.c,v 1.1 2007/03/26 04:26:09 hema Exp hema $
 *
 *  Bugs:	
 *
 *  Change Log:	<Date> <Author>
 *  		<Changes>
 -------------------------------------------------------------------------*/

/* 
 * Example of client using UDP protocol.
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "strings.h"
#include "string.h"
#include "include/DgFunctions.h"
#include <time.h>

#define PACKETSIZE 128
#define CODELENGTH 127
#define CRCLENGTH 1

/*-------------------------------------------------------------------------
 *  Usage -- Prints the usage for the program
 *    Args:	
 *    Returns:	Nothing
 *    Throws:	
 *    See:	
 *    Bugs:	
 -------------------------------------------------------------------------*/
void Usage () {
  printf ("Client ServerIPAddr ServerPort ClientIPAddress ClientPort\n");
}

char *computeCRC(char *);
char *induceError(char *);
/*-------------------------------------------------------------------------
 *  main -- Main Program for the Client - sends 10 lines of text to server
 *    Args:	Takes as arguments ServerIP, ServerPort, ClientIP, ClientPort
 *    Returns:	Nothing
 *    Throws:	
 *    See:	Unix Network Programming: Richard Stevens - Vol I
 *    Bugs:	
 -------------------------------------------------------------------------*/
 

int  main (int argc, char **argv)
{
    int                   sockFd;
    int                   serverPortNumber, clientPortNumber;
    struct sockaddr_in    clientAddr,serverAddr;
    char                  *sendMsg = "Testing UDP Protocol\n";

    int                   i;
    char                  tempString[256];

    srand(time(NULL));
    if (argc != 5) {
      Usage();
      exit(-1);
    }
    
     /* Fill in the structure "serverAddr " with the address of the 
     * server that we want to send to .
     */
    sscanf(argv[2],"%d", &serverPortNumber); 
    sscanf(argv[4],"%d", &clientPortNumber); 

    bzero(( char *) &serverAddr , sizeof (serverAddr));
    serverAddr.sin_family   = AF_INET;
    serverAddr.sin_addr.s_addr =inet_addr(argv[1]);
    serverAddr.sin_port  = htons(serverPortNumber);
  
  
    /* 
     * Open a UDP socket (an Internet datagram socket).
     */
     
    if (( sockFd = socket(AF_INET,SOCK_DGRAM ,0)) < 0) {
      printf("client : can't open datagram socket \n");
      exit(-1);
    }
    /* 
     * Bind any local address for us.
     */    

    bzero(( char *) &clientAddr , sizeof (clientAddr));
    clientAddr.sin_family   = AF_INET;
    clientAddr.sin_addr.s_addr =inet_addr(argv[3]);
    clientAddr.sin_port  = htons(clientPortNumber);
  
    if (bind (sockFd, (struct sockaddr *) &clientAddr ,sizeof(clientAddr)) < 0) {
      printf(" client: can't bind  local address\n");
    exit(-1);
    }
    printf ("Socket bound\n");
   
/*    for (i = 0; i < 10; i++) {
      tempString[0] = (char) (i+48);
      tempString[1] = ' ';
      tempString[2] = '\0';
      strcat(tempString, sendMsg);
      printf ("Message Sent = %s\n", sendMsg);
     DgClient(tempString, sockFd, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
    }*/

  char *crc;
  int j;
  char ch;
  char *str = (char *) malloc(sizeof(char)*(PACKETSIZE+1));

  FILE *encodedInput = fopen("codebook.txt","r");
   FILE* echoedInput= fopen("echoedMessage.txt", "w");
   fclose(echoedInput);
    i=0;
    while(1)
    {
      ch = fgetc(encodedInput);
      if(ch == EOF)
      {
        if(i==0)  
          break;
       // str[i++] = '1';
        for (; i<CODELENGTH-1; i++)
          str[i]='0';        
        str[i++] = '1';
      }
      else
      {
        str[i] = ch;
        i++;
      }
    
      if(i == CODELENGTH)
      {
        str[i] = '\0';
        crc = computeCRC(str);
        /*for(j=0;j<strlen(crc);j++)
        {
          str[i] = crc[j];
          i++;
        }
        str[i] = '\0';*/
        strcat(str,crc);
        DgClient(str, sockFd, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
        str = (char *) malloc(sizeof(char)*(PACKETSIZE+1));
        i=0;
      }
  }

  fclose(encodedInput);
  encodedInput = fopen("encodedFile.txt","r");
    i=0;
    while(1)
    {
      ch = fgetc(encodedInput);
      if(ch == EOF)
      {
        if(i==0)  
          break;
        str[i++] = EOF;
        for (; i<CODELENGTH; i++)
          str[i]='0';        
      }
      else
      {
        str[i] = ch;
        i++;
      }
    
      if(i == CODELENGTH)
      {
        str[i] = '\0';
        crc = computeCRC(str);
        /*for(j=0;j<strlen(crc);j++)
        {
          str[i] = crc[j];
          i++;
        }
        str[i] = '\0';*/
        strcat(str,crc);
        DgClient(str, sockFd, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
        str = (char *) malloc(sizeof(char)*(PACKETSIZE+1));
        i=0;
      }
  }
 
 
    close(sockFd);
    exit(0);
    return(0);
}	/*  End of main		End of main   */
      

/*
  Implementing an one bit crc
*/

char * computeCRC(char *str){

  int sum=0,i;
  char *crc = (char *) malloc(sizeof(char)*(CRCLENGTH+1));
  for(i=0;i<strlen(str);i++){
    if(str[i] == '1'){
      sum++;
    }
  }

  if(sum%2 == 0){
    strcpy(crc, "0");
    return crc;
  }
  else{
    crc = strcpy(crc, "1");
    return crc;
  }

}


/*-------------------------------------------------------------------------
 * $Log: Client.c,v $
 * Revision 1.1  2007/03/26 04:26:09  hema
 * Initial revision
 *
 *
 * Local Variables:
 * time-stamp-active: t
 * time-stamp-line-limit: 20
 * time-stamp-start: "Last modified:[ 	]+"
 * time-stamp-format: "%3a %02d-%3b-%:y %02H:%02M:%02S by %u"
 * time-stamp-end: "$"
 * End:
 *                        End of Client.c
 -------------------------------------------------------------------------*/
