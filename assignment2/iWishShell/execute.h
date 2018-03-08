//
// Created by Robin Sommer on 08/03/2018.
//

#ifndef IWISHSHELL_EXECUTE_H
#define IWISHSHELL_EXECUTE_H

void setOutput(char* tofile);
void setInput(char* fromfile);
int executeBackground(char **arguments);
int executeRedirect(char *command, char *inputFile, char *outputFile);
int execute(char* filePath, char **arguments);

#endif //IWISHSHELL_EXECUTE_H

