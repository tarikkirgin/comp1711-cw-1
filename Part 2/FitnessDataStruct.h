
#ifndef FITNESS_DATA_STRUCT_H
#define FITNESS_DATA_STRUCT_H

#include <stdio.h>

// Define an appropriate struct
typedef struct {
	char date[11];
	char time[6];
	int steps;
} FITNESS_DATA;

// Helper function prototypes
void tokeniseRecord(const char *input, const char *delimiter, char *date, char *time, char *steps);
void format_record(char* formatted_record, FITNESS_DATA record);
void print_menu_options();
void read_file(char filename[]);
FITNESS_DATA get_fewest_steps(FITNESS_DATA data[]);
FITNESS_DATA get_most_steps(FITNESS_DATA data[]);
int calc_mean_steps(FITNESS_DATA data[]);
void get_continous_period(int minimum_steps, FITNESS_DATA *start_record, FITNESS_DATA *end_record);

#endif // FITNESS_DATA_STRUCT_H
