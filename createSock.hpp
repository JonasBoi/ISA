#include <string>

#define EXIT_SOCK_FAILURE -1

/*
 * opens listening server socket 
 */
int setClientSock(int port);

/*
 * create udp socket for communication with dns server
 */
int setResolverSock(std::string server, int port);
