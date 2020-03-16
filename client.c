/*
   Create a TCP socket
   */
#include<stdio.h>
#include <stdlib.h>
#include<winsock2.h>
#pragma comment(lib,"ws2_32.lib") //Winsock Library
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <ctype.h>
#include<io.h>

#define CHAT
//#define FILE_TRANSFER

// Function designed for chat between client and server.
void func(int s)
{
    char buf[100];
    int k;

    while(1)
    {
        printf("\nTo server: ");
        fflush(stdout);
        fgets(buf, 100, stdin);

        if(strncmp( buf, "end", 3) == 0)//Use “end” to end communication with server
        break;

        k = send(s, buf, 100, 0);
        if(k == -1)
        {
            printf("Error in sending");
            exit(1);
        }

        k = recv(s, buf, 100, 0);
        if(k == -1)
        {
            printf("Error in receiving");
            exit(1);
        }
            printf("From server: %s",buf);
            fflush(stdout);
    }

}

int main(int argc , char *argv[])
{
  WSADATA wsa;
  SOCKET s;
  struct sockaddr_in client;
  char *message , server_reply[2000];
  char buf[100];
  memset(&client, 0, sizeof(client));
  int recv_size, k;

  printf("\nInitialising Winsock...");
  if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
  {
    printf("Failed. Error Code : %d",WSAGetLastError());
    return 1;
  }

  printf("Initialised.\n");
  fflush(stdout);

  //Create a socket
  if((s = socket(AF_INET , SOCK_STREAM , 0 )) == INVALID_SOCKET)
  {
    printf("Could not create socket : %d" , WSAGetLastError());
  }
  printf("Socket created.\n");
  fflush(stdout);


  //server.sin_addr.s_addr = inet_addr("74.125.235.20");
  client.sin_addr.s_addr = inet_addr("127.0.0.1");
  client.sin_family = AF_INET;
  //client.sin_port = htons( 80 );
  client.sin_port = htons( 8888 );
  //Connect to remote server
  if (connect(s , (struct sockaddr *)&client , sizeof(client)) < 0)
  {
    puts("connect error");
    fflush(stdout);
    return 1;
  }

  puts("Connected. Ready to send data.");
  fflush(stdout);


/* //Send some data
  message = "GET / HTTP/1.1\r\n\r\n";
  if( send(s , message , strlen(message) , 0) < 0)
  {
    puts("Send failed");
    fflush(stdout);
    return 1;
  }
  puts("Data Send\n");
  fflush(stdout);

  //Receive a reply from the server
  if((recv_size = recv(s , server_reply , 2000 , 0)) == SOCKET_ERROR)
  {
    puts("recv failed");
    fflush(stdout);
  }

  puts("Reply received\n");
  fflush(stdout);
  //Add a NULL terminating character to make it a proper string before printing
  server_reply[recv_size] = '\0';
  puts(server_reply);
  fflush(stdout);*/

#ifdef CHAT
  // function for chat
   func(s);

#endif // CHAT


    // file transfer

   /*FILE *f;
   int words = 0;

   char c;
   char sdbuff[255];

   f = fopen("file.txt", "r");

   while(c = getc(f) != EOF)
   {
       fflush(stdin);
       fscanf(f, "%s", buffer);
       //fflush(stdout);
       if(isspace(c) || c == '\t');
       words++;
   }

   write(s, &words, sizeof(int));
   rewind(f);

   char ch;
   while(ch != EOF)
   {

       fscanf(f, "%s", buffer);
       write(s, buffer, 255);
       ch = fgetc(f);
   }
   printf("\nfile has been successfully sent.");*/

#ifdef FILE_TRANSFER

   /* Send File to Server */
	//if(!fork())
	//{
		char* fs_name = "file1.txt";
		char sdbuf[100];
		printf("\nSending %s to the Server...", fs_name);
		FILE *fs = fopen(fs_name, "r");
		if(fs == NULL)
		{
		    printf("ERROR: File %s not found.\n", fs_name);
			exit(1);
		}

		//bzero(sdbuf, LENGTH);
        memset(&sdbuf, 0, sizeof(sdbuf));
		int fs_block_sz;
		//int success = 0;
		while((fs_block_sz = fread(sdbuf, sizeof(char), 100, fs))>0)
	    {
	        if(send(s, sdbuf, fs_block_sz, 0) < 0)
	        {
	            printf("ERROR: Failed to send file %s.\n", fs_name);
	            break;
	        }
	        //bzero(sdbuf, LENGTH);
	        memset(&sdbuf, 0, sizeof(sdbuf));
	    }
	    printf("Ok File %s from Client was Sent!\n", fs_name);
	    //success = 1;
	//}

	/* Receive File from Server */

	printf("\nReceiveing file from Server...");
	char* fr_name = "file2_2.txt";
	char revbuf[100]; // Receive buffer

	FILE *fr = fopen(fr_name, "a");
	if(fr == NULL)
		printf("File %s Cannot be opened.\n", fr_name);
	else
	{
	    //bzero(revbuf, LENGTH);
	    memset(&revbuf, 0, sizeof(revbuf));
	    int fr_block_sz = 0;
	    int success = 0;
	    //while(success == 0)
	    //{
	        while(fr_block_sz = recv(s, revbuf, 100, 0))
	        {
	            if(fr_block_sz < 0)
	            {
	                printf("Receive file error.\n");
	            }
	            int write_sz = fwrite(revbuf, sizeof(char), fr_block_sz, fr);
	            if(write_sz < fr_block_sz)
	            {
	                printf("File write failed.\n");
	            }
				else if(fr_block_sz)
				{
					break;
				}

	            //bzero(revbuf, LENGTH);
	            memset(&revbuf, 0, sizeof(revbuf));
	        }
	        printf("Ok received from server!\n");
	        success = 1;
	        fclose(fr);
	    //}
	}
#endif // FILE_TRANSFER



  closesocket(s);
  WSACleanup();
  return 0;
}
