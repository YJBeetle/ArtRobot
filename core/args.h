#ifndef ARGS_H
#define ARGS_H


class args
{
private:
    char *output;
    char *jsonfile;
    char *jsondata;

    void args_help(const char *argv0);
    void args_usage(const char *argv0);

public:
    args();
    int args_parse(int argc, char *argv[]);
    char *jsondata_s();
    char *output_s();
};

#endif // ARGS_H
