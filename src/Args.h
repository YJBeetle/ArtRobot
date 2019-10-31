#ifndef ARGS_H
#define ARGS_H


class Args
{
private:
    char *output;
    char *jsonfile;
    char *jsondata;

    void args_help(const char *argv0);
    void args_usage(const char *argv0);

public:
    Args();
    int args_parse(int argc, char *argv[]);
    char *jsondata_s(){return jsondata;}
    char *output_s(){return output;}
};

#endif // ARGS_H
