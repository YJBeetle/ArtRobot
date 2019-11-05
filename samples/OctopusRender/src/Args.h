#ifndef ARGS_H
#define ARGS_H

#include <string>
#include <cstring>

using namespace std;

#include "ArtRobot/Types/OutputType.h"

using namespace ArtRobot;

class Args
{
private:
    void args_help(const char *argv0);
    void args_usage(const char *argv0);

public:
    Args(int argc, char *argv[]);

    string output;
    string jsonfile;
    OutputType type = OutputTypeSvg;
};

#endif // ARGS_H
