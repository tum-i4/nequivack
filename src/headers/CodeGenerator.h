#ifndef CODEGENERATOR_H_
#define CODEGENERATOR_H_

void generateCodeForSearchPathInclude(const char*, char***, unsigned*);
void generateCodeForRelativeInclude(const char*, char***, unsigned*);

void addCodeLine(const char*, char***, unsigned*);

#endif
