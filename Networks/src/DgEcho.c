/*-------------------------------------------------------------------------
 *  DgEcho.c - Reads a string from Client and writes it back to Client
 *  Version:	$Name:  $
 *  Module:	
 *
 *  Purpose:	
 *  See:	
 *
 *  Author:	Hema Murthy (hema@bhairavi.iitm.ernet.in)
 *
 *  Created:        Sat 17-Mar-2007 10:58:43
 *  Last modified:  Sat 17-Mar-2007 14:12:40 by hema
 *  $Id: DgEcho.c,v 1.1 2007/03/26 04:26:09 hema Exp hema $
 *
 *  Bugs:	
 *
 *  Change Log:	<Date> <Author>
 *  		<Changes>
 -------------------------------------------------------------------------*/

 
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "string.h"
#include "stdlib.h"

#define  MAXMESG 2048


/*-------------------------------------------------------------------------
 *  DgEcho -- Reads a packet from client and sends it back to client
 *    Args:	
 *    Returns:	Nothing
 *    Throws:	
 *    See:	UNIX Network Programming - Richard Stevens: Vol I
 *    Bugs:	
 -------------------------------------------------------------------------*/
 char * detectError(char *);
void DgEcho(int sockFd, struct sockaddr *pcliAddr, socklen_t  maxCliLen)
  {
    int       n;
    socklen_t cliLen;
  	char mesg[MAXMESG];
  	printf("entered dgecho\n");
  	for  ( ; ;)
	  {
  		cliLen = maxCliLen;
  		n = recvfrom(sockFd, mesg, MAXMESG, 0, pcliAddr, &cliLen);
		printf("received string of size %d string is:%s\n",n, mesg);

  		if (n < 0) {
		   printf("dg_echo : recvfrom error");
  		      // err_dump("dg_echo : recvfrom error");*/
  		   exit(-1);
		}	
    strcpy(mesg, detectError(mesg));

		if (sendto (sockFd, mesg, n, 0, pcliAddr, cliLen) != n) {
		   printf("dg_echo : sendto  error\n");
                   exit(-1);
		}
		printf("Message sent back to Client:%s\n", mesg); 
	  }
  }	/*  End of DgEcho		End of DgEcho   */

	
char * detectError(char *mesg){

      int i=0, sum=0;
      for(i=0;i<strlen(mesg);i++){
        if(mesg[i] == '1'){
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



/*-------------------------------------------------------------------------
 * $Log: DgEcho.c,v $
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
 *                        End of DgEcho.c
 -------------------------------------------------------------------------*/
