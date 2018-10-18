#ifdef __linux__
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
typedef int os_handle;
typedef size_t os_size;
typedef ssize_t os_ssize;
#elif defined(_WIN32)
#include <windows.h>
typedef HANDLE os_handle;
typedef DWORD os_size;
typedef DWORD os_ssize;
#else
#error "Unknown OS!"
#endif
#include <stdio.h>
os_handle os_open(char *filename);
os_ssize os_read(os_handle fd, void* buffer, os_size count);
os_ssize os_write(os_handle fd, const void* buffer, os_size count);
void os_close(os_handle fd);
