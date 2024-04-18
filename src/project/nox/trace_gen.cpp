#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <zlib.h>

// Direction
#define WEST        0
#define NORTH       1
#define EAST        2
#define SOUTH       3

#define LEFT	    0
#define UP	      1
#define RIGHT	    2
#define DOWN	    3
#define THIS        4

#define EDGE       -1


int NUM_COLS=8, NUM_ROWS=8;
int NUM_NODES;
int RADIX=4;
// Traffic Pattern (for detail, see app.c)
#define NR              0 // Normal Random
#define TP              1 // Transpose
#define TN              2 // Tornado
#define BC              3 // Bit-Complement
#define NB              4 // Nearest-Neighbor
#define HT              5 // Hot Spot
#define CUST            7 // Custom 

int neighbor[64][5] = {0};
char tp[6][20] = {"NR", "TP", "TN", "BC", "NB", "HT"};
char file_names[64][5][5][30];
gzFile fp[64][5][5];
float load[11] = {0.03, 0.05};//0.005, 0.01,0.03,0.05,0.07,0.09,0.12,0.15,0.18,0.21,0.24};
int hot_spot[4] = {0,8,55,63};

int max_jobs=7000;
int per_source_jobs[64];

void node_init();
void pattern_print();

int main()
{
  int pattern = NR;
  int dnode = -1, tmpx, tmpy;
  NUM_NODES = NUM_ROWS * NUM_COLS;
  int packets=0;
  long long clock = 0;
  int done, inj_flag;

  node_init();
#if 1
  pattern_print();
#else
  for(int rate=0; rate < 2; rate++)
  {
    for(int pattern=5; pattern < 6; pattern++)
    {
      
      done = 0;
      clock = 0;
      for(int node=0; node < NUM_NODES ; node++)
      {
        sprintf(file_names[node][pattern][rate], "trace-%dn-%stp-%.2fl.gz",node,tp[pattern],load[rate]);
      //  printf("trace-%dn-%stp-%.2fl\n",node,tp[pattern],load[rate]);
        fp[node][pattern][rate] = gzopen(file_names[node][pattern][rate],"w");
        per_source_jobs[node] = 0;
      }

      while(done < NUM_NODES)
      {
        clock++;

        inj_flag = (  ((double)rand()/(double)RAND_MAX)  < load[rate] )? 1:0;

        if(inj_flag == 0)
          continue;

        for(int node=0; node < NUM_NODES; node++)
        {

          if(per_source_jobs[node] == max_jobs)
            done++;

          if(per_source_jobs[node] >= max_jobs)
            continue;

          per_source_jobs[node]++;

          switch(pattern)
          {
            case NR : // Normal Random
              {
                dnode = (int)( (double)(NUM_NODES-1) * (rand() / (double)RAND_MAX));
                break;
              }
            case TP : // Transpose
              {
                int tmpx = node % NUM_COLS; int tmpy = node / NUM_COLS;
                dnode = tmpx * NUM_COLS + tmpy;
                break;
              }
            case TN : // Tornado
              {
                int tmpx = node % NUM_COLS; int tmpy = node / NUM_COLS; int k=5;
                tmpx = (tmpx + (k-1)/2) % NUM_COLS;
                dnode = tmpy * NUM_COLS + tmpx;
                break; 
              }
            case BC : // Bit-Complement
              {
                int tmpx = node % NUM_COLS; int tmpy = node / NUM_COLS; int k=NUM_NODES-1;
                tmpx = (k-tmpx-1) % NUM_COLS; tmpy = (k-tmpy-1) % NUM_ROWS;
                if(tmpx < 0) tmpx += NUM_COLS;
                if(tmpy < 0) tmpy += NUM_ROWS;
                dnode = tmpy * NUM_COLS + tmpx;
                //FIXME hardcode for 64 nodes (5 bit node addresses)
                dnode = ~node & 0x3f;
                break; 
              }
            case NB : // Nearest Neighbor
              {
                do{
                  int port = (int)(rand() % RADIX);
                  dnode = neighbor[node][port];
                } while(dnode == EDGE);
                break; 
              }
            
            case HT : // Hot Spot
              {
                int n = (int)(rand() % 4);
                dnode = hot_spot[n];
                break; 
              }

          }// switch
          gzprintf(fp[node][pattern][rate],"%2d %2d %lld\n",node,dnode,clock);
          //printf("%s %2d %2d %lld\n",file_names[node][pattern][rate],node,dnode,clock);
        }// end for
      }//end clock
      printf("done pattern %s rate %.2f\n",tp[pattern],load[rate]);
      for(int node=0; node < NUM_NODES; node++)
        gzclose(fp[node][pattern][rate]);
    }//end pattern
  }// end rate
  //----------------------------------------
#endif
}

void pattern_print()
{

  int nc = NUM_COLS, nr = NUM_ROWS; // # of columns and rows
  int num_nodes = NUM_NODES;
  int node,dnode;
  int pattern;
  int d, m, row, col;
  
   for(row=0; row<nr; row++)
   {
     for(col=0; col<nc; col++) 
     {
       int h=0;
       node = row * nc + col;
       printf("%2d ",node);
       for(int n=0; n< NUM_NODES; n++)
       {
         int r = n/NUM_COLS;
         int c = n%NUM_COLS;
         h+= abs(r-row) + abs(c-col);
       }
//       printf("%4.2f ",h/(float)(NUM_NODES));
     }
     printf("\n");
   }
 //for(int pattern=0; pattern < 6; pattern++)
   pattern = TP;
 {
   printf("\n****%s****\n",tp[pattern]);
   for(row=0; row<nr; row++)
   {
     for(col=0; col<nc; col++) 
     {
       node = row * nc + col;

       switch(pattern)
       {
         case NR : // Normal Random
           {
             dnode = (int)( (double)(NUM_NODES-1) * (rand() / (double)RAND_MAX));
             break;
           }
         case TP : // Transpose
           {
             int tmpx = node % NUM_COLS; int tmpy = node / NUM_COLS;
             dnode = tmpx * NUM_COLS + tmpy;
             break;
           }
         case TN : // Tornado
           {
             int tmpx = node % NUM_COLS; int tmpy = node / NUM_COLS; int k=5;
             tmpx = (tmpx + (k-1)/2) % NUM_COLS;
             dnode = tmpy * NUM_COLS + tmpx;
             break; 
           }
         case BC : // Bit-Complement
           {
             int tmpx = node % NUM_COLS; int tmpy = node / NUM_COLS; int k=NUM_NODES-1;
             tmpx = (k-tmpx-1) % NUM_COLS; tmpy = (k-tmpy-1) % NUM_ROWS;
             if(tmpx < 0) tmpx += NUM_COLS;
             if(tmpy < 0) tmpy += NUM_ROWS;
             dnode = tmpy * NUM_COLS + tmpx;
             //FIXME hardcode for 64 nodes (5 bit node addresses)
             dnode = ~node & 0x3f;
             break; 
           }
         case NB : // Nearest Neighbor
           {
             do{
               int port = (int)(rand() % RADIX);
               dnode = neighbor[node][port];
             } while(dnode == EDGE);
             break; 
           }

         case HT : // Hot Spot
           {
             int n = (int)(rand() % 4);
             dnode = hot_spot[n];
             break; 
           }

       }// switch
       printf("%2d ",dnode);
     }// for col
     printf("\n");
   }
 }

}

void node_init()
{
  int current_node;
  int ln, rn, un, dn;
  int i,node = 0;
  // Following variables are used to simplify the code below.
  int nc = NUM_COLS, nr = NUM_ROWS; // # of columns and rows
  int d, m, row, col;
  int num_nodes = NUM_NODES;

  for(row=0; row<nr; row++)
    for(col=0; col<nc; col++) 
    {
      current_node = row * nc + col;

      dn = (row == 0)?    EDGE : (row-1)*nc+col;
      un = (row == nr-1)? EDGE : (row+1)*nc+col;
      ln = (col == 0)?    EDGE : (row*nc)+col-1;
      rn = (col == nc-1)? EDGE : (row*nc)+col+1;
      neighbor[current_node][LEFT]  = ln;
      neighbor[current_node][RIGHT] = rn;
      neighbor[current_node][UP]    = un;
      neighbor[current_node][DOWN]  = dn;
    }// for col
}
