#define _str(x) #x
#define str(x) _str(x)

#define this_is_secret1 
#define this_is_secret2

#define pwn str(
const char* flag =
#include "secret"
)

// include and define tricks
// refer: https://ctftime.org/writeup/21116

