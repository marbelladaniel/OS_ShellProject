#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>

#define MAX_COMMAND_LINE_LEN 1024
#define MAX_COMMAND_LINE_ARGS 128
#define MAX_PATH 4096

char prompt[] = "> ";
char delimiters[] = " \t\r\n";
char cwd[MAX_PATH]; 
char *dir_ptr; 
bool background = false; 
extern char **environ;


void exit_program(int sig){ //Signal Handling  
 
  printf("Control C is not a valid command in this shell.\n");
  fflush(stdout);
}

char **toParse(char* input_cmd) //parse input on commmand line 
{
  char **new_cmd = malloc(8 * sizeof(char *));
  char *this_arg;
  char *that_arg;
  char *space_here = " ";
  int i = 0;
  
  this_arg = strtok(input_cmd, space_here);
  while (this_arg != NULL) {
        new_cmd[i] = this_arg;
        that_arg= new_cmd[i];
    
    //checks for occurance of '$'to asssure there was input
    
        if(strchr(that_arg,'$') != NULL){
          memmove(that_arg, that_arg+1, strlen(that_arg));
          new_cmd[i] = getenv(that_arg);
          
        }
        i++;
        this_arg = strtok(NULL, space_here);
    }
  if(strcmp(new_cmd[i-1], "&") == 0){
    background = true;
  }
  
  new_cmd[i] = NULL;
  return new_cmd;  
}



int main() {
    // Stores the string typed into the command line.
    char command_line[MAX_COMMAND_LINE_LEN];
    char cmd_bak[MAX_COMMAND_LINE_LEN];
    char *input_ptr = input_cmd; 
  
    // Stores the tokenized command line input.
    char *arguments[MAX_COMMAND_LINE_ARGS];
    char **command; 
    	
    while (true) {
      
        do{ 
            
            signal(SIGINT, exit_program);
            background = false;
          
            // Print shell prompt.
            dir_ptr = getcwd(cwd, sizeof(cwd));
            printf("%s%s",dir_ptr, prompt);
            
            fflush(stdout);
            

            // Read input from stdin and store it in input_cmd. If there's an
            // error, exit immediately. (If you want to learn more about this line,
            // you can Google "man fgets")
        
            if ((fgets(input_cmd, MAX_COMMAND_LINE_LEN, stdin) == NULL) && ferror(stdin)) {
                fprintf(stderr, "fgets error");
                exit(0);
            }
          
          input_ptr = strsep(&input_ptr, "\n");
          command = toParse(input_ptr);
          
          
          if(strcmp(command[0], "exit") == 0){
            exit(0);
          }
          else if(strcmp(command[0], "pwd") == 0){
            dir_ptr = getcwd(cwd, sizeof(cwd));
            printf("%s \n", dir_ptr);
          }
          else if(strcmp(command[0], "echo") == 0){
            int i;
            for(i = 1; i < sizeof(command); i++){
              if(command[i] == NULL){
                break;
              }
              printf("%s ", command[i]);
            }
            printf("\n");
          }
          else if(strcmp(command[0], "env") == 0){
            int i;
            for (i = 0; environ[i] != NULL; i++) 
               printf("\n%s", environ[i]); 
            printf("\n");
          }
          else if(strcmp(command[0], "setenv") == 0){
            const char *str_arr[2];
            char *str = command[1];
            int i =0;
            str_arr[i] = strtok(str, "=");
            while(str_arr[i] != NULL) {
              str_arr[++i] = strtok(NULL,"=");
            }
            setenv(str_arr[0],str_arr[1],1);
          }
          else if(strcmp(command[0], "cd") == 0){
            if(chdir(command[1]) == -1){
              printf("No such directory. \n");
              exit(0);
            }
          }
          
          else{
            
            pid_t pid;
            pid = fork();
            if (pid == 0) {
              execvp(command[0], command);
              sleep(10);
              
              kill(pid, SIGTERM);
             } 
            else {
              if(!background){
                wait(0);
              }
             }
            }
          
 
        }while(input_cmd[0] == 0x0A);  // while just ENTER pressed

      
        // If the user input was EOF (ctrl+d), exit the shell.
        if (feof(stdin)) {
            printf("\n");
            fflush(stdout);
            fflush(stderr);
            return 0;
        }

        // TODO:
        // 
        
			  // 0. Modify the prompt to print the current working directory
			  
			
        // 1. Tokenize the command line input (split it on whitespace)

      
        // 2. Implement Built-In Commands
      
    
        // 3. Create a child process which will execute the command line input

  
        // 4. The parent process should wait for the child to complete unless its a background process
      
      
        // Hints (put these into Google):
        // man fork
        // man execvp
        // man wait
        // man strtok
        // man environ
        // man signals
        
        // Extra Credit
        // man dup2
        // man open
        // man pipes
    }
    // This should never be reached.
    return -1;
}