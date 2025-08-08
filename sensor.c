#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define FILE_MAGIC 0x53444C31u
#define FILE_VER 1u

#include "sensor.h"

float calculate_average_temperature(SensorData *data, int count)
{
    float sum = 0.0;
    for (int i = 0; i < count; i++)
    {
        sum += data[i].temperature;
    }

    return sum / count;
}

float calculate_average_humidity(SensorData *data, int count)
{
    float sum = 0.0;
    for (int i = 0; i < count; i++)
    {
        sum += data[i].humidity;
    }
    return sum / count;
}

void print_sensor(SensorData s, int index)
{
    printf("Sensor %d: Temp=%.1f°C, Humidity=%.1f%%", index, s.temperature, s.humidity);
    if (s.status.overheat)
        printf(" [OVERHEAT]");
    if (s.status.dry)
        printf(" [DRY]");
    printf("\n");
}

void update_flags(SensorData *data)
{
    data->status.overheat = (data->temperature > 30.0) ? 1 : 0;
    data->status.dry = (data->humidity < 40.0) ? 1 : 0;
}

void save_to_file(const char *filename, SensorData *data, int count)
{
    FILE *f = fopen(filename, "wb");
    if (!f)
    {
        printf("Failed to open file for writing.\n");
        return;
    }

    uint32_t magic = FILE_MAGIC;
    uint32_t version = FILE_VER;
    if (fwrite(&magic, sizeof(magic), 1, f) != 1 ||
        fwrite(&version, sizeof(version), 1, f) != 1 ||
        fwrite(&count, sizeof(count), 1, f) != 1)
    {
        printf("Failed to write header to file.\n");
        fclose(f);
        return;
    }

    if (count > 0 && fwrite(data, sizeof(SensorData), count, f) != (size_t)count)
    {
        printf("Failed to write sensor data to file.\n");
        fclose(f);
        return;
    }

    fclose(f);
    printf("Saved %d sensors to %s\n", count, filename);
}

void load_from_file(const char *filename, SensorData **data, int *count, int *capacity)
{
    *count = 0;
    FILE *f = fopen(filename, "rb");
    if (!f)
    {
        printf("Failed to open file for reading.\n");
        return;
    }

    uint32_t magic, version;
    int file_count = 0;

    if (fread(&magic, sizeof(magic), 1, f) != 1 ||
        fread(&version, sizeof(version), 1, f) != 1 ||
        magic != FILE_MAGIC || version != FILE_VER)
    {
        printf("Invalid file format.\n");
        fclose(f);
        return;
    }

    if (fread(&file_count, sizeof file_count, 1, f) != 1 || file_count < 0 || file_count > 100000)
    {
        printf("Bad count in fle.\n");
        fclose(f);
        return;
    }

    if (*capacity < file_count)
    {
        *capacity = file_count;
        SensorData *newp = realloc(*data, (*capacity) * sizeof(SensorData));
        if (!newp)
        {
            printf("Memory allocation failed.\n");
            fclose(f);
            return;
        }
        *data = newp;
    }

    if (file_count > 0)
    {
        size_t n = fread(*data, sizeof(SensorData), file_count, f);
        if (n != (size_t)file_count)
        {
            printf("Short read: expected %d, got %zu.\n", file_count, n);
            fclose(f);
            return;
        }
    }

    *count = file_count;
    fclose(f);
    printf("Loaded %d sensors from %s\n", *count, filename);
}