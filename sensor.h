#ifndef SENSOR_H
#define SENSOR_H

typedef struct
{
    float temperature;
    float humidity;
    struct
    {
        unsigned int overheat : 1;
        unsigned int dry : 1;
    } status;
} SensorData;

float calculate_average_temperature(SensorData *data, int count);
float calculate_average_humidity(SensorData *data, int count);
void update_flags(SensorData *data);
void save_to_file(const char *filename, SensorData *data, int count);
void load_from_file(const char *filename, SensorData **data, int *count, int *capacity);
void print_sensor(SensorData s, int index);
#endif