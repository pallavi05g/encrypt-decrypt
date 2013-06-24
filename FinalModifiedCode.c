#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>

#define TRUE 1
#define FALSE 0

FILE * getInputFromUser() {
    
    char fname[256];
    
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

    int encryptFlag = fgetc(fp);
    fseek(fp,0,0);
    if(encryptFlag == 'E')
        return TRUE;
    else return FALSE;
}

void encryptFileAndWriteToFile(FILE *fp) {
    
    size_t size;
    int esize,i,num;
    num = 10000;
    char *encrypt, *encrypted;
    FILE *encryptedFile = fopen("EncryptedData.txt","w+");
    if(encryptedFile == NULL)
    {
        perror("Writing encrypted file\n");
    }
    encrypt = malloc(num);    
    while((size = fread(encrypt,1,num,fp))>0)
    {
        encrypted = malloc(size*8); //dynamic allocation for encrypted buffer
        for(i=0;i<size;i++)
        {
            if(i==0)encrypted[i] = 'E';
            encrypted[i+1] = (int)encrypt[i]+5; //encryption
        }
        printf("\nEncrypted data is: %s\n",encrypted); //printing encrypted data as is
        esize = strlen(encrypted);
        fwrite(encrypted,1,esize,encryptedFile); //writing encrypted data to a file
        printf("Size of the encrypted data is %d\n",esize); //for debugging purposes

    }
    
    free(encrypt);
    free(encrypted);
    fclose(encryptedFile);
    

}

void decryptFileAndWriteToFile(FILE *fp) {
    size_t size;
    int i,dsize,num = 10000,ch;
    char *decrypted, *decrypt = malloc(num);
    FILE *decryptedFile = fopen("DecryptedData.txt","w+");
    if(decryptedFile == NULL)
    {
        perror("Writing decrypted file\n");
    }
    while((size = fread(decrypt,1,num,fp))>0)
    {
        
        decrypted = malloc(size*8); //dynamic allocation for decrypted buffer
        for(i=0;i<size-1;i++)
        {
            decrypted[i] = (int)decrypt[i+1]-5;//decryption
        }
        
       printf("\nDecrypted data is: %s\n",decrypted); //printing encrypted data as is
        dsize = strlen(decrypted);

        fwrite(decrypted,1,dsize,decryptedFile); //writing decrypted data to a file
        printf("Size of the decrypted data is %d\n",dsize-1); //for debugging purposes

        
    }
    free(decrypt);
    free(decrypted);
    fclose(decryptedFile);
    
}

int main(int argc, const char * argv[])
{
    FILE *fp;
    int encryptFlag;
    char ch;
	fp = getInputFromUser();
    if(fp == NULL)
    {
     return 1;   
    }
	encryptFlag = detectFileOperation(fp);

	if (encryptFlag == TRUE) {
		decryptFileAndWriteToFile(fp);
	} else{
            encryptFileAndWriteToFile(fp);
	}
    
    return 0;
}

