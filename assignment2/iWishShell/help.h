//
// Created by Robin Sommer on 09/03/2018.
//

#ifndef IWISHSHELL_HELP_H
#define IWISHSHELL_HELP_H

char* filePath(char* file);
void setOutput(char* tofile);
void setInput(char* fromfile);
char** getArguments(char** arguments, int start, int end);
int isBackGroundProcess(char** arguments, int numArgs);
int checkArgumentAtPosition(char** arguments, char* compareArg, int position);

#endif //IWISHSHELL_HELP_H
