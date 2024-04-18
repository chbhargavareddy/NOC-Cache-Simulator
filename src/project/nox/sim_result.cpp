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

/* SIM_RESULT.C - Print out the simulation result */


#include "main.h"
#include "router.h"
#include "nic.h"
#include "sim_result.h"
#include "shared.h"

#ifdef TR_INTEG
extern long long sim_start_clock;
#endif


void sim_result()
{
  //num_ejt_flit = control_ejt_msg;

  double cycle_time;
  long long total_num_msgs;

  double rt_leakage=0.0, nic_leakage=0.0;
  double total_energy=0;
  double avg_power=0.0; 
  double link_power=0.0; 
  double switch_power=0.0; 
  double buffer_power=0.0; 

  long long   total_link_usage=0, total_bus_usage=0, num_used_link=0, total_nic_buf_size;
  double link_throughput=0.0, bus_throughput=0.0;

  long long buf_usage=0, retrans_buf_usage=0;
  double buf_usage_ratio=0, retrans_buf_usage_ratio=0;

  int node, pc, vc, row, col;

  // Link throughput related calculation
  for(node=0; node<NUM_NODES; node++)
  {
    NUM_PC     = router_info[node].num_pc;
    NUM_NIC_PC = router_info[node].num_nic_pc;
    NUM_VC     = router_info[node].num_vc;
    if(router_info[node].type == GLOBAL)
      NUM_NIC_PC++;
    if(router_info[node].type == LOCAL)
     for(pc=0; pc<NUM_PC; pc++)
      total_bus_usage += link_usage[node][pc];
    
    if(router_info[node].type != LOCAL)
    for(pc=0; pc<NUM_PC-NUM_NIC_PC; pc++)
    {
      total_link_usage += link_usage[node][pc];
      if(neighbor[node][pc])
        num_used_link++;
    }
    
  }

  // link_usage is counted only after warm-up period (See main.c).
  link_throughput = (double)total_link_usage / 
    ( (double)(num_used_link) * (double)(sim_clock-warmup_cycle) );
  if(hybrid_topology == YES)
    bus_throughput = (double)total_bus_usage / 
      ( (double)(NUM_PE/concentration_degree) * (double)(sim_clock-warmup_cycle) );


  // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
  // 2. Power Calculation

  cycle_time = (arch == 2)? CYCLE_TIME2 : CYCLE_TIME3;
  total_num_msgs = /*(float)*/(num_ejt_msg + WARMUP_MSG); 

  // The power parameter values specified in the power.h file are "POWER" values. 
  // In other words, those values indicate the energy consumption per second (mW/s).
  // To measures the energy/power consumption, the simulator accumulates the ENERGY
  // consumption of each router module using the variables that begin with 'p_' as 
  // show below.
  // 
  // However, the number we have is "POWER" number, not "ENERGY" number. Therefore, 
  // each power parameter value should be multiplied with the cycle_time to get the
  // energy consumption of each module before they're added to the variables. 
  //
  //    POWER(W) = ENERGY(J) / TIME(s), and thus, ENERGY(J) = POWER(W) * TIME(s).
  // 
  // For example, if we assume that the PD_ROUTER (power consumption of the routing 
  // logic) is 4.9462(mW), it means that the routing logic consumes 4.9462 (mW) per
  // one second. What we need is the energy consumption during one cycle and therefore
  // 
  //    ENERGY = POWER * TIME = 4.9462 * cycle_time (J).
  // 
  // We can do this exact calculation at each point where the values are added, but
  // the multiplication of the cycle_time is common to all calculations and thus 
  // can be done only once at the end to simplify the calculation.
  // Thus, we don't multiply the cycle_time and just accumulate the "POWER" number
  // during the simulation and multiply the cycle_time here. This is why you see
  // the '* cycle_time' at the end of calculation below.
  //
  // The unit of the power number is 'mW' and cycle_time is 'ns' and therefore, the
  // energy is in 'mW*ns' = 'pW*s' = 'pJ'.
  // 
  //FIXME note the new pd numbers are in pJ 
  total_energy += (p_buffer + p_routing + p_vc_arb + p_sw_arb + p_xbar + 
      p_link + p_err_chk + p_retrans) /** cycle_time*/;

  // Calculate Leakage power for a node for a single cycle.
  rt_leakage = PL_BUFFER*ROUTER_INPUT_BUF_SIZE*NUM_PC*NUM_VC +           // Buffer leakage
    //PL_SW_ARB +                                                          // Arbiter logic leakage
    PL_ROUTER +                                                          // Routing logic leakage
    PL_XBAR_F +                                                          // Xbar leakage
    ((retrans_type==FEC || retrans_type ==HFEC)? 2*PL_SECDED : 0) +      // FEC leakage
    ((retrans_type==E2E)? 2*PL_CRCCHK + PL_E2ECTR +                      // E2E retransmission leakage
     E2E_SRC_BUF_SIZE*PL_E2EBUF*NUM_VC*NUM_PC:0) +                       // E2E src buffer leakage 
    ((retrans_type==HBH || retrans_type==HFEC || retrans_type==HE2E)? 
     PL_CRCCHK*2 + PL_HBHCTR +                                           // HBH retransmission leakage 
     5*PL_HBHBUF*NUM_VC*NUM_PC:0) +                                      // Additional HBH buffer(5 flits) leakage
     PL_LINK*num_used_link;                                               // Link leakage

  // Calculate total NIC buffer size
  total_nic_buf_size = NIC_INPUT_BUF_SIZE + NIC_OUTPUT_BUF_SIZE; // Injection/Ejection buffer
  nic_leakage = PL_BUFFER * total_nic_buf_size; 

  //FIXME Currently Ignore leakage Power/Energy
  // Add leakage energy consumption to the total energy consumption.
  //total_energy += ((rt_leakage + nic_leakage)*NUM_NODES) * cycle_time;

  // Calculate average power consumption
#ifdef TR_INTEG
  sim_clock = sim_clock - sim_start_clock;
#endif
  int num_routers = hybrid_topology == YES ? NUM_NODES/2 : NUM_NODES;  
  avg_power = (total_energy + pl_static_energy*num_routers) / (cycle_time*(double)sim_clock);
  link_power = (p_link) / (cycle_time*(double)sim_clock);
  switch_power = (p_xbar) / (cycle_time*(double)sim_clock);
  buffer_power = (p_buffer) / (cycle_time*(double)sim_clock);

  //  Power Calculation End
  // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
  // Calculate average buffer utilization.

  buf_usage = 0;
  for(node=0; node<NUM_NODES; node++)
  {
    row = node / NUM_COLS;
    col = node % NUM_COLS;

    if( (row > 0 && row < NUM_ROWS-1) &&
        (col > 0 && col < NUM_ROWS-1) ) 
      buf_usage += /*(float)*/(total_buf_usage[node]);
  }

  //buf_usage_ratio = buf_usage/(double)(((NUM_ROWS-2)*(NUM_COLS-2))*NUM_PC*NUM_VC*ROUTER_INPUT_BUF_SIZE*(sim_clock-warmup_cycle));
  buf_usage_ratio = buf_usage/(double)(NUM_NODES*NUM_PC*NUM_VC*ROUTER_INPUT_BUF_SIZE*(sim_clock-warmup_cycle));

  // Calculate average HBH retransmission buffer utilization.
  retrans_buf_usage = 0;
  retrans_buf_usage_ratio = 0;
  for(node=0; node<NUM_NODES; node++)
    retrans_buf_usage += /*(float)*/(total_retrans_buf_usage[node]);
  retrans_buf_usage_ratio = retrans_buf_usage / (double)(NUM_NODES*NUM_PC*NUM_VC*ROUTER_INPUT_BUF_SIZE*sim_clock);
  retrans_buf_usage_ratio *= 3; // Multiply by 3 since each flit stays in the retransmission buffer for 3 cycles.

  // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
  // Show the result of the simulation.
  printf("\n");
  printf("============ Summary of the simulation ============\n");
  printf("- Total simulation cycle        : %lld cycles\n",             (long long)sim_clock);
  printf("- Total Aged Flits (10~20/>20)  : %5d/%5d flits\n",           total_aged_flit10, total_aged_flit20);
  printf("- # ejected msg / # injected msg: %5d/%5d msgs\n",            num_ejt_msg, num_inj_msg);
  printf("- # ejected ft / # injected ft  : %5d/%5d flits\n",            num_ejt_flit, num_inj_flit);
  printf("- Injection Rate (Actual/Set)   : %5.3f%% / %5.3f%% for nodes:(%d/%d)\n",       (double)(num_inj_msg*100)/(double)(NUM_PE*sim_clock), load_rate*100, NUM_PE, NUM_NODES);
  printf("- Avg. Throughput               : %13.4lf packets/cycles \n",   (double)(num_ejt_msg)/(double)(sim_clock));
  printf("- Avg. Message Latency          : %13.6f cycles \n",          total_packet_delay_cycle / (double)(num_ejt_msg));
  printf("- Avg. Queueing Latency          : %13.6f cycles \n",          total_packet_queue_cycle / (double)(num_ejt_msg));
  printf("- Avg. Bus Latency              : %13.6f cycles \n",          total_bus_delay_cycle / (double)(num_ejt_msg));
  printf("- Avg. Flit Latency             : %13.6f cycles \n",          total_latency          / (double)(num_ejt_flit));
  printf("- Avg. Flit Queueing Latency    : %13.6f cycles \n",          total_queueing_latency / (double)(num_ejt_flit));
  printf("- Avg. Flit Blocking Latency    : %13.6f cycles \n",          total_delay_cycle      / (double)(num_ejt_flit));
  if(hybrid_topology == YES)
  printf("- Avg. Flit Bus(Blocking) Latency: %13.6f cycles \n",         bus_delay_cycle      / (double)(num_ejt_flit));
  printf("- Avg. Flit Transfer Latency    : %13.6f cycles \n",          (total_network_latency - total_delay_cycle) / (double)(num_ejt_flit));
  printf("- Avg. Flit Network  Latency    : %13.6f cycles \n",          total_network_latency  / (double)(num_ejt_flit));
  // When we calculate energy and power, we include messages during the warm-up stage too.
  printf("- Total Energy Consumption           : %13.6f nJ(10^-9)\n",        total_energy/1000); 
  /*if(hybrid_topology == YES)
  {
    printf("- Total Link Energy Local            : %13.6f nJ(10^-9) (xfers :%lld msgs :%lld)\n",         l_link/1000, l_xfers, l_msgs); 
    printf("- Total Link Energy Global           : %13.6f nJ(10^-9) (xfers :%lld msgs :%lld hop:%.2lf)\n", g_link/1000, g_xfers, num_ejt_msg - l_msgs, g_total_num_hop/(double)(num_ejt_msg - l_msgs)); 
  }*/
  printf("- Total Buffer Energy Consumption    : %13.6f nJ(10^-9)\n",         p_buffer/1000); 
  printf("- Total Switching Energy Consumption : %13.6f nJ(10^-9)\n",         p_xbar/1000); 
  printf("- Total Link Energy Consumption      : %13.6f nJ(10^-9)\n",         p_link/1000); 
  printf("- Total Arbiter Energy Consumption   : %13.6f nJ(10^-9)\n",      (p_sw_arb+p_vc_arb)/1000); 
  printf("- Avg. Energy per message       : %13.6f nJ(10^-9)\n",       (total_energy/1000) / total_num_msgs); 
  printf("- Avg. Energy Delay Product     : %13.6f nJ*cycle\n",        ((total_energy/1000) / num_ejt_msg) * ((double)total_packet_delay_cycle /(double)(num_ejt_msg)));
  printf("  (=Total Energy * Avg. Latency)\n");     
  printf("- Buffer Power consumption      : %13.6f W\n",               buffer_power);
  printf("- Switch Power consumption      : %13.6f W\n",               switch_power);
  printf("- Link Power consumption        : %13.6f W\n",               link_power);
  printf("- Avg. Power consumption        : %13.6f W\n",               avg_power);
  printf("- Power Delay Product           : %13.6f W*cycle\n",         avg_power  * ((double)total_packet_delay_cycle / (double)(num_ejt_msg)));
  printf("  (=Avg. Power * Avg. Latency)\n");     
  if(link_err_rate >0)      
    printf("- Link Error Rate               : %13.6f %% (%d detected), E2E NACK/RETRANS:[%d/%d]\n", link_err_rate * 100, num_link_err, num_e2e_nack, num_e2e_retrans);
  if(routing_err_rate >0)   
    printf("- Routing Error Rate            : %13.6f %% (%d detected)\n", routing_err_rate * 100, num_routing_err);
  if(swarbiter_err_rate >0) 
    printf("- Switch Arbiter Error Rate     : %13.6f %% (%d detected)\n", swarbiter_err_rate * 100, num_swarbiter_err);
  printf("- Avg. Response Time            : %13.6f ns     \n",          (total_latency          / (double)(num_ejt_flit)) * cycle_time);
  printf("- Avg. Network Response Time    : %13.6f ns     \n",          (total_network_latency  / (double)(num_ejt_flit)) * cycle_time);
  printf("- Avg. Number of hops per packet  : %13.6f hops   \n",          (double)total_num_hop    / (double)(num_ejt_msg));
  printf("- Avg. Link Throughput(%lld)      : %13.6f flits/node/cycle\n", num_used_link,link_throughput);
  printf("- Avg. Bus  Throughput(%lld)      : %13.6f flits/node/cycle\n", NUM_PE/concentration_degree,bus_throughput);
  printf("- Avg. Buffer Utilization       : %13.6f %%\n",               buf_usage_ratio*100);
  printf("- Avg. HBH Buffer Utilization   : %13.6f %%\n",               retrans_buf_usage_ratio*100);
  printf("- Total number of escape msgs   : %d \n",                     num_escape_msgs);
  // Control Msg Latencies
  printf("- Avg. Control Network Blocking Latency : %13.6f cycles \n",          cmsg_total_delay_cycle      / (double)(control_ejt_flit));
  printf("- Avg. Control Latency (per flit)       : %13.6f cycles \n",          cmsg_total_flit_latency     / (double)(control_ejt_flit));
  printf("- Avg. Control Latency (per msg)        : %13.6f cycles \n",          cmsg_total_msg_latency      / (double)(control_ejt_msg));
  printf("- Avg. Control Queueing Latency         : %13.6f cycles \n",          cmsg_total_queueing_latency / (double)(control_ejt_flit));
  printf("- Avg. Control Network Latency          : %13.6f cycles \n",          cmsg_total_network_latency  / (double)(control_ejt_flit));
  printf("- Avg. Data Network Blocking Latency    : %13.6f cycles \n",          dmsg_total_delay_cycle      / (double)(data_ejt_flit));
  printf("- Avg. Data Latency (per flit)          : %13.6f cycles \n",          dmsg_total_flit_latency     / (double)(data_ejt_flit));
  printf("- Avg. Data Latency (per msg)           : %13.6f cycles \n",          dmsg_total_msg_latency      / (double)(data_ejt_msg));
  printf("- Avg. Data Queueing Latency            : %13.6f cycles \n",          dmsg_total_queueing_latency / (double)(data_ejt_flit));
  printf("- Avg. Data Network Latency             : %13.6f cycles \n",          dmsg_total_network_latency  / (double)(data_ejt_flit));
  // Traffic Patterns 

  printf("- Total number of Control msgs          : %lld \n",                     (long long)control_ejt_flit);
  printf("- Total number of Data msgs             : %lld \n",                     (long long)(num_ejt_msg - control_ejt_flit));

  printf("- Total number of one hop flits         : %lld \n",                     flits_one_hop);
  printf("- Total number of two hop flits         : %lld \n",                     flits_two_hop);
  printf("- Total number of three hop flit        : %lld \n",                     flits_three_hop);
  printf("- Total number of three plus hop flit   : %lld \n",                     flits_three_plus_hop);
  if(xshare == YES) 
    printf("- Ratio of candidates found             : %.2lf\n",                     msg_combine_candidate_ratio);
  if(hybrid_topology == YES)
    printf("- Ratio of short flits                  : %.2lf\n",                     (1/mratio)*(short_flits_sum/(double)(num_ejt_flit)));
  else
    printf("- Ratio of short flits                  : %.2lf\n",                     short_flits_sum/(double)(num_ejt_flit));

  printf("===================================================\n");
  printf("- Number of buffer reads                  : %d \n",              num_buf_reads);
  printf("- Number of buffer writes                 : %d \n",              num_buf_writes);
  printf("- Number of VA used                       : %d \n",              num_va_used);
  printf("- Number of SA used                       : %d \n",              num_sa_used);
  printf("- Number of crossbar used                 : %d \n",              num_xbar_used);
  printf("- Number of link 1 used                   : %d \n",              num_link_used[0]);
  printf("- Number of link 2 used                   : %d \n",              num_link_used[1]);
  printf("- Number of link 3 used                   : %d \n",              num_link_used[2]);
  printf("- Number of link 4 used                   : %d \n",              num_link_used[3]);
  printf("- Number of link 5 used                   : %d \n",              num_link_used[4]);
  printf("- Number of link 6 used                   : %d \n",              num_link_used[5]);
  printf("- Number of link 7 used                   : %d \n",              num_link_used[6]);
  printf("===================================================\n");


  /*
  // Print out detailed node utilization.
  printf("Node Utilization...\n");
  for(node=0; node<NUM_NODES; node++)
  {
  printf("%f", (float)(total_activity[node])/(float)(sim_clock-warmup_cycle));
  if( (node % NUM_COLS) == NUM_COLS-1)
  printf("\n");
  else
  printf(", ");
  }

  // Print out detailed Buffer Occupancy.
  printf("Buffer Utilization...\n");
  for(node=0; node<NUM_NODES; node++)
  {
  printf("%f", (float)(total_buf_usage[node]*100)/(float)(NUM_PC*NUM_VC*ROUTER_INPUT_BUF_SIZE*(sim_clock-warmup_cycle)));

  if( (node % NUM_COLS) == NUM_COLS-1)
  printf("\n");
  else
  printf(", ");
  }
  */


  long long per_vc_utilization[MAX_VC]={0};
  double vc_util_sum=0;

  for(node=0; node < NUM_NODES; node++)
    for(pc=0; pc < NUM_PC-NUM_NIC_PC; pc++)
      for(vc=0; vc < NUM_VC; vc++)
      {
        vc_util_sum += vc_utilization[node][pc][vc];
        per_vc_utilization[vc] += vc_utilization[node][pc][vc];
      }

  printf("- VC Utilization...\n");
  for(vc=0; vc < NUM_VC; vc++)
    printf("- VC Utilzation for virtual circuit %d   : %.2lf \n", vc, (double)per_vc_utilization[vc]/vc_util_sum);

if(source_queue_logic_enabled)
{
  double min,max, avg, avg_base, sum, sum_inv;

  min=1000000; max=-1;
  avg=0; avg_base=0; sum=0;

  printf("Req Priorty Levels Matrix\n");
  for(int r=0; r < NUM_ROWS; r++)
  {
    for(int c=0; c < NUM_COLS; c++)
    {
      printf("%.2lf ",num_batch[r*NUM_COLS + c]/(double)last_batch_id[r*NUM_COLS + c]); 
    }
    printf("\n");
  }
  
  printf("Router Occupancy Matrix\n");
  for(int r=0; r < NUM_ROWS; r++)
  {
    for(int c=0; c < NUM_COLS; c++)
    {
      printf("%6.2lf ",router_occupancy[r*NUM_COLS + c]/(double)sim_clock); 
    }
    printf("\n");
  }

  printf("\nLatency Matrix\n");
  for(int r=0; r < NUM_ROWS; r++)
  {
    for(int c=0; c < NUM_COLS; c++)
    {
      int n = r*NUM_COLS + c;
      printf("%7.2lf ",source_job_latency[n]); 
    }
    printf("\n");
  }

  printf("\nLatency Slowdown Matrix\n");
  for(int r=0; r < NUM_ROWS; r++)
  {
    for(int c=0; c < NUM_COLS; c++)
    {
      int n = r*NUM_COLS + c;
      printf("%7.2lf ",source_job_latency[n]/source_base_latency[n]); 
      if(min > source_job_latency[n]/source_base_latency[n])
        min = source_job_latency[n]/source_base_latency[n];
      if(max < source_job_latency[n]/source_base_latency[n])
        max = source_job_latency[n]/source_base_latency[n];
      avg_base+= source_base_latency[n];
      avg += source_job_latency[n];
      sum += source_job_latency[n]/source_base_latency[n];
    }
    printf("\n");
  }

  printf("Latency Unfairness : %.2lf HSpeedup :  %.4lf : Average : %.4lf/%.4lf : %.2lf\n",max/min, 
      NUM_NODES/sum, avg_base/(double)NUM_NODES, avg/(double)NUM_NODES, avg_base/avg);

  printf("Throughput Base Matrix\n");
  for(int r=0; r < NUM_ROWS; r++)
  {
    for(int c=0; c < NUM_COLS; c++)
    {
      printf("%.4lf ",source_base_throughput[r*NUM_COLS + c]); 
      if(source_base_throughput[r*NUM_COLS +c] == -1)
        source_base_throughput[r*NUM_COLS + c] = load_rate_per_source[r*NUM_COLS + c];
    }
    printf("\n");
  }

 

 printf("Throughput Current Matrix\n");
  for(int r=0; r < NUM_ROWS; r++)
  {
    for(int c=0; c < NUM_COLS; c++)
    {
      printf("%.4lf ",source_throughput[r*NUM_COLS + c]); 
    }
    printf("\n");
  }

  min=1000000; max=-1;
  avg=0; avg_base=0; sum=0;

  printf("Throughput Slowdown Matrix\n");
  for(int r=0; r < NUM_ROWS; r++)
  {
    for(int c=0; c < NUM_COLS; c++)
    {
      int n = r*NUM_COLS + c;
      if(source_base_throughput[n] == -1 || source_throughput[n] == -1)
        printf("-1.0000 ");
      else
        printf("%.4lf ",source_base_throughput[r*NUM_COLS + c]/source_throughput[r*NUM_COLS + c]); 

      if(min > source_base_throughput[n]/source_throughput[n])
        min = source_base_throughput[n]/source_throughput[n];
      if(max < source_base_throughput[n]/source_throughput[n])
        max = source_base_throughput[n]/source_throughput[n];

      avg_base+= source_base_throughput[n];
      avg += source_throughput[n];
      sum +=  source_base_throughput[n]/source_throughput[n];
    }
    printf("\n");
  }
  
  printf("Job Completetion Time (Kilo Cycles) Matrix\n");
  for(int r=0; r < NUM_ROWS; r++)
  {
    for(int c=0; c < NUM_COLS; c++)
    {
      printf("%6.2lf ",source_job_completion[r*NUM_COLS + c]); 
    }
    printf("\n");
  }

  printf("Throughput Unfairness : %.2lf : HSpeedup : %.4lf : Avg Ratio : %.4lf/%.4lf : %.2lf\n",
      max/min, NUM_NODES/sum, avg, avg_base, avg/avg_base);
}
 
#ifdef TR_INTEG
  sim_clock = sim_clock + sim_start_clock;
#endif
  printf("%.4lf cycles %.4lf W\n",total_packet_delay_cycle / (double)(num_ejt_msg),avg_power);

} 