#include "helper.h"

static int read_line(char *buf, size_t n)
{
    if (!fgets(buf, (int)n, stdin))
        return 0;

    size_t len = strlen(buf);
    if (len && buf[len - 1] == '\n')
        buf[len - 1] = '\0';
    return 1;
}

static int read_int(const char *prompt, int *out)
{
    char line[128];
    for (;;)
    {
        printf("%s", prompt);
        if (!read_line(line, sizeof line))
            return 0;
        char *end = NULL;
        long v = strtol(line, &end, 10);
        if (end != line && *end == '\0')
        {
            *out = (int)v;
            return 1;
        }
        printf("Invalid integer. Try again.\n");
    }
}

static int read_float(const char *prompt, float *out)
{
    char line[128];
    for (;;)
    {
        printf("%s", prompt);
        if (!read_line(line, sizeof line))
            return 0;
        char *end = NULL;
        float v = strtof(line, &end);
        if (end != line && *end == '\0')
        {
            *out = v;
            return 1;
        }
        printf("Invalid number. Try again.\n");
    }
}

int fpeek(FILE *f)
{
    int c = fgetc(f);
    if (c != EOF)
        ungetc(c, f);
    return c;
}