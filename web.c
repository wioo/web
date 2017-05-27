#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define P_LENGHT 6
#define ADDR_LENGHT 15



// Functions
int set_server(char *  port, int status, struct addrinfo hints, struct addrinfo * servinfo);
int set_client(char * address, char * port, int status, struct addrinfo hints, struct addrinfo * servinfo);
int show_ip(int argc, char * first_arg);



// Structs
struct server_args{
	char port[P_LENGHT];
	char *port_p;
};


struct client_args{
	char address[ADDR_LENGHT];
	char port[P_LENGHT];
	
	char *address_p;
	char *port_p;
};

char *  usage(int i){
	static 	char  usage_s[] = "Usage: web \"argument\"\n"
				"Arguments:	c	client\n"
		"		s	server\n";
	static  char  error_s[] = "First argument must be \"-c\" or \"-s\".\n";	
	
	if(i == 1) {
		return usage_s;
	}	
	else {
		return error_s;
	}
}


int parse_cmd(int argc, char *argv[]){

	if(argc == 1){
	printf("%s",usage(1));
		exit(1);
	}

	if(argv[1][0] != '-')
	{
		printf("%s", usage(1));
		exit(1);
	}
	
	if(argv[1][1] != 'c' && argv[1][1] != 's')
	{
		printf("%s\n", usage(2));
		exit(1);
	}
return 0;
}



int main(int argc, char *argv[])
{
	
	parse_cmd(argc, argv);

	int status = 0;
	struct addrinfo hints;
	struct addrinfo *servinfo;

//	char arg_n1 = (char)argv[1];

//implement string comparision function for parsing cmd command

	switch (argv[1][0]) {
		case 'c':
			printf("character c entered\n");
			break;
	}

	
	struct client_args new_client_args;
	new_client_args.address_p = new_client_args.address;
	new_client_args.port_p = new_client_args.port;

//TODO: read adress and port from command line

	new_client_args.address_p = (char *)"172.0.0.1";
	new_client_args.port_p =(char *) "3490";

	set_client(new_client_args.address_p, new_client_args.port_p, status, hints, servinfo);	

	// Setup a server
	
	struct server_args new_server_args;
	new_server_args.port_p = new_server_args.port;

// TODO: read port number from command line ... argv[]...
	new_server_args.port_p = (char *)"3490";

	set_server(new_server_args.port_p, status, hints, servinfo);


	// Setup a client
//	
//	struct client_args new_client_args;
//	new_client_args.address_p = new_client_args.address;
//	new_client_args.port_p = new_client_args.port;
//	yauk-tstp-yhcj-fewt
//TODO: read adress and port from command line

//	new_client_args.address_p = "172.0.0.1";
//	new_client_args.port_p = "3490";
//
//	set_client(new_client_args.address_p, new_client_args.port_p, status, hints, servinfo);	
	

/* Show IP

	int arg_num = argc;	
	char *first_arg = argv[1];
	show_ip(arg_num, first_arg);
*/


	return 0;
}




int set_server(char * port, int status, struct addrinfo hints, struct addrinfo * servinfo)
{
	struct sockaddr_storage their_addr;
	socklen_t addr_size;
	int backlog = 10;
	int sockfd;
	int new_fd;
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype= SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	if ((status = getaddrinfo(NULL, port, &hints, &servinfo)) != 0){
		fprintf(stderr, "getddrinfo errot: %s\n", gai_strerror(status));
		exit(1);
	}
	
//	printf("test\n");	
	sockfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
	printf("socket descriptor %d\n", sockfd);

	bind(sockfd, servinfo->ai_addr, servinfo->ai_addrlen);
	listen(sockfd, backlog);
	addr_size = sizeof their_addr;
	new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &addr_size);
	printf("new socket destriptor %d\n", new_fd);

	//	printf("new connection from %d\n", their_addr.);


	freeaddrinfo(servinfo);

	return 0;
}

int set_client(char * address, char * port, int status, struct addrinfo hints, struct addrinfo * servinfo)
{
	int sockfd;
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if ((status = getaddrinfo(address, port, &hints, &servinfo)) != 0){
                fprintf(stderr, "getddrinfo errot: %s\n", gai_strerror(status));
                exit(1);
        }
	
	sockfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);

	printf("socket number is %d\n", sockfd);

	connect(sockfd, servinfo->ai_addr, servinfo->ai_addrlen); 

	freeaddrinfo(servinfo);
	
	return 0;
}

int show_ip(int argc, char *first_arg)
{

    struct addrinfo hints, *res, *p;
    int status;
    char ipstr[INET6_ADDRSTRLEN];

    if (argc != 2) {
        fprintf(stderr,"usage: showip hostname\n");
        return 1;
    }

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // AF_INET or AF_INET6 to force version
    hints.ai_socktype = SOCK_STREAM;

    if ((status = getaddrinfo(first_arg, NULL, &hints, &res)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return 2;
    }

    printf("IP addresses for %s:\n\n", first_arg);

    for(p = res;p != NULL; p = p->ai_next) {
        void *addr;
        char *ipver;

        // get the pointer to the address itself,
        // different fields in IPv4 and IPv6:
        if (p->ai_family == AF_INET) { // IPv4
            struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
            addr = &(ipv4->sin_addr);
            ipver = (char *)"IPv4";
        } else { // IPv6
            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
            addr = &(ipv6->sin6_addr);
            ipver = (char *)"IPv6";
        }

        // convert the IP to a string and print it:
        inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
        printf("  %s: %s\n", ipver, ipstr);
    }

    freeaddrinfo(res); // free the linked list
	return 0;
}
