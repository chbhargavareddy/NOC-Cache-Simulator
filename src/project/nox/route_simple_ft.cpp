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
 * Jan. 31, 2006  Version 1.0 released by Dongkook Park 
 *
 */

/* ROUTE_SIMPLE_FT.C - Implements the simple fault-tolerant routing algorithm */

#include <stdlib.h>
#include "main.h"
#include "router_common.h"
#include "route_simple_ft.h"
#include "shared.h"

void dest_in_straight_ft(int dir, int cn, int pc, int vc, int dx, int dy, int *dest_pc, int *dest_vc, int *is_blocked, int *credit)
{
    int node = cn;
    router_t *cur_router = &(router[cn]);
    int rs; // router status of the node in straight direction(dir).
    int i, p1, p2;
    int DN1 = (dir+3)%4; //     left direction relative to the dir.
    int DN2 = (dir+1)%4; //    right direction relative to the dir.
    int bdir = (dir+2)%4; // backward direction relative to the dir.
    int IN1[3] = {dir, DN1, bdir};
    int IN2[3] = {dir, DN2, bdir};

    ////////////////////////////////////////
    // 1. Check forward direction first.
    ////////////////////////////////////////
    
    // Check whether forward node is destination node.
    int is_next_node_dest = (
        (dx ==  1 && dy ==  0 && dir == EAST)  || 
        (dx == -1 && dy ==  0 && dir == WEST)  ||
        (dx ==  0 && dy ==  1 && dir == NORTH) || 
        (dx ==  0 && dy == -1 && dir == SOUTH) 
      )? YES:NO;

    // Check the status of the forward node.
    rs = (neighbor[cn][dir] == EDGE)? FAIL: (router[neighbor[cn][dir]].health_status);
    
    if(rs != FAIL)
    { *dest_pc = dir; return; } // Yes, we can go straight.
    
    ////////////////////////////////////////
    // 2. We cannot go straight. 
    // Check left-side and right-side.
    ////////////////////////////////////////

    // Get path scores.
    p1 = (neighbor[cn][DN1] == EDGE)? FAIL: (router[neighbor[cn][DN1]].health_status);
    p2 = (neighbor[cn][DN2] == EDGE)? FAIL: (router[neighbor[cn][DN2]].health_status);
    
    // If both paths are blocked, we cannot use thme.
    if(p1 == HEALTHY || p2 == HEALTHY)
    {
      // A. If only DN1 is available, select DN1.
      if     (p1 == HEALTHY && p2 == FAIL)    { *dest_pc = DN1; return; }
      
      // B. If only DN2 is available, select DN2.
      else if(p1 == FAIL    && p2 == HEALTHY) { *dest_pc = DN2; return; }
      
      // C. Both left and right directions are available.
      // Check buffer and distance for final routing decision.
      // Under fault-free environment, all routing decisions 
      // should be made here (both p1 and p2 are always available).
      // Here, we don't consider the distance to destination 
      // since they're the same. (Dest is in straight direction 
      // and D1 and D2 are orthogonal to this direction.)
      else { *dest_pc = (credit[DN1] >= credit[DN2])? DN1 : DN2; return; }
    }
    else
    {
      ////////////////////////////////////////
      // 3. All the three directions (left, straight, right) are blocked.
      // If the only left direction(bdir) is also blocked, this flit is 
      // generated within trapped region which should be avoided in simulation.
      // If the bdir is available, that's the only option we have.
      // Go to the backward direction.
      ////////////////////////////////////////
      *dest_pc = bdir;
      return;
    }
}

void dest_in_diagonal_ft(int DN1, int DN2, int cn, int pc, int vc, int dx, int dy, int *dest_pc, int *dest_vc, int *is_blocked, int *credit)
{
    int stage;
    int node = cn;
    int r1, r2; // router status of the nodes in left-side and right-side relative to the dir.
    int i, p1, p2, dist1, dist2;
    int blocked1, blocked2;
    int BDN1 = (DN1+(NUM_PC-NUM_NIC_PC)/2) % (NUM_PC-NUM_NIC_PC);
    int BDN2 = (DN2+(NUM_PC-NUM_NIC_PC)/2) % (NUM_PC-NUM_NIC_PC);
    int IN1[2][3] = { {DN2, DN1, BDN2}, {DN2, BDN1, BDN2}};
    int IN2[2][3] = { {DN1, DN2, BDN1}, {DN1, BDN2, BDN1}};

    router_t *cur_router = &(router[cn]);
    
    for(stage=0; stage<2; stage++)
    {
      p1 = (neighbor[cn][DN1] == EDGE)? FAIL: (router[neighbor[cn][DN1]].health_status);
      p2 = (neighbor[cn][DN2] == EDGE)? FAIL: (router[neighbor[cn][DN2]].health_status);

      // If both paths are blocked, we cannot use thme.
      if(p1 == FAIL  && p2 == FAIL)
      {
	// Switch to the backward directions.
	DN1 = BDN1; DN2 = BDN2;
	continue;
      }
    
      // A. If only DN1 is available, select DN1.
      if     (p1 == HEALTHY && p2 == FAIL)    { *dest_pc = DN1; return; }
      
      // B. If only DN2 is available, select DN2.
      else if(p1 == FAIL    && p2 == HEALTHY) { *dest_pc = DN2; return; }
      
      // C. Both p1 and p2 has same score.
      // Check buffer and distance for final routing decision.
      // Under fault-free environment, all routing decisions 
      // should be made here (both p1 and p2 are always 0).
      // Here, we need to consider the distance to destination 
      // since they might be different.
      else 
      { 
	if     (credit[DN1] > credit[DN2]) *dest_pc = DN1;
	else if(credit[DN1] < credit[DN2]) *dest_pc = DN2;
	else // Compare distance... Here...
	{ 
	  switch(DN1)
	  {
	    case WEST : {dist1 = -dx; break;}
	    case NORTH: {dist1 =  dy; break;}
	    case EAST : {dist1 =  dx; break;}
	    case SOUTH: {dist1 = -dy; break;}
	  }
	  switch(DN2)
	  {
	    case WEST : {dist2 = -dx; break;}
	    case NORTH: {dist2 =  dy; break;}
	    case EAST : {dist2 =  dx; break;}
	    case SOUTH: {dist2 = -dy; break;}
	  }

	  // In stage 0, both DN1 and DN2 are toward the destination.
	  // Thus, choose direction with the longest distance.
	  if(stage == 0)
	    *dest_pc = (dist1 >= dist2)? DN1:DN2; 
	  // In stage 1, both DN1 and DN2 are away from the destination.
	  // Thus, choose direction with the shortest distance.
	  else // stage == 1
	    *dest_pc = (dist1 <= dist2)? DN1:DN2; 
	} 
	return; 
      }// else
      

    }// for stage
}

void simple_ft_route(int cn, int dn, int pc, int vc, int *dest_pc, int *dest_vc)
{
    int cur_x, cur_y, dest_x, dest_y, dx, dy; /* hold the coordinates of the current node */
    int credit[MAX_PC-MAX_NIC_PC], best_vc[MAX_PC-MAX_NIC_PC]; // number of available buffer
    int is_blocked[4];  // blocking state
    int xdir, ydir;

    /* Calculate cur and dest node coordinates */
    calc_coord(cn, &cur_x, &cur_y);
    calc_coord(dn, &dest_x, &dest_y);
    dx = dest_x - cur_x;    // offset to go right (negative value means left)
    dy = dest_y - cur_y;    // offset to go up    (negative value means down)

    ///////////////////////////////////////////////////////////////////////////////
    // Based on the router.FA_direction_info[][], make routing decision          //
    // (set new_pc and new_vc).                                                  //
    ///////////////////////////////////////////////////////////////////////////////

    // Check the maximum number of available buffer of VCs in a PC.
    get_best_credit(cn, credit, best_vc);
    /* Calculate blocked signals */
    get_blocked(cn, is_blocked);

    
    /*
    // 1.5 Check if escape VC.
    // Disable the code between two lines to test 
    // new deadlock recovery scheme.
    //  ------------------------------------------  
    if(topology == MESH)
    {
      xdir  = (dx > 0)? RIGHT : LEFT;    ydir  = (dy > 0)? UP : DOWN;   

      if(vc == 0 && (dx !=0 || dy !=0))
      { 
	*dest_pc = (dx != 0)? xdir : ydir;
	*dest_vc = vc; 
	return; 
      }
    }
    else if(topology == TORUS)
    {
      if(vc < 2)
      { 
	*dest_pc = (dx != 0)? xdir : ydir;
	*dest_vc = vc; 
	return;
      }
    }
    //  ------------------------------------------  
   */ 

    // 1. Ejection at this node.
    if(dx == 0 && dy == 0)
      *dest_pc = THIS; // PC for EJECTION

    // 2. Go somewhere...
    else if(dx == 0 && dy  > 0)   dest_in_straight_ft(NORTH,       cn, pc, vc, dx, dy, dest_pc, dest_vc, is_blocked, credit);
    else if(dx  > 0 && dy  > 0)	  dest_in_diagonal_ft(NORTH, EAST, cn, pc, vc, dx, dy, dest_pc, dest_vc, is_blocked, credit);
    else if(dx  > 0 && dy == 0)	  dest_in_straight_ft(EAST,        cn, pc, vc, dx, dy, dest_pc, dest_vc, is_blocked, credit);
    else if(dx  > 0 && dy  < 0)	  dest_in_diagonal_ft(EAST, SOUTH, cn, pc, vc, dx, dy, dest_pc, dest_vc, is_blocked, credit);
    else if(dx == 0 && dy  < 0)	  dest_in_straight_ft(SOUTH,       cn, pc, vc, dx, dy, dest_pc, dest_vc, is_blocked, credit);
    else if(dx  < 0 && dy  < 0)	  dest_in_diagonal_ft(SOUTH, WEST, cn, pc, vc, dx, dy, dest_pc, dest_vc, is_blocked, credit);
    else if(dx  < 0 && dy == 0)	  dest_in_straight_ft(WEST,        cn, pc, vc, dx, dy, dest_pc, dest_vc, is_blocked, credit);
    else                      	  dest_in_diagonal_ft(WEST, NORTH, cn, pc, vc, dx, dy, dest_pc, dest_vc, is_blocked, credit);

    // If no VCs have available buffer (best_vc[cur_dir] == -1), use the same VC as before.
    *dest_vc = (best_vc[*dest_pc] != -1)? best_vc[*dest_pc] : vc;
}

