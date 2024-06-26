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

/* FLIT.C - flit allocation */

#include <stdio.h>
#include <stdlib.h>
#include "router.h"
#include "main.h"
#include "flit.h"
#include "shared.h"
#include "router_common.h"


flit_t * flit_alloc(void)
{
  flit_t *retval;

  if(free_list_head != NULL)
  {
    retval = free_list_head;
    free_list_head = free_list_head->next_free;
  }
  else
  {
    retval = NULL;
    printf("Mem allocation failed\n");
    fflush(stdout);
  }
  
  if(free_list_head == NULL)
    free_list_tail = NULL;

  return retval;
}

void flit_free(flit_t *flit_ptr)
{
  if(flit_ptr == NULL) 
  {
    fprintf(stderr, "flit_free: bad flitptr %p\n", flit_ptr);
    abort();
  }
  else 
  {
    flit_ptr->next_free = NULL;
   
    if(free_list_tail != NULL) 
    {
      free_list_tail->next_free = flit_ptr;
      free_list_tail = flit_ptr;
    }
    else
      free_list_head = free_list_tail = flit_ptr;
  }// else

  return;
}
      
void init_free_list(void)
{
  flit_t *flit_store;
  int i;

  /* flit_alloc.h:#define FLIT_POOL_SZ 1000000L */
  flit_store = (flit_t *)calloc(FLIT_POOL_SZ, sizeof(flit_t));

  if(flit_store == NULL) 
  {
    perror("init_free_list");
    exit(1);
  }
  else 
  {
    free_list_head = flit_store;
    for(i=0; i<FLIT_POOL_SZ; i++)
      flit_store[i].next_free = (flit_store + i + 1);
      
    flit_store[FLIT_POOL_SZ - 1].next_free = NULL;
    free_list_tail = flit_store + FLIT_POOL_SZ - 1;
  }
    
  return;
}

void make_body_flit(flit_t *flit_ptr, flit_t *src_ptr)
{
 // Set common information
        flit_ptr->inj_tm        = src_ptr->inj_tm; 
        flit_ptr->msg_inj_tm    = src_ptr->msg_inj_tm;    
        flit_ptr->vc_num        = src_ptr->vc_num;
        flit_ptr->error         = src_ptr->error;
        flit_ptr->error_penalty = src_ptr->error_penalty;
        flit_ptr->e2e_penalty   = src_ptr->e2e_penalty;
        flit_ptr->blk_chk_done  = src_ptr->blk_chk_done;
        flit_ptr->pos           = src_ptr->pos+1;
        flit_ptr->flit_num      = src_ptr->flit_num+1;
        flit_ptr->delay_cycle   = src_ptr->delay_cycle;
        flit_ptr->entry_time    = src_ptr->entry_time;
        flit_ptr->num_hop       = src_ptr->num_hop;
        flit_ptr->msglen        = src_ptr->msglen;
        flit_ptr->llen          = src_ptr->llen;
        flit_ptr->msgtype       = src_ptr->msgtype;
        flit_ptr->is_nack       = src_ptr->is_nack;
	flit_ptr->data.inj_pc   = src_ptr->data.inj_pc;
#ifdef TR_INTEG
        flit_ptr->trid          = src_ptr->trid;
#endif
        flit_ptr->flit_type     = MIDL;
        flit_ptr->data.snode    = src_ptr->data.snode;
        flit_ptr->data.dnode    = src_ptr->data.dnode;
        flit_ptr->msgtype       = src_ptr->msgtype;
        flit_ptr->is_sx_less_than_dx = src_ptr->is_sx_less_than_dx;
        flit_ptr->is_sy_less_than_dy = src_ptr->is_sy_less_than_dy; 
        flit_ptr->data.ejt_pc = src_ptr->data.ejt_pc; 
        if(xshare == YES)
          flit_ptr->short_flit = ((double)(rand() / (double)RAND_MAX)) < short_flit_ratio ? YES:NO;
        else
          flit_ptr->short_flit = NO;
}
