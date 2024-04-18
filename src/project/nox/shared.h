#ifndef SHARED_H
#define SHARED_H
//flit.h
#include <zlib.h>

extern flit_t *free_list_head;
extern flit_t *free_list_tail;

//link.h
extern mbox_t link_buf[MAX_NODES][MAX_PC-MAX_NIC_PC][MAX_VC];
extern mbox_t link_buf2[MAX_NODES][MAX_PC-MAX_NIC_PC][MAX_VC];
extern mbox_t link_buf3[MAX_NODES][MAX_PC-MAX_NIC_PC][MAX_VC];
extern mbox_t link_buf4[MAX_NODES][MAX_PC-MAX_NIC_PC][MAX_VC];
extern mbox_t link_buf5[MAX_NODES][MAX_PC-MAX_NIC_PC][MAX_VC];
extern mbox_t link_buf6[MAX_NODES][MAX_PC-MAX_NIC_PC][MAX_VC];
extern mbox_t link_buf7[MAX_NODES][MAX_PC-MAX_NIC_PC][MAX_VC];
extern int    link_info[MAX_NODES][MAX_PC-MAX_NIC_PC];

extern router_info_t router_info[MAX_NODES];
//main.h
extern FILE *fsql;
extern short int sim_end_flag;
#ifdef TR_INTEG
extern long long sim_clock;
#else
extern unsigned int sim_start_clock;
extern unsigned int sim_clock;
#endif
extern unsigned int num_inj_msg;
extern unsigned int num_ejt_msg;
extern unsigned int control_ejt_msg;
extern unsigned int data_ejt_msg;
extern unsigned int warmup_cycle;
extern unsigned int num_ejt_flit;
extern unsigned int control_ejt_flit;
extern unsigned int data_ejt_flit;
extern unsigned int num_inj_flit;
extern long long        total_delay_cycle;
extern long long        total_bus_delay_cycle;
extern long long        bus_delay_cycle;
extern long long        total_packet_delay_cycle;
extern long long        total_packet_queue_cycle;
extern long long        total_latency;
extern long long        total_queueing_latency; 
extern long long        total_network_latency;
extern long long        cmsg_total_delay_cycle;
extern long long        cmsg_total_flit_latency;
extern long long        cmsg_total_msg_latency;
extern long long        cmsg_total_queueing_latency;
extern long long        cmsg_total_network_latency;
extern long long        dmsg_total_delay_cycle;
extern long long        dmsg_total_flit_latency;
extern long long        dmsg_total_msg_latency;
extern long long        dmsg_total_queueing_latency;
extern long long        dmsg_total_network_latency;
extern unsigned int total_num_hop;
extern unsigned int total_aged_flit10;
extern unsigned int total_aged_flit20;
extern unsigned int total_activity[MAX_NODES];
extern long long        total_buf_usage[MAX_NODES];
extern long long        total_retrans_buf_usage[MAX_NODES];
extern int	     num_escape_msgs;
extern long long     flits_combine_stat;

// Variables for simulation option
extern int retrans_type;
extern int pattern;
extern int pattern_per_source[MAX_NODES];
extern int arch;
extern int in_traffic;
extern int early_ejection;
extern int num_link_err;
extern int num_routing_err;
extern int num_swarbiter_err;
extern int num_failed_link;
extern int num_failed_node;
extern int num_e2e_nack;
extern int num_e2e_retrans;
extern int routing_algorithm;
extern int topology;
extern int verbose;
extern int probe_verbose;
extern int sql;
extern float load_rate;
extern float load_rate_per_source[MAX_NODES];
extern int msgs_to_inject[MAX_NODES];
extern int msgs_to_eject[MAX_NODES];
extern int warmup_msgs[MAX_NODES];
extern float link_err_rate;
extern float routing_err_rate;
extern float swarbiter_err_rate;

//nic.h
extern mbox_t nic_input_buf [MAX_NODES][MAX_NIC_PC][MAX_VC];
extern mbox_t nic_output_buf[MAX_NODES][MAX_NIC_PC][MAX_VC];

//power.h
extern double p_buffer;      // power consumed at router input buffer 
extern double p_routing;     // power consumed at the routing logic 
extern double p_vc_arb;      // power consumed at the VC arbitration
extern double p_sw_arb;      // power consumed at the switch arbitration
extern double p_xbar;        // power consumed at the crossbar
extern double p_link;        // power consumed at the link
extern double p_err_chk;     // power consumed at the error checking routine
extern double p_retrans;     // power consumed at the retransmission routine 
extern double g_link;        // power consumed at the link
extern double l_link;        // power consumed at the link
extern long long g_xfers;
extern long long l_xfers;
extern long long l_msgs;
extern long long g_total_num_hop; 

//router.h
extern xbar_out_t   xbar_out          [MAX_NODES][MAX_PC][MAX_VC];
extern router_t     router            [MAX_NODES];
extern vc_info_t    vc_info           [MAX_NODES][MAX_PC][MAX_VC];
extern vc_info_t    xb_info           [MAX_NODES][MAX_PC][MAX_VC]; 
extern int          neighbor          [MAX_NODES][MAX_PC-MAX_NIC_PC];
extern int          neighbor_pc       [MAX_NODES][MAX_PC-MAX_NIC_PC];
extern ready_flag_t is_ready          [MAX_NODES][MAX_PC][MAX_VC];
extern has_flit_t   has_flit          [MAX_NODES][MAX_PC][MAX_VC];
extern int          credit_table      [MAX_NODES][MAX_PC][MAX_VC];
extern int          rt_credit_table   [MAX_NODES][MAX_PC][MAX_VC]; 
extern mbox_t       router_input_buf  [MAX_NODES][MAX_PC][MAX_VC];
extern mbox_t       xbar_buf          [MAX_NODES][MAX_PC][MAX_VC];
extern long         nerr_chk_done     [MAX_NODES];
extern long         nerr_found        [MAX_NODES];
extern unsigned int link_usage        [MAX_NODES][MAX_PC-MAX_NIC_PC];

extern vc_info_t    vc_nic_info           [MAX_NODES][MAX_PC-MAX_NIC_PC][MAX_VC];
extern vc_info_t    rinbuf_info           [MAX_NODES][MAX_PC-MAX_NIC_PC][MAX_VC];

extern int NUM_NODES;
extern int NUM_ROWS;
extern int NUM_COLS;
extern int NUM_PE;


extern int RESP_RQ_TRAFFIC;


extern short NUM_PC;
extern short NUM_NIC_PC;
extern short NUM_VC;
//extern short NUM_NIC_VC;

extern butterfly_t bfly;

extern int MSG_LEN;
extern int hybrid_topology;
extern int concentrated;
extern int concentration_degree;
extern int xbar_in_free[MAX_NODES][MAX_PC];
extern int xbar_out_free[MAX_NODES][MAX_PC];
extern int router_inp_buf_size;
extern char vc_state[10][20];
extern short node_switch[MAX_NODES];

extern int global_msg_len;
extern int local_msg_len;
extern double mratio;
extern int xshare;
extern int xshare_allowed_xfers;
extern double msg_combine_candidate_ratio;

extern int FLIT_WIDTH;
extern float PD_ROUTER;
extern float PD_BUFFER;
extern float PD_XBAR_F;
extern float PD_LINK;
extern float PD_P_1_ARB;
extern float PD_V_1_ARB;
extern float PD_PV_1_ARB;

extern float PL_P_1_ARB;
extern float PL_V_1_ARB;
extern float PL_PV_1_ARB;
extern float PL_LINK;
extern float PL_XBAR_F;
extern float PL_BUFFER;
extern float PL_ROUTER;
extern float pl_static_energy;

extern int technology_node;

extern int BUS_WIDTH;
extern float PD_LINK_BUS;
extern float pd_p_1_bus;
extern float pd_pv_1_bus;
extern float pd_v_1_bus;

// Additional power counters added by Jin
extern unsigned int num_buf_reads      ; 
extern unsigned int num_buf_writes      ; 
extern unsigned int num_va_used        ; 
extern unsigned int num_sa_used        ; 
extern unsigned int num_xbar_used      ; 
extern unsigned int num_link_used[7]   ; 

extern long long flits_one_hop;
extern long long flits_two_hop;
extern long long flits_three_hop;
extern long long flits_three_plus_hop;


extern int link2;
extern int balanced_edge;

extern long long vc_utilization[MAX_NODES][MAX_PC][MAX_VC];


extern long long MSG_TO_INJECT;
extern long long  WARMUP_MSG; 
extern long long  MSG_TO_EJECT;
extern long long WARMUP_CYCLES;

extern long long short_flits_sum; 

extern float short_flit_ratio;
extern int CLINE_SIZE_BITS;
extern int two_networks;//NO
extern long long nox_stat_interval;
extern int global_priority_matrix[MAX_NODES][64];

extern int ARB_TYPE;
extern int ranking_type[MAX_NODES];
extern int MAX_PRIORITY_LEVELS;
extern bool age_based_arbitration;
extern bool slack_based_arbitration;

extern bool marking_update_immediate;
extern long long num_batch[MAX_NODES];
extern int batching_mode[MAX_NODES];
extern int marking_cap[MAX_NODES];
extern int source_batch_marking_counter[MAX_NODES];
extern int local_batch_marking_counter[MAX_NODES];
extern long long batching_interval[MAX_NODES];
extern long long ranking_interval[MAX_NODES];
extern long long current_batch[MAX_NODES];
extern long long last_batch_id[MAX_NODES];
extern long long serviced_flits_curr_batch[MAX_NODES];
extern long long entering_flits[MAX_NODES];
extern long long local_sjf_counter[MAX_NODES][MAX_NODES];
extern long long local_source_jobs_done[MAX_NODES];
extern long long global_source_jobs_done[MAX_NODES];

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
//Source Queue logic
extern bool source_queue_logic_enabled;
extern int OT_JOB_SIZE[64]; 
extern double source_queue_size_total[MAX_NODES];
extern double source_queue_size_samples[MAX_NODES];
extern double source_queue_size_avg[MAX_NODES];
extern double source_jobs_done[MAX_NODES];
extern double source_jobs[MAX_NODES];
extern double source_job_delay[MAX_NODES];
extern double source_job_latency[MAX_NODES];
extern double source_throughput[MAX_NODES];
extern double source_base_throughput[MAX_NODES];
extern double source_base_latency[MAX_NODES];
extern double source_job_completion[MAX_NODES];
extern int source_sim_done;

extern double router_occupancy[MAX_NODES];

extern trace_line_t trace_buffer[64][7000];
extern int trace_ptr[64];
extern gzFile trace_fp[64];
extern long long trace_clock_recycle[64];
extern int synthetic_trace_enabled;

extern int active_source;
extern char tp[5][20]; 
extern float th_alone_per_source[MAX_NODES];
extern float BaseRRHop[MAX_NODES]; 
extern float BaseRRLatency[MAX_NODES]; 
extern int slack_per_hop_latency;
extern double local_traffic_ratio;
extern short SA_win_id[MAX_NODES][MAX_PC];
extern short VA_win_id[MAX_NODES][MAX_PC][MAX_VC];

extern int num_priority_levels;
extern int ATOMIC_BUFFER;
extern int PRIORITY_VC_SET;
extern int ranking_vector_NR[64];
extern int ranking_vector_TP[64];
extern int ranking_vector_HT[64];
extern int ranking_vector_SJF[64];
extern int ranking_vector_Static[64];
//GSF
extern int GSFHeadFrameId[4096];
extern int GSFWindowSize;

extern int GSFInjectionFrameId[4096];
extern int GSFCredit[4096];
extern int GSFReservedCredit[4096];
extern bool GSFRouterEnabled;
extern int GSFBarrierPenalty;
extern double ejt_pc_utilization[MAX_NODES][MAX_NIC_PC];
extern double GSFFlitCount[64];
//#ifdef WIDE_PATH
//extern int DATA_PATH_WIDTH; 
//extern int rinbuf_recv[MAX_NODES][MAX_PC][MAX_VC]; 
//extern int xbuf_recv[MAX_NODES][MAX_PC][MAX_VC];
//extern int lbuf_recv[MAX_NODES][MAX_PC][MAX_VC];
//extern int ninbuf_recv[MAX_NODES][MAX_PC][MAX_VC];
//#endif
#endif
