#include<stdio.h>
#include<stdlib.h>


void init_array3(int dim1, int dim2, int dim3, int ***arr)
{
  arr = (int ***)malloc(sizeof(int)*dim1*dim2*dim3);
  for(int i = 0; i< dim1; i++)
    for(int j = 0; j< dim2; j++)
      for(int k = 0; k< dim1; k++)
        arr[i][j][k] = 2;
}

void print_array3(int dim1, int dim2, int dim3, int ***arr)
{
  for(int i = 0; i< dim1; i++)
    for(int j = 0; j< dim2; j++)
    {
      for(int k = 0; k< dim1; k++)
        printf("%d ",arr[i][j][k]);;
      printf("\n");
    }
}


//int a[10][24][34] ={0};
main()
{
  int ***a;
  init_array3(10,10,10,a);//(int ***)(&(a[0][0][0])));
  print_array3(10,10,10,a);//(int ***)(&(a[0][0][0])));
  return 0;
}
