/* C program to list all the primitive polynomials of n degree using LFSR */
#include <stdio.h>
#include <stdlib.h>
#include<math.h>
#include<string.h>

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
void lfsr(long int poly_bit,unsigned n)//function to design lfsr
{
    int i;
    long int seed_bit = 0b1000;//as a seed I am giving 1000
    unsigned period = 0;//keeps count for each poly_bit
    long int padder = 0b1<<(n-1);//this padder helps to push 1 in the first place for lfsr
    char* binpoly_bit = toBinary(poly_bit,n);
    do
    {
        long int temp = poly_bit & seed_bit;//temp has 1 in a position only when both poly_bit and seed_bit has 1 
        int counter = num_setbits(temp);//number of 1's in temp tells us the positions in which XOR operation will happen
        if(counter%2 == 0)//if number of 1's is even then XOR of even number of 1 gives 0 and 0 comes in the first position 
            //for the next step
        {
            seed_bit = seed_bit >> 1;//shifting in right makes the first bit 0
        }
        else //Odd number of 1's gives 1 after XOR operation so in this case 1 will come in the first bit in the next step
        {
            seed_bit = (seed_bit >> 1) | padder;//here padder=1000..0 where number of 0 is n-1 , so OR operation with padder
            //with 0seed_bit gives us the required next step
        }
        period++;
        

    }
    while(seed_bit != 0b1000);
    if(period  == (pow(2, n) -1))//checking if the polynomial is primitive or not
    {
        printf("1 ");
        for(int i = 0; i < strlen(binpoly_bit); i++) //printing the polynomial
        {
            if(binpoly_bit[i] == '1')
            {
                printf("+ x^%d ", i + 1);
            }
        }
        printf("\n");
    }
    
}

void main()
{

    long int i;
    unsigned n;
    printf("Please enter the degree of the polynomial i.e the value of n\n");
    scanf("%d",&n);
    printf("The primitive polynomials of %d degree are\n",n);
    for(i=1;i<pow(2,n);i+=2)
    {
        lfsr(i,n);

    }
}

    
