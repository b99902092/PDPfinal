#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
	char buffer[500];
	int listenfd, connfd, fd;
	socklen_t length;
	struct sockaddr_in serverAddress, clientAddress;
	char ip[INET_ADDRSTRLEN], *tmp;
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	bzero(&serverAddress, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(7788);
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	bind(listenfd, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
	listen(listenfd, 1);
	connfd = accept(listenfd, (struct sockaddr *) &clientAddress, &length);
	printf("connect success\n");
	while(1) 
	{
		printf("wait for board information\n");
		read(connfd, buffer, sizeof(buffer));
		fd = open("./input", O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR);
		write(fd, buffer, strlen(buffer));
		close(fd);
		printf("write file success\n");
		
		system("./main ./input > ./output");
		fd = open("./output", O_RDONLY);
		memset(buffer, 0, sizeof(buffer));
		read(fd, &buffer, sizeof(buffer));
		close(fd);
		
		printf("read file success\n");
		printf("%s\n", buffer);
		write(connfd, buffer, strlen(buffer));
	}
	close(connfd);
}
