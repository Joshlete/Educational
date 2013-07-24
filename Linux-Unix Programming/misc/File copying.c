#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define TRUE 1
#define FALSE 0

#define BUFFER_SIZE 50

void printHelp(){
	printf("usage: copy [-m] <target file> <destination file>\n");
	printf("usage: copy [-h]\n");
}

void copy_read_write(char* targeth, char* desth);
void copy_mmap(char* in, char* out);

int main(int argc, char** argv){

	char* host="ASD";
	char* target;
	char option;

	int input_fd;
	int output_fd;


	char mapflag=FALSE;


	if (argc==1){
		printHelp();
		exit(1);
	}

	while ( (option=getopt(argc, argv, "mh"))!=-1 ){
		switch(option){
		case 'm':
			mapflag=TRUE;
			break;
		case 'h':
			printHelp();
			exit(1);
			break;
		default:
			printHelp();
			exit(1);
			break;
		}
	}

    if (mapflag==TRUE)copy_mmap(argv[optind], argv[optind+1]);
    else copy_read_write(argv[optind], argv[optind+1]);


    close (input_fd);
    close (output_fd);

    return (EXIT_SUCCESS);
}

void copy_read_write(char* in, char* out){

	int ret_in, ret_out;
	int input_fd, output_fd;
	char buffer[BUFFER_SIZE];

    input_fd = open (in, O_RDONLY);
    if (input_fd == -1) {
            printf("Error opening input file.\n");
            exit(2);
    }

    output_fd = open(out, O_WRONLY | O_CREAT, S_IWUSR | S_IRUSR );
    if(output_fd == -1){
        printf("Error opening output file.\n");
        exit(3);
    }

    struct stat st;
    fstat(input_fd, &st);
    ftruncate(output_fd, st.st_size);



    while((ret_in = read (input_fd, &buffer, BUFFER_SIZE)) > 0){
        ret_out = write (output_fd, &buffer, (ssize_t) ret_in);
        if(ret_out != ret_in){
            printf("Error in writing.\n");
            exit(1);
        }
    }


}

void copy_mmap(char* in, char* out){

	int input_fd, output_fd;

    input_fd = open (in, O_RDONLY);
    if (input_fd == -1) {
            printf("Error opening input file.\n");
            exit(2);
    }

    output_fd = open(out, O_RDWR | O_CREAT, S_IWUSR | S_IRUSR);
    if(output_fd == -1){
        printf("Error opening output file.\n");
        exit(3);
    }
	
    struct stat st;
    fstat(input_fd, &st);
    ftruncate(output_fd, st.st_size);

    char* target;
    target=mmap(0, st.st_size, PROT_READ, MAP_SHARED, input_fd, 0);
    if (target==(void*) -1){
    	printf("Error mapping target with errno: %d.\n", errno);
    	exit(6);
    }


	char* destination;
	destination=mmap(0, st.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, output_fd, 0);
	if (destination==(void*) -1){
		printf("Error mapping destination with errno: %d.\n", errno);
		exit(5);
	}

	memmove(destination, target, st.st_size);
	munmap(destination, st.st_size);

}






