#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>
#include<string.h>
#include<time.h>
#include<stdbool.h>

int main(){

	bool returned[3] = {false, false, false};
	char script_path1[450];
	char script_path2[450];
	char script_path3[450];

	getcwd(script_path1, sizeof(script_path1));
	strcat(script_path1, "/s1.sh");

	getcwd(script_path2, sizeof(script_path2));
	strcat(script_path2, "/s2.sh");

	getcwd(script_path3, sizeof(script_path3));
	strcat(script_path3, "/s3.sh");
	
	int pids[3];
	struct timespec t1[3];
	struct timespec t2[3];



	for (int i = 0; i < 3; i++){
		clock_gettime(CLOCK_MONOTONIC, &t1[i]);
		pids[i] = fork();
		if (pids[i] < 0 ){
			perror("fork");
			exit(0);
		}
		if ( pids[i] == 0 ) {
			if (i == 0){
				if ( execl(script_path1, "s1.sh", NULL) < 0){
					perror("execl");	
				}
			}
			if ( i == 1){
				if (execl(script_path2, "s2.sh", NULL) < 0 ){
					perror("execl");
				}

			}
			if (i == 2){
				if (execl(script_path3, "s3.sh", NULL) < 0 ){
					perror("execl");
				}
			}

		}
	}

	while(wait(NULL) > 0){
		for (int i = 0; i < 3; i++){
			if ( (waitpid(pids[i], NULL, WNOHANG)) && (returned[i] == false)    ){
				clock_gettime(CLOCK_MONOTONIC, &t2[i]);
				returned[i] = true;
			}
		}
	}
	for (int i = 0; i < 3; i++){
		printf("pid %d time taken: %ld \n", pids[i], t2[i].tv_sec - t1[i].tv_sec);
	}
}