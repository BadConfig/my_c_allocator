#include <unistd.h>
#include "libs.h"
#include "alloc.h"
int main(void){
        int* a  = (int*)alloc(sizeof(int));
        *a = 440;
        long int* b = (long int*)alloc(sizeof(long int));
        printf("%ld\n",*b);
        b  = (long int*)realloc(a,sizeof(long int));
        printf("%d -> %ld\n",*a,*b);
        memory_set(b,sizeof(double),0);
        printf("%ld set to 0\n",*b); return 0;
}
