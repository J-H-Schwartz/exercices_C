/*
*****************************************************************************

  File        : main.cpp

  The MIT License (MIT)
  Copyright (c) 2019 STMicroelectronics

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.

*****************************************************************************
*/

#include<stdio.h>
#include<string.h>	//strlen
#include<sys/socket.h>
#include<arpa/inet.h>	//inet_addr
#include<unistd.h>
#include"debug.h"
#include<errno.h>

int main(int argc , char *argv[])
{
	while (1) {
		int socket_desc;
		struct sockaddr_in server;
		char *message1;
		char *message2;
		char *message;

		//Create socket
		socket_desc = socket(AF_INET , SOCK_STREAM , 0);
		if (socket_desc == -1)
		{
			error_printf("Could not create socket. Error = %d (%s)\n", errno, strerror(errno));

			continue;
		}
		int connexion_tries = 10;
		while (socket_desc != -1){
			server.sin_addr.s_addr = inet_addr("127.0.0.1");
			server.sin_family = AF_INET;
			server.sin_port = htons( 5001 );
			int connexion_status = connect(socket_desc , (struct sockaddr *)&server , sizeof(server));
			//Connect to remote server
			if (connexion_status != 0)
			{
				//puts("connect error");
				error_printf("Connection failed. Error = %d (%s)\n", errno, strerror(errno));
				connexion_tries -= 1;
				if (connexion_tries < 0) socket_desc = -1;
				sleep(2);
				continue;
			}

			puts("Connected\n");
			message1 = "Sweet !\n";
			message2 = "Dude !\n";
			message = message2;
			//Send some data
			int message_sending_tries = 10;
			while (connexion_status == 0){
				if (message == message1){
					message = message2;
				}
				else message = message1;
				if( send(socket_desc , message , strlen(message) , 0) < 0)
				{
					error_printf("Message sending failed. Error = %d (%s)\n", errno, strerror(errno));
					sleep(2);
					message_sending_tries -= 1;
					if (message_sending_tries < 0) connexion_status = -1;
					continue;
				}
				puts("Data Send\n");
				sleep(2);
			}
		}
	}
	return 0;
}
