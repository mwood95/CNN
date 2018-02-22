#include "stdio.h"
#include "string.h"

int main()
{
	FILE *fd;
	char name[1024];
	float buffer[8][8];
	int i, j;

	printf("___________________________________________\n");
	printf("|       Convolutional Neural Network	  |\n");
	printf("|              _---~~(~~-_.        	  |\n");
	printf("|            _{        )   )           	  |\n");
	printf("|         (  `-,_..`., )-- '_,)      	  |\n");
	printf("|        ( ` _)  (  -~( -_ `,  }    	  |\n");
	printf("|        (_-  _  ~_-~~~~`,  ,' )   	  |\n");
	printf("|          `~ -^(    __;-,((()))      	  |\n");
	printf("|               ~~~~ {_ -_(())       	  |\n");
	printf("|                       `|  }       	  |\n");
	printf("|                         { }          	  |\n");
	printf("___________________________________________\n");





	printf("When you are finished type 'Done'\n");
	while(name != "Done")
	{
		printf("type the name of the file to read from: ");
		scanf("%s", &name);
		fd = fopen(name, "r");
		if(fd == NULL)
		{
			printf("Error Opening File\n");
			fclose(fd);
		}
		for(i = 0; i < 8; i++)
		{
			for(j = 0; j < 8; j++)
			{
				fscanf(fd, "%f", &buffer[i][j]);
				printf("%f	", buffer[i][j]);
			}
			printf("\n\n");
		}


		fclose(fd);
	}
	return 0;
}
