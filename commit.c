#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main (int argc, char *argv[]){

  int pid;
  int st;

  for (int i = 0 ; i < 3 ; i++){
	
	sleep(1);
	
    //Create a new process
    pid = fork();
    if (pid == 0 && i == 0){
      //git add --all
      execlp("git", "git", "add", "--all", NULL);
      perror("errore 1");
      _exit(0);
    } else if (pid == 0 && i == 1){
      //git commit -m "mesaggio"
      execlp("git", "git", "commit", "-m", argv[1], NULL);
      perror("errore 2");
      _exit(0);
    } else if (pid == 0 && i == 2){
      //git push --set-upstrea origin main
      execlp("git", "git", "push", NULL);
      perror("errore 3");
      _exit(0);
    }

  }
    if (pid > 0){
      for (int i = 0 ; i < 3 ; i++){
        wait(&st);
        printf("Il processo è terminato con stato : %d\n", st);
      }
    }






  return 0;

}
