
#include "main.h"
#include "router_common.h"
#include "route_north_last.h"
#include "shared.h"

void north_last_route(int cn, int dn, int pc, int vc, int *dest_pc, int *dest_vc)
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
  if( xdist > 0) { *dest_pc = xdir ;  *dest_vc = vc; return; }// WEST (=LEFT) direction should be taken first at all times.
  else if(xdist == 0)   { *dest_pc = ydir; *dest_vc = vc; return; } // Only y-directional hops are left.
   
  else // (xdist !=0 && ydist !=0)
  {
    if(xdist > ydist)
    {
      if(credit[xdir] > 0)      cur_dir = xdir;
      else if(credit[ydir] > 0 && ydir == DOWN) cur_dir = ydir;
      else                      cur_dir = xdir;
    }
    else if(xdist < ydist)
    {
      if(credit[ydir] > 0 && ydir == DOWN )      cur_dir = ydir;
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

