#include <string>
#include <vector>

/*
 * checks dns packet
 */
int parseDnsPacket(char *buffer, std::vector<std::string> unwanted);

/*
 * parse domain request name since its lenght is not fixed
 */
char* parse_domain(char *questionStart, std::string &domain);

/*
 * check if domain in the dns question isnt one of the unwanted domains
 */
int check_domain(std::string domain, std::vector<std::string> unwanted);

/*
 * sets the header of packet to err and sends it to client
 */
void setAnswerErr(char *buffer, int rcode);
