#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define an appropriate struct
typedef struct {
	char date[11];
	char time[6];
	int steps;
} FITNESS_DATA;

// Define any additional variables here



// This is your helper function. Do not change it in any way.
// Inputs: character array representing a row; the delimiter character
// Ouputs: date character array; time character array; steps character array
void tokeniseRecord(const char *input, const char *delimiter,
                    char *date, char *time, char *steps) {
    // Create a copy of the input string as strtok modifies the string
    char *inputCopy = strdup(input);
    
    // Tokenize the copied string
    char *token = strtok(inputCopy, delimiter);
    if (token != NULL) {        strcpy(date, token);
    }
    
    token = strtok(NULL, delimiter);
    if (token != NULL) {
        strcpy(time, token);
    }
    
    token = strtok(NULL, delimiter);
    if (token != NULL) {
        strcpy(steps, token);
    }
    
    // Free the duplicated string
    free(inputCopy);

}

// Complete the main function
int main() {
    char* filename = "FitnessData_2023.csv";
    FILE *file = fopen(filename, "r");
    // exit if file is null
    if (file == NULL) {
        perror("File I/O");
        return 1;
    }

    // declare array of structs
    FITNESS_DATA data[300];

    // create buffer
    int buffer_size = 50;
    char line_buffer[buffer_size];

    // initalise variable to store end of array
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
    // close file
    fclose(file);

    // print number of records
    printf("Number of records in file: %d\n", tail);

    // print out first 3 rows of data
    for (int i = 0; i < 3; i++)
    {
        printf("%s/%s/%d\n", data[i].date, data[i].time, data[i].steps);
    }

    return 0;
}
