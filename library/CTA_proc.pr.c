/* Process model C form file: CTA_proc.pr.c */
/* Portions of this file copyright 1986-2008 by OPNET Technologies, Inc. */



/* This variable carries the header into the object file */
const char CTA_proc_pr_c [] = "MIL_3_Tfile_Hdr_ 145A 30A op_runsim 7 59100F62 59100F62 1 DESKTOP-FFL2R3R user 0 0 none none 0 0 none 0 0 0 0 0 0 0 0 1e80 8                                                                                                                                                                                                                                                                                                                                                                                                   ";
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

#define ACAP_BIFS_intr_Code 1
#define ACAP_BIFS_intr       (op_intrpt_type() == OPC_INTRPT_SELF && op_intrpt_code() == ACAP_BIFS_intr_Code)

#define ACAP_RIFS_intr_Code 2
#define ACAP_RIFS_intr       (op_intrpt_type() == OPC_INTRPT_SELF && op_intrpt_code() == ACAP_RIFS_intr_Code)

#define ACAP_PacketSent_intr_code 3
#define ACAP_PacketSent_intr (op_intrpt_type() == OPC_INTRPT_SELF && op_intrpt_code() == ACAP_PacketSent_intr_code)

#define Start_CTA_intr_Code 4
#define Start_CTA_intr       (op_intrpt_type() == OPC_INTRPT_SELF && op_intrpt_code() == Start_CTA_intr_Code)

#define Start_of_specialCTA_intr_Code 5
#define Start_of_specialCTA_intr       (op_intrpt_type() == OPC_INTRPT_SELF && op_intrpt_code() == Start_of_specialCTA_intr_Code)

#define End_of_specialCTA_intr_Code 6
#define End_of_specialCTA_intr       (op_intrpt_type() == OPC_INTRPT_SELF && op_intrpt_code() == End_of_specialCTA_intr_Code)

#define SIFS_intr_Code 7
#define SIFS_intr       (op_intrpt_type() == OPC_INTRPT_SELF && op_intrpt_code() == SIFS_intr_Code)



#define ACAP_QEmpty 	(op_subq_stat(1, OPC_QSTAT_PKSIZE) == 0.0)
double SIFS_time;


int en_CTA;
//double CTA_start_time;
double CTA_location;
double CTA_Duration;
double service_rate=900000000;


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
	Evhandle	               		Start_CTA_intr_handle                           ;
	Packet *	               		copyPkt                                         ;
	int	                    		res_timeOK                                      ;
	int	                    		ack_correct                                     ;
	Objid	                  		node_id                                         ;
	int	                    		CTA_assigned                                    ;
	double	                 		CTA_start_time                                  ;
	Evhandle	               		End_of_specialCTA_intr_handle                   ;
	Evhandle	               		SIFS_intr_handle                                ;
	} CTA_proc_state;

#define Start_CTA_intr_handle   		op_sv_ptr->Start_CTA_intr_handle
#define copyPkt                 		op_sv_ptr->copyPkt
#define res_timeOK              		op_sv_ptr->res_timeOK
#define ack_correct             		op_sv_ptr->ack_correct
#define node_id                 		op_sv_ptr->node_id
#define CTA_assigned            		op_sv_ptr->CTA_assigned
#define CTA_start_time          		op_sv_ptr->CTA_start_time
#define End_of_specialCTA_intr_handle		op_sv_ptr->End_of_specialCTA_intr_handle
#define SIFS_intr_handle        		op_sv_ptr->SIFS_intr_handle

/* These macro definitions will define a local variable called	*/
/* "op_sv_ptr" in each function containing a FIN statement.	*/
/* This variable points to the state variable data structure,	*/
/* and can be used from a C debugger to display their values.	*/
#undef FIN_PREAMBLE_DEC
#undef FIN_PREAMBLE_CODE
#define FIN_PREAMBLE_DEC	CTA_proc_state *op_sv_ptr;
#define FIN_PREAMBLE_CODE	\
		op_sv_ptr = ((CTA_proc_state *)(OP_SIM_CONTEXT_PTR->_op_mod_state_ptr));


/* Function Block */

#if !defined (VOSD_NO_FIN)
enum { _op_block_origin = __LINE__ + 2};
#endif

static void creat_End_of_specialCTA_Intr()
	{
	//
	FIN (creat_End_of_specialCTA_Intr ());
	End_of_specialCTA_intr_handle=op_intrpt_schedule_self(op_sim_time()+CTA_Duration,End_of_specialCTA_intr_Code);
	//printf("CTA_Duration=%2g,op_sim_time()=%2g",CTA_Duration,op_sim_time());
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
	op_subq_pk_insert (1, op_pk_get(op_intrpt_strm()), OPC_QPOS_TAIL);
	FOUT;
	}
/**************************************************************************************/
/**************************************************************************************/

static void check_ack()
	{
	int dest_ID;
	int frame_type;
	Packet* ack_pk;
	FIN (check_ack());
	ack_pk=op_pk_get(op_intrpt_strm());
	op_pk_nfd_get_int32 (ack_pk, "frame_type",&frame_type ); //acording to the standard, type value of beacon is zero
	op_pk_nfd_get_int32 (ack_pk, "dest_ID",&dest_ID);
	if (frame_type==1 && dest_ID==node_id)
		{
		ack_correct=1;
		creat_SIFS_Intr();
		}
	else
		{
		ack_correct=0;
		printf("\n CTA_Proc: dest_ID=%d,node_id=%d,frame_type=%d,pkt_id=%d\n",dest_ID,node_id,frame_type,op_pk_id (ack_pk));
		}
		op_pk_destroy(ack_pk);
	FOUT;
	
	}
/**************************************************************************************/
/**************************************************************************************/
static void check_res_time()
	{
	double ACK_svc_time;
	double pk_svc_time;
	FIN (check_res_time());
	ACK_svc_time=0;
	pk_svc_time=720/service_rate;
	if (CTA_Duration-op_sim_time()+ CTA_start_time< pk_svc_time +2*SIFS_time+ACK_svc_time)
		{
		res_timeOK=0;
		}
	else
		{
		res_timeOK=1;
		}
	//printf("CTA_start_time=%2g,CTA_Duration=%2g,pk_svc_time=%2g,SIFS_time=%2g,res_timeOK=%d\n",CTA_start_time,CTA_Duration,pk_svc_time,SIFS_time,res_timeOK);
		
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
	void CTA_proc (OP_SIM_CONTEXT_ARG_OPT);
	VosT_Obtype _op_CTA_proc_init (int * init_block_ptr);
	void _op_CTA_proc_diag (OP_SIM_CONTEXT_ARG_OPT);
	void _op_CTA_proc_terminate (OP_SIM_CONTEXT_ARG_OPT);
	VosT_Address _op_CTA_proc_alloc (VosT_Obtype, int);
	void _op_CTA_proc_svar (void *, const char *, void **);


#if defined (__cplusplus)
} /* end of 'extern "C"' */
#endif




/* Process model interrupt handling procedure */


void
CTA_proc (OP_SIM_CONTEXT_ARG_OPT)
	{
#if !defined (VOSD_NO_FIN)
	int _op_block_origin = 0;
#endif
	FIN_MT (CTA_proc ());

		{


		FSM_ENTER ("CTA_proc")

		FSM_BLOCK_SWITCH
			{
			/*---------------------------------------------------------*/
			/** state (init) enter executives **/
			FSM_STATE_ENTER_FORCED_NOLABEL (0, "init", "CTA_proc [init enter execs]")
				FSM_PROFILE_SECTION_IN ("CTA_proc [init enter execs]", state0_enter_exec)
				{
				//Start_CTA_intr_handle=op_intrpt_schedule_self( op_sim_time ()+CTA_location,Start_CTA_intr_Code);
				}
				FSM_PROFILE_SECTION_OUT (state0_enter_exec)

			/** state (init) exit executives **/
			FSM_STATE_EXIT_FORCED (0, "init", "CTA_proc [init exit execs]")


			/** state (init) transition processing **/
			FSM_TRANSIT_FORCE (1, state1_enter_exec, ;, "default", "", "init", "Idle", "tr_3", "CTA_proc [init -> Idle : default / ]")
				/*---------------------------------------------------------*/



			/** state (Idle) enter executives **/
			FSM_STATE_ENTER_UNFORCED (1, "Idle", state1_enter_exec, "CTA_proc [Idle enter execs]")

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (3,"CTA_proc")


			/** state (Idle) exit executives **/
			FSM_STATE_EXIT_UNFORCED (1, "Idle", "CTA_proc [Idle exit execs]")


			/** state (Idle) transition processing **/
			FSM_PROFILE_SECTION_IN ("CTA_proc [Idle trans conditions]", state1_trans_conds)
			FSM_INIT_COND (CTAFrameArrival && !Start_of_specialCTA_intr)
			FSM_TEST_COND (Start_of_specialCTA_intr  && ACAP_QEmpty && !CTAFrameArrival)
			FSM_TEST_COND (Start_of_specialCTA_intr && !ACAP_QEmpty )
			FSM_TEST_COND (Start_of_specialCTA_intr &&  CTAFrameArrival)
			FSM_DFLT_COND
			FSM_TEST_LOGIC ("Idle")
			FSM_PROFILE_SECTION_OUT (state1_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 1, state1_enter_exec, Qframe();;, "CTAFrameArrival && !Start_of_specialCTA_intr", "Qframe();", "Idle", "Idle", "tr_37", "CTA_proc [Idle -> Idle : CTAFrameArrival && !Start_of_specialCTA_intr / Qframe();]")
				FSM_CASE_TRANSIT (1, 7, state7_enter_exec, ;, "Start_of_specialCTA_intr  && ACAP_QEmpty && !CTAFrameArrival", "", "Idle", "set_param1", "tr_66", "CTA_proc [Idle -> set_param1 : Start_of_specialCTA_intr  && ACAP_QEmpty && !CTAFrameArrival / ]")
				FSM_CASE_TRANSIT (2, 8, state8_enter_exec, ;, "Start_of_specialCTA_intr && !ACAP_QEmpty ", "", "Idle", "set_param2", "tr_68", "CTA_proc [Idle -> set_param2 : Start_of_specialCTA_intr && !ACAP_QEmpty  / ]")
				FSM_CASE_TRANSIT (3, 9, state9_enter_exec, ;, "Start_of_specialCTA_intr &&  CTAFrameArrival", "", "Idle", "set_param3", "tr_72", "CTA_proc [Idle -> set_param3 : Start_of_specialCTA_intr &&  CTAFrameArrival / ]")
				FSM_CASE_TRANSIT (4, 1, state1_enter_exec, ;, "default", "", "Idle", "Idle", "tr_43", "CTA_proc [Idle -> Idle : default / ]")
				}
				/*---------------------------------------------------------*/



			/** state (srv_Qpkt) enter executives **/
			FSM_STATE_ENTER_FORCED (2, "srv_Qpkt", state2_enter_exec, "CTA_proc [srv_Qpkt enter execs]")
				FSM_PROFILE_SECTION_IN ("CTA_proc [srv_Qpkt enter execs]", state2_enter_exec)
				{
				check_res_time();
				if (res_timeOK==1)
					{
					copyPkt = op_pk_copy (op_subq_pk_remove (1, OPC_QPOS_HEAD));
					node_id=op_id_self();		
					op_pk_nfd_set_int32 (copyPkt, "src_ID",node_id);
					op_pk_nfd_set_int32 (copyPkt, "dest_ID",0);
					op_pk_nfd_set_int32 (copyPkt, "frame_type", 4);//acording to the standard, type value of data is 4
				
					}
				}
				FSM_PROFILE_SECTION_OUT (state2_enter_exec)

			/** state (srv_Qpkt) exit executives **/
			FSM_STATE_EXIT_FORCED (2, "srv_Qpkt", "CTA_proc [srv_Qpkt exit execs]")


			/** state (srv_Qpkt) transition processing **/
			FSM_PROFILE_SECTION_IN ("CTA_proc [srv_Qpkt trans conditions]", state2_trans_conds)
			FSM_INIT_COND (res_timeOK)
			FSM_TEST_COND (!res_timeOK)
			FSM_TEST_LOGIC ("srv_Qpkt")
			FSM_PROFILE_SECTION_OUT (state2_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 5, state5_enter_exec, op_pk_send (copyPkt,StreamToLink );;, "res_timeOK", "op_pk_send (copyPkt,StreamToLink );", "srv_Qpkt", "wait_ack", "tr_51", "CTA_proc [srv_Qpkt -> wait_ack : res_timeOK / op_pk_send (copyPkt,StreamToLink );]")
				FSM_CASE_TRANSIT (1, 1, state1_enter_exec, ;, "!res_timeOK", "", "srv_Qpkt", "Idle", "tr_57", "CTA_proc [srv_Qpkt -> Idle : !res_timeOK / ]")
				}
				/*---------------------------------------------------------*/



			/** state (WaitforData) enter executives **/
			FSM_STATE_ENTER_UNFORCED (3, "WaitforData", state3_enter_exec, "CTA_proc [WaitforData enter execs]")

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (7,"CTA_proc")


			/** state (WaitforData) exit executives **/
			FSM_STATE_EXIT_UNFORCED (3, "WaitforData", "CTA_proc [WaitforData exit execs]")


			/** state (WaitforData) transition processing **/
			FSM_PROFILE_SECTION_IN ("CTA_proc [WaitforData trans conditions]", state3_trans_conds)
			FSM_INIT_COND (CTAFrameArrival && !End_of_specialCTA_intr)
			FSM_TEST_COND (End_of_specialCTA_intr)
			FSM_TEST_COND (End_of_specialCTA_intr && CTAFrameArrival)
			FSM_DFLT_COND
			FSM_TEST_LOGIC ("WaitforData")
			FSM_PROFILE_SECTION_OUT (state3_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 4, state4_enter_exec, ;, "CTAFrameArrival && !End_of_specialCTA_intr", "", "WaitforData", "srv_pkt", "tr_46", "CTA_proc [WaitforData -> srv_pkt : CTAFrameArrival && !End_of_specialCTA_intr / ]")
				FSM_CASE_TRANSIT (1, 1, state1_enter_exec, ;, "End_of_specialCTA_intr", "", "WaitforData", "Idle", "tr_64", "CTA_proc [WaitforData -> Idle : End_of_specialCTA_intr / ]")
				FSM_CASE_TRANSIT (2, 1, state1_enter_exec, Qframe();;, "End_of_specialCTA_intr && CTAFrameArrival", "Qframe();", "WaitforData", "Idle", "tr_65", "CTA_proc [WaitforData -> Idle : End_of_specialCTA_intr && CTAFrameArrival / Qframe();]")
				FSM_CASE_TRANSIT (3, 3, state3_enter_exec, ;, "default", "", "WaitforData", "WaitforData", "tr_19", "CTA_proc [WaitforData -> WaitforData : default / ]")
				}
				/*---------------------------------------------------------*/



			/** state (srv_pkt) enter executives **/
			FSM_STATE_ENTER_FORCED (4, "srv_pkt", state4_enter_exec, "CTA_proc [srv_pkt enter execs]")
				FSM_PROFILE_SECTION_IN ("CTA_proc [srv_pkt enter execs]", state4_enter_exec)
				{
				check_res_time();
				if (res_timeOK==1)
					{
					copyPkt = op_pk_copy (op_pk_get(op_intrpt_strm()));
					node_id=op_id_self();		
					op_pk_nfd_set_int32 (copyPkt, "src_ID",node_id);
					op_pk_nfd_set_int32 (copyPkt, "dest_ID",0);
					op_pk_nfd_set_int32 (copyPkt, "frame_type", 4);//acording to the standard, type value of data is 4
				
					}
				}
				FSM_PROFILE_SECTION_OUT (state4_enter_exec)

			/** state (srv_pkt) exit executives **/
			FSM_STATE_EXIT_FORCED (4, "srv_pkt", "CTA_proc [srv_pkt exit execs]")


			/** state (srv_pkt) transition processing **/
			FSM_PROFILE_SECTION_IN ("CTA_proc [srv_pkt trans conditions]", state4_trans_conds)
			FSM_INIT_COND (res_timeOK)
			FSM_TEST_COND (!res_timeOK)
			FSM_TEST_LOGIC ("srv_pkt")
			FSM_PROFILE_SECTION_OUT (state4_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 5, state5_enter_exec, op_pk_send (copyPkt,StreamToLink );;, "res_timeOK", "op_pk_send (copyPkt,StreamToLink );", "srv_pkt", "wait_ack", "tr_50", "CTA_proc [srv_pkt -> wait_ack : res_timeOK / op_pk_send (copyPkt,StreamToLink );]")
				FSM_CASE_TRANSIT (1, 1, state1_enter_exec, ;, "!res_timeOK", "", "srv_pkt", "Idle", "tr_56", "CTA_proc [srv_pkt -> Idle : !res_timeOK / ]")
				}
				/*---------------------------------------------------------*/



			/** state (wait_ack) enter executives **/
			FSM_STATE_ENTER_UNFORCED (5, "wait_ack", state5_enter_exec, "CTA_proc [wait_ack enter execs]")

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (11,"CTA_proc")


			/** state (wait_ack) exit executives **/
			FSM_STATE_EXIT_UNFORCED (5, "wait_ack", "CTA_proc [wait_ack exit execs]")


			/** state (wait_ack) transition processing **/
			FSM_PROFILE_SECTION_IN ("CTA_proc [wait_ack trans conditions]", state5_trans_conds)
			FSM_INIT_COND (CTAFrameArrival)
			FSM_TEST_COND (StrFromLinkArrival)
			FSM_DFLT_COND
			FSM_TEST_LOGIC ("wait_ack")
			FSM_PROFILE_SECTION_OUT (state5_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 5, state5_enter_exec, Qframe();;, "CTAFrameArrival", "Qframe();", "wait_ack", "wait_ack", "tr_39", "CTA_proc [wait_ack -> wait_ack : CTAFrameArrival / Qframe();]")
				FSM_CASE_TRANSIT (1, 6, state6_enter_exec, ;, "StrFromLinkArrival", "", "wait_ack", "check_ack", "tr_60", "CTA_proc [wait_ack -> check_ack : StrFromLinkArrival / ]")
				FSM_CASE_TRANSIT (2, 5, state5_enter_exec, ;, "default", "", "wait_ack", "wait_ack", "tr_24", "CTA_proc [wait_ack -> wait_ack : default / ]")
				}
				/*---------------------------------------------------------*/



			/** state (check_ack) enter executives **/
			FSM_STATE_ENTER_FORCED (6, "check_ack", state6_enter_exec, "CTA_proc [check_ack enter execs]")
				FSM_PROFILE_SECTION_IN ("CTA_proc [check_ack enter execs]", state6_enter_exec)
				{
				check_ack();
				}
				FSM_PROFILE_SECTION_OUT (state6_enter_exec)

			/** state (check_ack) exit executives **/
			FSM_STATE_EXIT_FORCED (6, "check_ack", "CTA_proc [check_ack exit execs]")


			/** state (check_ack) transition processing **/
			FSM_PROFILE_SECTION_IN ("CTA_proc [check_ack trans conditions]", state6_trans_conds)
			FSM_INIT_COND ( ack_correct==1 && ACAP_QEmpty )
			FSM_TEST_COND (ack_correct==1 && !ACAP_QEmpty )
			FSM_DFLT_COND
			FSM_TEST_LOGIC ("check_ack")
			FSM_PROFILE_SECTION_OUT (state6_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 10, state10_enter_exec, ;, " ack_correct==1 && ACAP_QEmpty ", "", "check_ack", "wait_2SIFS1", "tr_76", "CTA_proc [check_ack -> wait_2SIFS1 :  ack_correct==1 && ACAP_QEmpty  / ]")
				FSM_CASE_TRANSIT (1, 11, state11_enter_exec, ;, "ack_correct==1 && !ACAP_QEmpty ", "", "check_ack", "wait_2SIFS2", "tr_78", "CTA_proc [check_ack -> wait_2SIFS2 : ack_correct==1 && !ACAP_QEmpty  / ]")
				FSM_CASE_TRANSIT (2, 5, state5_enter_exec, ;, "default", "", "check_ack", "wait_ack", "tr_61", "CTA_proc [check_ack -> wait_ack : default / ]")
				}
				/*---------------------------------------------------------*/



			/** state (set_param1) enter executives **/
			FSM_STATE_ENTER_FORCED (7, "set_param1", state7_enter_exec, "CTA_proc [set_param1 enter execs]")
				FSM_PROFILE_SECTION_IN ("CTA_proc [set_param1 enter execs]", state7_enter_exec)
				{
				CTA_start_time=op_sim_time();
				creat_End_of_specialCTA_Intr();
				}
				FSM_PROFILE_SECTION_OUT (state7_enter_exec)

			/** state (set_param1) exit executives **/
			FSM_STATE_EXIT_FORCED (7, "set_param1", "CTA_proc [set_param1 exit execs]")


			/** state (set_param1) transition processing **/
			FSM_TRANSIT_FORCE (3, state3_enter_exec, ;, "default", "", "set_param1", "WaitforData", "tr_67", "CTA_proc [set_param1 -> WaitforData : default / ]")
				/*---------------------------------------------------------*/



			/** state (set_param2) enter executives **/
			FSM_STATE_ENTER_FORCED (8, "set_param2", state8_enter_exec, "CTA_proc [set_param2 enter execs]")

			/** state (set_param2) exit executives **/
			FSM_STATE_EXIT_FORCED (8, "set_param2", "CTA_proc [set_param2 exit execs]")
				FSM_PROFILE_SECTION_IN ("CTA_proc [set_param2 exit execs]", state8_exit_exec)
				{
				CTA_start_time=op_sim_time();
				creat_End_of_specialCTA_Intr();
				}
				FSM_PROFILE_SECTION_OUT (state8_exit_exec)


			/** state (set_param2) transition processing **/
			FSM_TRANSIT_FORCE (2, state2_enter_exec, ;, "default", "", "set_param2", "srv_Qpkt", "tr_69", "CTA_proc [set_param2 -> srv_Qpkt : default / ]")
				/*---------------------------------------------------------*/



			/** state (set_param3) enter executives **/
			FSM_STATE_ENTER_FORCED (9, "set_param3", state9_enter_exec, "CTA_proc [set_param3 enter execs]")
				FSM_PROFILE_SECTION_IN ("CTA_proc [set_param3 enter execs]", state9_enter_exec)
				{
				Qframe();
				CTA_start_time=op_sim_time();
				creat_End_of_specialCTA_Intr();
				}
				FSM_PROFILE_SECTION_OUT (state9_enter_exec)

			/** state (set_param3) exit executives **/
			FSM_STATE_EXIT_FORCED (9, "set_param3", "CTA_proc [set_param3 exit execs]")


			/** state (set_param3) transition processing **/
			FSM_TRANSIT_FORCE (2, state2_enter_exec, ;, "default", "", "set_param3", "srv_Qpkt", "tr_73", "CTA_proc [set_param3 -> srv_Qpkt : default / ]")
				/*---------------------------------------------------------*/



			/** state (wait_2SIFS1) enter executives **/
			FSM_STATE_ENTER_UNFORCED (10, "wait_2SIFS1", state10_enter_exec, "CTA_proc [wait_2SIFS1 enter execs]")

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (21,"CTA_proc")


			/** state (wait_2SIFS1) exit executives **/
			FSM_STATE_EXIT_UNFORCED (10, "wait_2SIFS1", "CTA_proc [wait_2SIFS1 exit execs]")


			/** state (wait_2SIFS1) transition processing **/
			FSM_PROFILE_SECTION_IN ("CTA_proc [wait_2SIFS1 trans conditions]", state10_trans_conds)
			FSM_INIT_COND (SIFS_intr)
			FSM_TEST_COND (CTAFrameArrival)
			FSM_DFLT_COND
			FSM_TEST_LOGIC ("wait_2SIFS1")
			FSM_PROFILE_SECTION_OUT (state10_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 3, state3_enter_exec, ;, "SIFS_intr", "", "wait_2SIFS1", "WaitforData", "tr_77", "CTA_proc [wait_2SIFS1 -> WaitforData : SIFS_intr / ]")
				FSM_CASE_TRANSIT (1, 10, state10_enter_exec, Qframe();;, "CTAFrameArrival", "Qframe();", "wait_2SIFS1", "wait_2SIFS1", "tr_82", "CTA_proc [wait_2SIFS1 -> wait_2SIFS1 : CTAFrameArrival / Qframe();]")
				FSM_CASE_TRANSIT (2, 10, state10_enter_exec, ;, "default", "", "wait_2SIFS1", "wait_2SIFS1", "tr_80", "CTA_proc [wait_2SIFS1 -> wait_2SIFS1 : default / ]")
				}
				/*---------------------------------------------------------*/



			/** state (wait_2SIFS2) enter executives **/
			FSM_STATE_ENTER_UNFORCED (11, "wait_2SIFS2", state11_enter_exec, "CTA_proc [wait_2SIFS2 enter execs]")

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (23,"CTA_proc")


			/** state (wait_2SIFS2) exit executives **/
			FSM_STATE_EXIT_UNFORCED (11, "wait_2SIFS2", "CTA_proc [wait_2SIFS2 exit execs]")


			/** state (wait_2SIFS2) transition processing **/
			FSM_PROFILE_SECTION_IN ("CTA_proc [wait_2SIFS2 trans conditions]", state11_trans_conds)
			FSM_INIT_COND (SIFS_intr)
			FSM_TEST_COND (CTAFrameArrival)
			FSM_DFLT_COND
			FSM_TEST_LOGIC ("wait_2SIFS2")
			FSM_PROFILE_SECTION_OUT (state11_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 2, state2_enter_exec, ;, "SIFS_intr", "", "wait_2SIFS2", "srv_Qpkt", "tr_79", "CTA_proc [wait_2SIFS2 -> srv_Qpkt : SIFS_intr / ]")
				FSM_CASE_TRANSIT (1, 11, state11_enter_exec, Qframe();;, "CTAFrameArrival", "Qframe();", "wait_2SIFS2", "wait_2SIFS2", "tr_83", "CTA_proc [wait_2SIFS2 -> wait_2SIFS2 : CTAFrameArrival / Qframe();]")
				FSM_CASE_TRANSIT (2, 11, state11_enter_exec, ;, "default", "", "wait_2SIFS2", "wait_2SIFS2", "tr_81", "CTA_proc [wait_2SIFS2 -> wait_2SIFS2 : default / ]")
				}
				/*---------------------------------------------------------*/



			}


		FSM_EXIT (0,"CTA_proc")
		}
	}




void
_op_CTA_proc_diag (OP_SIM_CONTEXT_ARG_OPT)
	{
	/* No Diagnostic Block */
	}




void
_op_CTA_proc_terminate (OP_SIM_CONTEXT_ARG_OPT)
	{

	FIN_MT (_op_CTA_proc_terminate ())


	/* No Termination Block */

	Vos_Poolmem_Dealloc (op_sv_ptr);

	FOUT
	}


/* Undefine shortcuts to state variables to avoid */
/* syntax error in direct access to fields of */
/* local variable prs_ptr in _op_CTA_proc_svar function. */
#undef Start_CTA_intr_handle
#undef copyPkt
#undef res_timeOK
#undef ack_correct
#undef node_id
#undef CTA_assigned
#undef CTA_start_time
#undef End_of_specialCTA_intr_handle
#undef SIFS_intr_handle

#undef FIN_PREAMBLE_DEC
#undef FIN_PREAMBLE_CODE

#define FIN_PREAMBLE_DEC
#define FIN_PREAMBLE_CODE

VosT_Obtype
_op_CTA_proc_init (int * init_block_ptr)
	{
	VosT_Obtype obtype = OPC_NIL;
	FIN_MT (_op_CTA_proc_init (init_block_ptr))

	obtype = Vos_Define_Object_Prstate ("proc state vars (CTA_proc)",
		sizeof (CTA_proc_state));
	*init_block_ptr = 0;

	FRET (obtype)
	}

VosT_Address
_op_CTA_proc_alloc (VosT_Obtype obtype, int init_block)
	{
#if !defined (VOSD_NO_FIN)
	int _op_block_origin = 0;
#endif
	CTA_proc_state * ptr;
	FIN_MT (_op_CTA_proc_alloc (obtype))

	ptr = (CTA_proc_state *)Vos_Alloc_Object (obtype);
	if (ptr != OPC_NIL)
		{
		ptr->_op_current_block = init_block;
#if defined (OPD_ALLOW_ODB)
		ptr->_op_current_state = "CTA_proc [init enter execs]";
#endif
		}
	FRET ((VosT_Address)ptr)
	}



void
_op_CTA_proc_svar (void * gen_ptr, const char * var_name, void ** var_p_ptr)
	{
	CTA_proc_state		*prs_ptr;

	FIN_MT (_op_CTA_proc_svar (gen_ptr, var_name, var_p_ptr))

	if (var_name == OPC_NIL)
		{
		*var_p_ptr = (void *)OPC_NIL;
		FOUT
		}
	prs_ptr = (CTA_proc_state *)gen_ptr;

	if (strcmp ("Start_CTA_intr_handle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->Start_CTA_intr_handle);
		FOUT
		}
	if (strcmp ("copyPkt" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->copyPkt);
		FOUT
		}
	if (strcmp ("res_timeOK" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->res_timeOK);
		FOUT
		}
	if (strcmp ("ack_correct" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->ack_correct);
		FOUT
		}
	if (strcmp ("node_id" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->node_id);
		FOUT
		}
	if (strcmp ("CTA_assigned" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->CTA_assigned);
		FOUT
		}
	if (strcmp ("CTA_start_time" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->CTA_start_time);
		FOUT
		}
	if (strcmp ("End_of_specialCTA_intr_handle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->End_of_specialCTA_intr_handle);
		FOUT
		}
	if (strcmp ("SIFS_intr_handle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->SIFS_intr_handle);
		FOUT
		}
	*var_p_ptr = (void *)OPC_NIL;

	FOUT
	}

