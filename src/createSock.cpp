#include <iostream>
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <stdio.h>

#include <arpa/inet.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include "createSock.hpp"


/*
 * opens listening server socket 
 */
int setClientSock(int port) {

    int sockdesc; //socket descriptor
    struct sockaddr_in servaddr; //server address structure

    // create the server socket
    if ((sockdesc = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        std::cerr << "Socket se nepodarilo otevrit.\n";
        return EXIT_SOCK_FAILURE;
    }

    memset(&servaddr, 0, sizeof(servaddr)); 

    // set server properties
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(port);

    // bind the socket to the port
    if (bind(sockdesc, (const struct sockaddr *)&servaddr,  sizeof(servaddr)) < 0) 
    {
        std::cerr << "Socket bind se nepodarilo provest.\n";
        return EXIT_SOCK_FAILURE;
    }
    
    // return socker descriptor
    return sockdesc;
}

/*
 * create udp socket for communication with dns server
 */
int setResolverSock(std::string server, int port) {
    struct addrinfo hints;
    struct addrinfo *result;
    int dnsSock;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;

    if((getaddrinfo(server.c_str(), (std::to_string(port)).c_str(), &hints, &result)) != 0)
    {
        std::cerr << "Zadany DNS resolver se nepodarilo kontaktovat.\n";
        return EXIT_SOCK_FAILURE;
    }

    while(result != NULL)
    {
        if(result->ai_family == AF_INET || result->ai_family == AF_INET6)
        {
            if((dnsSock = socket(result->ai_family, SOCK_DGRAM, 0)) == -1)
            {
                std::cerr << "Socket pro komunikaci s dns se nepodarilo otevrit.\n";
                return EXIT_SOCK_FAILURE;
            }
            if((connect(dnsSock, result->ai_addr, result->ai_addrlen)) == -1)
            {
                std::cerr << "K dns serveru se nepodarilo pripojit.\n";
                return EXIT_SOCK_FAILURE;
            }

            return dnsSock;
        }
        result = result->ai_next;
    }
    std::cerr << "Zadany DNS resolver se nepodarilo kontaktovat.\n";
    return EXIT_SOCK_FAILURE;
}
