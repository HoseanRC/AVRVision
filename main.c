#include <stdio.h>
#include <stdbool.h>
#include <argp.h>
#include <stdbool.h>

int logging = true;
const char *argp_program_version = "0.0.1";
const char *argp_program_bug_address = "hoseanrc@outlook.com";
static char doc[] = "setup simple avr project with out of the box libraries. compilable with GCC";
static char args_doc[] = "[COMMAND] ...";
static struct argp_option options[] = {
    {"directory", 'd', "directory", 0, "the project's root directory (defaults to the current directory)"},
    {"raw", 'r', 0, 0, "use raw output"},
    {0}};

struct arguments
{
    char *dir;
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
