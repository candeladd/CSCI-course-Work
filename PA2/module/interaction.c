#include <stdio.h>
#include <fcntl.h>
#include <string.h>




int main() {
	char userinput[5];
	char buffer[1024];
	char stuff[100];
	int wrtoff =1;
	int rdoff= 0;
	int fp = open("/dev/simple_char_driver", O_RDWR);

	while(userinput[0] !='3'){
		printf("please choose an option \n1. read \n2. write \n3. quit \n");
		fgets(userinput, 2, stdin);


		if (userinput[0] == '1'){
            rdoff += read(fp, buffer, wrtoff, rdoff);
        }

		else if (userinput[0] == '2'){
			printf("give me something to write \n");
			fgets(stuff, 100, stdin);
			fgets(buffer, 1023, stdin);
			wrtoff += write(fp, buffer,  strlen(buffer)), wrtoff;
		}

		else if (userinput[0] == '3'){
			return 0;
		}

	}
	return 0;
}

