//Encryption Decryption Code which handles dynamic file sizes

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include <sys/stat.h>

int main(int argc, const char * argv[])
{
    char *data, *encrypted, *decrypted,fname[256];
    struct stat st;
    int i,fp;
    size_t size,size_alloc;
    printf("Enter filename:\n");
    scanf("%s",fname);
    stat(fname, &st);
    size_alloc = st.st_size;
    encrypted = malloc(sizeof(size_alloc));
    data = malloc(sizeof(size_alloc));
    if((fp=open(fname,O_RDWR))<0)
	{
		perror("File doesn't exist..\n");
        exit(0);
	}
	else
	{
        while((size=read(fp,data,size_alloc))>0)
        {
            for(i=0;i<size;i++)
            {
                if(i==0) encrypted[i]='E';
                encrypted[i+1] = (int)data[i] + 5;
            }
            
            printf("\nEncrypted data is: %s\n",encrypted);
            decrypted = malloc(sizeof(encrypted));
            printf("\nDecrypted data\n");
            
            for(i=0;i<size;i++)
            {
                if(encrypted[0]!='E') break;
                decrypted[i+1] = (int)encrypted[i+1] - 5;
                printf("%c",decrypted[i]);
                
            }
        }
        
	}
	close(fp);
    return 0;
}



