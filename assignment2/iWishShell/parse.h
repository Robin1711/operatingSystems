#ifndef PARSINGCOMMANDLINEARG_PARSE_H
#define PARSINGCOMMANDLINEARG_PARSE_H

char* read_line();
//char** split_line(char* line);
char** split_line(char* line, char* splitToken, int* length);
int isBackGroundProcess(char** arguments, int numArgs);

#endif //PARSINGCOMMANDLINEARG_PARSE_H
