#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/user.h>


int pid = 0x12345678;


int main(int argc, char **argv) {
    // Make the malware stop waiting for our output by forking a child process:
    if (fork() != 0) {
        // Kill the parent process so we stop waiting from the malware
        return 0;
    } else {
        // Close the output stream so we stop waiting from the malware
        fclose(stdout);
    }

    // The rest of your code goes here


    if (ptrace(PTRACE_ATTACH, pid, NULL, NULL) == -1) {  // Attach to the process
      perror("attach");
      return 1;
    }
    int status;

    // Do your magic here

    while ( waitpid(pid, &status, 0) && !WIFEXITED(status)) {
    
        struct user_regs_struct regs; 
        if( ptrace(PTRACE_GETREGS, pid, NULL, &regs) == -1) {
            perror("get regs");
            return 1;
        }

        //if orig_eax is 3, this is read syscall.
        if (regs.orig_eax == 3)
        {
            //if it's read syscall we want to put 0 in edx, which is the register that save the number of bytes the syscall will read.
            //if edx==0 it's will read 0 bytes. All read syscall will fail.
            regs.edx = 0;

            if (ptrace(PTRACE_SETREGS, pid, NULL,&regs) == -1) {
                perror("set regs");
                return 1;
            }
        }
        //stop right before/after the next syscall
        if (ptrace(PTRACE_SYSCALL,pid,NULL,NULL) == -1) {
            perror("syscall");
            return 1;
        }

        }

    if (ptrace(PTRACE_DETACH, pid, NULL, NULL) == -1) {  // Detach when done
      perror("detach");
      return 1;
    }



    return 0;
}
