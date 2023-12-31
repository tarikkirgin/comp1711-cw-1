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

char filename[50];
FitnessData data[500];
FitnessData sorted_data[500];
int record_count = 0;

void open_file(char filename[]);
int record_is_valid(char line[]);
void sort_records(FitnessData fitness_data_array[]);
void write_file(char filename[], FitnessData fitness_data_array[]);

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

            // clone input string as tokeniseRecord mutates the string
            char *input = strdup(line_buffer);

            // extract data from line of file
            tokeniseRecord(input, ',', date, time, &steps);

            // create new FITNESS_DATA struct
            FitnessData record;

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
    // based on an idea from here:
    // https://stackoverflow.com/questions/16013031/ensure-a-string-matches-a-certain-format-in-c

    const char pattern[] =
        "%1[0-9]%1[0-9]%1[0-9]%1[0-9]" // year
        "-"
        "%1[0-1]%1[0-9]" // month
        "-"
        "%1[0-3]%1[0-9]" // date
        ","
        "%1[0-2]%1[0-9]" // hour
        ":"
        "%1[0-5]%1[0-9]" // minute
        ","
        "%d"; // steps


    // idea to use &(char){0} to discard values from here:
    // https://stackoverflow.com/questions/10150976/c-sscanf-assignment-suppression-and-the-return-value/65718687#65718687
    
    // sscanf() returns the number of variables successfully read
    // should be 13 if record is correctly formatted (12 in date & time, 1 for the steps)

    if(13 != sscanf(line, pattern, &(char){0}, &(char){0}, &(char){0}, &(char){0}, &(char){0}, &(char){0}, &(char){0}, &(char){0}, &(char){0}, &(char){0}, &(char){0} ,&(char){0} , &(int){0})){
        return 0;
    }
    return 1;
}

void sort_records(FitnessData fitness_data_array[])
{
    int tail = 0;
    for (int i = 0; i < record_count; i++)
    {
        FitnessData highest_steps_record;
        int highest_steps = -1;
        int index;
        for (int record = 0; record < record_count; record++)
        {
            if(fitness_data_array[record].steps > highest_steps)
            {
                highest_steps_record = fitness_data_array[record];
                highest_steps = fitness_data_array[record].steps;
                // store index of record
                index = record;
            }
        }
        // add highest steps remaining record to sorted_data array
        sorted_data[tail] = highest_steps_record;
        // change record steps to -1 so it is ignored in future iterations
        data[index].steps = -1;
        tail++;
    }
}

void write_file(char filename[], FitnessData fitness_data_array[])
{
    FILE *file = fopen(filename, "w");
    for (int record = 0; record < record_count; record++)
    {
        fprintf(file, "%s\t%s\t%d\n", fitness_data_array[record].date, fitness_data_array[record].time, fitness_data_array[record].steps);
    }
    fclose(file);
}

int main()
{
    printf("Enter filename: ");
    scanf(" %s", filename);
    open_file(filename);

    sort_records(data);

    char new_filename[54];
    strcpy(new_filename, filename);
    strcat(new_filename, ".tsv");

    write_file(new_filename, sorted_data);
    printf("Data sorted and written to %s\n", new_filename);

    return 0;
}
