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

/* ROUTE_WEST_FIRST.C - Implements the Partially Adaptive West-First routing algorithm */


#include "main.h"
#include "router_common.h"
#include "route_west_first.h"
#include "shared.h"

void west_first_route(int cn, int dn, int pc, int vc, int *dest_pc, int *dest_vc)
/* returns the pc to go out */
{
  int credit[MAX_PC-MAX_NIC_PC], best_vc[MAX_PC-MAX_NIC_PC]; // number of available buffer
  int is_blocked[4];                          // blocking state
  int cx, cy, dx, dy, hx, hy;                 // hold the coordinates of the current node 
  int xdir, ydir, xdist, ydist, cur_dir;      // the direction the packet is moving in 

  // Calculate cur and dest node coordinates 
  calc_coord(cn, &cx, &cy);
  calc_coord(dn, &dx, &dy);
  hx = NUM_COLS / 2; hy = NUM_ROWS / 2;
  get_best_credit(cn, credit, best_vc); // Check the maximum number of available buffer of VCs in a PC.
  get_blocked(cn, is_blocked);         // Calculate blocked signals 
  cur_dir = -1;

  // Calculate xdir, ydir, xdist, ydist
  if(topology == MESH)
  {
    xdir  = (dx > cx)? RIGHT : LEFT;    ydir  = (dy > cy)? UP    : DOWN;   
    xdist = (dx > cx)? dx-cx : cx-dx;   ydist = (dy > cy)? dy-cy : cy-dy;
  }// if MESH
  else if(topology == TORUS)
  {
    xdir  = (dx > cx)? ((dx-cx <= hx)? RIGHT : LEFT)             : ((cx-dx< hx)? LEFT  : RIGHT);
    ydir  = (dy > cy)? ((dy-cy <= hy)? UP    : DOWN)             : ((cy-dy< hy)? DOWN  : UP   );
    xdist = (dx > cx)? ((dx-cx <= hx)? dx-cx : NUM_COLS-(dx-cx)) : ((cx-dx< hx)? cx-dx : NUM_COLS-(cx-dx) );
    ydist = (dy > cy)? ((dy-cy <= hy)? dy-cy : NUM_COLS-(dy-cy)) : ((cy-dy< hy)? cy-dy : NUM_COLS-(cy-dy) );
  }// if TORUS

  //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
  // Now, make routing decision.
  if(dn == cn) { *dest_pc = THIS; *dest_vc = vc; return; }// Reached the destination
  
  if(xdir == LEFT && xdist > 0) { *dest_pc = LEFT;  *dest_vc = vc; return; }// WEST (=LEFT) direction should be taken first at all times.
  else if(ydist == 0)   { *dest_pc = RIGHT; *dest_vc = vc; return; }// Since only East directional hops are left, take EAST (=RIGHT).
  else if(xdist == 0)   { *dest_pc = ydir; *dest_vc = vc; return; } // Only y-directional hops are left. 
  else // (xdist !=0 && ydist !=0)
  {
    if(xdist > ydist)
    {
      if(credit[xdir] > 0)      cur_dir = xdir;
      else if(credit[ydir] > 0) cur_dir = ydir;
      else                      cur_dir = xdir;
    }
    else if(xdist < ydist)
    {
      if(credit[ydir] > 0)      cur_dir = ydir;
      else if(credit[xdir] > 0) cur_dir = xdir;
      else                      cur_dir = ydir;
    }
    else // (xdist == ydist)
      cur_dir = (credit[xdir] >= credit[ydir])? xdir:ydir;

    *dest_pc = cur_dir; 
    *dest_vc = vc;

  return;
  }
}

