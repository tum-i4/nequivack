#ifndef TOKENIZER_H_
#define TOKENIZER_H_

#include <clang-c/Index.h>

void getTokensFromSourceFile(const char*, CXToken**, unsigned*);

#endif
