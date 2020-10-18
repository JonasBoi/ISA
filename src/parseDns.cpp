#include <iostream>
#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
#include <string.h>

#include <netdb.h>

#include "parseDns.hpp"
#include "dnsPacketStruct.hpp"

#define A 1
#define NOTIMP 4
#define REFUSED 5

char* parse_domain(char *questionStart, std::string &domain) {
    // empty string that means root of dns tree
    if (*questionStart == 0)
    {
        domain = "<root>";
    }

    // parse the labels of domain name
    while(*questionStart != 0) {

        int8_t labelValue = *((int8_t *)(questionStart));

        for(int8_t i = 0; i < labelValue; i++)
        {
            domain += questionStart[i + 1];
        }

        questionStart = questionStart + labelValue + 1;
        domain += ".";
    }

    // remove the last dot
    domain.pop_back();

    return questionStart + 1;
}

int check_domain(std::string domain, std::vector<std::string> unwanted) {

    int size = unwanted.size();
    std::string subdomain;

    //check for every unwanted
    for (int i = 0; i < size; i++) {
        //check for domain
        if(domain == unwanted[i]) {
            // std::cerr << "Nezadouci domena \"" << domain << "\".\n";
            return EXIT_FAILURE;
        }

        //check for subdomain
        subdomain = "." + unwanted[i];
        if(domain.find(subdomain) != std::string::npos) {
            // std::cerr << "Nezadouci domena \"" << domain << "\".\n";
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}

int parseDnsPacket(char *buffer, std::vector<std::string> unwanted) {
    // typecast header
    dns_header *header = (dns_header *)buffer;

    // check qr
    header->qr = ntohs(header->qr);
    if (header->qr != 0) {
        return REFUSED;
    }
    header->qr = htons(header->qr);
    
    // get the pointer *question that points at the beggining of question section
    char *question = ((char *)header) + sizeof(dns_header);

    // the fixed part of question section
    dns_question *dnsQTypeClass = NULL;

    header->qdcount = ntohs(header->qdcount);
    // iterate over question sections
    for(int i = 0; i < header->qdcount; i++) {
        // get domain and typecast the rest of fixed q section to qeustion struct
        std::string domain;
        dnsQTypeClass = (dns_question*)(parse_domain(question, domain));

        // check if the domain and its subdomains aren't unwanted
        if (check_domain(domain, unwanted) != 0) {
            //set correct byte order
            header->qdcount = htons(header->qdcount);
            return REFUSED;
        }

        // check for question type
        dnsQTypeClass->type = ntohs(dnsQTypeClass->type);

        if (dnsQTypeClass->type != A) {
            //set correct byte order
            header->qdcount = htons(header->qdcount);
            dnsQTypeClass->type = htons(dnsQTypeClass->type);
            return NOTIMP;
        }
        //set correct byte order
        dnsQTypeClass->type = htons(dnsQTypeClass->type);
    }

    //set correct byte order
    header->qdcount = htons(header->qdcount);

    return EXIT_SUCCESS;
}

void setAnswerErr(char *buffer, int rcode) {
    // typecast header
    dns_header *header = (dns_header *)buffer;

    // set header
    header->qr = 1; 
    header->rcode = rcode; 
}
