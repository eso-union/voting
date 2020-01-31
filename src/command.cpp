#include "command.h"

std::string
    Command::execute(const std::string &command)
{
    std::string output;
    const int BUFSIZE = 128;
    char buf[BUFSIZE];
    FILE *fp;

    if((fp = popen(command.c_str(), "r")) == NULL)
    {
        return("error opening pipe");
    }

    while(fgets(buf, BUFSIZE, fp) != NULL)
    {
        output+= buf;
    }

    if(pclose(fp))
    {
        return("error: command not found or exited with error status");
    }

    return output;
}
