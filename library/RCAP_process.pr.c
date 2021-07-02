/* Process model C form file: RCAP_process.pr.c */
/* Portions of this file copyright 1986-2008 by OPNET Technologies, Inc. */



/* This variable carries the header into the object file */
const char RCAP_process_pr_c [] = "MIL_3_Tfile_Hdr_ 145A 30A modeler 7 5946B33C 5946B33C 1 DESKTOP-FFL2R3R user 0 0 none none 0 0 none 0 0 0 0 0 0 0 0 1e80 8                                                                                                                                                                                                                                                                                                                                                                                                     ";
#include <string.h>



/* OPNET system definitions */
#include <opnet.h>



/* Header Block */

/* packet stream definitions */
//#define StreamFromSrcCTA  2
#define StreamFromSrcCAP  1
#define StreamFromLink 0
#define StreamToLink   0


#define PowerUp         (op_intrpt_type() == OPC_INTRPT_BEGSIM)
#define CAPFrameArrival (op_intrpt_type() == OPC_INTRPT_STRM && op_intrpt_strm() == StreamFromSrcCAP)
//#define CTAFrameArrival (op_intrpt_type() == OPC_INTRPT_STRM && op_intrpt_strm() == StreamFromSrcCTA)
#define StrFromLinkArrival  (op_intrpt_type() == OPC_INTRPT_STRM && op_intrpt_strm() == StreamFromLink)

#define Timeout_Code 1
#define Timeout         (op_intrpt_type() == OPC_INTRPT_SELF && op_intrpt_code() == Timeout_Code)

#define BIFS_intr_Code 2
#define BIFS_intr       (op_intrpt_type() == OPC_INTRPT_SELF && op_intrpt_code() == BIFS_intr_Code)

#define RIFS_intr_Code 3
#define RIFS_intr       (op_intrpt_type() == OPC_INTRPT_SELF && op_intrpt_code() == RIFS_intr_Code)

#define NextSD_intr_code 4
#define NextSD_intr (op_intrpt_type() == OPC_INTRPT_SELF && op_intrpt_code() == NextSD_intr_code)

#define invoke_ACAP_intr_code 5
#define invoke_ACAP_intr (op_intrpt_type() == OPC_INTRPT_SELF && op_intrpt_code() == invoke_ACAP_intr_code)

#define SIFS_intr_Code 6
#define SIFS_intr       (op_intrpt_type() == OPC_INTRPT_SELF && op_intrpt_code() == SIFS_intr_Code)


#define ch_busy 0
#define channel_busy (op_stat_local_read(ch_busy)==1)


#define LinkFail 	    (op_intrpt_type() == OPC_INTRPT_FAIL)
#define LinkUp		    (op_intrpt_type() == OPC_INTRPT_RECOVER)
#define QEmpty 		    (op_subq_stat(0, OPC_QSTAT_PKSIZE) == 0.0)
#define ACAP_QEmpty 	(op_subq_stat(1, OPC_QSTAT_PKSIZE) == 0.0)
int en_beacon,en_RCAP,en_ACAP,en_MCTA,en_CTA;
double beacon_time;
double RCAP_time;
double ACAP_time;
double MCTA_time;
double CTA_time;
double SD_time;
double beacon_start_time;
double RCAP_start_time;
double ACAP_start_time;
double MCTA_start_time;
double CTA_start_time;

double SIFS_time;
double ACK_svc_time;
int req_sended;

//int BackoffExponent;
//int BackoffWindow;
//int channel_busy;
//int ack_correct;

/* End of Header Block */

#if !defined (VOSD_NO_FIN)
#undef	BIN
#undef	BOUT
#define	BIN		FIN_LOCAL_FIELD(_op_last_line_passed) = __LINE__ - _op_block_origin;
#define	BOUT	BIN
#define	BINIT	FIN_LOCAL_FIELD(_op_last_line_passed) = 0; _op_block_origin = __LINE__;
#else
#define	BINIT
#endif /* #if !defined (VOSD_NO_FIN) */



/* State variable definitions */
typedef struct
	{
	/* Internal state tracking for FSM */
	FSM_SYS_STATE
	/* State Variables */
	double	                 		service_rate                                    ;
	Evhandle	               		BIFS_intr_handle                                ;
	Evhandle	               		RIFS_intr_handle                                ;
	Evhandle	               		Timeout_handle                                  ;
	Evhandle	               		NextSD_intr_handle                              ;
	Stathandle	             		ete_gsh                                         ;
	Objid	                  		node_id                                         ;
	Distribution *	         		address_dist                                    ;
	Packet *	               		Data_pkptr                                      ;
	int	                    		BackoffWindow                                   ;
	int	                    		BackoffExponent                                 ;
	int	                    		ack_correct                                     ;
	Packet *	               		ack_pk                                          ;
	int	                    		RetryCount                                      ;
	Packet *	               		copyPkt                                         ;
	Packet *	               		extra_pkptr                                     ;
	double	                 		tx_state                                        ;
	Evhandle	               		invoke_ACAP_intr_handle                         ;
	Prohandle	              		ACAP_proc                                       ;
	int	                    		WaitForSD                                       ;
	double	                 		pk_svc_time                                     ;
	Packet *	               		Data_pkptrQ                                     ;
	Evhandle	               		SIFS_intr_handle                                ;
	double	                 		NSendPKT                                        ;
	} RCAP_process_state;

#define service_rate            		op_sv_ptr->service_rate
#define BIFS_intr_handle        		op_sv_ptr->BIFS_intr_handle
#define RIFS_intr_handle        		op_sv_ptr->RIFS_intr_handle
#define Timeout_handle          		op_sv_ptr->Timeout_handle
#define NextSD_intr_handle      		op_sv_ptr->NextSD_intr_handle
#define ete_gsh                 		op_sv_ptr->ete_gsh
#define node_id                 		op_sv_ptr->node_id
#define address_dist            		op_sv_ptr->address_dist
#define Data_pkptr              		op_sv_ptr->Data_pkptr
#define BackoffWindow           		op_sv_ptr->BackoffWindow
#define BackoffExponent         		op_sv_ptr->BackoffExponent
#define ack_correct             		op_sv_ptr->ack_correct
#define ack_pk                  		op_sv_ptr->ack_pk
#define RetryCount              		op_sv_ptr->RetryCount
#define copyPkt                 		op_sv_ptr->copyPkt
#define extra_pkptr             		op_sv_ptr->extra_pkptr
#define tx_state                		op_sv_ptr->tx_state
#define invoke_ACAP_intr_handle 		op_sv_ptr->invoke_ACAP_intr_handle
#define ACAP_proc               		op_sv_ptr->ACAP_proc
#define WaitForSD               		op_sv_ptr->WaitForSD
#define pk_svc_time             		op_sv_ptr->pk_svc_time
#define Data_pkptrQ             		op_sv_ptr->Data_pkptrQ
#define SIFS_intr_handle        		op_sv_ptr->SIFS_intr_handle
#define NSendPKT                		op_sv_ptr->NSendPKT

/* These macro definitions will define a local variable called	*/
/* "op_sv_ptr" in each function containing a FIN statement.	*/
/* This variable points to the state variable data structure,	*/
/* and can be used from a C debugger to display their values.	*/
#undef FIN_PREAMBLE_DEC
#undef FIN_PREAMBLE_CODE
#define FIN_PREAMBLE_DEC	RCAP_process_state *op_sv_ptr;
#define FIN_PREAMBLE_CODE	\
		op_sv_ptr = ((RCAP_process_state *)(OP_SIM_CONTEXT_PTR->_op_mod_state_ptr));


/* Function Block */

#if !defined (VOSD_NO_FIN)
enum { _op_block_origin = __LINE__ + 2};
#endif

#include<stdio.h>
/**************************************************************************************/
/**************************************************************************************/
static void backoff ()
	{		
	FIN (backoff ());
	//printf("BackoffExponent=%d",BackoffExponent);
	BackoffWindow=op_dist_outcome (op_dist_load ("uniform_int",0,(2<<BackoffExponent)-1));
	//printf("backoffWindow=%d",BackoffWindow);
	
	FOUT;
	}
/**************************************************************************************/
/**************************************************************************************/
static void creat_BIFS_Intr()
	{
	double BIFS_time=0.0000065;//0.00000065; 
	
	FIN (creat_BIFS_Intr ());
	if (RCAP_time-op_sim_time ()+RCAP_start_time<pk_svc_time+2*SIFS_time+ACK_svc_time)
		{
		//printf("CAP_start_time=%2g,RCAP_start_time+SD_time+ BIFS_time,BIFS_intr_Code=%2g\n",RCAP_start_time,RCAP_start_time+SD_time+ BIFS_time,BIFS_intr_Code);
		BIFS_intr_handle=op_intrpt_schedule_self(RCAP_start_time+SD_time+ BIFS_time,BIFS_intr_Code);
		}
	else
		BIFS_intr_handle=op_intrpt_schedule_self(op_sim_time()+ BIFS_time,BIFS_intr_Code);
	//printf("backoffWindow=%d",BackoffWindow);
	//printf("channel_busy=%d",channel_busy);
	FOUT;
	}

/**************************************************************************************/
/**************************************************************************************/
static void CheckTime_ForSend_pkt()
	{
	
	FIN (CheckTime_ForSend_pkt ());
		
	if (RCAP_time-op_sim_time ()+RCAP_start_time<pk_svc_time+2*SIFS_time+ACK_svc_time)
		{
 		NextSD_intr_handle=op_intrpt_schedule_self(RCAP_start_time+SD_time,NextSD_intr_code);
		WaitForSD=1;
		}
	else
		WaitForSD=0;
		
	FOUT;
	}
/**************************************************************************************/
/**************************************************************************************/
static void creat_RIFS_Intr()
	{
	double RIFS_time=0.000009;//0.0000009; 
	FIN (creat_BIFS_Intr ());
	RIFS_intr_handle=op_intrpt_schedule_self(op_sim_time()+ RIFS_time,RIFS_intr_Code);
	FOUT;
	}
/**************************************************************************************/
/**************************************************************************************/
static void creat_SIFS_Intr()
	{
	FIN(creat_SIFS_Intr());
	SIFS_intr_handle=op_intrpt_schedule_self(op_sim_time()+2*SIFS_time,SIFS_intr_Code);
	FOUT;
	}
/**************************************************************************************/
/**************************************************************************************/
static void Qframe()
	{ 
	FIN (Qframe ());
	op_subq_pk_insert (0, op_pk_get(op_intrpt_strm()), OPC_QPOS_TAIL);
	FOUT;
	}
/**************************************************************************************/
/**************************************************************************************/
static void check_channel()
	{		
		FIN (check_channel ());
						
		if (channel_busy==1 )
		{
			creat_BIFS_Intr();			
		}
		else if(BackoffWindow>0)
		{
			BackoffWindow=BackoffWindow-1;
			creat_BIFS_Intr();
		}

		FOUT;

	}

/**************************************************************************************/
/**************************************************************************************/
static void check_ack()
	{
	int dest_ID;
	int frame_type;
	double throughput;
	FIN (check_ack());
	ack_pk=op_pk_get(op_intrpt_strm());
	op_pk_total_size_get(ack_pk);
	op_pk_nfd_get_int32 (ack_pk, "frame_type",&frame_type ); //acording to the standard, type value of beacon is zero
	op_pk_nfd_get_int32 (ack_pk, "dest_ID",&dest_ID);
	
	if (frame_type==1 && dest_ID==node_id)
		{
		ack_correct=1;
		creat_SIFS_Intr();
		NSendPKT=NSendPKT+1;
		//printf("NSendPKT[%d]=%f \n",node_id,NSendPKT);
		if (op_sim_time()>300)
			{
				printf("NSendPKT[%d]=%f \n",node_id,NSendPKT);
				throughput=NSendPKT*1000/300;
				printf("throughput[%d]=%f,NSendPKT=%f \n",node_id,throughput,NSendPKT);
			}
		}
	else
		{
		ack_correct=0;
		//printf("\n RCAP_Proc: dest_ID=%d,node_id=%d,frame_type=%d",dest_ID,node_id,frame_type);
		//printf("pkID=%d,op_sim_time()=%2g\n",op_pk_id (ack_pk),op_sim_time());
		}
	op_pk_destroy(ack_pk);
	FOUT;
	
	}
static void invoke_ACAP()
	{
	FIN (invoke_ACAP());
	en_RCAP=0;
	en_ACAP=1;
	req_sended=0;
	ACAP_start_time=op_sim_time ();
	//printf("ACAP_QEmpty=%d",ACAP_QEmpty);
	op_pro_invoke (ACAP_proc, OPC_NIL);
	op_intrpt_port_register (OPC_PORT_TYPE_STRM, 0, ACAP_proc);
	op_intrpt_port_register (OPC_PORT_TYPE_STRM, 2, ACAP_proc);
	invoke_ACAP_intr_handle=op_intrpt_schedule_self (op_sim_time()+SD_time, invoke_ACAP_intr_code);
	FOUT;
	}
static void send_packet()
	{
	OpT_Packet_Size  pk_len;
	FIN (send_packet());	
	pk_len= op_pk_total_size_get(copyPkt);
	/* determine the time required to complete 	service of the packet	*/
	pk_svc_time=pk_len/ service_rate;
	ACK_svc_time=496/ service_rate;

	op_pk_send (copyPkt,StreamToLink );

	Timeout_handle=op_intrpt_schedule_self(op_sim_time ()+ pk_svc_time+2*SIFS_time+ACK_svc_time,Timeout_Code);
	FOUT;
	}


/* End of Function Block */

/* Undefine optional tracing in FIN/FOUT/FRET */
/* The FSM has its own tracing code and the other */
/* functions should not have any tracing.		  */
#undef FIN_TRACING
#define FIN_TRACING

#undef FOUTRET_TRACING
#define FOUTRET_TRACING

#if defined (__cplusplus)
extern "C" {
#endif
	void RCAP_process (OP_SIM_CONTEXT_ARG_OPT);
	VosT_Obtype _op_RCAP_process_init (int * init_block_ptr);
	void _op_RCAP_process_diag (OP_SIM_CONTEXT_ARG_OPT);
	void _op_RCAP_process_terminate (OP_SIM_CONTEXT_ARG_OPT);
	VosT_Address _op_RCAP_process_alloc (VosT_Obtype, int);
	void _op_RCAP_process_svar (void *, const char *, void **);


#if defined (__cplusplus)
} /* end of 'extern "C"' */
#endif




/* Process model interrupt handling procedure */


void
RCAP_process (OP_SIM_CONTEXT_ARG_OPT)
	{
#if !defined (VOSD_NO_FIN)
	int _op_block_origin = 0;
#endif
	FIN_MT (RCAP_process ());

		{
		/* Temporary Variables */
		//int RetryCount=0;
		int BWRandom=0;
		int max_RetryCount=3;
		
		/* End of Temporary Variables */


		FSM_ENTER ("RCAP_process")

		FSM_BLOCK_SWITCH
			{
			/*---------------------------------------------------------*/
			/** state (Idle) enter executives **/
			FSM_STATE_ENTER_UNFORCED (0, "Idle", state0_enter_exec, "RCAP_process [Idle enter execs]")
				FSM_PROFILE_SECTION_IN ("RCAP_process [Idle enter execs]", state0_enter_exec)
				{
				/* Register handle for statistic collection. */
				ete_gsh = op_stat_reg("ETE Delay", OPC_STAT_INDEX_NONE, OPC_STAT_GLOBAL);
				BackoffExponent=3;
				RetryCount=0;
				
				}
				FSM_PROFILE_SECTION_OUT (state0_enter_exec)

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (1,"RCAP_process")


			/** state (Idle) exit executives **/
			FSM_STATE_EXIT_UNFORCED (0, "Idle", "RCAP_process [Idle exit execs]")


			/** state (Idle) transition processing **/
			FSM_PROFILE_SECTION_IN ("RCAP_process [Idle trans conditions]", state0_trans_conds)
			FSM_INIT_COND (CAPFrameArrival && en_RCAP==1&& QEmpty)
			FSM_TEST_COND (StrFromLinkArrival)
			FSM_TEST_COND (CAPFrameArrival && en_RCAP==0)
			FSM_TEST_COND (en_RCAP==1 && !QEmpty)
			FSM_DFLT_COND
			FSM_TEST_LOGIC ("Idle")
			FSM_PROFILE_SECTION_OUT (state0_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 1, state1_enter_exec, ;, "CAPFrameArrival && en_RCAP==1&& QEmpty", "", "Idle", "svc_packet", "tr_1", "RCAP_process [Idle -> svc_packet : CAPFrameArrival && en_RCAP==1&& QEmpty / ]")
				FSM_CASE_TRANSIT (1, 11, state11_enter_exec, ;, "StrFromLinkArrival", "", "Idle", "del_extra_rcv_packet1", "tr_41", "RCAP_process [Idle -> del_extra_rcv_packet1 : StrFromLinkArrival / ]")
				FSM_CASE_TRANSIT (2, 0, state0_enter_exec, Qframe();;, "CAPFrameArrival && en_RCAP==0", "Qframe();", "Idle", "Idle", "tr_52", "RCAP_process [Idle -> Idle : CAPFrameArrival && en_RCAP==0 / Qframe();]")
				FSM_CASE_TRANSIT (3, 12, state12_enter_exec, ;, "en_RCAP==1 && !QEmpty", "", "Idle", "svc_Q", "tr_79", "RCAP_process [Idle -> svc_Q : en_RCAP==1 && !QEmpty / ]")
				FSM_CASE_TRANSIT (4, 0, state0_enter_exec, ;, "default", "", "Idle", "Idle", "tr_24", "RCAP_process [Idle -> Idle : default / ]")
				}
				/*---------------------------------------------------------*/



			/** state (svc_packet) enter executives **/
			FSM_STATE_ENTER_FORCED (1, "svc_packet", state1_enter_exec, "RCAP_process [svc_packet enter execs]")

			/** state (svc_packet) exit executives **/
			FSM_STATE_EXIT_FORCED (1, "svc_packet", "RCAP_process [svc_packet exit execs]")
				FSM_PROFILE_SECTION_IN ("RCAP_process [svc_packet exit execs]", state1_exit_exec)
				{
				/* Get pointer to packet from stream index. */
				Data_pkptr=op_pk_get(op_intrpt_strm());
				pk_svc_time=op_pk_total_size_get (Data_pkptr)/service_rate;
				copyPkt = op_pk_copy (Data_pkptr);
				node_id=op_id_self();
				//printf("node_id=%d",node_id);		
				op_pk_nfd_set_int32 (Data_pkptr, "src_ID",node_id);
				op_pk_nfd_set_int32 (Data_pkptr, "dest_ID",0);
				op_pk_nfd_set_int32 (copyPkt, "src_ID",node_id);
				op_pk_nfd_set_int32 (copyPkt, "dest_ID",0);
				op_pk_nfd_set_int32 (copyPkt, "frame_type", 4);//acording to the standard, type value of data is 4
				
				
				}
				FSM_PROFILE_SECTION_OUT (state1_exit_exec)


			/** state (svc_packet) transition processing **/
			FSM_TRANSIT_FORCE (2, state2_enter_exec, ;, "default", "", "svc_packet", "backoff", "tr_54", "RCAP_process [svc_packet -> backoff : default / ]")
				/*---------------------------------------------------------*/



			/** state (backoff) enter executives **/
			FSM_STATE_ENTER_FORCED (2, "backoff", state2_enter_exec, "RCAP_process [backoff enter execs]")
				FSM_PROFILE_SECTION_IN ("RCAP_process [backoff enter execs]", state2_enter_exec)
				{
				backoff ();
				creat_BIFS_Intr();
				}
				FSM_PROFILE_SECTION_OUT (state2_enter_exec)

			/** state (backoff) exit executives **/
			FSM_STATE_EXIT_FORCED (2, "backoff", "RCAP_process [backoff exit execs]")


			/** state (backoff) transition processing **/
			FSM_TRANSIT_FORCE (8, state8_enter_exec, ;, "default", "", "backoff", "wait_BIFS", "tr_19", "RCAP_process [backoff -> wait_BIFS : default / ]")
				/*---------------------------------------------------------*/



			/** state (check_channel) enter executives **/
			FSM_STATE_ENTER_FORCED (3, "check_channel", state3_enter_exec, "RCAP_process [check_channel enter execs]")
				FSM_PROFILE_SECTION_IN ("RCAP_process [check_channel enter execs]", state3_enter_exec)
				{
				check_channel();
				}
				FSM_PROFILE_SECTION_OUT (state3_enter_exec)

			/** state (check_channel) exit executives **/
			FSM_STATE_EXIT_FORCED (3, "check_channel", "RCAP_process [check_channel exit execs]")


			/** state (check_channel) transition processing **/
			FSM_TRANSIT_FORCE (8, state8_enter_exec, ;, "default", "", "check_channel", "wait_BIFS", "tr_21", "RCAP_process [check_channel -> wait_BIFS : default / ]")
				/*---------------------------------------------------------*/



			/** state (wait_ACK) enter executives **/
			FSM_STATE_ENTER_UNFORCED (4, "wait_ACK", state4_enter_exec, "RCAP_process [wait_ACK enter execs]")

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (9,"RCAP_process")


			/** state (wait_ACK) exit executives **/
			FSM_STATE_EXIT_UNFORCED (4, "wait_ACK", "RCAP_process [wait_ACK exit execs]")


			/** state (wait_ACK) transition processing **/
			FSM_PROFILE_SECTION_IN ("RCAP_process [wait_ACK trans conditions]", state4_trans_conds)
			FSM_INIT_COND (Timeout && (RetryCount+1)<=max_RetryCount)
			FSM_TEST_COND (Timeout&& (RetryCount+1)>max_RetryCount)
			FSM_TEST_COND (StrFromLinkArrival)
			FSM_TEST_COND (CAPFrameArrival)
			FSM_DFLT_COND
			FSM_TEST_LOGIC ("wait_ACK")
			FSM_PROFILE_SECTION_OUT (state4_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 6, state6_enter_exec, ;, "Timeout && (RetryCount+1)<=max_RetryCount", "", "wait_ACK", "PKTSending_faild", "tr_13", "RCAP_process [wait_ACK -> PKTSending_faild : Timeout && (RetryCount+1)<=max_RetryCount / ]")
				FSM_CASE_TRANSIT (1, 5, state5_enter_exec, ;, "Timeout&& (RetryCount+1)>max_RetryCount", "", "wait_ACK", "PKTSending_success", "tr_18", "RCAP_process [wait_ACK -> PKTSending_success : Timeout&& (RetryCount+1)>max_RetryCount / ]")
				FSM_CASE_TRANSIT (2, 9, state9_enter_exec, ;, "StrFromLinkArrival", "", "wait_ACK", "check_ack", "tr_36", "RCAP_process [wait_ACK -> check_ack : StrFromLinkArrival / ]")
				FSM_CASE_TRANSIT (3, 4, state4_enter_exec, Qframe();;, "CAPFrameArrival", "Qframe();", "wait_ACK", "wait_ACK", "tr_44", "RCAP_process [wait_ACK -> wait_ACK : CAPFrameArrival / Qframe();]")
				FSM_CASE_TRANSIT (4, 4, state4_enter_exec, ;, "default", "", "wait_ACK", "wait_ACK", "tr_25", "RCAP_process [wait_ACK -> wait_ACK : default / ]")
				}
				/*---------------------------------------------------------*/



			/** state (PKTSending_success) enter executives **/
			FSM_STATE_ENTER_FORCED (5, "PKTSending_success", state5_enter_exec, "RCAP_process [PKTSending_success enter execs]")
				FSM_PROFILE_SECTION_IN ("RCAP_process [PKTSending_success enter execs]", state5_enter_exec)
				{
				RetryCount=0;
				BackoffExponent=3;
				}
				FSM_PROFILE_SECTION_OUT (state5_enter_exec)

			/** state (PKTSending_success) exit executives **/
			FSM_STATE_EXIT_FORCED (5, "PKTSending_success", "RCAP_process [PKTSending_success exit execs]")


			/** state (PKTSending_success) transition processing **/
			FSM_PROFILE_SECTION_IN ("RCAP_process [PKTSending_success trans conditions]", state5_trans_conds)
			FSM_INIT_COND (QEmpty)
			FSM_TEST_COND (!QEmpty)
			FSM_TEST_LOGIC ("PKTSending_success")
			FSM_PROFILE_SECTION_OUT (state5_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 0, state0_enter_exec, ;, "QEmpty", "", "PKTSending_success", "Idle", "tr_12", "RCAP_process [PKTSending_success -> Idle : QEmpty / ]")
				FSM_CASE_TRANSIT (1, 12, state12_enter_exec, ;, "!QEmpty", "", "PKTSending_success", "svc_Q", "tr_77", "RCAP_process [PKTSending_success -> svc_Q : !QEmpty / ]")
				}
				/*---------------------------------------------------------*/



			/** state (PKTSending_faild) enter executives **/
			FSM_STATE_ENTER_FORCED (6, "PKTSending_faild", state6_enter_exec, "RCAP_process [PKTSending_faild enter execs]")
				FSM_PROFILE_SECTION_IN ("RCAP_process [PKTSending_faild enter execs]", state6_enter_exec)
				{
				RetryCount=RetryCount+1;
				BackoffExponent=BackoffExponent+1;
				copyPkt = op_pk_copy (Data_pkptr);
				op_pk_nfd_set_int32 (copyPkt, "src_ID",node_id);
				op_pk_nfd_set_int32 (copyPkt, "dest_ID",0);
				op_pk_nfd_set_int32 (copyPkt, "frame_type", 4);//acording to the standard, type value of data is 4
				
				creat_RIFS_Intr();
				}
				FSM_PROFILE_SECTION_OUT (state6_enter_exec)

			/** state (PKTSending_faild) exit executives **/
			FSM_STATE_EXIT_FORCED (6, "PKTSending_faild", "RCAP_process [PKTSending_faild exit execs]")


			/** state (PKTSending_faild) transition processing **/
			FSM_TRANSIT_FORCE (7, state7_enter_exec, ;, "default", "", "PKTSending_faild", "wait_RIFS", "tr_15", "RCAP_process [PKTSending_faild -> wait_RIFS : default / ]")
				/*---------------------------------------------------------*/



			/** state (wait_RIFS) enter executives **/
			FSM_STATE_ENTER_UNFORCED (7, "wait_RIFS", state7_enter_exec, "RCAP_process [wait_RIFS enter execs]")

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (15,"RCAP_process")


			/** state (wait_RIFS) exit executives **/
			FSM_STATE_EXIT_UNFORCED (7, "wait_RIFS", "RCAP_process [wait_RIFS exit execs]")


			/** state (wait_RIFS) transition processing **/
			FSM_PROFILE_SECTION_IN ("RCAP_process [wait_RIFS trans conditions]", state7_trans_conds)
			FSM_INIT_COND (CAPFrameArrival)
			FSM_TEST_COND (RIFS_intr)
			FSM_TEST_COND (StrFromLinkArrival)
			FSM_DFLT_COND
			FSM_TEST_LOGIC ("wait_RIFS")
			FSM_PROFILE_SECTION_OUT (state7_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 7, state7_enter_exec, Qframe();;, "CAPFrameArrival", "Qframe();", "wait_RIFS", "wait_RIFS", "tr_46", "RCAP_process [wait_RIFS -> wait_RIFS : CAPFrameArrival / Qframe();]")
				FSM_CASE_TRANSIT (1, 2, state2_enter_exec, ;, "RIFS_intr", "", "wait_RIFS", "backoff", "tr_64", "RCAP_process [wait_RIFS -> backoff : RIFS_intr / ]")
				FSM_CASE_TRANSIT (2, 19, state19_enter_exec, ;, "StrFromLinkArrival", "", "wait_RIFS", "del_extra_rcv_packet4", "tr_90", "RCAP_process [wait_RIFS -> del_extra_rcv_packet4 : StrFromLinkArrival / ]")
				FSM_CASE_TRANSIT (3, 7, state7_enter_exec, ;, "default", "", "wait_RIFS", "wait_RIFS", "tr_27", "RCAP_process [wait_RIFS -> wait_RIFS : default / ]")
				}
				/*---------------------------------------------------------*/



			/** state (wait_BIFS) enter executives **/
			FSM_STATE_ENTER_UNFORCED (8, "wait_BIFS", state8_enter_exec, "RCAP_process [wait_BIFS enter execs]")

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (17,"RCAP_process")


			/** state (wait_BIFS) exit executives **/
			FSM_STATE_EXIT_UNFORCED (8, "wait_BIFS", "RCAP_process [wait_BIFS exit execs]")


			/** state (wait_BIFS) transition processing **/
			FSM_PROFILE_SECTION_IN ("RCAP_process [wait_BIFS trans conditions]", state8_trans_conds)
			FSM_INIT_COND (BIFS_intr && (BackoffWindow>0 || channel_busy==1))
			FSM_TEST_COND (StrFromLinkArrival)
			FSM_TEST_COND (CAPFrameArrival)
			FSM_TEST_COND (BIFS_intr && BackoffWindow==0 && channel_busy==0)
			FSM_DFLT_COND
			FSM_TEST_LOGIC ("wait_BIFS")
			FSM_PROFILE_SECTION_OUT (state8_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 3, state3_enter_exec, ;, "BIFS_intr && (BackoffWindow>0 || channel_busy==1)", "", "wait_BIFS", "check_channel", "tr_20", "RCAP_process [wait_BIFS -> check_channel : BIFS_intr && (BackoffWindow>0 || channel_busy==1) / ]")
				FSM_CASE_TRANSIT (1, 10, state10_enter_exec, ;, "StrFromLinkArrival", "", "wait_BIFS", "del_extra_rcv_packet", "tr_38", "RCAP_process [wait_BIFS -> del_extra_rcv_packet : StrFromLinkArrival / ]")
				FSM_CASE_TRANSIT (2, 8, state8_enter_exec, Qframe();;, "CAPFrameArrival", "Qframe();", "wait_BIFS", "wait_BIFS", "tr_45", "RCAP_process [wait_BIFS -> wait_BIFS : CAPFrameArrival / Qframe();]")
				FSM_CASE_TRANSIT (3, 13, state13_enter_exec, ;, "BIFS_intr && BackoffWindow==0 && channel_busy==0", "", "wait_BIFS", "check_time", "tr_55", "RCAP_process [wait_BIFS -> check_time : BIFS_intr && BackoffWindow==0 && channel_busy==0 / ]")
				FSM_CASE_TRANSIT (4, 8, state8_enter_exec, ;, "default", "", "wait_BIFS", "wait_BIFS", "tr_57", "RCAP_process [wait_BIFS -> wait_BIFS : default / ]")
				}
				/*---------------------------------------------------------*/



			/** state (check_ack) enter executives **/
			FSM_STATE_ENTER_FORCED (9, "check_ack", state9_enter_exec, "RCAP_process [check_ack enter execs]")

			/** state (check_ack) exit executives **/
			FSM_STATE_EXIT_FORCED (9, "check_ack", "RCAP_process [check_ack exit execs]")
				FSM_PROFILE_SECTION_IN ("RCAP_process [check_ack exit execs]", state9_exit_exec)
				{
				check_ack();
				}
				FSM_PROFILE_SECTION_OUT (state9_exit_exec)


			/** state (check_ack) transition processing **/
			FSM_PROFILE_SECTION_IN ("RCAP_process [check_ack trans conditions]", state9_trans_conds)
			FSM_INIT_COND (ack_correct==1)
			FSM_DFLT_COND
			FSM_TEST_LOGIC ("check_ack")
			FSM_PROFILE_SECTION_OUT (state9_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 17, state17_enter_exec, ;, "ack_correct==1", "", "check_ack", "wait_2*SIFS", "tr_85", "RCAP_process [check_ack -> wait_2*SIFS : ack_correct==1 / ]")
				FSM_CASE_TRANSIT (1, 4, state4_enter_exec, ;, "default", "", "check_ack", "wait_ACK", "tr_37", "RCAP_process [check_ack -> wait_ACK : default / ]")
				}
				/*---------------------------------------------------------*/



			/** state (del_extra_rcv_packet) enter executives **/
			FSM_STATE_ENTER_FORCED (10, "del_extra_rcv_packet", state10_enter_exec, "RCAP_process [del_extra_rcv_packet enter execs]")
				FSM_PROFILE_SECTION_IN ("RCAP_process [del_extra_rcv_packet enter execs]", state10_enter_exec)
				{
				extra_pkptr=op_pk_get(op_intrpt_strm());
				//printf("extra packet witd id packet =%d deleted\n",op_pk_id (extra_pkptr));
				op_pk_destroy(extra_pkptr);
				
				
				}
				FSM_PROFILE_SECTION_OUT (state10_enter_exec)

			/** state (del_extra_rcv_packet) exit executives **/
			FSM_STATE_EXIT_FORCED (10, "del_extra_rcv_packet", "RCAP_process [del_extra_rcv_packet exit execs]")


			/** state (del_extra_rcv_packet) transition processing **/
			FSM_TRANSIT_FORCE (8, state8_enter_exec, ;, "default", "", "del_extra_rcv_packet", "wait_BIFS", "tr_40", "RCAP_process [del_extra_rcv_packet -> wait_BIFS : default / ]")
				/*---------------------------------------------------------*/



			/** state (del_extra_rcv_packet1) enter executives **/
			FSM_STATE_ENTER_FORCED (11, "del_extra_rcv_packet1", state11_enter_exec, "RCAP_process [del_extra_rcv_packet1 enter execs]")

			/** state (del_extra_rcv_packet1) exit executives **/
			FSM_STATE_EXIT_FORCED (11, "del_extra_rcv_packet1", "RCAP_process [del_extra_rcv_packet1 exit execs]")
				FSM_PROFILE_SECTION_IN ("RCAP_process [del_extra_rcv_packet1 exit execs]", state11_exit_exec)
				{
				extra_pkptr=op_pk_get(op_intrpt_strm());
				op_pk_destroy(extra_pkptr);
				}
				FSM_PROFILE_SECTION_OUT (state11_exit_exec)


			/** state (del_extra_rcv_packet1) transition processing **/
			FSM_TRANSIT_FORCE (0, state0_enter_exec, ;, "default", "", "del_extra_rcv_packet1", "Idle", "tr_42", "RCAP_process [del_extra_rcv_packet1 -> Idle : default / ]")
				/*---------------------------------------------------------*/



			/** state (svc_Q) enter executives **/
			FSM_STATE_ENTER_FORCED (12, "svc_Q", state12_enter_exec, "RCAP_process [svc_Q enter execs]")
				FSM_PROFILE_SECTION_IN ("RCAP_process [svc_Q enter execs]", state12_enter_exec)
				{
				Data_pkptrQ=op_subq_pk_remove (0, OPC_QPOS_HEAD);
				copyPkt = op_pk_copy (Data_pkptrQ);
				pk_svc_time=op_pk_total_size_get (Data_pkptrQ)/service_rate;
				node_id=op_id_self();
				//printf("node_id=%d",node_id);		
				op_pk_nfd_set_int32 (copyPkt, "src_ID",node_id);
				op_pk_nfd_set_int32 (copyPkt, "dest_ID",0);
				op_pk_nfd_set_int32 (copyPkt, "frame_type", 4);//acording to the standard, type value of data is 4
				}
				FSM_PROFILE_SECTION_OUT (state12_enter_exec)

			/** state (svc_Q) exit executives **/
			FSM_STATE_EXIT_FORCED (12, "svc_Q", "RCAP_process [svc_Q exit execs]")


			/** state (svc_Q) transition processing **/
			FSM_TRANSIT_FORCE (2, state2_enter_exec, ;, "default", "", "svc_Q", "backoff", "tr_51", "RCAP_process [svc_Q -> backoff : default / ]")
				/*---------------------------------------------------------*/



			/** state (check_time) enter executives **/
			FSM_STATE_ENTER_FORCED (13, "check_time", state13_enter_exec, "RCAP_process [check_time enter execs]")
				FSM_PROFILE_SECTION_IN ("RCAP_process [check_time enter execs]", state13_enter_exec)
				{
				CheckTime_ForSend_pkt();
				}
				FSM_PROFILE_SECTION_OUT (state13_enter_exec)

			/** state (check_time) exit executives **/
			FSM_STATE_EXIT_FORCED (13, "check_time", "RCAP_process [check_time exit execs]")


			/** state (check_time) transition processing **/
			FSM_PROFILE_SECTION_IN ("RCAP_process [check_time trans conditions]", state13_trans_conds)
			FSM_INIT_COND (WaitForSD==0)
			FSM_TEST_COND (WaitForSD==1)
			FSM_TEST_LOGIC ("check_time")
			FSM_PROFILE_SECTION_OUT (state13_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 15, state15_enter_exec, ;, "WaitForSD==0", "", "check_time", "send_packet", "tr_68", "RCAP_process [check_time -> send_packet : WaitForSD==0 / ]")
				FSM_CASE_TRANSIT (1, 14, state14_enter_exec, ;, "WaitForSD==1", "", "check_time", "WaitFor_nextSD", "tr_69", "RCAP_process [check_time -> WaitFor_nextSD : WaitForSD==1 / ]")
				}
				/*---------------------------------------------------------*/



			/** state (WaitFor_nextSD) enter executives **/
			FSM_STATE_ENTER_UNFORCED (14, "WaitFor_nextSD", state14_enter_exec, "RCAP_process [WaitFor_nextSD enter execs]")

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (29,"RCAP_process")


			/** state (WaitFor_nextSD) exit executives **/
			FSM_STATE_EXIT_UNFORCED (14, "WaitFor_nextSD", "RCAP_process [WaitFor_nextSD exit execs]")


			/** state (WaitFor_nextSD) transition processing **/
			FSM_PROFILE_SECTION_IN ("RCAP_process [WaitFor_nextSD trans conditions]", state14_trans_conds)
			FSM_INIT_COND (NextSD_intr)
			FSM_TEST_COND (StrFromLinkArrival)
			FSM_TEST_COND (CAPFrameArrival)
			FSM_DFLT_COND
			FSM_TEST_LOGIC ("WaitFor_nextSD")
			FSM_PROFILE_SECTION_OUT (state14_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 15, state15_enter_exec, ;, "NextSD_intr", "", "WaitFor_nextSD", "send_packet", "tr_70", "RCAP_process [WaitFor_nextSD -> send_packet : NextSD_intr / ]")
				FSM_CASE_TRANSIT (1, 20, state20_enter_exec, ;, "StrFromLinkArrival", "", "WaitFor_nextSD", "del_extra_rcv_packet5", "tr_92", "RCAP_process [WaitFor_nextSD -> del_extra_rcv_packet5 : StrFromLinkArrival / ]")
				FSM_CASE_TRANSIT (2, 14, state14_enter_exec, Qframe();;, "CAPFrameArrival", "Qframe();", "WaitFor_nextSD", "WaitFor_nextSD", "tr_94", "RCAP_process [WaitFor_nextSD -> WaitFor_nextSD : CAPFrameArrival / Qframe();]")
				FSM_CASE_TRANSIT (3, 14, state14_enter_exec, ;, "default", "", "WaitFor_nextSD", "WaitFor_nextSD", "tr_72", "RCAP_process [WaitFor_nextSD -> WaitFor_nextSD : default / ]")
				}
				/*---------------------------------------------------------*/



			/** state (send_packet) enter executives **/
			FSM_STATE_ENTER_FORCED (15, "send_packet", state15_enter_exec, "RCAP_process [send_packet enter execs]")
				FSM_PROFILE_SECTION_IN ("RCAP_process [send_packet enter execs]", state15_enter_exec)
				{
				send_packet();
				}
				FSM_PROFILE_SECTION_OUT (state15_enter_exec)

			/** state (send_packet) exit executives **/
			FSM_STATE_EXIT_FORCED (15, "send_packet", "RCAP_process [send_packet exit execs]")


			/** state (send_packet) transition processing **/
			FSM_TRANSIT_FORCE (4, state4_enter_exec, ;, "default", "", "send_packet", "wait_ACK", "tr_71", "RCAP_process [send_packet -> wait_ACK : default / ]")
				/*---------------------------------------------------------*/



			/** state (init) enter executives **/
			FSM_STATE_ENTER_FORCED_NOLABEL (16, "init", "RCAP_process [init enter execs]")
				FSM_PROFILE_SECTION_IN ("RCAP_process [init enter execs]", state16_enter_exec)
				{
				max_RetryCount=3;
				service_rate=900000000;
				BackoffExponent=3;
				RetryCount=0;
				NSendPKT=0;
				
				}
				FSM_PROFILE_SECTION_OUT (state16_enter_exec)

			/** state (init) exit executives **/
			FSM_STATE_EXIT_FORCED (16, "init", "RCAP_process [init exit execs]")


			/** state (init) transition processing **/
			FSM_PROFILE_SECTION_IN ("RCAP_process [init trans conditions]", state16_trans_conds)
			FSM_INIT_COND (en_RCAP==1 && !QEmpty)
			FSM_DFLT_COND
			FSM_TEST_LOGIC ("init")
			FSM_PROFILE_SECTION_OUT (state16_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 12, state12_enter_exec, ;, "en_RCAP==1 && !QEmpty", "", "init", "svc_Q", "tr_81", "RCAP_process [init -> svc_Q : en_RCAP==1 && !QEmpty / ]")
				FSM_CASE_TRANSIT (1, 0, state0_enter_exec, ;, "default", "", "init", "Idle", "tr_76", "RCAP_process [init -> Idle : default / ]")
				}
				/*---------------------------------------------------------*/



			/** state (wait_2*SIFS) enter executives **/
			FSM_STATE_ENTER_UNFORCED (17, "wait_2*SIFS", state17_enter_exec, "RCAP_process [wait_2*SIFS enter execs]")

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (35,"RCAP_process")


			/** state (wait_2*SIFS) exit executives **/
			FSM_STATE_EXIT_UNFORCED (17, "wait_2*SIFS", "RCAP_process [wait_2*SIFS exit execs]")


			/** state (wait_2*SIFS) transition processing **/
			FSM_PROFILE_SECTION_IN ("RCAP_process [wait_2*SIFS trans conditions]", state17_trans_conds)
			FSM_INIT_COND (SIFS_intr)
			FSM_TEST_COND (CAPFrameArrival)
			FSM_TEST_COND (StrFromLinkArrival)
			FSM_DFLT_COND
			FSM_TEST_LOGIC ("wait_2*SIFS")
			FSM_PROFILE_SECTION_OUT (state17_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 5, state5_enter_exec, ;, "SIFS_intr", "", "wait_2*SIFS", "PKTSending_success", "tr_84", "RCAP_process [wait_2*SIFS -> PKTSending_success : SIFS_intr / ]")
				FSM_CASE_TRANSIT (1, 17, state17_enter_exec, Qframe();;, "CAPFrameArrival", "Qframe();", "wait_2*SIFS", "wait_2*SIFS", "tr_87", "RCAP_process [wait_2*SIFS -> wait_2*SIFS : CAPFrameArrival / Qframe();]")
				FSM_CASE_TRANSIT (2, 18, state18_enter_exec, ;, "StrFromLinkArrival", "", "wait_2*SIFS", "del_extra_rcv_packet3", "tr_88", "RCAP_process [wait_2*SIFS -> del_extra_rcv_packet3 : StrFromLinkArrival / ]")
				FSM_CASE_TRANSIT (3, 17, state17_enter_exec, ;, "default", "", "wait_2*SIFS", "wait_2*SIFS", "tr_86", "RCAP_process [wait_2*SIFS -> wait_2*SIFS : default / ]")
				}
				/*---------------------------------------------------------*/



			/** state (del_extra_rcv_packet3) enter executives **/
			FSM_STATE_ENTER_FORCED (18, "del_extra_rcv_packet3", state18_enter_exec, "RCAP_process [del_extra_rcv_packet3 enter execs]")
				FSM_PROFILE_SECTION_IN ("RCAP_process [del_extra_rcv_packet3 enter execs]", state18_enter_exec)
				{
				extra_pkptr=op_pk_get(op_intrpt_strm());
				op_pk_destroy(extra_pkptr);
				}
				FSM_PROFILE_SECTION_OUT (state18_enter_exec)

			/** state (del_extra_rcv_packet3) exit executives **/
			FSM_STATE_EXIT_FORCED (18, "del_extra_rcv_packet3", "RCAP_process [del_extra_rcv_packet3 exit execs]")


			/** state (del_extra_rcv_packet3) transition processing **/
			FSM_TRANSIT_FORCE (17, state17_enter_exec, ;, "default", "", "del_extra_rcv_packet3", "wait_2*SIFS", "tr_89", "RCAP_process [del_extra_rcv_packet3 -> wait_2*SIFS : default / ]")
				/*---------------------------------------------------------*/



			/** state (del_extra_rcv_packet4) enter executives **/
			FSM_STATE_ENTER_FORCED (19, "del_extra_rcv_packet4", state19_enter_exec, "RCAP_process [del_extra_rcv_packet4 enter execs]")

			/** state (del_extra_rcv_packet4) exit executives **/
			FSM_STATE_EXIT_FORCED (19, "del_extra_rcv_packet4", "RCAP_process [del_extra_rcv_packet4 exit execs]")


			/** state (del_extra_rcv_packet4) transition processing **/
			FSM_TRANSIT_FORCE (7, state7_enter_exec, ;, "default", "", "del_extra_rcv_packet4", "wait_RIFS", "tr_91", "RCAP_process [del_extra_rcv_packet4 -> wait_RIFS : default / ]")
				/*---------------------------------------------------------*/



			/** state (del_extra_rcv_packet5) enter executives **/
			FSM_STATE_ENTER_FORCED (20, "del_extra_rcv_packet5", state20_enter_exec, "RCAP_process [del_extra_rcv_packet5 enter execs]")
				FSM_PROFILE_SECTION_IN ("RCAP_process [del_extra_rcv_packet5 enter execs]", state20_enter_exec)
				{
				extra_pkptr=op_pk_get(op_intrpt_strm());
				//printf("extra packet witd id packet =%d deleted\n",op_pk_id (extra_pkptr));
				op_pk_destroy(extra_pkptr);
				
				
				}
				FSM_PROFILE_SECTION_OUT (state20_enter_exec)

			/** state (del_extra_rcv_packet5) exit executives **/
			FSM_STATE_EXIT_FORCED (20, "del_extra_rcv_packet5", "RCAP_process [del_extra_rcv_packet5 exit execs]")


			/** state (del_extra_rcv_packet5) transition processing **/
			FSM_TRANSIT_FORCE (14, state14_enter_exec, ;, "default", "", "del_extra_rcv_packet5", "WaitFor_nextSD", "tr_93", "RCAP_process [del_extra_rcv_packet5 -> WaitFor_nextSD : default / ]")
				/*---------------------------------------------------------*/



			}


		FSM_EXIT (16,"RCAP_process")
		}
	}




void
_op_RCAP_process_diag (OP_SIM_CONTEXT_ARG_OPT)
	{
	/* No Diagnostic Block */
	}




void
_op_RCAP_process_terminate (OP_SIM_CONTEXT_ARG_OPT)
	{

	FIN_MT (_op_RCAP_process_terminate ())


	/* No Termination Block */

	Vos_Poolmem_Dealloc (op_sv_ptr);

	FOUT
	}


/* Undefine shortcuts to state variables to avoid */
/* syntax error in direct access to fields of */
/* local variable prs_ptr in _op_RCAP_process_svar function. */
#undef service_rate
#undef BIFS_intr_handle
#undef RIFS_intr_handle
#undef Timeout_handle
#undef NextSD_intr_handle
#undef ete_gsh
#undef node_id
#undef address_dist
#undef Data_pkptr
#undef BackoffWindow
#undef BackoffExponent
#undef ack_correct
#undef ack_pk
#undef RetryCount
#undef copyPkt
#undef extra_pkptr
#undef tx_state
#undef invoke_ACAP_intr_handle
#undef ACAP_proc
#undef WaitForSD
#undef pk_svc_time
#undef Data_pkptrQ
#undef SIFS_intr_handle
#undef NSendPKT

#undef FIN_PREAMBLE_DEC
#undef FIN_PREAMBLE_CODE

#define FIN_PREAMBLE_DEC
#define FIN_PREAMBLE_CODE

VosT_Obtype
_op_RCAP_process_init (int * init_block_ptr)
	{
	VosT_Obtype obtype = OPC_NIL;
	FIN_MT (_op_RCAP_process_init (init_block_ptr))

	obtype = Vos_Define_Object_Prstate ("proc state vars (RCAP_process)",
		sizeof (RCAP_process_state));
	*init_block_ptr = 32;

	FRET (obtype)
	}

VosT_Address
_op_RCAP_process_alloc (VosT_Obtype obtype, int init_block)
	{
#if !defined (VOSD_NO_FIN)
	int _op_block_origin = 0;
#endif
	RCAP_process_state * ptr;
	FIN_MT (_op_RCAP_process_alloc (obtype))

	ptr = (RCAP_process_state *)Vos_Alloc_Object (obtype);
	if (ptr != OPC_NIL)
		{
		ptr->_op_current_block = init_block;
#if defined (OPD_ALLOW_ODB)
		ptr->_op_current_state = "RCAP_process [init enter execs]";
#endif
		}
	FRET ((VosT_Address)ptr)
	}



void
_op_RCAP_process_svar (void * gen_ptr, const char * var_name, void ** var_p_ptr)
	{
	RCAP_process_state		*prs_ptr;

	FIN_MT (_op_RCAP_process_svar (gen_ptr, var_name, var_p_ptr))

	if (var_name == OPC_NIL)
		{
		*var_p_ptr = (void *)OPC_NIL;
		FOUT
		}
	prs_ptr = (RCAP_process_state *)gen_ptr;

	if (strcmp ("service_rate" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->service_rate);
		FOUT
		}
	if (strcmp ("BIFS_intr_handle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->BIFS_intr_handle);
		FOUT
		}
	if (strcmp ("RIFS_intr_handle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->RIFS_intr_handle);
		FOUT
		}
	if (strcmp ("Timeout_handle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->Timeout_handle);
		FOUT
		}
	if (strcmp ("NextSD_intr_handle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->NextSD_intr_handle);
		FOUT
		}
	if (strcmp ("ete_gsh" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->ete_gsh);
		FOUT
		}
	if (strcmp ("node_id" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->node_id);
		FOUT
		}
	if (strcmp ("address_dist" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->address_dist);
		FOUT
		}
	if (strcmp ("Data_pkptr" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->Data_pkptr);
		FOUT
		}
	if (strcmp ("BackoffWindow" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->BackoffWindow);
		FOUT
		}
	if (strcmp ("BackoffExponent" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->BackoffExponent);
		FOUT
		}
	if (strcmp ("ack_correct" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->ack_correct);
		FOUT
		}
	if (strcmp ("ack_pk" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->ack_pk);
		FOUT
		}
	if (strcmp ("RetryCount" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->RetryCount);
		FOUT
		}
	if (strcmp ("copyPkt" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->copyPkt);
		FOUT
		}
	if (strcmp ("extra_pkptr" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->extra_pkptr);
		FOUT
		}
	if (strcmp ("tx_state" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->tx_state);
		FOUT
		}
	if (strcmp ("invoke_ACAP_intr_handle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->invoke_ACAP_intr_handle);
		FOUT
		}
	if (strcmp ("ACAP_proc" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->ACAP_proc);
		FOUT
		}
	if (strcmp ("WaitForSD" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->WaitForSD);
		FOUT
		}
	if (strcmp ("pk_svc_time" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->pk_svc_time);
		FOUT
		}
	if (strcmp ("Data_pkptrQ" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->Data_pkptrQ);
		FOUT
		}
	if (strcmp ("SIFS_intr_handle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->SIFS_intr_handle);
		FOUT
		}
	if (strcmp ("NSendPKT" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->NSendPKT);
		FOUT
		}
	*var_p_ptr = (void *)OPC_NIL;

	FOUT
	}

