#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <zlib.h>
#include <vector>
#include <algorithm>
#include "router.h"
#include "main.h"
#include "nic.h"
#include "link.h"
#include "app.h"
#include "router_common.h"
#include "sim_result.h"
#include "string.h"
#include "defines.h"
#include "shared.h"
#include "gsf.h"

#ifdef TR_INTEG
#undef INVALID
#include "processor.h"
#include "globals.h"

#undef INVALID
#define INVALID 1

#endif
int GSF_OS_Weights_12[64] = { 
 8,  8,  8,  8,  8,  8,  8,  8, 
 8,  8,  8,  8,  8,  8,  8,  8, 
 8,  8,  8,  8,  8,  8,  8,  8, 
 8,  8,  8,  8,  8,  8,  8,  8, 
24, 24, 24, 24, 24, 24, 24, 24, 
24, 24, 24, 24, 24, 24, 24, 24, 
24, 24, 24, 24, 24, 24, 24, 24, 
24, 24, 24, 24, 24, 24, 24, 24 
}; 

int GSF_OS_Weights_1228[64] = { 
 5,  5,  5,  5,  5,  5,  5,  5, 
10, 10, 10, 10, 10, 10, 10, 10, 
10, 10, 10, 10, 10, 10, 10, 10, 
40, 40, 40, 40, 40, 40, 40, 40, 
 5,  5,  5,  5,  5,  5,  5,  5, 
10, 10, 10, 10, 10, 10, 10, 10, 
10, 10, 10, 10, 10, 10, 10, 10, 
40, 40, 40, 40, 40, 40, 40, 40
}; 

int GSF_OS_Weights_1to8[64] = { 
 4,  4,  4,  4,  4,  4,  4,  4, 
 7,  7,  7,  7,  7,  7,  7,  7, 
11, 11, 11, 11, 11, 11, 11, 11, 
14, 14, 14, 14, 14, 14, 14, 14, 
18, 18, 18, 18, 18, 18, 18, 18, 
21, 21, 21, 21, 21, 21, 21, 21, 
25, 25, 25, 25, 25, 25, 25, 25, 
28, 28, 28, 28, 28, 28, 28, 28, 
}; 

long GSFAbsoluteFrames=0;

#ifndef TR_INTEG
long long global_clock;
#endif

int mbox_count_frame_flits(mbox_t *mbox,int FrameId)
{
  mbox_item_t *tmp;
  int sum=0;

  flit_t *flit_ptr;
  tmp = mbox->next;
  while(tmp != NULL)
  {
    flit_ptr = tmp->ptr;
    if(flit_ptr->frame_id == FrameId)
      sum++;
    tmp = tmp->next;
  }
  return sum;
}

bool mbox_head_frame_present(mbox_t *mbox,int flowId)
{
  mbox_item_t *tmp;
  flit_t *flit_ptr;
  tmp = mbox->next;
  while(tmp != NULL)
  {
    flit_ptr = tmp->ptr;
    if(flit_ptr->frame_id == GSFHeadFrameId[flowId])
      return true;
    tmp = tmp->next;
  }
  return false;
}

int CountFrameFlits(int FrameId)
{
  int node, pc, vc;
  int sum=0;

  for(node=0; node<NUM_NODES; node++)
  {
    NUM_PC     = router_info[node].num_pc;
    NUM_NIC_PC = router_info[node].num_nic_pc;
    NUM_VC     = router_info[node].num_vc;
    
    for(pc=0; pc<NUM_PC; pc++)
      for(vc=0; vc<NUM_VC; vc++)
      {
        sum +=mbox_count_frame_flits(&(router_input_buf[node][pc][vc]),FrameId);
        if(pc < NUM_NIC_PC)
          sum += mbox_count_frame_flits(&(nic_output_buf[node][pc][vc]),FrameId);
      }//vc & pc
  }//node

  return sum;
}

void FlitCount()
{
  for(int node=0; node<NUM_NODES; node++)
  {
    printf("%.2fK ", GSFFlitCount[node]/1000);
    if((node+1)%NUM_COLS == 0)
      printf("\n");
  }
}

bool HeadFrameActive(int flowId)
{
  int node, pc, vc;

  for(node=0; node<NUM_NODES; node++)
  {
    NUM_PC     = router_info[node].num_pc;
    NUM_NIC_PC = router_info[node].num_nic_pc;
    NUM_VC     = router_info[node].num_vc;
    
    for(pc=0; pc<NUM_PC; pc++)
      for(vc=0; vc<NUM_VC; vc++)
      {
        if(mbox_head_frame_present(&(router_input_buf[node][pc][vc]),flowId))
          return true;
      
        if(pc < NUM_NIC_PC)
          if(mbox_head_frame_present(&(nic_output_buf[node][pc][vc]),flowId))
              return true;
      }//vc & pc
  }//node

  return false;
}


void UpdateHeadFrame()
{
  int flowId;
  static long long GSFBarrierDelay = -1;
  static int CurrentHeadFrame=-1;
  static int GSFEpoch = 10000;

	global_clock = sim_clock;
  //Debug
  //if(global_clock == 9720)
    //verbose = YES;

  int now = GSFHeadFrameId[0];
  //Debug
  if(verbose == YES)
  {
    for(int i=0; i<NUM_NODES; i++)
      //for(int j=0; j<NUM_NODES; j++)
      {
        //flowId = i*NUM_NODES + j;
        flowId = i;
        if(GSFHeadFrameId[flowId] != now)
          printf("Head frames not matching flow:%d HF:%d now:%d!!! @clock %lld\n",flowId,GSFHeadFrameId[flowId],
              now,global_clock);
      }
  }
  //Debug End
  
  if(HeadFrameActive(0) == false && GSFBarrierDelay == -1)
  //if(!(global_clock % GSFEpoch))
  {
    GSFBarrierDelay = global_clock + GSFBarrierPenalty;
    CurrentHeadFrame = GSFHeadFrameId[0];
    if(verbose == YES)
    {
      printf("HeadFrame %d Done will update @%lld cycles!!\n", CurrentHeadFrame,GSFBarrierDelay);
      for(int f=0; f< GSFWindowSize; f++)
        printf("Frame[%d] has %d flits now is Head(%s)\n",f, CountFrameFlits(f), f==CurrentHeadFrame ? "yes" : "no");
    }
  }

  if(GSFBarrierDelay != -1 && GSFBarrierDelay == global_clock)
  {
     //DBGRMV
    //if(verbose == YES)
    if(GSFAbsoluteFrames%100 == 0)
    {
      printf("Updating HeadFrame to %d @%lld AbsoluteFrameNumber %lld!!\n", (CurrentHeadFrame+1)%GSFWindowSize,global_clock,GSFAbsoluteFrames);
      FlitCount();
    }
    GSFAbsoluteFrames++;

    if(verbose == YES)
    {
      for(int f=0; f< GSFWindowSize; f++)
        printf("Frame[%d] has %d flits now is Head(%s)\n",f, CountFrameFlits(f), f==CurrentHeadFrame ? "yes" : "no");
    }

    for(int i=0; i<NUM_NODES; i++)
      //for(int j=0; j<NUM_NODES; j++)
      {
        //flowId = i*NUM_NODES + j;
        flowId = i;

        //Debug
        if(verbose == YES)
          if(HeadFrameActive(flowId))
            printf("Head frame should not be active [%d]!!! @clock %lld\n",flowId,global_clock);
        //Debug End

        GSFHeadFrameId[flowId] = (GSFHeadFrameId[flowId] + 1) % GSFWindowSize;
        //printf("Updating HeadFrame for flow:%d to %d\n",flowId,GSFHeadFrameId[flowId]);
        if(GSFInjectionFrameId[flowId] == GSFHeadFrameId[flowId])
        {
          GSFInjectionFrameId[flowId] = (GSFInjectionFrameId[flowId] + 1) % GSFWindowSize;
          GSFCredit[flowId] = min(GSFReservedCredit[flowId] , GSFCredit[flowId] + GSFReservedCredit[flowId]);
          if(verbose == YES)
            printf("Updating InjectionFrame for flow:%d to %d because of HeadFrame conflict\n",flowId,GSFInjectionFrameId[flowId]);
        }
      }
    GSFBarrierDelay = -1;
  }

  //Debug
  //if(global_clock == 9720)
    //exit(1);
}

int FindFrameNumber(int flowId, int packetSize)
{
  int FrameNum = 0;
 if(GSFCredit[flowId] > 0)
 {
   GSFCredit[flowId] -= packetSize;
   return GSFInjectionFrameId[flowId]; 
 }
 else
 {
   if(verbose == YES)
     printf("Run out of credit Updating InjectionFrame for flow:%d current IF:%d HF:%d\n",flowId,GSFInjectionFrameId[flowId],GSFHeadFrameId[flowId]);
   while((GSFInjectionFrameId[flowId] + 1) % GSFWindowSize != GSFHeadFrameId[flowId] && GSFCredit[flowId] < 0)
   {
     GSFCredit[flowId] += GSFReservedCredit[flowId];
     GSFInjectionFrameId[flowId] = (GSFInjectionFrameId[flowId] + 1) % GSFWindowSize;
     //DBGRMV
     if(verbose == YES)
     //if(flowId == 8)
     printf("Updating InjectionFrame for flow:%d to %d\n",flowId,GSFInjectionFrameId[flowId]);
   }
   if(GSFCredit[flowId] > 0)
   {
     GSFCredit[flowId] -= packetSize;
     return GSFInjectionFrameId[flowId]; 
   }
 }
 return -1;

}
int calcGSFPriority(int FrameNum, int Node)
{
  int pri = FrameNum - GSFHeadFrameId[Node];
  if(FrameNum < GSFHeadFrameId[Node])
    pri += GSFWindowSize;
  return GSFWindowSize - pri - 1;
}

void initGSF(int Framesize, int WindowSize)
{

  int flowId, credits;
  for(int i=0; i<NUM_NODES; i++)
    //for(int j=0; j<NUM_NODES; j++)
    {
      //flowId = i*NUM_NODES + j;
      flowId = i;//i*NUM_NODES + j;
      //credits = Framesize*GSF_OS_Weights_1to8[flowId];
      //credits = Framesize*GSF_OS_Weights_1228[flowId];
      //credits = GSF_OS_Weights_12[flowId];
      credits = Framesize*1000/NUM_NODES;
      GSFInjectionFrameId[flowId] = 1;
      GSFHeadFrameId[flowId] = 0;
      GSFCredit[flowId] = credits;
      GSFReservedCredit[flowId] = credits;
         
    }
  GSFWindowSize = WindowSize;
  GSFBarrierPenalty = 16;
}
