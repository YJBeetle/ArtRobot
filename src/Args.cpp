#include "default.h"
#include <libgen.h>
#include <getopt.h>

#include "Args.h"

void Args::args_help(const char *argv0)
{
    char *argv0_copy = strdup(argv0);
    char *argv0_base = basename(argv0_copy);

    printf("Usage: %s [OPTIONS] \n", argv0_base);
    printf("%s - \n", argv0_base);
    puts("");
    printf("  -f, --jsonfile\tjson file\n");
    printf("  -o, --output\toutput file\n");
    printf("  -t, --type\toutput type(pdf svg png)\n");
    puts("");
    printf("  --help\t\tGive this help list\n");
    printf("  -V, --version\t\tPrint program version\n");

    free(argv0_copy);
}

void Args::args_usage(const char *argv0)
{
    char *argv0_copy = strdup(argv0);
    char *argv0_base = basename(argv0_copy);

    printf("Usage: %s [OPTION] \n", argv0_base);
    printf("Try `%s --help' for more information.\n", argv0_base);

    free(argv0_copy);
}

Args::Args(int argc, char *argv[])
{
    static const char args_optstring[] = "f:o:t:V";
    static struct option args_options[] = {
        /* name,		has_arg,	flag,	val */
        {"jsonfile", 1, 0, 'f'},
        {"output", 1, 0, 'o'},
        {"type", 1, 0, 't'},
        {"version", 0, 0, 'V'},
        {0}};

    int c;

    while (1)
    {
        c = getopt_long(argc, argv, args_optstring, args_options, NULL);
        if (c == -1)
            break;

        //printf("|%c,%s|",c,optarg);

        switch (c)
        {
        case 'f':
            this->jsonfile = optarg;
            break;
        case 'o':
            this->output = optarg;
            break;
        case 't':
            if (!strcasecmp(optarg, "pdf"))
                type = OutputTypePdf;
            else if (!strcasecmp(optarg, "svg"))
                type = OutputTypeSvg;
            else if (!strcasecmp(optarg, "png"))
                type = OutputTypePng;
            break;
        case 'V':
            printf("version\n");
            exit(0);
            break;
        case '?':
            args_help(argv[0]);
            exit(1);
            break;
        default:
            fprintf(stderr, "Unhandled option: %d\n", c);
            exit(1);
            break;
        }
    }

    if (argc - optind >= 1)
    {
        this->jsonfile = argv[optind++];
        if (argc - optind >= 1)
        {
            this->output = argv[optind++];
            if (argc - optind > 0)
            {
                args_usage(argv[0]);
                exit(1);
            }
        }
    }

    if (this->jsonfile.empty())
    {
        args_usage(argv[0]);
        exit(1);
    }
}
