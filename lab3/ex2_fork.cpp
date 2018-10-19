#include <iostream>
#include <string>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

using namespace std;
int globalVariable = 2;

int main()
{
  string sIdentifier;
  int iStackVariable = 20;
  pid_t pID = fork();
  if (pID == 0) // child
  {
    // Code only executed by child process
    sIdentifier = "Child Process: ";
    globalVariable++;
    iStackVariable++;
    cout << sIdentifier;
    cout << " Global variable: " << globalVariable;
    cout << " Stack variable: " << iStackVariable << endl;
    exit(0);
  }
  else if (pID < 0) // failed to fork
  {
    cerr << "Failed to fork" << endl;
    exit(1);
    // Throw exception
  }
  else // parent
  {
    // Code only executed by parent process
    sIdentifier = "Parent Process:";
  }
  // executed only by parent
  cout << sIdentifier;
  cout << " Global variable: " << globalVariable;
  cout << " Stack variable: " << iStackVariable << endl;
  exit(0);
}
