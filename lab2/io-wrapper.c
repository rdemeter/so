#include "io-wrapper.h"
#ifdef __linux__

os_ssize os_read(os_handle fd, void *buffer, os_size count) {
  return read(fd, buffer, count);
}
os_ssize os_write(os_handle fd, const void *buffer, os_size count) {
  return write(fd, buffer, count);
}
#elif defined(_WIN32)

os_ssize os_read(os_handle fd, void *buffer, os_size count) {
  os_ssize result = -1;
  ReadFile(fd, buffer, count, &result, NULL);
  return result;
}
os_ssize os_write(os_handle fd, void *buffer, os_size count) {
  os_ssize result = -1;
  WriteFile(fd, buffer, count, &result, NULL);
  return result;
}
#endif

int main(void) {
  os_handle fd=os_open();
  os_read(fd, buf, 10);
  printf("%s\n",buf); 
  os_close(fd);
}

