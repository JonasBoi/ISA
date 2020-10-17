#include <string>
#include <iostream>
#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
#include <vector>

#include <arpa/inet.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include "paramParse.hpp"
#include "createSock.hpp"
#include "parseDns.hpp"
#include "dnsPacketStruct.hpp"

#define MAX_DNS_SIZE 1024
#define DNS_PORT 53

/*
 * recieves requests from clients that are further being processed
 */
void startServer(int clientsockdesc, int dnssockdesc, std::vector<std::string> unwanted) {

    struct sockaddr_in cliaddr;
    char buffer [MAX_DNS_SIZE];
    int n;
    socklen_t len = sizeof(cliaddr);

    memset(&cliaddr, 0, sizeof(cliaddr)); 

    // loop to recieve queries from clients, gradually serve the incoming queries
    while(true) {
        // recieve a message
        n = recvfrom(clientsockdesc, (char*)&buffer, MAX_DNS_SIZE,  MSG_WAITALL, (struct sockaddr *) &cliaddr, &len); 
        
        // parse DNS packet, check for unwanted domains and question type
        int rcode = parseDnsPacket(buffer, unwanted);

        // if any problem occurs, send the packet back to client
        if(rcode != 0) {
            setAnswerErr(buffer, rcode);

            //test
            /*
            dns_header *header = (dns_header *)buffer;
            header->qr = ntohs(header->qr);
            std::cerr << header->qr << "\n";
            header->qr = htons(header->qr);
            */

            sendto(clientsockdesc, (const char *)buffer, n, MSG_CONFIRM, (const struct sockaddr *) &cliaddr, sizeof(cliaddr));
            continue;
        }  

        // send query to resolver
        send(dnssockdesc, buffer, n, 0);

        // recieve an answer from resolver
        int recieved = 0;
        if((recieved = recv(dnssockdesc, buffer, MAX_DNS_SIZE, 0)) < 0)
        {
            std::cerr << "An error has occured.\n";
            return;
        }

        // send answer to client
        //sendto(clientsockdesc, (const char *)buffer, recieved, MSG_CONFIRM, (const struct sockaddr *) &cliaddr, sizeof(cliaddr));
        send(clientsockdesc, buffer, recieved, 0);
    }
}

//////////////////////////////////////////////////
int main(int argc, char* argv[]) {

    // initialize struct for storing args
    Prog_args* prog_args = new Prog_args;

    // parse arguments
	int err_code = parse_args(argc, argv, prog_args);
    if (err_code != 0) {
        return EXIT_SUCCESS;
    }

    // create socket for client communication
    int clisockdesc = setClientSock(prog_args->port);
    if (clisockdesc < 0) {
        return EXIT_FAILURE;
    }

    // create socket for communication with dns resolver
    int dnssockdesc = setResolverSock(prog_args->server, DNS_PORT);
    if (dnssockdesc < 0) {
        return EXIT_FAILURE;
    }

    // get unwanted domains from file
    std::vector<std::string> unwanted = getUnwantedDomains(prog_args->unwantedDomainFile);
    if (checkUnwanted(&unwanted) != 0) {
        return EXIT_SUCCESS;
    }

    // start server with given opts
    startServer(clisockdesc, dnssockdesc, unwanted);

    free(prog_args);
    return EXIT_SUCCESS;
}
