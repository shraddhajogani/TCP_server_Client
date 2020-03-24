/*
    Live Server on port 8888
*/
#include<io.h>
#include<stdio.h>
#include<winsock2.h>
#include <string.h>
#pragma comment(lib,"ws2_32.lib") //Winsock Library

#define CHAT
//#define FILE_TRANSFER

// Function designed for chat between client and server.
void func(int new_socket)
{
     char buf[100];
     int k;

     while(1)
    {
        k = recv(new_socket,buf,100,0);
        if(k == -1)
        {
            printf("Error in receiving");
            exit(1);
        }

        printf("\nFrom client: %s", buf);
        fflush(stdout);
        printf("To client: ");
        fflush(stdout);
        fgets(buf,100,stdin);

        if(strncmp(buf,"end", 3) == 0)
        break;

        k = send(new_socket,buf,100,0);

        if(k == -1)
        {
            printf("Error in sending");
            exit(1);
        }
    }
}
int main(int argc , char *argv[])
{
    WSADATA wsa;
    SOCKET s , new_socket;
    struct sockaddr_in server , client;
    int c, k;
    char *message;
    memset(&server,0,sizeof(server));
    memset(&client,0,sizeof(client));
    //char M[50];
    char buf[100];
    int count = 0;

    printf("\nInitialising Winsock...and b4 WSAStartup\n");

    fflush(stdout);

    memset((char *) &wsa, 0, sizeof(wsa));

    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0) {
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

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    //server.sin_addr.s_addr = INADDR_ANY;
    server.sin_addr.s_addr = inet_addr("192.168.178.54");
    server.sin_port = htons( 8888 );

    //Bind
    if( bind(s ,(struct sockaddr *)&server , sizeof(server)) == SOCKET_ERROR)
    {
        printf("Bind failed with error code : %d" , WSAGetLastError());
        exit(EXIT_FAILURE);
    }

    puts("Bind done");
    fflush(stdout);
    //Listen to incoming connections
    listen(s , 3);

    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    fflush(stdout);

    c = sizeof(client);

   // while( (new_socket = accept(s , (struct sockaddr *)&client, &c)) != INVALID_SOCKET )
    //{
    new_socket = accept(s , (struct sockaddr *)&client, &c);

        puts("Connection accepted");
        fflush(stdout);

        //Reply to the client
        //count++;
        //message = "Hello Client , I have received your connection. But I have to go now, from A-Driver. bye\n";


       /*	message = (char *)  malloc (200);
        sprintf(message, "%s",
                "Hello Client , I have received your connection.");

        send(new_socket , message , strlen(message) , 0);
    //}

    if (new_socket == INVALID_SOCKET)
    {
        printf("accept failed with error code : %d" , WSAGetLastError());
        fflush(stdout);
        return 1;
    }*/

#ifdef CHAT

    // Function for chatting between client and server
    func(new_socket);

#endif // CHAT

   /* // file receive

    FILE *fp;

    int ch = 0;
    fp = fopen("file_received.txt", "a");

    char buffer[255];
    int words;

    read(new_socket, &words, sizeof(int));

    while(ch != words)
    {
        read(new_socket, buffer, 255);
        fprintf(fp, "%s ", buffer);
        fflush(stdout);
        ch++;
    }

    printf("\nThe file has been received successfully. The file name is file_received.txt");*/

#ifdef FILE_TRANSFER

    /*Receive File from Client */
        char revbuf[100]; // Receive buffer
		char* fr_name = "file1_1.txt";
		FILE *fr = fopen(fr_name, "a");
		if(fr == NULL)
			printf("File %s Cannot be opened file on server.\n", fr_name);
		else
		{
			//bzero(revbuf, LENGTH);
			memset(&revbuf, 0, sizeof(revbuf));

			int fr_block_sz = 0;
			//int success = 0;
			//while(success == 0)
			//{
			    while(fr_block_sz = recv(new_socket, revbuf, 100, 0)) //could it be sockfd?
			    {
			        if(fr_block_sz < 0)
			        {
			            printf("Error receiving file from client to server.\n");
			        }
			        int write_sz = fwrite(revbuf, sizeof(char), fr_block_sz, fr);
			        if(write_sz < fr_block_sz)
			        {
			            printf("File write failed on server.\n");
			        }
					else if(fr_block_sz)
					{
						break;
					}
			        //bzero(revbuf, LENGTH);
			        memset(&revbuf, 0, sizeof(revbuf));
			    }
			    printf("\nOk.. received from client!\n");
			    fclose(fr);
			//}
		}

		/* Call the Script */
		//system("cd ; chmod +x script.sh ; ./script.sh");

		/* Send File to Client */
	    //if(!fork())
	    //{
	        char* fs_name = "file2.txt";
	        char sdbuf[100]; // Send buffer
	        printf("\nSending %s to the Client...", fs_name);
	        FILE *fs = fopen(fs_name, "r");
	        if(fs == NULL)
	        {
	            printf("ERROR: File %s not found on server.\n", fs_name);
				exit(1);
	        }

	       // bzero(sdbuf, LENGTH);
            memset(&sdbuf, 0, sizeof(sdbuf));
	        int fs_block_sz;
	        while((fs_block_sz = fread(sdbuf, sizeof(char), 100, fs))>0)
	        {
	            if(send(new_socket, sdbuf, fs_block_sz, 0) < 0)
	            {
	                printf("ERROR: Failed to send file %s.\n", fs_name);
	                exit(1);
	            }
	            //bzero(sdbuf, LENGTH);
	            memset(&sdbuf, 0, sizeof(sdbuf));
	        }
	        printf("Ok sent to client!\n");


	    //}
#endif // FILE_TRANSFER


    closesocket(new_socket);
    WSACleanup();

    return 0;
}
