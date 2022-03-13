/* Implementing Stream Cipher*/
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<assert.h>

//SIZE_PLAIN_TEXT is equal to the lemgth of my plain text in byte
#define SIZE_PLAIN_TEXT (1<<20)  

typedef unsigned long long unint64_t ;

char* toBinary(int n, int len)  //function to return binary of an integer
{
    char* binary = (char*)malloc(sizeof(char) * len);
    int k = 0;
    for (unsigned i = (1 << len - 1); i > 0; i = i / 2)
    {
        binary[k++] = (n & i) ? '1' : '0';

    }
    binary[k] = '\0';
    return binary;
}


int num_setbits(long long int n) // function to calculate the number of set bits i.e the number of 1's in a given integer
{
    int count = 0;
    while(n)
    {
        int val = n & (-n); //n&(-n) gives the right most 1 in the number
        n -= val;//n-val gives n' where n' has all the elements same as n only the rightmost 1 becomes 0 in it
        count++;//this count keeps count of all the 1's present in n
    }
    return count;
}
long int lfsr(long int poly_bit,unsigned n,long int seed_bit)//function to design lfsr
{
    int i;
    //long int seed_bit = 0b1000;//as a seed I am giving 1000
    //unsigned period = 0;//keeps count for each poly_bit
    long int padder = 0b1<<(n-1);//this padder helps to push 1 in the first place for lfsr
    //char* binpoly_bit = toBinary(poly_bit,n);
    
    long int temp = poly_bit & seed_bit;//temp has 1 in a position only when both poly_bit and seed_bit has 1 
    int counter = num_setbits(temp);//number of 1's in temp tells us the positions in which XOR operation will happen
    if(counter%2 == 0)
    {
        seed_bit = seed_bit >> 1;
    }
    else
    {
        seed_bit = (seed_bit >> 1) | padder;
    }
    return seed_bit;
 }

unint64_t* Encryption(unint64_t* message,unint64_t** catcher)
{
    unint64_t key_64 = 0,bit;
    long int seed1,seed2,seed3;
    seed1= seed2 = seed3 = 0b1000;
    unint64_t* Key = (unint64_t*)malloc((SIZE_PLAIN_TEXT/sizeof(unint64_t))*sizeof(unint64_t));
    unint64_t* Cipher = (unint64_t*)malloc((SIZE_PLAIN_TEXT/sizeof(unint64_t))*sizeof(unint64_t));
    int i,j,k=0;
    for(i=0,j=0,k=0;i<(SIZE_PLAIN_TEXT);i++,j++)
    {
        if(j == sizeof(unint64_t)*8)
        {
            Key[k] = key_64;
            k++;
            key_64 = 0;
            j = 0;
            Cipher[k] = Key[k]^message[k];
        }
        seed1= lfsr(131,7,seed1);
        seed2 = lfsr(2053,11,seed2);
        seed3 = lfsr(8275,13,seed3);
        if(seed3 & 1)
        {
            bit = seed2 & 1;
        }
        else
        {
            bit = seed1 & 1;
        }
        key_64 =key_64 << 1;
        key_64 = key_64|bit;
    }
    *catcher = Key;
    return Cipher;

}

unint64_t* get_message(const char* file_name)
{
    FILE *fp = fopen(file_name,"r");
    unint64_t* message = (unint64_t *)malloc((SIZE_PLAIN_TEXT/sizeof(unint64_t))*sizeof(unint64_t));
    int i,j;
    char *buffer =(char *)malloc((SIZE_PLAIN_TEXT+1)*(sizeof(char)));
    (void)fread(buffer,1,SIZE_PLAIN_TEXT,fp);
    for(i=0,j=0;i<SIZE_PLAIN_TEXT;i+=8,j++)
    {
        unint64_t *temp = (unint64_t *)((char *)buffer + i) ;
        message[j] = *temp;
    }
    free(buffer);
    fclose(fp);
    return message;

}
void Decryption(unint64_t *cipher,unint64_t *key)
{
    int k;
    char Buffer[9];
    Buffer[8]=0;
    unint64_t decrypt_text;
    FILE *df = fopen("deciphered_text.txt","w");
    for(k=0;k<(SIZE_PLAIN_TEXT/sizeof(unint64_t));k++)
    {
        decrypt_text = cipher[k]^key[k];
        memcpy(Buffer,&decrypt_text,8);
        (void)fprintf(df,"%s",Buffer);
    }
    fclose(df);

}
  

void main()
{
    assert(sizeof(unint64_t) == 8) ;
    int i;
    unint64_t* Stream_cipher_key;
    unint64_t *msg = get_message("/home/samipa/Coding_Practice/Crypto_Assignments_Coding/scraped_data.txt") ;
    unint64_t* cipher_text=Encryption(msg,&Stream_cipher_key);
    FILE *of =fopen("cipher_text.txt","w");
    for(i=0;i<((SIZE_PLAIN_TEXT)/sizeof(unint64_t));i++)
    {
        fprintf(of,"%llu\n",cipher_text[i]);
    }
    fclose(of);
    Decryption(cipher_text,Stream_cipher_key);
}

