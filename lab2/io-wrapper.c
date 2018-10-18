#include "io-wrapper.h"
#ifdef __linux__
// todo: os_open()
os_ssize os_read(os_handle fd, void *buffer, os_size count) {
  return read(fd, buffer, count);
}
os_ssize os_write(os_handle fd, const void *buffer, os_size count) {
  return write(fd, buffer, count);
}
//todo: os_close()
#elif defined(_WIN32)
// todo: os_open
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
// todo: os_close()
#endif

int main(void) {
  char buf[101];
  os_handle fd = os_open(filename);
  os_read(fd, buf, 100);
  printf("buf=%s\n",buf); 
  os_close(fd);
}

