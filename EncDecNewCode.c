/* This program handles files of any size dynamically and writes the encrypted data to a file, prints the decrypted part after encrypting it
 What it doesn't do yet - Reading an encrypted file and decrypting it*/


#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>


int main(int argc, const char * argv[])
{
    char *encrypt,*encrypted,*decrypted; //encrypt holds the data to be                                    encrypted, encrypted holds data after it is encrypted, decrypted holds data after decryption
    char fname[256];
    int fsize,esize,i,fp,fp1;
    size_t size;
    
    
    printf("Enter file to be encrypted:\n");
    scanf("%s",fname); //reads the file that has to be encrypted
    
    fp1 = open("EncData.txt",O_CREAT|O_RDWR,0777); //holds encrypted data

    if((fp=open(fname,O_RDWR))<0)
	{
		perror("File doesn't exist..\n");
        exit(0);
	}
	else
	{
        fsize = lseek(fp,0,2); //to fetch the size of the file
        
        lseek(fp,0,0); //so ensure the file pointer goes back to the beginning of the file
        
        printf("Size of the file is %d\n",fsize-1); //for debugging purpose
        
        encrypt = malloc(fsize); //dynamic allocation based on file size
        
        while((size=read(fp,encrypt,fsize))>0) //till there are no more bytes to read
        {
            encrypted = malloc(size*8); //dynamic allocation for encrypted buffer
            for(i=0;i<size;i++)
            {
                encrypted[i] = (char)((int)encrypt[i]+5); //encryption
            }
            printf("\nEncrypted data is: %s\n",encrypted); //printing encrypted data as is
            esize = strlen(encrypted);
            write(fp1,encrypted,esize); //writing encrypted data to a file
            printf("Size of the encrypted data is %d\n",esize-1); //for debugging purposes
            
            //Decryption
            printf("\nDecrypted data\n");
            decrypted = malloc(esize*8); //dynamic allocation for decrypted buffer

            for(i=0;i<size;i++)
            {
                decrypted[i] = (char)((int)encrypted[i]-5);//decryption 
                printf("%c",decrypted[i]); //to print decrypted data
                
            }
        }
    }
    close(fp);
    close(fp1);
    return 0;
}


