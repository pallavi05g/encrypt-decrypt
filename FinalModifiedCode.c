//Dynamic buffer allocation
//But buffer length depends on filesize


#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<errno.h>

#define TRUE 1
#define FALSE 0
#define ERROR 2

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
        return ERROR;
    int encryptFlag = fgetc(fp);
    fseek(fp,0,0);
    if(encryptFlag == 'E')
        return TRUE;
    else return FALSE;
}

void create_dynamic_filename(char ext[]) //to create a filename dynamically
{
    int i,j;
    
    for(i=0;fname[i]!='.';i++); //traversal till '.' is reached
    
    for(j=0;ext[j]!='\0';++j,++i)
    {
        fname[i] = ext[j];
    }
    fname[i] = '\0';
    
    
}

int calc_file_size(FILE *fp)
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
    char en_ext[] = "-en.txt";
    
    create_dynamic_filename(en_ext);     //calls create dyn filename func.
    
    printf("File %s created to store encrypted data\n",fname);
    
    FILE *encryptedFile = fopen(fname,"w+");
    if(encryptedFile == NULL)
    {
        perror("Writing encrypted file\n");
    }
    
    fsize = calc_file_size(fp);
    
    data_to_encrypt = malloc(fsize);
    while(fgetc(fp)!=EOF)
    {
   	 size = fread(data_to_encrypt,1,fsize,fp);
    }   
     if(size==0)
    {
        perror("No data\n");
    }
    encrypted_data = malloc(size);//dynamic allocation for encrypted buffer
    encrypted_data[0] = 'E';
    
    for(i=0;i<size;i++)
    {	
        encrypted_data[i+1] = (int)data_to_encrypt[i]+5; //encryption
    }
    esize = strlen(encrypted_data);
    fwrite(encrypted_data,1,esize,encryptedFile); //writing encrypted data to a file
    printf("Size of the encrypted data is %d\n",esize); //for debugging purposes
   // printf("\nEncrypted data is: %s\n",encrypted_data); //printing encrypted data as is
    
    free(data_to_encrypt);
    free(encrypted_data);
    fclose(encryptedFile);
    
}

void decryptFileAndWriteToFile(FILE *fp) {
    size_t size;
    int i,dsize,fsize;
    char *decrypted_data,*data_to_decrypt;
    char dec_ext[] = "-dec.txt";
    
    create_dynamic_filename(dec_ext); //calls create dyn filename func.
    
    FILE *decryptedFile = fopen(fname,"w+");
    if(decryptedFile == NULL)
    {
        perror("Writing decrypted file\n");
    }
    fsize = calc_file_size(fp);
    data_to_decrypt = malloc(fsize);
    
    size = fread(data_to_decrypt,1,fsize,fp);
    if(size==0)
    {
        perror("No data\n");
    }
    decrypted_data = malloc(size);//dynamic allocation for encrypted buffer
    for(i=0;i<size-1;i++)
    {
        decrypted_data[i] = (int)data_to_decrypt[i+1]-5;//decryption
    }
    
  //  printf("\nDecrypted data is: %s\n",decrypted_data); //printing decrypted data as is
    dsize = strlen(decrypted_data);
    fwrite(decrypted_data,1,dsize,decryptedFile); //writing decrypted data to a file
    printf("Size of the decrypted data is %d\n",dsize-1); //for debugging purposes
    
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
    else
    {
        return 1;
        
    }
    return 0;
}

