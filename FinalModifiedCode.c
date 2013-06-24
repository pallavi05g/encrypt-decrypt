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
    
    size_t size, read_data = 0;
    const size_t block = 1024;
    int esize,i;
    unsigned char *encrypt, *encrypted;
    FILE *encryptedFile = fopen("EncryptedData.txt","w+");
    if(encryptedFile == NULL)
    {
        perror("Writing encrypted file\n");
    }
    encrypt = malloc(block);
    while((size = fread(encrypt + read_data,1,block,fp))>0)
    {
        read_data+=size;
        unsigned char *buf = malloc(read_data+block);
        memcpy(buf,encrypt,read_data);
        free(encrypt);
        encrypt = buf;
        encrypted = malloc(read_data+block);//dynamic allocation for encrypted buffer
    }

    encrypted[0] = 'E';

    for(i=0;i<read_data;i++)
    {
            encrypted[i+1] = (int)encrypt[i]+5; //encryption
    }
    printf("\nEncrypted data is: %s\n",encrypted); //printing encrypted data as is
    esize = strlen(encrypted);
    fwrite(encrypted,1,esize,encryptedFile); //writing encrypted data to a file
    printf("Size of the encrypted data is %d\n",esize); //for debugging purposes
    
    free(encrypt);
    free(encrypted);
    fclose(encryptedFile);
    

}

void decryptFileAndWriteToFile(FILE *fp) {
    size_t size, read_data = 0;
    const size_t block = 1024;
    int i,dsize, ch;
    char *decrypted, *decrypt = malloc(block);
    FILE *decryptedFile = fopen("DecryptedData.txt","w+");
    if(decryptedFile == NULL)
    {
        perror("Writing decrypted file\n");
    }
    while((size = fread(decrypt + read_data,1,block,fp))>0)
    {
        read_data+=size;
        unsigned char *buf = malloc(read_data+block);
        memcpy(buf,decrypt,read_data);
        free(decrypt);
        decrypt = buf;
    }

    decrypted = malloc(size*8); //dynamic allocation for decrypted buffer
    for(i=0;i<read_data-1;i++)
    {
        decrypted[i] = (int)decrypt[i+1]-5;//decryption
    }
    
    printf("\nDecrypted data is: %s\n",decrypted); //printing encrypted data as is
    dsize = strlen(decrypted);
    fwrite(decrypted,1,dsize,decryptedFile); //writing decrypted data to a file
    printf("Size of the decrypted data is %d\n",dsize-1); //for debugging purposes

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

