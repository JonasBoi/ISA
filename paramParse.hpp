#include <string.h>
#include <string>
#include <stdio.h>
#include <vector>
#include <fstream>

/*
 * program arguments structure
 */
typedef struct prog_args {
	std::string server;
	int port = 53;
	std::string unwantedDomainFile;
} Prog_args;

/*
 * parse args
 * argc - arg count, arg - arguments, *prog_args - structer pointer to save flags in 
 * return 0 on success, 1 on failure
 */
int parse_args(int argc, char* argv[], Prog_args *prog_args);

/*
 * reads unwanted domains from file and returns them as a vector of strings
 */
std::vector<std::string> getUnwantedDomains(std::string fileName);

/*
 * checks if unwanted domains were parsed successfully
 */
int checkUnwanted (std::vector<std::string> *unwanted);
