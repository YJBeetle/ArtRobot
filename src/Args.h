#ifndef ARGS_H
#define ARGS_H

class Args
{
private:
    void args_help(const char *argv0);
    void args_usage(const char *argv0);

public:
    Args(int argc, char *argv[]);

    string output;
    string jsonfile;
};

#endif // ARGS_H
