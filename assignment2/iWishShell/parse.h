#ifndef IWISHSHELL_PARSE_H
#define IWISHSHELL_PARSE_H

// Reads the line of the commandline and returns a string
char* read_line();

// Splits the line on spaces. Returns a completely new allocated 2D char
// array, in which the input is copied according to the splitToken.
char** split_line(char* line, char* splitToken, int* length);

// Check whether an argument at the specified position is the specified argument
int checkArgumentAtPosition(char** arguments, char* compareArg, int position);

// Check whether an argument at the specified position is the specified argument
int checkArgumentAtPosition(char** arguments, char* compareArg, int position);

// from the arguments given, return an array with the arguments from idx start up to and including end
char** getArguments(char**source, int start, int length);

// Append an argument to a given source arguments list
void appendArgument(char**sourceArgs, int* args, char* newArg);

#endif //IWISHSHELL_PARSE_H
