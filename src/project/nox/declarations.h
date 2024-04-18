#ifndef DECLS_H
#define DECLS_H
#include <vector>
//flit.h
flit_t *free_list_head;
flit_t *free_list_tail;

//link.h
mbox_t link_buf[MAX_NODES][MAX_PC-MAX_NIC_PC][MAX_VC]={0};
mbox_t link_buf2[MAX_NODES][MAX_PC-MAX_NIC_PC][MAX_VC]={0};
mbox_t link_buf3[MAX_NODES][MAX_PC-MAX_NIC_PC][MAX_VC]={0};
mbox_t link_buf4[MAX_NODES][MAX_PC-MAX_NIC_PC][MAX_VC]={0};
mbox_t link_buf5[MAX_NODES][MAX_PC-MAX_NIC_PC][MAX_VC]={0};
mbox_t link_buf6[MAX_NODES][MAX_PC-MAX_NIC_PC][MAX_VC]={0};
mbox_t link_buf7[MAX_NODES][MAX_PC-MAX_NIC_PC][MAX_VC]={0};
int    link_info[MAX_NODES][MAX_PC-MAX_NIC_PC]={0};

router_info_t router_info[MAX_NODES];

//main.h
FILE *fsql;
short int sim_end_flag;
#ifdef TR_INTEG
long long sim_clock;
#else
unsigned int sim_start_clock=0;
unsigned int sim_clock;
#endif
unsigned int num_inj_msg;
unsigned int num_ejt_msg;
unsigned int control_ejt_msg;
unsigned int data_ejt_msg;
unsigned int warmup_cycle;
unsigned int num_ejt_flit;
unsigned int control_ejt_flit;
unsigned int data_ejt_flit;
unsigned int num_inj_flit;
long long        total_delay_cycle;
long long        total_bus_delay_cycle;
long long        total_packet_queue_cycle;
long long        bus_delay_cycle;
long long        total_packet_delay_cycle;
long long        total_latency;
long long        total_queueing_latency;
long long        total_network_latency;
long long        cmsg_total_delay_cycle;
long long        cmsg_total_flit_latency;
long long        cmsg_total_msg_latency;
long long        cmsg_total_queueing_latency;
long long        cmsg_total_network_latency;
long long        dmsg_total_delay_cycle;
long long        dmsg_total_flit_latency;
long long        dmsg_total_msg_latency;
long long        dmsg_total_queueing_latency;
long long        dmsg_total_network_latency;
unsigned int total_num_hop;
unsigned int total_aged_flit10;
unsigned int total_aged_flit20;
unsigned int total_activity[MAX_NODES];
long long        total_buf_usage[MAX_NODES];
long long        total_retrans_buf_usage[MAX_NODES];
int	     num_escape_msgs;
long long     flits_combine_stat;

// Variables for simulation option
int retrans_type;
int pattern;
int pattern_per_source[MAX_NODES];
int arch;
int in_traffic;
int early_ejection;
int num_link_err;
int num_routing_err;
int num_swarbiter_err;
int num_failed_link;
int num_failed_node;
int num_e2e_nack;
int num_e2e_retrans;
int routing_algorithm;
int topology;
int verbose;
int probe_verbose;
int sql;
float load_rate;
float load_rate_per_source[MAX_NODES];
int msgs_to_inject[MAX_NODES];
int msgs_to_eject[MAX_NODES];
int warmup_msgs[MAX_NODES];

float link_err_rate;
float routing_err_rate;
float swarbiter_err_rate;

//nic.h
mbox_t nic_input_buf [MAX_NODES][MAX_NIC_PC][MAX_VC]={0};
mbox_t nic_output_buf[MAX_NODES][MAX_NIC_PC][MAX_VC]={0};

//power.h
double p_buffer;      // power consumed at router input buffer 
double p_routing;     // power consumed at the routing logic 
double p_vc_arb;      // power consumed at the VC arbitration
double p_sw_arb;      // power consumed at the switch arbitration
double p_xbar;        // power consumed at the crossbar
double p_link;        // power consumed at the link
double p_err_chk;     // power consumed at the error checking routine
double p_retrans;     // power consumed at the retransmission routine 
double g_link=0.0;        // power consumed at the link
double l_link=0.0;        // power consumed at the link
long long g_xfers=0;
long long l_xfers=0;
long long l_msgs=0;
long long g_total_num_hop=0; 

//router.h
xbar_out_t   xbar_out          [MAX_NODES][MAX_PC][MAX_VC];
router_t     router            [MAX_NODES];
vc_info_t    vc_info           [MAX_NODES][MAX_PC][MAX_VC];
vc_info_t    xb_info           [MAX_NODES][MAX_PC][MAX_VC]; 
int          neighbor          [MAX_NODES][MAX_PC-MAX_NIC_PC] = {0};
int          neighbor_pc       [MAX_NODES][MAX_PC-MAX_NIC_PC] = {0};
ready_flag_t is_ready          [MAX_NODES][MAX_PC][MAX_VC]    = {0};
has_flit_t   has_flit          [MAX_NODES][MAX_PC][MAX_VC]    = {0};
int          credit_table      [MAX_NODES][MAX_PC][MAX_VC]    = {0};
int          rt_credit_table   [MAX_NODES][MAX_PC][MAX_VC]    = {0}; 
mbox_t       router_input_buf  [MAX_NODES][MAX_PC][MAX_VC]    = {0};
mbox_t       xbar_buf          [MAX_NODES][MAX_PC][MAX_VC]    = {0};
long         nerr_chk_done     [MAX_NODES]                    = {0};
long         nerr_found        [MAX_NODES]                    = {0};
unsigned int link_usage        [MAX_NODES][MAX_PC-MAX_NIC_PC] = {0};

vc_info_t    vc_nic_info           [MAX_NODES][MAX_PC-MAX_NIC_PC][MAX_VC];
vc_info_t    rinbuf_info           [MAX_NODES][MAX_PC-MAX_NIC_PC][MAX_VC];

int NUM_ROWS = 4;
int NUM_COLS = 4;
int NUM_NODES = 16;
int NUM_PE = 16;


short NUM_PC=5;
short NUM_NIC_PC=1;
short NUM_VC=4;
//short NUM_NIC_VC=16;

int MSG_LEN=4;

butterfly_t bfly;
int hybrid_topology = NO;
int concentrated = NO;
int concentration_degree = 1;
int xbar_in_free[MAX_NODES][MAX_PC] = {0};
int xbar_out_free[MAX_NODES][MAX_PC] = {0};
int router_inp_buf_size = 4;
short node_switch[MAX_NODES]={0};


/*char vc_state[11][20] = {
"VC_IDLE",         //  0 // VC is idle
"VC_ROUTING",      //  1 // Routing Done
"VC_VA_DONE",      //  2 // VA is done 
"VC_SA_DONE",      //  3 // SA is done 
"VC_IDLE_TMP",     //  4 // VC is idle
"VC_ROUTING_TMP",  //  5 // Routing Done
"VC_VA_DONE_TMP",  //  6 // VA is done 
"VC_SA_DONE_TMP",  //  7 // SA is done 
"VC_SA_DONE_TMP1", //  8 // SA is done 
"VC_SA_DONE_TMP2", //  9 // SA is done 
"VC_SA_DONE_TMP3"  //  10 // SA is done 
};*/

char vc_state[11][20] = {
"ID",         //  0 // VC is idle
"RT",      //  1 // Routing Done
"VA",      //  2 // VA is done 
"SA",      //  3 // SA is done 
"IDT",     //  4 // VC is idle
"RTT",  //  5 // Routing Done
"VAT",  //  6 // VA is done 
"SAT",  //  7 // SA is done 
"VC_SA_DONE_TMP1", //  8 // SA is done 
"VC_SA_DONE_TMP2", //  9 // SA is done 
"VC_SA_DONE_TMP3"  //  10 // SA is done 
};

int RESP_RQ_TRAFFIC=0;

int global_msg_len = 2*MSG_LEN;
int local_msg_len  = MSG_LEN;
double mratio = 0.5;
int xshare = NO;
int xshare_allowed_xfers=4;
double msg_combine_candidate_ratio = 0.0;

// Power Parameters
// @50nm low swing

int FLIT_WIDTH    = 64;
float PD_LINK     = 0.0;
float PD_ROUTER   = 0.087;
float PD_BUFFER   = 1.28;
float PD_XBAR_F   = 5.3214;
float PD_P_1_ARB  = 0.367;
float PD_V_1_ARB  = 0.367;
float PD_PV_1_ARB = 0.367;

float PL_P_1_ARB  = 0.0;
float PL_V_1_ARB  = 0.0;
float PL_PV_1_ARB = 0.0;
float PL_LINK     = 0.0;
float PL_ROUTER   = 0.0;
float PL_BUFFER   = 0.0;
float PL_XBAR_F   = 0.0;
float pl_static_energy = 0.0;
int technology_node = 50;

int BUS_WIDTH = 1024;
float PD_LINK_BUS = 0.0313;
float pd_p_1_bus =0.0;
float pd_pv_1_bus =0.0;
float pd_v_1_bus =0.0;

// Additional power counters added by Jin
unsigned int num_buf_reads      = 0; 
unsigned int num_buf_writes     = 0; 
unsigned int num_va_used        = 0; 
unsigned int num_sa_used        = 0; 
unsigned int num_xbar_used      = 0; 
unsigned int num_link_used[7]   = {0}; 

long long flits_one_hop =0;
long long flits_two_hop =0;
long long flits_three_hop =0;
long long flits_three_plus_hop =0;

int link2 = 0;
int balanced_edge = 1;

long long vc_utilization[MAX_NODES][MAX_PC][MAX_VC];

long long MSG_TO_INJECT = 400000; 
long long  WARMUP_MSG      = 1000;  
long long  MSG_TO_EJECT  = 399000; 
long long nox_stat_interval=10000;
long long WARMUP_CYCLES=20000;


long long short_flits_sum=0; 
double short_flit_ratio = 0.0;
int two_networks=1;//NO is 1
int CLINE_SIZE_BITS=1024;



/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
//priority shared variables
int global_priority_matrix[MAX_NODES][64];
int ARB_TYPE;
int ranking_type[MAX_NODES];
int MAX_PRIORITY_LEVELS;
bool age_based_arbitration=false;
bool slack_based_arbitration=false;

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
//batching shared variables
bool marking_update_immediate=false;
int batching_mode[MAX_NODES];
int marking_cap[MAX_NODES];
int source_batch_marking_counter[MAX_NODES];
int local_batch_marking_counter[MAX_NODES];
long long batching_interval[MAX_NODES];
long long ranking_interval[MAX_NODES];
long long current_batch[MAX_NODES];
long long num_batch[MAX_NODES];
long long last_batch_id[MAX_NODES];
long long serviced_flits_curr_batch[MAX_NODES];
long long entering_flits[MAX_NODES];
long long local_sjf_counter[MAX_NODES][MAX_NODES];
long long local_source_jobs_done[MAX_NODES];
long long global_source_jobs_done[MAX_NODES];

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
//Source Queue logic
bool source_queue_logic_enabled=false;
int OT_JOB_SIZE[64]; 

std::vector<source_queue_t> source_queue[MAX_NODES]; 
double source_queue_size_total[MAX_NODES];
double source_queue_size_samples[MAX_NODES];
double source_queue_size_avg[MAX_NODES];
double source_jobs_done[MAX_NODES];
double source_throughput[MAX_NODES];
double source_base_throughput[MAX_NODES];
double source_base_latency[MAX_NODES];
double source_jobs[MAX_NODES];
double source_job_delay[MAX_NODES];
double source_job_latency[MAX_NODES];
double source_job_completion[MAX_NODES];

double router_occupancy[MAX_NODES];

int source_sim_done=0;
int active_source=-1;

trace_line_t trace_buffer[64][7000];
int trace_ptr[64];
gzFile trace_fp[64];
long long trace_clock_recycle[64];
int synthetic_trace_enabled=0;

char tp[6][20] = {"NR", "TP", "TN", "BC", "NB", "HT"};
float th_alone_per_source[MAX_NODES];
float BaseRRLatency[MAX_NODES] = {3178.00, 1743.63,  660.20,  144.78,  650.83, 1174.62, 2803.00, 3076.60, 
3318.28, 1892.13,  740.75,  195.83,  652.67, 1294.27, 2536.52, 3568.70, 
3265.10, 1842.91,  759.48,  230.33,  665.96, 1195.60, 3038.90, 3308.09, 
3031.68, 1703.92,  588.50,  179.24,  664.25, 1364.26, 2890.59, 3139.90, 
2980.14, 1658.23,  581.19,  141.96,  573.91, 1271.96, 2666.46, 3443.01, 
3323.17, 1833.35,  699.89,  185.99,  684.00, 1213.74, 2813.86, 2907.16, 
3115.58, 1858.12,  692.88,  304.79,  644.01, 1313.94, 2892.47, 3610.96, 
2865.35, 1713.56,  738.41,  252.45,  653.08, 1324.38, 2843.52, 3329.96};


float BaseRRHop[MAX_NODES] = {7.00, 6.25, 5.75, 5.50, 5.50, 5.75, 6.25, 7.00, 
6.25, 5.50, 5.00, 4.75, 4.75, 5.00, 5.50, 6.25, 
5.75, 5.00, 4.50, 4.25, 4.25, 4.50, 5.00, 5.75, 
5.50, 4.75, 4.25, 4.00, 4.00, 4.25, 4.75, 5.50, 
5.50, 4.75, 4.25, 4.00, 4.00, 4.25, 4.75, 5.50, 
5.75, 5.00, 4.50, 4.25, 4.25, 4.50, 5.00, 5.75, 
6.25, 5.50, 5.00, 4.75, 4.75, 5.00, 5.50, 6.25, 
7.00, 6.25, 5.75, 5.50, 5.50, 5.75, 6.25, 7.00}; 

int slack_per_hop_latency = -1;
double control_energy_ratio = 1.0;


double local_traffic_ratio = 0.75;
short SA_win_id[MAX_NODES][MAX_PC];
short VA_win_id[MAX_NODES][MAX_PC][MAX_VC];

int num_priority_levels=4;
int ATOMIC_BUFFER=0;
int PRIORITY_VC_SET;

		//Hot Spot
int ranking_vector_HT[64] = 
{
15, 14, 13, 12, 11, 10, 9, 8, 
14, 13, 12, 11, 10, 9,  8, 7, 
13, 12, 11, 10,  9, 8,  7, 6, 
12, 11, 10,  9,  8, 7,  6, 5, 
11, 10,  9,  8,  7, 6,  5, 4, 
10,  9,  8,  7,  6, 5,  4, 3, 
9 ,  8,  7,  6,  5, 4,  3, 2, 
8 ,  7,  6,  5,  4, 3,  2, 1};

/*{
  2, 2, 2, 2, 2, 3, 3, 3, 
2, 2, 2, 2, 3, 3, 3, 3, 
2, 2, 2, 3, 3, 3, 3, 3, 
2, 2, 3, 3, 3, 3, 2, 2, 
1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 
0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0};
*/
		//Transpose
int ranking_vector_TP[64] = 
{0, 1, 1, 2, 3, 3, 3, 3, 
0, 0, 0, 2, 2, 3, 3, 3, 
0, 1, 0, 0, 2, 2, 3, 3, 
1, 1, 1, 0, 0, 2, 2, 2, 
2, 2, 1, 1, 0, 0, 2, 1, 
3, 3, 2, 1, 1, 0, 0, 1, 
3, 3, 3, 2, 1, 1, 0, 0, 
3, 3, 3, 3, 2, 1, 1, 0};


		//Uniform Random
int ranking_vector_NR[64] = 
{3, 2, 1, 0, 0, 1, 2, 3, 
3, 3, 1, 1, 0, 1, 2, 3, 
3, 3, 2, 0, 0, 1, 2, 2, 
3, 3, 1, 0, 0, 0, 2, 3, 
3, 3, 2, 0, 0, 0, 1, 2, 
3, 3, 1, 0, 0, 1, 2, 3, 
3, 2, 1, 1, 0, 1, 2, 3, 
3, 2, 1, 0, 0, 1, 1, 3}; 

//SJF
int ranking_vector_SJF[64] = 
{1, 1, 1, 1, 1, 1, 1, 1, 
0, 0, 0, 0, 0, 0, 0, 0,
1, 1, 1, 1, 1, 1, 1, 1, 
0, 0, 0, 0, 0, 0, 0, 0,
1, 1, 1, 1, 1, 1, 1, 1, 
0, 0, 0, 0, 0, 0, 0, 0,
1, 1, 1, 1, 1, 1, 1, 1, 
0, 0, 0, 0, 0, 0, 0, 0};

int ranking_vector_Static[64] =  
{1, 1, 1, 1, 1, 1, 1, 1, 
0, 0, 0, 0, 0, 0, 0, 0,
1, 1, 1, 1, 1, 1, 1, 1, 
0, 0, 0, 0, 0, 0, 0, 0,
1, 1, 1, 1, 1, 1, 1, 1, 
0, 0, 0, 0, 0, 0, 0, 0,
1, 1, 1, 1, 1, 1, 1, 1, 
0, 0, 0, 0, 0, 0, 0, 0};

//GSF
int GSFHeadFrameId[4096] = {-1};
int GSFWindowSize = 6;
int GSFInjectionFrameId[4096];
int GSFCredit[4096];
int GSFReservedCredit[4096];

bool GSFRouterEnabled=false;
int GSFBarrierPenalty=16;
double ejt_pc_utilization[MAX_NODES][MAX_NIC_PC];

double GSFFlitCount[64];
#endif
