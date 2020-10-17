typedef struct dns_headers{
    unsigned id :16;
    
    #if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    unsigned rd :1;
    unsigned tc :1;
    unsigned aa :1;
    unsigned opcode :4;
    unsigned qr :1; // query(0) or response(1) bit 
    unsigned rcode :4;
    unsigned reserved :3;
    unsigned ra :1;
    #else // BIG_ENDIAN
    unsigned qr :1;
    unsigned opcode :4;
    unsigned aa :1;
    unsigned tc :1;
    unsigned rd :1;
    unsigned ra :1;
    unsigned reserved :3;
    unsigned rcode :4;    
    #endif

    unsigned qdcount :16;
    unsigned ancount :16;
    unsigned nscount :16;
    unsigned arcount :16;
}dns_header;

typedef struct dns_questions{
    int type :16;
    int qclass :16;
    
}dns_question;
