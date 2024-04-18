/* NoX (NoC Simulator)
 *
 * Dept. of Computer Science & Engineering, Pennsylvania State University.
 * All Rights Reserved.
 *  
 * 1. License     
 * NoX is distributed free of charge for academic, educational, noncommercial 
 * research purposes as long as this notice in its entirety is preserved in
 * every file included in this package.
 * All commercial use of this program requires separate licence. Contact the
 * author for details.
 * 
 * 2. All the publications that used the simulation results generated by the 
 * NoX should notify the author of the publication information and put 
 * following reference.
 *
 *  http://www.cse.psu.edu/~dpark/nox/
 * 
 * 3. Modification of the source code is permitted and encouraged as long as 
 * it follows the terms described in this copyright notice.
 *
 * 4. The author is not responsible for any problems caused by possible errors
 * of the NoX package. Therefore, users should verify the simulation result
 * before using it in their publication.
 *
 * Dept. of Computer Science & Engineering, Pennsylvania State University.
 * Contact: dpark@cse.psu.edu 
 * 
 * 6. If problems are found with the NoX package, please send an email to the
 * author for discussion and correction.

 */

/* Update History
 *
 *
 */

/* NODE_MARKING.C - Initializes the health status of nodes and links */

// The information generated in the functions in this file
// is used in route_proximity_aware.c for routing decision.
#include <stdlib.h>
#include "main.h"
#include "router.h"
#include "node_marking.h"
#include "shared.h"

void health_init(void)
{
  int i,left,right,up,down;
  int col, row, hv, rind, cind, flag;
  // Set some of the node as Fail to test fault tolerant routing.
  // Set the value to either 'H'(Healthy) or 'F'(Fail).
  char node_status_map [MAX_ROWS][MAX_COLS];
  char hlink_status_map[MAX_ROWS][MAX_COLS];
  char vlink_status_map[MAX_ROWS][MAX_COLS];

  //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
  // 1. Initialization of the node state.
  //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

  // Initialize every node to 'H'
  for(col=0; col<NUM_COLS; col++)
    for(row=0; row<NUM_ROWS; row++)
      node_status_map[row][col] = 'H';

  if(num_failed_node > 0)
  {

    for(i=0; i<num_failed_node; i++)
    {
      do // generate node failure
      {
	rind = (int)((float)((NUM_ROWS-1)*rand())/(float)RAND_MAX); 
	cind = (int)((float)((NUM_COLS-1)*rand())/(float)RAND_MAX);
      } while(node_status_map[rind][cind] == 'F');
      
      node_status_map[rind][cind] = 'F'; // Mark selected node as fail;
    }// for i
    
  }// if(num_failed_node > 0)

  //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
  // 2. Initialization of the link state.
  //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
  
  // If topology is MESH, set flag to 1.
  // If you add other topology other than MESH and TORUS, you should
  // modify following line appropriately.
  // Only topologies that have wrap-around link should set the flag to 0.
  flag = (topology == MESH)? 1:0;

  // Initialize every link to 'H'
  // Exception: When the topology is MESH, there are NUM_COLS-1 horizontal
  //            links and NUM_ROWS-1 vertical links. Mark unused array to 'F'.
  for(col=0; col<NUM_COLS; col++)
    for(row=0; row<NUM_ROWS; row++)
    {
      hlink_status_map[row][col] = (flag == 1 && col == NUM_COLS-1)? 'F':'H';
      vlink_status_map[row][col] = (flag == 1 && row == NUM_ROWS-1)? 'F':'H';
    }

  if(num_failed_link > 0)
  {

    for(i=0; i<num_failed_link; i++)
    {
      hv = ((float)rand()/(float)RAND_MAX > 0.5)? 0:1;
      if(hv == 0)
      { 
	do // generate horizontal link failure
	{
	  rind = (int)((float)((NUM_ROWS-1     )*rand())/(float)RAND_MAX); 
	  cind = (int)((float)((NUM_COLS-1-flag)*rand())/(float)RAND_MAX);
	} while(hlink_status_map[rind][cind] == 'F');
	
	hlink_status_map[rind][cind] = 'F'; // Mark selected link as fail;
	  
      }// if(hv == 0)
      else
      { 
	do // generate vertical link failure
	{
	  rind = (int)((float)((NUM_ROWS-1-flag)*rand())/(float)RAND_MAX); 
	  cind = (int)((float)((NUM_COLS-1     )*rand())/(float)RAND_MAX);
	} while(vlink_status_map[rind][cind] == 'F');

	vlink_status_map[rind][cind] = 'F'; // Mark selected link as fail;
      }// else
    }// for i
    
  }// if(num_failed_link > 0)

  
  // Update router structure with the node/link status.
  for(i=0; i < NUM_NODES; i++)
  {
    if(i >= NUM_NODES/2 && hybrid_topology == YES)
    {
      router[i].health_status = HEALTHY;
      continue;
    }
		if(topology == BFLY)
		{
      router[i].health_status = HEALTHY;
      continue;
		}

    // 1. Set node status...
    router[i].health_status = 
      (node_status_map[i/NUM_COLS][i%NUM_COLS] == 'H')? HEALTHY:FAIL;

    // 2. Set link status...
    // 2-1. horizontal (left and right) 
    left = i%NUM_COLS-1;
    right = i%NUM_COLS;

    // 2-1-1. Check left link (consider wrap-around)
    if(left<0)
	router[i].FA_link_info[LEFT]=                         // left edge
	  (hlink_status_map[i/NUM_ROWS][NUM_COLS-1 ] == 'H')? HEALTHY:FAIL; 
    else
        router[i].FA_link_info[LEFT]=
          (hlink_status_map[i/NUM_ROWS][left       ] == 'H')? HEALTHY:FAIL;

    // 2-1-2. Check right link 
    router[i].FA_link_info[RIGHT]=
          (hlink_status_map[i/NUM_ROWS][right      ] == 'H')? HEALTHY:FAIL;

    // 2-2. vertical (up and down)
    down = i/NUM_ROWS-1;
    up   = i/NUM_ROWS;

    // 2-2-1. Check down link (consider wrap-around)
    if(down<0)
        router[i].FA_link_info[DOWN]=                         // down edge
	  (vlink_status_map[NUM_ROWS-1 ][i%NUM_COLS] == 'H')? HEALTHY:FAIL; 
    else
        router[i].FA_link_info[DOWN]=
	  (vlink_status_map[down       ][i%NUM_COLS] == 'H')? HEALTHY:FAIL;

    // 2-2-2. Check up link
    router[i].FA_link_info[UP]=
          (vlink_status_map[up         ][i%NUM_COLS] == 'H')? HEALTHY:FAIL;
  }
}


