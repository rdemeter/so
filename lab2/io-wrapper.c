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

os_handle os_open(){
  int fd = open ("gabi.txt", O_RDONLY);
  if (fd < 0) {
    perror ("open");
    return 0;
  }
  return fd;
}
void os_close(os_handle x)
{
  close(x);
}

int main(void) {
  char buf[11];
  os_handle t=os_open();
  os_read(t,buf, 10);
  buf[11]='\0';
  printf("%s\n",buf); 
  os_close(t);
}

