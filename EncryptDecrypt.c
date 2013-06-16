//Encryption and Decryption

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>

#define BUFSIZE 10000

int main(int argc, const char * argv[])
{
    char *encrypt = malloc(BUFSIZE);
    char *encrypted = malloc(BUFSIZE);
    char fname[256];
    int number[BUFSIZE],number1[BUFSIZE];
    int i,fp,fp1;
    size_t size;
    printf("Enter file to be encrypted:\n");
    scanf("%s",fname);
    if((fp=open(fname,O_RDWR))<0)
	{
		perror("File doesn't exist..\n");
        exit(0);
	}
	else
	{
        while((size=read(fp,encrypt,BUFSIZE))>0)
        {
            for(i=0;i<size;i++)
            {
                number[i] = (int)encrypt[i];
                // printf("%d ",number[i]);
            }
            
            //Encrypted numbers
            for(i=0;i<size;i++)
            {
                number1[i] = number[i] + 5;
                encrypted[i]=number1[i];
                //printf("%d ",number1[i]);
            }
            printf("\nEncrypted data is: %s\n",encrypted);
            
            //Decryption
            printf("\nDecrypted data\n");
    
            for(i=0;i<size;i++)
            {
                number1[i] = encrypted[i];
                number[i] = number1[i] - 5;
                printf("%c",(char)number[i]);
        
            }
        }
    }
    close(fp);
return 0;
}

