///Author : Hasan KAYA 150112041
///mail address: hasankaya921@hotmail.com
///Operating System Project #2
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <errno.h>
#include <assert.h>
pthread_mutex_t mutex;
int count=0;
int usAl(int x);
void save_number(char* fname,int* array,int max);
int partition (int arr[], int l, int h);
void quickSort(int arr[], int l, int h);
void* fonk_quicksort(void* t);
void* random_number(void *ptr);
void truck(int* a, int* b);

typedef struct mediator
{
    int h;    //upper limit
    int l;    //lower limit
    int* array;
    int size;
    int first;
    int last;
    int m;
}mediator;


int main(int argc, char* argv[])
{
     if(argc!=15){
        printf("Error!  Usage : %s -r 2 -m 6 -l 30 -h 50 -n numbers.txt -s 4 -o sorted.txt\n",argv[0]);
        exit(-1);
    }


    pthread_mutex_init(&mutex, NULL);

    int low,high,max,r,s,i,j;
    char ifname[32],ofname[32];

        for(i=1;i<argc;i++)
        {
            if(argv[i][0] == '-')
            {
                if(argv[i][1] =='r')
                    r=atoi(&argv[i][3]);

                if(argv[i][1] == 'm')
                    max=usAl(atoi(&argv[i][3]));

                if(argv[i][1] == 'l')
                     low=atoi(&argv[i][3]);

                if(argv[i][1] == 'h')
                       high=atoi(&argv[i][3]);

                if(argv[i][1] == 'n')
                        strcpy(ifname,&argv[i][3]);

                if(argv[i][1] == 's')
                       s=atoi(&argv[i][3]);

                if(argv[i][1] == 'o')
                        strcpy(ofname,&argv[i][3]);
            }
        }

        mediator t1[r];
        pthread_t thr[r];
        int *array=malloc(max * sizeof *array);

                for(i=0;i<r;i++)
                    {
                        t1[i].h=high;
                        t1[i].l=low;
                        t1[i].m=max/2;
                        t1[i].size=max;
                        t1[i].array=array;
                        pthread_create( &thr[i], NULL, random_number, &t1[i]);
                    }

        for(i=0;i<r;i++)
            pthread_join(thr[i],NULL);

            save_number(ifname,array,max);
        mediator t2[s];
        pthread_t thr2[s];
        int first=0;
        int last=(max/s)-1;

        for(i=0;i<s;i++)
        {
            t2[i].first=first;
            t2[i].last=last;
            t2[i].array=array;
            pthread_create( &thr2[i], NULL, fonk_quicksort, &t2[i]);

            first+=max/s;
            last+=max/s;
        }
        for(i=0;i<s;i++)
            pthread_join(thr2[i],NULL);
            save_number(ofname,array,max);

}

int usAl(int u)
{
    int i,pw=1;
    for(i=0;i<u;i++)
        {pw*=2;}
    return pw;
}

void* random_number(void *ptr)
{

    srand(time(NULL));

    mediator* mt=(mediator*)ptr;

    int i,random;

        for(i=0;i<mt->m;i++)
            {
            random=rand()%(mt->h-mt->l)+mt->l+1;
            pthread_mutex_lock( &mutex );
            if(count != mt->size)
            {
            mt->array[count]=random;
            count++;
            }
            pthread_mutex_unlock( &mutex );
            }

}

void save_number(char* fname,int* array,int max)
{
    int i,j;
    FILE *ptr=fopen(fname,"w");
        fprintf(ptr,"%d\n",max);
            for(i=0;i<max;i++)
               { fprintf(ptr,"%d\n",array[i]);
                }
fclose(ptr);
}

void truck(int* a, int* b)
{
    int t = *a;
    *a = *b;
    *b = t;
}

int partition (int arr[], int l, int h)  ///Parçalara ayýrma fonksiyonu /// referans: internet kaynaklarý
{
    int i;
    int j = (l -1);
    int x = arr[h]; // pivot elementi seçtik

    for (i =l;i<=h-1;i++)
    {
        if (arr[i]<=x)
        {
            j++;
            truck(&arr[j], &arr[i]);
        }
    }
    truck(&arr[j+1], &arr[h]);
    return (j+1);
}

void quickSort(int arr[], int l, int h)
{
    if (l < h)
    {
        int p = partition(arr, l, h);
        quickSort(arr, l, p-1);
        quickSort(arr, p+1, h);
    }
}

void* fonk_quicksort(void* t)
{
    mediator* t1=(mediator*)t;
    quickSort(t1->array, t1->first, t1->last);

}
