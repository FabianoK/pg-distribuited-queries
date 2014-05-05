#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netdb.h>
#include "net.h"
#include <unistd.h>

using namespace std;

int Net::sockConnect(){
	struct addrinfo host_info;
	struct addrinfo *host_info_list;
	int st;

	memset(&host_info, 0, sizeof host_info);
	host_info.ai_family = AF_UNSPEC;
	host_info.ai_socktype = SOCK_STREAM;

	st = getaddrinfo("localhost", "80", &host_info, &host_info_list);

	sockFD = socket(host_info_list->ai_family, 
                    host_info_list->ai_socktype, 
                    host_info_list->ai_protocol);

	st = connect(sockFD, host_info_list->ai_addr, host_info_list->ai_addrlen);

	return st;
}


int Net::sendMessage(char *msg){
	//msg = "GET / HTTP/1.1\nhost: www.google.com\n\n";
	int len;
	ssize_t bytes_sent;
	len = strlen(msg);
	bytes_sent = send(sockFD, msg, len, 0);
	return bytes_sent;
}

int Net::recMessage(){
	ssize_t bytes_recieved;
	char incoming_data_buffer[1000];
	bytes_recieved = recv(sockFD, incoming_data_buffer,1000, 0);
	if (bytes_recieved == 0) std::cout << "Sem dados de resposta." << endl ;
	if (bytes_recieved == -1)std::cout << "Erro no recebimento!" << endl ;
	cout << bytes_recieved << " bytes recieved :" << endl ;
	cout << incoming_data_buffer << endl;
	close(sockFD);
	return bytes_recieved;
}
