#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include "sensor.h"

#define MAX_INPUT 100

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

int main()
{
    print_current_directory();
    SensorData *data = NULL;
    int count = 0;
    int capacity = 0;

    int running = 1;
    while (running)
    {
        printf("\n--- Sensor Analyzer Menu ---\n");
        printf("1. Add new sensor\n");
        printf("2. View all sensors\n");
        printf("3. Save sensors to file (binary)\n");
        printf("4. Load sensors from file (binary)\n");
        printf("5. Exit\n");
        printf("6. Show averages\n");

        int choice;
        if (!read_int("Choose an option: ", &choice))
        {
            printf("Input error.\n");
            break;
        }

        switch (choice)
        {
        case 1:
            if (count == capacity)
            {
                capacity = (capacity == 0) ? 2 : capacity * 2;
                data = realloc(data, capacity * sizeof(SensorData));
                if (data == NULL)
                {
                    printf("Memory allocation failed.\n");
                    return 1;
                }
            }
            if (!read_float("  Enter temperature: ", &data[count].temperature))
                break;
            if (!read_float("  Enter humidity: ", &data[count].humidity))
                break;
            update_flags(&data[count]);
            count++;
            break; // ✅ Add this

        case 2:
            if (count == 0)
            {
                printf("No sensors logged.\n");
            }
            else
            {
                for (int i = 0; i < count; i++)
                {
                    print_sensor(data[i], i);
                }
            }
            break;

        case 3:
            printf("Saving to sensor_data.txt...\n");
            save_to_file("sensor_data.txt", data, count);
            printf("Saved.\n");
            break;

        case 4:
            printf("Trying to load from sensor_data.txt...\n");
            load_from_file("sensor_data.txt", &data, &count, &capacity);
            printf("Load completed.\n");
            break;

        case 5:
            running = 0;
            break;

        default:
            printf("Invalid option. Try again.\n");
            break;
        }
    }

    free(data);

    return 0;
}