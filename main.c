#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <argp.h>
#include <stdbool.h>
#include <string.h>
#ifdef WIN32
#include <io.h>
#define F_OK 0
#define access _access
#else
#include <unistd.h>
#endif

#include "filenames.h"

#define CHECK_HELP "\nTry `a.out --help' or `a.out --usage' for more information."

int logging = true;
const char *argp_program_version = "0.0.1";
const char *argp_program_bug_address = "hoseanrc@outlook.com";
static char doc[] = "setup simple AVR project with out of the box libraries. compilable with GCC";
static char args_doc[] = "[COMMAND] ...";
static struct argp_option options[] = {
    {"directory", 'd', "directory", 0, "the project's root directory (defaults to the current directory)"},
    {"raw", 'r', 0, 0, "use raw output"},
    {"target", 't', "target", 0, "microcontroller target (use \"list\" for all possible targets)"},
    {"frequency", 'f', "frequency", 0, "frequency of the microcontroller in Hz"},
    {"skip-config", 's', 0, 0, "skip config file"},
    {0}};

struct arguments
{
    char *dir;
    char *target;
    long frequency;
    bool skip_config;
    bool raw;
};

static error_t parse_opt(int key, char *arg, struct argp_state *state)
{
    struct arguments *arguments = state->input;
    switch (key)
    {
    case 'd':
        arguments->dir = arg;
        break;
    case 'r':
        arguments->raw = true;
        break;
    case 't':
        arguments->target = arg;
        break;
    case 's':
        arguments->skip_config = true;
        break;
    case 'f':
        char *end;
        arguments->frequency = strtol(arg, &end, 10);
        if (*end)
        {
            argp_failure(state, 1, 0, "frequency have to be a number." CHECK_HELP);
            exit(ARGP_ERR_UNKNOWN);
        }
        break;
    case ARGP_KEY_END:
        bool config_exists = false;
        if (!arguments->skip_config)
        {
            if (arguments->dir != NULL)
            {
                char *config_path;
                config_path = malloc(strlen(arguments->dir) + sizeof("/" PROJECT_DIR "/" CONFIG_FILE));
                strcpy(config_path, arguments->dir);
                strcat(config_path, "/" PROJECT_DIR "/" CONFIG_FILE);
                config_exists = access(config_path, F_OK) == 0;
            }
            else if (access("./" PROJECT_DIR "/" CONFIG_FILE, F_OK) == 0)
                config_exists = true;
        }
        chdir(arguments->dir);
        if(config_exists){
            char *pwd = getcwd(NULL, 0);
            printf("config file found.\nusing config file at: %s" PATH_SLASH PROJECT_DIR PATH_SLASH CONFIG_FILE "\n", pwd);
            free(pwd);
        }
        if (!config_exists && (arguments->frequency == NULL || arguments->target == NULL)) // TODO: fix frequency for chips that dont have frequency setting
        {
            argp_failure(state, 1, 0, "required frequency and target." CHECK_HELP);
            exit(ARGP_ERR_UNKNOWN);
        }
    case ARGP_KEY_ARG:
        return 0;
    default:
        return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

static struct argp argp = {options, parse_opt, args_doc, doc, 0, 0, 0};

int printLog(const char *__restrict__ text, ...)
{
    if (logging)
    {
        printf(text);
    }
}

int main(int argc, char **argv)
{
    struct arguments arguments;

    arguments.raw = false;
    arguments.dir = ".";

    argp_parse(&argp, argc, argv, 0, 0, &arguments);

    logging = !arguments.raw;
}
