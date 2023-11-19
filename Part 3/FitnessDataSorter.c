#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define the struct for the fitness data
typedef struct
{
    char date[11];
    char time[6];
    int steps;
} FitnessData;

// Function to tokenize a record
void tokeniseRecord(char *record, char delimiter, char *date, char *time, int *steps)
{
    char *ptr = strtok(record, &delimiter);
    if (ptr != NULL)
    {
        strcpy(date, ptr);
        ptr = strtok(NULL, &delimiter);
        if (ptr != NULL)
        {
            strcpy(time, ptr);
            ptr = strtok(NULL, &delimiter);
            if (ptr != NULL)
            {
                *steps = atoi(ptr);
            }
        }
    }
}

// Global variables

char filename[20];
FitnessData data[300];
int record_count = 0;

void open_file(char filename[]);
int record_is_valid(char line[]);

// Function to open a record
void open_file(char filename[])
{
    FILE *file = fopen(filename, "r");
    // exit if file is null
    if (file == NULL)
    {
        printf("Error: invalid file\n");
        exit(1);
    }
    else
    {
        // create buffer
        int buffer_size = 50;
        char line_buffer[buffer_size];

        // initialise variable to store end of array
        int tail = 0;

        // read in file contents
        while (fgets(line_buffer, buffer_size, file) != NULL)
        {
            // check record is valid
            if (!record_is_valid(line_buffer))
            {
                printf("Error: invalid file\n");
                exit(1);
            }

            // create variables to hold data
            char date[11];
            char time[6];
            int steps;

            // extract data from line of file
            tokeniseRecord(line_buffer, ',', date, time, &steps);

            // create new FITNESS_DATA struct
            FitnessData record = {};

            // populate struct with values extracted from file
            strcpy(record.date, date);
            strcpy(record.time, time);
            record.steps = steps;

            // add struct to array
            data[tail] = record;
            // increment end of array marker
            tail++;
        }
        // save the number of records to a global variable
        record_count = tail;

        // close file
        fclose(file);
    }
}

// Checks if record is valid
int record_is_valid(char line[])
{
    int error_found = 0;

    // based on an idea from here:
    // https://stackoverflow.com/questions/16013031/ensure-a-string-matches-a-certain-format-in-c

    // asterisks mean values are not saved
    // learned this from here:
    // https://stackoverflow.com/questions/7607550/scanf-skip-variable

    char pattern[] =
        "%*1[0-9]%*1[0-9]%*1[0-9]%*1[0-9]" // year
        "-"
        "%*1[0-1]%*1[0-9]" // month
        "-"
        "%*1[0-3]%*1[0-9]" // date
        ","
        "%*1[0-2]%*1[0-9]" // hour
        ":"
        "%*1[0-5]%*1[0-9]" // minute
        ","
        "%d"; // steps

    // sscanf() returns the number of variables successfully read
    // should be 13 if record is correctly formatted (12 in date & time, 1 for the steps)

    // char year_1, year_2, year_3, year_4, month_1, month_2, date_1, date_2, hour_1, hour_2, minute_1, minute_2;
    // int steps;

    // if (sscanf(line, pattern, &year_1, &year_2, &year_3, &year_4, &month_1, &month_2, &date_1, &date_2, &hour_1, &hour_2, &minute_1, &minute_2, &steps) != 13)
    // {
    //     error_found = 1;
    // }

    if (sscanf(line, pattern) != 13)
    {
        error_found = 1;
    }


    return !error_found;
}



int main()
{
    printf("Enter filename: \n");
    scanf(" %s", filename);
    open_file(filename);


}
