//Dynamic buffer allocation
//Alignments fixed, variable names modified

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<errno.h>

#define TRUE 1
#define FALSE 0
#define ERROR 2
#define LEN 1024

char fname[256];

FILE * getInputFromUser() {
    
    printf("Enter file name:\n");
    scanf("%s",fname);
    FILE *fp;
    if((fp=fopen(fname,"r")) == NULL)
    {
	printf("File doesn't exist..\n");
        return NULL;
    }
    else
    {
        printf("File found\n");
        return fp;
        
    }
}

int detectFileOperation(FILE *fp) {
    
    if(fp == NULL)
    {
    	return ERROR;
    }
    int encryptFlag = fgetc(fp);
    fseek(fp,0,0); //if I remove this line, the first char is lost because prev line moves the file pointer to the next character
    if(encryptFlag == 'E')
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }

}

void createDynamicFileName(char ext[]) //to create a filename dynamically
{
    int i,j;
    
    for(i=0;fname[i]!='.';i++); //traversal till '.' is reached
    
    for(j=0;ext[j]!='\0';++j,++i)
    {
        fname[i] = ext[j];
    }
    fname[i] = '\0';
    
    
}

int calcFileSize(FILE *fp)
{
    int fsize;
    fseek(fp,0,2);
    fsize = ftell(fp);
    //printf("Size of the file is %d",fsize);
    fseek(fp,0,0);
    return fsize;
}

void encryptFileAndWriteToFile(FILE *fp) {
    
    size_t size;
    int esize,i,fsize;
    unsigned char *data_to_encrypt, *encrypted_data;
    char encExtension[] = "-en.txt";
    
    createDynamicFileName(encExtension);     //calls create dyn filename func.
    
    printf("File %s created to store encrypted data\n",fname);
    
    FILE *encryptedFile = fopen(fname,"w+");
    if(encryptedFile == NULL)
    {
        perror("Couldn't open the file\n");
    }
    data_to_encrypt = malloc(LEN);
    encrypted_data = malloc(LEN);//dynamic allocation for encrypted buffer
    fputc('E',encryptedFile);
    //int loc = ftell(encryptedFile);
    //printf("%d is the location\n",loc);
    
    fseek(encryptedFile,1,0);
    int currentPoint,count = 0;
    do {
        size = fread(data_to_encrypt,1,LEN,fp);
        for(i = 0; i<size; i++)
        {
            encrypted_data[i] = (int)data_to_encrypt[i]+5;
            
        }
        // esize = strlen(encrypted_data);
        fwrite(encrypted_data,1,size,encryptedFile); //writing encrypted data to a file
        ++count;
        
    } while (size>0);
    currentPoint = ftell(fp);
    
    printf("Size of encrypted data is %d and count is %d\n",currentPoint,count);
    free(data_to_encrypt);
    free(encrypted_data);
    fclose(encryptedFile);
    
}

void decryptFileAndWriteToFile(FILE *fp) {
    size_t size;
    int i,dsize,fsize,count = 0 ;
    char *decrypted_data,*data_to_decrypt;
    char decExtension[] = "-dec.txt";
    
    createDynamicFileName(decExtension); //calls create dyn filename func.
    
    FILE *decryptedFile = fopen(fname,"w+");
    if(decryptedFile == NULL)
    {
        perror("Writing decrypted file\n");
    }
    data_to_decrypt = malloc(LEN);
    decrypted_data = malloc(LEN);//dynamic allocation for encrypted buffer
    
    do {
        size = fread(data_to_decrypt,1,LEN,fp);
        for(i = 0;i<size;i++) //When I made this begin from 0, an additional @ was appearing coz of the way you exit from vim i.e. :
           // Testing this resulted in the same file content but diff command showed otherwise. I couldn't understand why. It said binary files differ
            
        {
            decrypted_data[i] = (int)data_to_decrypt[i]-5;//decryption
        }
        fwrite(decrypted_data,1,size,decryptedFile); //writing decrypted data to a
        ++count;
        
    } while (size>0);
    int currentPoint = ftell(fp);
    
    printf("Size of the decrypted data is %d and count is %d\n",currentPoint,count); //for debugging purposes
    
    free(data_to_decrypt);
    free(decrypted_data);
    fclose(decryptedFile);
    
}

int main(int argc, const char * argv[])
{
    FILE *fp;
    int encryptFlag;
    char ch;
	fp = getInputFromUser();
    encryptFlag = detectFileOperation(fp);
    
    if (encryptFlag == TRUE)
    {
		decryptFileAndWriteToFile(fp);
    }
    else if(encryptFlag == FALSE)
    {
        encryptFileAndWriteToFile(fp);
    }
    else if(encryptFlag == ERROR)
    {
        return 1;    
    }
    return 0;
}

