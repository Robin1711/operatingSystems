
#ifndef IWISHSHELL_HELPERFUNTIONS_H
#define IWISHSHELL_HELPERFUNTIONS_H

void execute(char **arguments, int numArgs);

// Sets the ouput to where this writes
void setOutput(char* tofile);

// Sets the input from where this reads
void setInput(char* fromfile);

// filePath returns the path to a specified file
// If the file does not exist, the function returns NULL
char* filePath(char* file);


#endif //IWISHSHELL_HELPERFUNTIONS_H
