#ifdef WIN32
    #include <io.h>
    #define F_OK 0
    #define access _access
#else
    #include <unistd.h>
#endif

#include <file/file_exists.h>

bool file_exists(const char * path) {
    return access(path, F_OK) == 0;
}