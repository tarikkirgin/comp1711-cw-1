#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "FitnessDataStruct.h"

// Struct moved to header file

// Define any additional variables here
// Global variables for filename and FITNESS_DATA array
char filename[50];
FITNESS_DATA data[500];
int record_count = 0;
char record[20];
char choice;

// This is your helper function. Do not change it in any way.
// Inputs: character array representing a row; the delimiter character
// Ouputs: date character array; time character array; steps character array
void tokeniseRecord(const char *input, const char *delimiter,
                    char *date, char *time, char *steps)
{
    // Create a copy of the input string as strtok modifies the string
    char *inputCopy = strdup(input);

    // Tokenize the copied string
    char *token = strtok(inputCopy, delimiter);
    if (token != NULL)
    {
        strcpy(date, token);
    }

    token = strtok(NULL, delimiter);
    if (token != NULL)
    {
        strcpy(time, token);
    }

    token = strtok(NULL, delimiter);
    if (token != NULL)
    {
        strcpy(steps, token);
    }

    // Free the duplicated string
    free(inputCopy);
}

void format_record(char *formatted_record, FITNESS_DATA record)
{
    sprintf(formatted_record, "%s %s", record.date, record.time);
}

void print_menu_options()
{
    printf("Menu Options:\n");
    printf("A: Specify the filename to be imported\n");
    printf("B: Display the total number of records in the file\n");
    printf("C: Find the date and time of the timeslot with the fewest steps\n");
    printf("D: Find the date and time of the timeslot with the largest number of steps\n");
    printf("E: Find the mean step count of all the records in the file\n");
    printf("F: Find the longest continous period where the step count is above 500 steps\n");
    printf("Q: Quit\n");
}

void read_file(char filename[])
{
    FILE *file = fopen(filename, "r");
    // exit if file is null
    if (file == NULL)
    {
        printf("Error: could not open file\n");
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
            // create variables to hold data
            char date[11];
            char time[6];
            char steps[10];

            // extract data from line of file
            tokeniseRecord(line_buffer, ",", date, time, steps);

            // create new FITNESS_DATA struct
            FITNESS_DATA row;

            // populate struct with values extracted from file
            strcpy(row.date, date);
            strcpy(row.time, time);
            row.steps = atoi(steps); // use atoi() to convert the steps string to an integer

            // add struct to array
            data[tail] = row;
            // increment end of array marker
            tail++;
        }
        // save the number of records to a global variable
        record_count = tail;

        // close file
        fclose(file);

        printf("File successfully loaded.\n");
    }
}

FITNESS_DATA get_fewest_steps(FITNESS_DATA data[])
{
    FITNESS_DATA lowest_step_record = data[0];
    for (int i = 1; i < record_count; i++)
    {
        if (data[i].steps < lowest_step_record.steps)
        {
            lowest_step_record = data[i];
        }
    }
    return lowest_step_record;
}

FITNESS_DATA get_most_steps(FITNESS_DATA data[])
{
    FITNESS_DATA highest_step_record = data[0];
    for (int i = 1; i < record_count; i++)
    {
        if (data[i].steps > highest_step_record.steps)
        {
            highest_step_record = data[i];
        }
    }
    return highest_step_record;
}

int calc_mean_steps(FITNESS_DATA data[])
{
    float sum = 0;
    for (int i = 0; i < record_count; i++)
    {
        sum += data[i].steps;
    }
    float mean = sum / record_count;
    // assert if decimal part is above 0.5 for rounding
    if (mean - (int)mean >= 0.5)
    {
        return mean + 1;
    }
    return mean;
}

void get_continous_period(int minimum_steps, FITNESS_DATA *start_record, FITNESS_DATA *end_record)
{
    int longest_period_length = 0;
    int curr_period_length = 0;
    for (int i = 0; i < record_count; i++)
    {
        if (data[i].steps >= minimum_steps)
        {
            for (int j = i + 1; j < record_count; j++)
            {
                if (data[j].steps < minimum_steps)
                {
                    if (curr_period_length > longest_period_length)
                    {
                        longest_period_length = curr_period_length;
                        *start_record = data[i];
                        *end_record = data[j - 1];
                    }
                    curr_period_length = 0;
                    break;
                }
                curr_period_length++;
            }
        }
    }
}

// Complete the main function
int main()
{
    while (1)
    {
        print_menu_options();
        printf("Enter choice: ");
        scanf(" %c", &choice);
        if (choice != 'A' && choice != 'Q')
        {
            if (record_count == 0)
            {
                printf("Error: No file loaded yet\n");
                exit(1);
            }
        }
        switch (choice)
        {
        case 'A': // filename input
        case 'a':
            printf("Input filename: ");
            scanf(" %s", filename);
            read_file(filename);
            break;
        case 'B': // display total number of records
        case 'b':
            printf("Total records: %d\n", record_count);
            break;
        case 'C': // find date and time of timeslot with fewest steps
        case 'c':
            format_record(record, get_fewest_steps(data));
            printf("Fewest steps: %s\n", record);
            break;
        case 'D': // find date and time of timeslot with most steps
        case 'd':
            format_record(record, get_most_steps(data));
            printf("Largest steps: %s\n", record);
            break;
        case 'E': // find mean step count
        case 'e':
            printf("Mean step count: %d\n", calc_mean_steps(data));
            break;
        case 'F':  // find longest continious period of over 500 steps
        case 'f':; //(; exists due to C weirdness) learned from here: https://stackoverflow.com/questions/18496282/why-do-i-get-a-label-can-only-be-part-of-a-statement-and-a-declaration-is-not-a
            FITNESS_DATA start_record = {};
            FITNESS_DATA end_record = {};
            get_continous_period(500, &start_record, &end_record);

            format_record(record, start_record);
            printf("Longest period start: %s\n", record);
            format_record(record, end_record);
            printf("Longest period end: %s\n", record);
            break;
        case 'Q': // quit program with 0 exit code
        case 'q':
            exit(0);
        default:
            printf("Invalid choice. Try again.\n");
            break;
        }
    }
    return 0;
}
