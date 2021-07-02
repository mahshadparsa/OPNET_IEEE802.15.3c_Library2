/* Process model C form file: ACAP_proc.pr.c */
/* Portions of this file copyright 1986-2008 by OPNET Technologies, Inc. */



/* This variable carries the header into the object file */
const char ACAP_proc_pr_c [] = "MIL_3_Tfile_Hdr_ 145A 30A op_runsim 7 59100F61 59100F61 1 DESKTOP-FFL2R3R user 0 0 none none 0 0 none 0 0 0 0 0 0 0 0 1e80 8                                                                                                                                                                                                                                                                                                                                                                                                   ";
#include <string.h>



/* OPNET system definitions */
#include <opnet.h>



/* Header Block */

/* packet stream definitions */
#define StreamFromSrcCTA  2
#define StreamFromSrcCAP  1
#define StreamFromLink 0
#define StreamToLink   0


#define PowerUp         (op_intrpt_type() == OPC_INTRPT_BEGSIM)
#define CAPFrameArrival (op_intrpt_type() == OPC_INTRPT_STRM && op_intrpt_strm() == StreamFromSrcCAP)
#define CTAFrameArrival (op_intrpt_type() == OPC_INTRPT_STRM && op_intrpt_strm() == StreamFromSrcCTA)
#define StrFromLinkArrival  (op_intrpt_type() == OPC_INTRPT_STRM && op_intrpt_strm() == StreamFromLink)

#define ACAP_Timeout_Code 0
#define ACAP_Timeout         (op_intrpt_type() == OPC_INTRPT_SELF && op_intrpt_code() == ACAP_Timeout_Code)

#define ACAP_BIFS_intr_Code 4
#define ACAP_BIFS_intr       (op_intrpt_type() == OPC_INTRPT_SELF && op_intrpt_code() == ACAP_BIFS_intr_Code)

#define ACAP_RIFS_intr_Code 2
#define ACAP_RIFS_intr       (op_intrpt_type() == OPC_INTRPT_SELF && op_intrpt_code() == ACAP_RIFS_intr_Code)

#define ACAP_PacketSent_intr_code 3
#define ACAP_PacketSent_intr (op_intrpt_type() == OPC_INTRPT_SELF && op_intrpt_code() == ACAP_PacketSent_intr_code)

#define Req_intr_Code 5
#define Req_intr       (op_intrpt_type() == OPC_INTRPT_SELF && op_intrpt_code() == Req_intr_Code)

#define CheckR_intr_Code 6
#define CheckR_intr       (op_intrpt_type() == OPC_INTRPT_SELF && op_intrpt_code() == CheckR_intr_Code)

#define ch_busy 0
#define channel_busy (op_stat_local_read(ch_busy)==1)


#define LinkFail 	    (op_intrpt_type() == OPC_INTRPT_FAIL)
#define LinkUp		    (op_intrpt_type() == OPC_INTRPT_RECOVER)
#define ACAP_QEmpty 	(op_subq_stat(1, OPC_QSTAT_PKSIZE) == 0.0)
int en_beacon,en_ACAP,en_MCTA,en_CTA;
double beacon_time;
double RCAP_time;
double ACAP_time;
double MCTA_time;
double CTA_time;
double SD_time;
double beacon_start_time;
double ACAP_start_time;
double MCTA_start_time;
double CTA_start_time;

double SIFS_time;

double ACAP_pk_svc_time;
double ACAP_ACK_svc_time;

int associated;
//int req_sended;

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
	Evhandle	               		ACAP_BIFS_intr_handle                           ;
	Evhandle	               		ACAP_RIFS_intr_handle                           ;
	Evhandle	               		ACAP_Timeout_handle                             ;
	Evhandle	               		ACAP_PacketSent_intr_handle                     ;
	Stathandle	             		ete_gsh                                         ;
	Objid	                  		node_id                                         ;
	Evhandle	               		Req_intr_handle                                 ;
	int	                    		ACAP_BackoffWindow                              ;
	int	                    		ACAP_BackoffExponent                            ;
	int	                    		ACAP_ack_correct                                ;
	Packet *	               		ACAP_ack_pk                                     ;
	int	                    		ACAP_RetryCount                                 ;
	Packet *	               		ACAP_copyPkt                                    ;
	Packet *	               		ACAP_extra_pkptr                                ;
	double	                 		ACAP_tx_state                                   ;
	Packet *	               		CTA_req_pk                                      ;
	Evhandle	               		CheckR_intr_handle                              ;
	int	                    		req_sended                                      ;
	} ACAP_proc_state;

#define service_rate            		op_sv_ptr->service_rate
#define ACAP_BIFS_intr_handle   		op_sv_ptr->ACAP_BIFS_intr_handle
#define ACAP_RIFS_intr_handle   		op_sv_ptr->ACAP_RIFS_intr_handle
#define ACAP_Timeout_handle     		op_sv_ptr->ACAP_Timeout_handle
#define ACAP_PacketSent_intr_handle		op_sv_ptr->ACAP_PacketSent_intr_handle
#define ete_gsh                 		op_sv_ptr->ete_gsh
#define node_id                 		op_sv_ptr->node_id
#define Req_intr_handle         		op_sv_ptr->Req_intr_handle
#define ACAP_BackoffWindow      		op_sv_ptr->ACAP_BackoffWindow
#define ACAP_BackoffExponent    		op_sv_ptr->ACAP_BackoffExponent
#define ACAP_ack_correct        		op_sv_ptr->ACAP_ack_correct
#define ACAP_ack_pk             		op_sv_ptr->ACAP_ack_pk
#define ACAP_RetryCount         		op_sv_ptr->ACAP_RetryCount
#define ACAP_copyPkt            		op_sv_ptr->ACAP_copyPkt
#define ACAP_extra_pkptr        		op_sv_ptr->ACAP_extra_pkptr
#define ACAP_tx_state           		op_sv_ptr->ACAP_tx_state
#define CTA_req_pk              		op_sv_ptr->CTA_req_pk
#define CheckR_intr_handle      		op_sv_ptr->CheckR_intr_handle
#define req_sended              		op_sv_ptr->req_sended

/* These macro definitions will define a local variable called	*/
/* "op_sv_ptr" in each function containing a FIN statement.	*/
/* This variable points to the state variable data structure,	*/
/* and can be used from a C debugger to display their values.	*/
#undef FIN_PREAMBLE_DEC
#undef FIN_PREAMBLE_CODE
#define FIN_PREAMBLE_DEC	ACAP_proc_state *op_sv_ptr;
#define FIN_PREAMBLE_CODE	\
		op_sv_ptr = ((ACAP_proc_state *)(OP_SIM_CONTEXT_PTR->_op_mod_state_ptr));


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
	ACAP_BackoffWindow=op_dist_outcome (op_dist_load ("uniform_int",0,(2<<ACAP_BackoffExponent)-1));
	printf("ACAP_BackoffWindow=%d\n",ACAP_BackoffWindow);
	
	FOUT;
	}
/**************************************************************************************/
/**************************************************************************************/
static void creat_BIFS_Intr()
	{
	double BIFS_time=0.0000065;//0.00000065; 
	
	FIN (creat_BIFS_Intr ());
	if (ACAP_time-op_sim_time ()+ACAP_start_time<ACAP_pk_svc_time+2*SIFS_time+ACAP_ACK_svc_time)
		{
		//printf("CAP_start_time=%2g,ACAP_start_time+SD_time+ BIFS_time=%2g\n",ACAP_start_time,ACAP_start_time+SD_time+ BIFS_time);
		ACAP_BIFS_intr_handle=op_intrpt_schedule_self(ACAP_start_time+SD_time+ BIFS_time,ACAP_BIFS_intr_Code);
		}
	else
		ACAP_BIFS_intr_handle=op_intrpt_schedule_self(op_sim_time()+ BIFS_time,ACAP_BIFS_intr_Code);
	//printf("backoffWindow=%d",BackoffWindow);
	//printf("channel_busy=%d",channel_busy);
	FOUT;
	}
/**************************************************************************************/
/**************************************************************************************/
static void creat_Sent_pkt_Intr()
	{
	OpT_Packet_Size  pk_len=720;
	
	FIN (creat_Sent_pkt_Intr ());
	
	/* determine the time required to complete 	service of the packet	*/
	
	pk_len= op_pk_total_size_get(ACAP_copyPkt);
	ACAP_pk_svc_time=pk_len/ service_rate;
	ACAP_ACK_svc_time=496/ service_rate;
	//printf("op_sim_time ()=%2g, ACAP_pk_svc_time=%2g,2*SIFS_time=%2g,ACAP_ACK_svc_time=%2g,ACAP_start_time=%2g",op_sim_time (), ACAP_pk_svc_time,2*SIFS_time,ACAP_ACK_svc_time,ACAP_start_time);	
	if (ACAP_time-op_sim_time ()+ACAP_start_time<ACAP_pk_svc_time+2*SIFS_time+ACAP_ACK_svc_time)
		{
		
		//PacketSent_intr_handle=op_intrpt_schedule_self(CAP_start_time+SD_time,PacketSent_intr_code);//pk_svc_time
		ACAP_Timeout_handle=op_intrpt_schedule_self(ACAP_start_time+SD_time+ ACAP_pk_svc_time+2*SIFS_time+ACAP_ACK_svc_time,ACAP_Timeout_Code); 		
		}
		else
		{
		//PacketSent_intr_handle=op_intrpt_schedule_self(op_sim_time (),PacketSent_intr_code);//pk_svc_time
		ACAP_Timeout_handle=op_intrpt_schedule_self(op_sim_time ()+ ACAP_pk_svc_time+2*SIFS_time+ACAP_ACK_svc_time,ACAP_Timeout_Code);
		
		}
	FOUT;
	}
/**************************************************************************************/
/**************************************************************************************/
static void creat_RIFS_Intr()
	{
	double RIFS_time=0.000009;//0.0000009; 
	FIN (creat_BIFS_Intr ());
	ACAP_RIFS_intr_handle=op_intrpt_schedule_self(op_sim_time()+ RIFS_time,ACAP_RIFS_intr_Code);
	FOUT;
	}
/**************************************************************************************/
/**************************************************************************************/
static void Qframe()
	{ 
	FIN (Qframe ());
	op_subq_pk_insert (1, op_pk_get(op_intrpt_strm()), OPC_QPOS_TAIL);
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
		else if(ACAP_BackoffWindow>0)
		{
			ACAP_BackoffWindow=ACAP_BackoffWindow-1;
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
	FIN (check_ack());
	ACAP_ack_pk=op_pk_get(op_intrpt_strm());
	op_pk_nfd_get_int32 (ACAP_ack_pk, "frame_type",&frame_type ); //acording to the standard, type value of beacon is zero
	op_pk_nfd_get_int32 (ACAP_ack_pk, "dest_ID",&dest_ID);
	if (frame_type==1 && dest_ID==node_id)
		{
		ACAP_ack_correct=1;
		req_sended=1;
		}
	else
		{
		ACAP_ack_correct=0;
		printf("\n ACAP_Proc: dest_ID=%d,node_id=%d",dest_ID,node_id);
		printf("pkID=%d\n",op_pk_id (ACAP_ack_pk));
		}
	op_pk_destroy(ACAP_ack_pk);
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
	void ACAP_proc (OP_SIM_CONTEXT_ARG_OPT);
	VosT_Obtype _op_ACAP_proc_init (int * init_block_ptr);
	void _op_ACAP_proc_diag (OP_SIM_CONTEXT_ARG_OPT);
	void _op_ACAP_proc_terminate (OP_SIM_CONTEXT_ARG_OPT);
	VosT_Address _op_ACAP_proc_alloc (VosT_Obtype, int);
	void _op_ACAP_proc_svar (void *, const char *, void **);


#if defined (__cplusplus)
} /* end of 'extern "C"' */
#endif




/* Process model interrupt handling procedure */


void
ACAP_proc (OP_SIM_CONTEXT_ARG_OPT)
	{
#if !defined (VOSD_NO_FIN)
	int _op_block_origin = 0;
#endif
	FIN_MT (ACAP_proc ());

		{
		/* Temporary Variables */
		//int RetryCount=0;
		int BWRandom=0;
		int max_RetryCount=3;
		
		/* End of Temporary Variables */


		FSM_ENTER ("ACAP_proc")

		FSM_BLOCK_SWITCH
			{
			/*---------------------------------------------------------*/
			/** state (Idle) enter executives **/
			FSM_STATE_ENTER_UNFORCED (0, "Idle", state0_enter_exec, "ACAP_proc [Idle enter execs]")
				FSM_PROFILE_SECTION_IN ("ACAP_proc [Idle enter execs]", state0_enter_exec)
				{
				/* Register handle for statistic collection. */
				ete_gsh = op_stat_reg("ETE Delay", OPC_STAT_INDEX_NONE, OPC_STAT_GLOBAL);
				ACAP_BackoffExponent=3;
				ACAP_RetryCount=0;
				max_RetryCount=3;
				service_rate=900000000;
				}
				FSM_PROFILE_SECTION_OUT (state0_enter_exec)

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (1,"ACAP_proc")


			/** state (Idle) exit executives **/
			FSM_STATE_EXIT_UNFORCED (0, "Idle", "ACAP_proc [Idle exit execs]")


			/** state (Idle) transition processing **/
			FSM_PROFILE_SECTION_IN ("ACAP_proc [Idle trans conditions]", state0_trans_conds)
			FSM_INIT_COND (StrFromLinkArrival && !Req_intr )
			FSM_TEST_COND (CTAFrameArrival)
			FSM_TEST_COND (Req_intr)
			FSM_TEST_COND (CheckR_intr)
			FSM_DFLT_COND
			FSM_TEST_LOGIC ("Idle")
			FSM_PROFILE_SECTION_OUT (state0_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 11, state11_enter_exec, ;, "StrFromLinkArrival && !Req_intr ", "", "Idle", "del_extra_rcv_packet1", "tr_41", "ACAP_proc [Idle -> del_extra_rcv_packet1 : StrFromLinkArrival && !Req_intr  / ]")
				FSM_CASE_TRANSIT (1, 0, state0_enter_exec, Qframe();;, "CTAFrameArrival", "Qframe();", "Idle", "Idle", "tr_52", "ACAP_proc [Idle -> Idle : CTAFrameArrival / Qframe();]")
				FSM_CASE_TRANSIT (2, 1, state1_enter_exec, ;, "Req_intr", "", "Idle", "svc_packet", "tr_36_0", "ACAP_proc [Idle -> svc_packet : Req_intr / ]")
				FSM_CASE_TRANSIT (3, 13, state13_enter_exec, ;, "CheckR_intr", "", "Idle", "init", "tr_37_0", "ACAP_proc [Idle -> init : CheckR_intr / ]")
				FSM_CASE_TRANSIT (4, 0, state0_enter_exec, ;, "default", "", "Idle", "Idle", "tr_24", "ACAP_proc [Idle -> Idle : default / ]")
				}
				/*---------------------------------------------------------*/



			/** state (svc_packet) enter executives **/
			FSM_STATE_ENTER_FORCED (1, "svc_packet", state1_enter_exec, "ACAP_proc [svc_packet enter execs]")

			/** state (svc_packet) exit executives **/
			FSM_STATE_EXIT_FORCED (1, "svc_packet", "ACAP_proc [svc_packet exit execs]")
				FSM_PROFILE_SECTION_IN ("ACAP_proc [svc_packet exit execs]", state1_exit_exec)
				{
				/* Get pointer to packet from stream index. */
				CTA_req_pk = op_pk_create_fmt ("CTA_req_pkt");
				op_pk_total_size_set (CTA_req_pk, 720); 
				ACAP_copyPkt = op_pk_copy (CTA_req_pk);
				node_id=op_id_self();
				op_pk_nfd_set_int32 (CTA_req_pk, "src_ID",node_id);
				op_pk_nfd_set_int32 (CTA_req_pk, "dest_ID",0);
				op_pk_nfd_set_int32 (CTA_req_pk, "frame_type", 3);//acording to the standard, type value of association request is 3
				op_pk_nfd_set_int32 (CTA_req_pk, "CTRqB_Target_IDlist", node_id);
				
				op_pk_nfd_set_int32 (ACAP_copyPkt, "src_ID",node_id);
				op_pk_nfd_set_int32 (ACAP_copyPkt, "dest_ID",0);
				op_pk_nfd_set_int32 (ACAP_copyPkt, "frame_type", 3);//acording to the standard, type value of association request is 3
				op_pk_nfd_set_int32 (ACAP_copyPkt, "CTRqB_Target_IDlist", node_id);
				
				
				}
				FSM_PROFILE_SECTION_OUT (state1_exit_exec)


			/** state (svc_packet) transition processing **/
			FSM_TRANSIT_FORCE (2, state2_enter_exec, ;, "default", "", "svc_packet", "backoff", "tr_54", "ACAP_proc [svc_packet -> backoff : default / ]")
				/*---------------------------------------------------------*/



			/** state (backoff) enter executives **/
			FSM_STATE_ENTER_FORCED (2, "backoff", state2_enter_exec, "ACAP_proc [backoff enter execs]")
				FSM_PROFILE_SECTION_IN ("ACAP_proc [backoff enter execs]", state2_enter_exec)
				{
				backoff ();
				creat_BIFS_Intr();
				}
				FSM_PROFILE_SECTION_OUT (state2_enter_exec)

			/** state (backoff) exit executives **/
			FSM_STATE_EXIT_FORCED (2, "backoff", "ACAP_proc [backoff exit execs]")


			/** state (backoff) transition processing **/
			FSM_TRANSIT_FORCE (8, state8_enter_exec, ;, "default", "", "backoff", "wait_BIFS", "tr_19", "ACAP_proc [backoff -> wait_BIFS : default / ]")
				/*---------------------------------------------------------*/



			/** state (check_channel) enter executives **/
			FSM_STATE_ENTER_FORCED (3, "check_channel", state3_enter_exec, "ACAP_proc [check_channel enter execs]")
				FSM_PROFILE_SECTION_IN ("ACAP_proc [check_channel enter execs]", state3_enter_exec)
				{
				check_channel();
				}
				FSM_PROFILE_SECTION_OUT (state3_enter_exec)

			/** state (check_channel) exit executives **/
			FSM_STATE_EXIT_FORCED (3, "check_channel", "ACAP_proc [check_channel exit execs]")


			/** state (check_channel) transition processing **/
			FSM_TRANSIT_FORCE (8, state8_enter_exec, ;, "default", "", "check_channel", "wait_BIFS", "tr_21", "ACAP_proc [check_channel -> wait_BIFS : default / ]")
				/*---------------------------------------------------------*/



			/** state (wait_ACK) enter executives **/
			FSM_STATE_ENTER_UNFORCED (4, "wait_ACK", state4_enter_exec, "ACAP_proc [wait_ACK enter execs]")

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (9,"ACAP_proc")


			/** state (wait_ACK) exit executives **/
			FSM_STATE_EXIT_UNFORCED (4, "wait_ACK", "ACAP_proc [wait_ACK exit execs]")


			/** state (wait_ACK) transition processing **/
			FSM_PROFILE_SECTION_IN ("ACAP_proc [wait_ACK trans conditions]", state4_trans_conds)
			FSM_INIT_COND (ACAP_Timeout && (ACAP_RetryCount+1)<=max_RetryCount)
			FSM_TEST_COND (ACAP_Timeout&& (ACAP_RetryCount+1)>max_RetryCount)
			FSM_TEST_COND (StrFromLinkArrival)
			FSM_TEST_COND (CTAFrameArrival)
			FSM_DFLT_COND
			FSM_TEST_LOGIC ("wait_ACK")
			FSM_PROFILE_SECTION_OUT (state4_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 6, state6_enter_exec, ;, "ACAP_Timeout && (ACAP_RetryCount+1)<=max_RetryCount", "", "wait_ACK", "PKTSending_faild", "tr_13", "ACAP_proc [wait_ACK -> PKTSending_faild : ACAP_Timeout && (ACAP_RetryCount+1)<=max_RetryCount / ]")
				FSM_CASE_TRANSIT (1, 5, state5_enter_exec, ;, "ACAP_Timeout&& (ACAP_RetryCount+1)>max_RetryCount", "", "wait_ACK", "PKTSending_success", "tr_18", "ACAP_proc [wait_ACK -> PKTSending_success : ACAP_Timeout&& (ACAP_RetryCount+1)>max_RetryCount / ]")
				FSM_CASE_TRANSIT (2, 9, state9_enter_exec, ;, "StrFromLinkArrival", "", "wait_ACK", "check_ack", "tr_36", "ACAP_proc [wait_ACK -> check_ack : StrFromLinkArrival / ]")
				FSM_CASE_TRANSIT (3, 4, state4_enter_exec, Qframe();;, "CTAFrameArrival", "Qframe();", "wait_ACK", "wait_ACK", "tr_44", "ACAP_proc [wait_ACK -> wait_ACK : CTAFrameArrival / Qframe();]")
				FSM_CASE_TRANSIT (4, 4, state4_enter_exec, ;, "default", "", "wait_ACK", "wait_ACK", "tr_25", "ACAP_proc [wait_ACK -> wait_ACK : default / ]")
				}
				/*---------------------------------------------------------*/



			/** state (PKTSending_success) enter executives **/
			FSM_STATE_ENTER_FORCED (5, "PKTSending_success", state5_enter_exec, "ACAP_proc [PKTSending_success enter execs]")
				FSM_PROFILE_SECTION_IN ("ACAP_proc [PKTSending_success enter execs]", state5_enter_exec)
				{
				ACAP_RetryCount=0;
				ACAP_BackoffExponent=3;
				}
				FSM_PROFILE_SECTION_OUT (state5_enter_exec)

			/** state (PKTSending_success) exit executives **/
			FSM_STATE_EXIT_FORCED (5, "PKTSending_success", "ACAP_proc [PKTSending_success exit execs]")


			/** state (PKTSending_success) transition processing **/
			FSM_PROFILE_SECTION_IN ("ACAP_proc [PKTSending_success trans conditions]", state5_trans_conds)
			FSM_INIT_COND (req_sended==1)
			FSM_TEST_COND (req_sended==0 && en_ACAP==1)
			FSM_TEST_LOGIC ("PKTSending_success")
			FSM_PROFILE_SECTION_OUT (state5_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 0, state0_enter_exec, ;, "req_sended==1", "", "PKTSending_success", "Idle", "tr_12", "ACAP_proc [PKTSending_success -> Idle : req_sended==1 / ]")
				FSM_CASE_TRANSIT (1, 1, state1_enter_exec, ;, "req_sended==0 && en_ACAP==1", "", "PKTSending_success", "svc_packet", "tr_30", "ACAP_proc [PKTSending_success -> svc_packet : req_sended==0 && en_ACAP==1 / ]")
				}
				/*---------------------------------------------------------*/



			/** state (PKTSending_faild) enter executives **/
			FSM_STATE_ENTER_FORCED (6, "PKTSending_faild", state6_enter_exec, "ACAP_proc [PKTSending_faild enter execs]")
				FSM_PROFILE_SECTION_IN ("ACAP_proc [PKTSending_faild enter execs]", state6_enter_exec)
				{
				ACAP_RetryCount=ACAP_RetryCount+1;
				ACAP_BackoffExponent=ACAP_BackoffExponent+1;
				ACAP_copyPkt = op_pk_copy (CTA_req_pk);
				op_pk_nfd_set_int32 (ACAP_copyPkt, "src_ID",node_id);
				op_pk_nfd_set_int32 (ACAP_copyPkt, "dest_ID",0);
				op_pk_nfd_set_int32 (ACAP_copyPkt, "frame_type", 3);//acording to the standard, type value of association request is 3
				op_pk_nfd_set_int32 (ACAP_copyPkt, "CTRqB_Target_IDlist", node_id);
				creat_RIFS_Intr();
				}
				FSM_PROFILE_SECTION_OUT (state6_enter_exec)

			/** state (PKTSending_faild) exit executives **/
			FSM_STATE_EXIT_FORCED (6, "PKTSending_faild", "ACAP_proc [PKTSending_faild exit execs]")


			/** state (PKTSending_faild) transition processing **/
			FSM_TRANSIT_FORCE (7, state7_enter_exec, ;, "default", "", "PKTSending_faild", "wait_RIFS", "tr_15", "ACAP_proc [PKTSending_faild -> wait_RIFS : default / ]")
				/*---------------------------------------------------------*/



			/** state (wait_RIFS) enter executives **/
			FSM_STATE_ENTER_UNFORCED (7, "wait_RIFS", state7_enter_exec, "ACAP_proc [wait_RIFS enter execs]")

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (15,"ACAP_proc")


			/** state (wait_RIFS) exit executives **/
			FSM_STATE_EXIT_UNFORCED (7, "wait_RIFS", "ACAP_proc [wait_RIFS exit execs]")


			/** state (wait_RIFS) transition processing **/
			FSM_PROFILE_SECTION_IN ("ACAP_proc [wait_RIFS trans conditions]", state7_trans_conds)
			FSM_INIT_COND (ACAP_RIFS_intr)
			FSM_TEST_COND (CTAFrameArrival)
			FSM_TEST_COND (StrFromLinkArrival)
			FSM_DFLT_COND
			FSM_TEST_LOGIC ("wait_RIFS")
			FSM_PROFILE_SECTION_OUT (state7_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 2, state2_enter_exec, ;, "ACAP_RIFS_intr", "", "wait_RIFS", "backoff", "tr_17", "ACAP_proc [wait_RIFS -> backoff : ACAP_RIFS_intr / ]")
				FSM_CASE_TRANSIT (1, 7, state7_enter_exec, Qframe();;, "CTAFrameArrival", "Qframe();", "wait_RIFS", "wait_RIFS", "tr_46", "ACAP_proc [wait_RIFS -> wait_RIFS : CTAFrameArrival / Qframe();]")
				FSM_CASE_TRANSIT (2, 14, state14_enter_exec, ;, "StrFromLinkArrival", "", "wait_RIFS", "del_extra_rcv_packet2", "tr_38_0", "ACAP_proc [wait_RIFS -> del_extra_rcv_packet2 : StrFromLinkArrival / ]")
				FSM_CASE_TRANSIT (3, 7, state7_enter_exec, ;, "default", "", "wait_RIFS", "wait_RIFS", "tr_27", "ACAP_proc [wait_RIFS -> wait_RIFS : default / ]")
				}
				/*---------------------------------------------------------*/



			/** state (wait_BIFS) enter executives **/
			FSM_STATE_ENTER_UNFORCED (8, "wait_BIFS", state8_enter_exec, "ACAP_proc [wait_BIFS enter execs]")

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (17,"ACAP_proc")


			/** state (wait_BIFS) exit executives **/
			FSM_STATE_EXIT_UNFORCED (8, "wait_BIFS", "ACAP_proc [wait_BIFS exit execs]")


			/** state (wait_BIFS) transition processing **/
			FSM_PROFILE_SECTION_IN ("ACAP_proc [wait_BIFS trans conditions]", state8_trans_conds)
			FSM_INIT_COND (ACAP_BIFS_intr && (ACAP_BackoffWindow>0 || channel_busy==1))
			FSM_TEST_COND (StrFromLinkArrival)
			FSM_TEST_COND (CTAFrameArrival)
			FSM_TEST_COND (ACAP_BIFS_intr && ACAP_BackoffWindow==0 && channel_busy==0)
			FSM_DFLT_COND
			FSM_TEST_LOGIC ("wait_BIFS")
			FSM_PROFILE_SECTION_OUT (state8_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 3, state3_enter_exec, ;, "ACAP_BIFS_intr && (ACAP_BackoffWindow>0 || channel_busy==1)", "", "wait_BIFS", "check_channel", "tr_20", "ACAP_proc [wait_BIFS -> check_channel : ACAP_BIFS_intr && (ACAP_BackoffWindow>0 || channel_busy==1) / ]")
				FSM_CASE_TRANSIT (1, 10, state10_enter_exec, ;, "StrFromLinkArrival", "", "wait_BIFS", "del_extra_rcv_packet", "tr_38", "ACAP_proc [wait_BIFS -> del_extra_rcv_packet : StrFromLinkArrival / ]")
				FSM_CASE_TRANSIT (2, 8, state8_enter_exec, Qframe();;, "CTAFrameArrival", "Qframe();", "wait_BIFS", "wait_BIFS", "tr_45", "ACAP_proc [wait_BIFS -> wait_BIFS : CTAFrameArrival / Qframe();]")
				FSM_CASE_TRANSIT (3, 12, state12_enter_exec, ;, "ACAP_BIFS_intr && ACAP_BackoffWindow==0 && channel_busy==0", "", "wait_BIFS", "send_pkt", "tr_55", "ACAP_proc [wait_BIFS -> send_pkt : ACAP_BIFS_intr && ACAP_BackoffWindow==0 && channel_busy==0 / ]")
				FSM_CASE_TRANSIT (4, 8, state8_enter_exec, ;, "default", "", "wait_BIFS", "wait_BIFS", "tr_26", "ACAP_proc [wait_BIFS -> wait_BIFS : default / ]")
				}
				/*---------------------------------------------------------*/



			/** state (check_ack) enter executives **/
			FSM_STATE_ENTER_FORCED (9, "check_ack", state9_enter_exec, "ACAP_proc [check_ack enter execs]")

			/** state (check_ack) exit executives **/
			FSM_STATE_EXIT_FORCED (9, "check_ack", "ACAP_proc [check_ack exit execs]")
				FSM_PROFILE_SECTION_IN ("ACAP_proc [check_ack exit execs]", state9_exit_exec)
				{
				check_ack();
				}
				FSM_PROFILE_SECTION_OUT (state9_exit_exec)


			/** state (check_ack) transition processing **/
			FSM_PROFILE_SECTION_IN ("ACAP_proc [check_ack trans conditions]", state9_trans_conds)
			FSM_INIT_COND (ACAP_ack_correct==1)
			FSM_DFLT_COND
			FSM_TEST_LOGIC ("check_ack")
			FSM_PROFILE_SECTION_OUT (state9_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 5, state5_enter_exec, ;, "ACAP_ack_correct==1", "", "check_ack", "PKTSending_success", "tr_35", "ACAP_proc [check_ack -> PKTSending_success : ACAP_ack_correct==1 / ]")
				FSM_CASE_TRANSIT (1, 4, state4_enter_exec, ;, "default", "", "check_ack", "wait_ACK", "tr_37", "ACAP_proc [check_ack -> wait_ACK : default / ]")
				}
				/*---------------------------------------------------------*/



			/** state (del_extra_rcv_packet) enter executives **/
			FSM_STATE_ENTER_FORCED (10, "del_extra_rcv_packet", state10_enter_exec, "ACAP_proc [del_extra_rcv_packet enter execs]")
				FSM_PROFILE_SECTION_IN ("ACAP_proc [del_extra_rcv_packet enter execs]", state10_enter_exec)
				{
				ACAP_extra_pkptr=op_pk_get(op_intrpt_strm());
				//printf("extra packet witd id packet =%d deleted",op_pk_id (ACAP_extra_pkptr));
				op_pk_destroy(ACAP_extra_pkptr);
				}
				FSM_PROFILE_SECTION_OUT (state10_enter_exec)

			/** state (del_extra_rcv_packet) exit executives **/
			FSM_STATE_EXIT_FORCED (10, "del_extra_rcv_packet", "ACAP_proc [del_extra_rcv_packet exit execs]")


			/** state (del_extra_rcv_packet) transition processing **/
			FSM_TRANSIT_FORCE (8, state8_enter_exec, ;, "default", "", "del_extra_rcv_packet", "wait_BIFS", "tr_40", "ACAP_proc [del_extra_rcv_packet -> wait_BIFS : default / ]")
				/*---------------------------------------------------------*/



			/** state (del_extra_rcv_packet1) enter executives **/
			FSM_STATE_ENTER_FORCED (11, "del_extra_rcv_packet1", state11_enter_exec, "ACAP_proc [del_extra_rcv_packet1 enter execs]")

			/** state (del_extra_rcv_packet1) exit executives **/
			FSM_STATE_EXIT_FORCED (11, "del_extra_rcv_packet1", "ACAP_proc [del_extra_rcv_packet1 exit execs]")
				FSM_PROFILE_SECTION_IN ("ACAP_proc [del_extra_rcv_packet1 exit execs]", state11_exit_exec)
				{
				ACAP_extra_pkptr=op_pk_get(op_intrpt_strm());
				
				}
				FSM_PROFILE_SECTION_OUT (state11_exit_exec)


			/** state (del_extra_rcv_packet1) transition processing **/
			FSM_TRANSIT_FORCE (0, state0_enter_exec, ;, "default", "", "del_extra_rcv_packet1", "Idle", "tr_42", "ACAP_proc [del_extra_rcv_packet1 -> Idle : default / ]")
				/*---------------------------------------------------------*/



			/** state (send_pkt) enter executives **/
			FSM_STATE_ENTER_FORCED (12, "send_pkt", state12_enter_exec, "ACAP_proc [send_pkt enter execs]")
				FSM_PROFILE_SECTION_IN ("ACAP_proc [send_pkt enter execs]", state12_enter_exec)
				{
				creat_Sent_pkt_Intr();
				op_pk_send (ACAP_copyPkt,StreamToLink );
				}
				FSM_PROFILE_SECTION_OUT (state12_enter_exec)

			/** state (send_pkt) exit executives **/
			FSM_STATE_EXIT_FORCED (12, "send_pkt", "ACAP_proc [send_pkt exit execs]")


			/** state (send_pkt) transition processing **/
			FSM_TRANSIT_FORCE (4, state4_enter_exec, ;, "default", "", "send_pkt", "wait_ACK", "tr_56", "ACAP_proc [send_pkt -> wait_ACK : default / ]")
				/*---------------------------------------------------------*/



			/** state (init) enter executives **/
			FSM_STATE_ENTER_FORCED (13, "init", state13_enter_exec, "ACAP_proc [init enter execs]")
				FSM_PROFILE_SECTION_IN ("ACAP_proc [init enter execs]", state13_enter_exec)
				{
				if (req_sended==0 && en_ACAP==1&& !ACAP_QEmpty)
					Req_intr_handle=op_intrpt_schedule_self( op_sim_time ()+.0000030,Req_intr_Code);	
				else
				    CheckR_intr_handle=op_intrpt_schedule_self( op_sim_time ()+.0000065,CheckR_intr_Code);
						
				}
				FSM_PROFILE_SECTION_OUT (state13_enter_exec)

			/** state (init) exit executives **/
			FSM_STATE_EXIT_FORCED (13, "init", "ACAP_proc [init exit execs]")


			/** state (init) transition processing **/
			FSM_TRANSIT_FORCE (0, state0_enter_exec, ;, "default", "", "init", "Idle", "tr_34", "ACAP_proc [init -> Idle : default / ]")
				/*---------------------------------------------------------*/



			/** state (del_extra_rcv_packet2) enter executives **/
			FSM_STATE_ENTER_FORCED (14, "del_extra_rcv_packet2", state14_enter_exec, "ACAP_proc [del_extra_rcv_packet2 enter execs]")
				FSM_PROFILE_SECTION_IN ("ACAP_proc [del_extra_rcv_packet2 enter execs]", state14_enter_exec)
				{
				ACAP_extra_pkptr=op_pk_get(op_intrpt_strm());
				}
				FSM_PROFILE_SECTION_OUT (state14_enter_exec)

			/** state (del_extra_rcv_packet2) exit executives **/
			FSM_STATE_EXIT_FORCED (14, "del_extra_rcv_packet2", "ACAP_proc [del_extra_rcv_packet2 exit execs]")


			/** state (del_extra_rcv_packet2) transition processing **/
			FSM_TRANSIT_FORCE (7, state7_enter_exec, ;, "default", "", "del_extra_rcv_packet2", "wait_RIFS", "tr_38_1", "ACAP_proc [del_extra_rcv_packet2 -> wait_RIFS : default / ]")
				/*---------------------------------------------------------*/



			}


		FSM_EXIT (13,"ACAP_proc")
		}
	}




void
_op_ACAP_proc_diag (OP_SIM_CONTEXT_ARG_OPT)
	{
	/* No Diagnostic Block */
	}




void
_op_ACAP_proc_terminate (OP_SIM_CONTEXT_ARG_OPT)
	{

	FIN_MT (_op_ACAP_proc_terminate ())


	/* No Termination Block */

	Vos_Poolmem_Dealloc (op_sv_ptr);

	FOUT
	}


/* Undefine shortcuts to state variables to avoid */
/* syntax error in direct access to fields of */
/* local variable prs_ptr in _op_ACAP_proc_svar function. */
#undef service_rate
#undef ACAP_BIFS_intr_handle
#undef ACAP_RIFS_intr_handle
#undef ACAP_Timeout_handle
#undef ACAP_PacketSent_intr_handle
#undef ete_gsh
#undef node_id
#undef Req_intr_handle
#undef ACAP_BackoffWindow
#undef ACAP_BackoffExponent
#undef ACAP_ack_correct
#undef ACAP_ack_pk
#undef ACAP_RetryCount
#undef ACAP_copyPkt
#undef ACAP_extra_pkptr
#undef ACAP_tx_state
#undef CTA_req_pk
#undef CheckR_intr_handle
#undef req_sended

#undef FIN_PREAMBLE_DEC
#undef FIN_PREAMBLE_CODE

#define FIN_PREAMBLE_DEC
#define FIN_PREAMBLE_CODE

VosT_Obtype
_op_ACAP_proc_init (int * init_block_ptr)
	{
	VosT_Obtype obtype = OPC_NIL;
	FIN_MT (_op_ACAP_proc_init (init_block_ptr))

	obtype = Vos_Define_Object_Prstate ("proc state vars (ACAP_proc)",
		sizeof (ACAP_proc_state));
	*init_block_ptr = 26;

	FRET (obtype)
	}

VosT_Address
_op_ACAP_proc_alloc (VosT_Obtype obtype, int init_block)
	{
#if !defined (VOSD_NO_FIN)
	int _op_block_origin = 0;
#endif
	ACAP_proc_state * ptr;
	FIN_MT (_op_ACAP_proc_alloc (obtype))

	ptr = (ACAP_proc_state *)Vos_Alloc_Object (obtype);
	if (ptr != OPC_NIL)
		{
		ptr->_op_current_block = init_block;
#if defined (OPD_ALLOW_ODB)
		ptr->_op_current_state = "ACAP_proc [init enter execs]";
#endif
		}
	FRET ((VosT_Address)ptr)
	}



void
_op_ACAP_proc_svar (void * gen_ptr, const char * var_name, void ** var_p_ptr)
	{
	ACAP_proc_state		*prs_ptr;

	FIN_MT (_op_ACAP_proc_svar (gen_ptr, var_name, var_p_ptr))

	if (var_name == OPC_NIL)
		{
		*var_p_ptr = (void *)OPC_NIL;
		FOUT
		}
	prs_ptr = (ACAP_proc_state *)gen_ptr;

	if (strcmp ("service_rate" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->service_rate);
		FOUT
		}
	if (strcmp ("ACAP_BIFS_intr_handle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->ACAP_BIFS_intr_handle);
		FOUT
		}
	if (strcmp ("ACAP_RIFS_intr_handle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->ACAP_RIFS_intr_handle);
		FOUT
		}
	if (strcmp ("ACAP_Timeout_handle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->ACAP_Timeout_handle);
		FOUT
		}
	if (strcmp ("ACAP_PacketSent_intr_handle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->ACAP_PacketSent_intr_handle);
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
	if (strcmp ("Req_intr_handle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->Req_intr_handle);
		FOUT
		}
	if (strcmp ("ACAP_BackoffWindow" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->ACAP_BackoffWindow);
		FOUT
		}
	if (strcmp ("ACAP_BackoffExponent" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->ACAP_BackoffExponent);
		FOUT
		}
	if (strcmp ("ACAP_ack_correct" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->ACAP_ack_correct);
		FOUT
		}
	if (strcmp ("ACAP_ack_pk" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->ACAP_ack_pk);
		FOUT
		}
	if (strcmp ("ACAP_RetryCount" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->ACAP_RetryCount);
		FOUT
		}
	if (strcmp ("ACAP_copyPkt" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->ACAP_copyPkt);
		FOUT
		}
	if (strcmp ("ACAP_extra_pkptr" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->ACAP_extra_pkptr);
		FOUT
		}
	if (strcmp ("ACAP_tx_state" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->ACAP_tx_state);
		FOUT
		}
	if (strcmp ("CTA_req_pk" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->CTA_req_pk);
		FOUT
		}
	if (strcmp ("CheckR_intr_handle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->CheckR_intr_handle);
		FOUT
		}
	if (strcmp ("req_sended" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->req_sended);
		FOUT
		}
	*var_p_ptr = (void *)OPC_NIL;

	FOUT
	}

