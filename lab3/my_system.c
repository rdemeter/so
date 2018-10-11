#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

int my_system(const char *command)
{
  int pid, status;

  switch ((pid=fork()))
  {
    case -1:
    //error forking return -1;
    case 0:
    {
      const char *argv[] = {"/bin/bash", "-c", command, NULL};
      execv("/bin/bash", (char *const *)argv);
      /* exec se poate întoarce doar cu cod de eroare (de ex. când
      nu se găseste fisierul de executat - în cazul nostru
      /bin/bash. În caz de eroare, terminăm procesul copil */
      exit(-1);
    }
  }

  //doar procesul părinte ajunge aici, si doar dacă fork() s-a terminat cu succes
  waitpid(pid, &status, 0);

  // obtinem codul de eroare cu care s-a terminat copilul
  if (WIFEXITED(status))
    printf("Child %d terminated normally, with code %d\n", pid, WEXITSTATUS(status));

return status;
}

int main()
{
  my_system("ls");
  return 0;
}

