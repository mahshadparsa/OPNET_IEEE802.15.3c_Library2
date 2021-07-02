/* Process model C form file: MAC_PROC_OrdinaryNode.pr.c */
/* Portions of this file copyright 1986-2008 by OPNET Technologies, Inc. */



/* This variable carries the header into the object file */
const char MAC_PROC_OrdinaryNode_pr_c [] = "MIL_3_Tfile_Hdr_ 145A 30A op_runsim 7 594645ED 594645ED 1 Parsa-Rasuli Parsa 0 0 none none 0 0 none 0 0 0 0 0 0 0 0 1e80 8                                                                                                                                                                                                                                                                                                                                                                                                     ";
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
#define StrFromLinkArrival   (op_intrpt_type() == OPC_INTRPT_STRM && op_intrpt_strm() == StreamFromLink)

#define beacon_intr_Code 0
#define beacon_intr       (op_intrpt_type() == OPC_INTRPT_SELF && op_intrpt_code() == beacon_intr_Code)

#define RCAP_intr_Code 1
#define RCAP_intr       (op_intrpt_type() == OPC_INTRPT_SELF && op_intrpt_code() == RCAP_intr_Code)

#define ACAP_intr_Code 2
#define ACAP_intr       (op_intrpt_type() == OPC_INTRPT_SELF && op_intrpt_code() == ACAP_intr_Code)

#define MCTA_intr_Code 3
#define MCTA_intr       (op_intrpt_type() == OPC_INTRPT_SELF && op_intrpt_code() == MCTA_intr_Code)

#define CTA_intr_Code 4
#define CTA_intr       (op_intrpt_type() == OPC_INTRPT_SELF && op_intrpt_code() == CTA_intr_Code)

#define Star_of_specialCTA_intr_Code 5
#define Start_of_specialCTA_intr       (op_intrpt_type() == OPC_INTRPT_SELF && op_intrpt_code() == Star_of_specialCTA_intr_Code)

#define End_of_specialCTA_intr_Code 6
#define End_of_specialCTA_intr       (op_intrpt_type() == OPC_INTRPT_SELF && op_intrpt_code() == End_of_specialCTA_intr_Code)

#define ch_busy 0
#define channel_busy (op_stat_local_read(ch_busy)==1)


int en_beacon,en_RCAP,en_ACAP,en_MCTA,en_CTA;

double beacon_start_time;
double RCAP_start_time;
double ACAP_start_time;
double MCTA_start_time;
//double CTA_start_time;

double beacon_time=0.000535;
double RCAP_time=0.025000;
double ACAP_time=0.005000;
double MCTA_time=0.005000;
double CTA_time=0.030000;
double SD_time=0.065535;

double SIFS_time=0.0000025;


double superframe_duration;
double CAP_end_time;
double CTA_Duration;
//double CTA_location;
int req_sended;

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
	Prohandle	              		beacon_proc                                     ;
	Prohandle	              		RCAP_process                                    ;
	Prohandle	              		MCTA_proc                                       ;
	Prohandle	              		CTA_proc                                        ;
	Evhandle	               		RCAP_intr_handle                                ;
	Evhandle	               		MCTA_intr_handle                                ;
	Evhandle	               		CTA_intr_handle                                 ;
	Evhandle	               		beacon_intr_handle                              ;
	Packet *	               		beacon_pkptr                                    ;
	double	                 		tx_state                                        ;
	Evhandle	               		ACAP_intr_handle                                ;
	Prohandle	              		ACAP_proc                                       ;
	Objid	                  		node_id                                         ;
	Evhandle	               		Star_of_specialCTA_intr_handle                  ;
	Evhandle	               		End_of_specialCTA_intr_handle                   ;
	double	                 		CTA_location                                    ;
	int	                    		CTA_assigned                                    ;
	double	                 		CTA_start_time                                  ;
	Packet *	               		extra_pkptr                                     ;
	} MAC_PROC_OrdinaryNode_state;

#define beacon_proc             		op_sv_ptr->beacon_proc
#define RCAP_process            		op_sv_ptr->RCAP_process
#define MCTA_proc               		op_sv_ptr->MCTA_proc
#define CTA_proc                		op_sv_ptr->CTA_proc
#define RCAP_intr_handle        		op_sv_ptr->RCAP_intr_handle
#define MCTA_intr_handle        		op_sv_ptr->MCTA_intr_handle
#define CTA_intr_handle         		op_sv_ptr->CTA_intr_handle
#define beacon_intr_handle      		op_sv_ptr->beacon_intr_handle
#define beacon_pkptr            		op_sv_ptr->beacon_pkptr
#define tx_state                		op_sv_ptr->tx_state
#define ACAP_intr_handle        		op_sv_ptr->ACAP_intr_handle
#define ACAP_proc               		op_sv_ptr->ACAP_proc
#define node_id                 		op_sv_ptr->node_id
#define Star_of_specialCTA_intr_handle		op_sv_ptr->Star_of_specialCTA_intr_handle
#define End_of_specialCTA_intr_handle		op_sv_ptr->End_of_specialCTA_intr_handle
#define CTA_location            		op_sv_ptr->CTA_location
#define CTA_assigned            		op_sv_ptr->CTA_assigned
#define CTA_start_time          		op_sv_ptr->CTA_start_time
#define extra_pkptr             		op_sv_ptr->extra_pkptr

/* These macro definitions will define a local variable called	*/
/* "op_sv_ptr" in each function containing a FIN statement.	*/
/* This variable points to the state variable data structure,	*/
/* and can be used from a C debugger to display their values.	*/
#undef FIN_PREAMBLE_DEC
#undef FIN_PREAMBLE_CODE
#define FIN_PREAMBLE_DEC	MAC_PROC_OrdinaryNode_state *op_sv_ptr;
#define FIN_PREAMBLE_CODE	\
		op_sv_ptr = ((MAC_PROC_OrdinaryNode_state *)(OP_SIM_CONTEXT_PTR->_op_mod_state_ptr));


/* Function Block */

#if !defined (VOSD_NO_FIN)
enum { _op_block_origin = __LINE__ + 2};
#endif

static void creat_beacon_Intr()
	{
	//double beacon_time=0.000535;
	FIN (creat_beacon_Intr ());
	beacon_intr_handle=op_intrpt_schedule_self(op_sim_time()+ beacon_time,beacon_intr_Code);
	//printf("beacon_intr=%2g\n",op_sim_time()+ beacon_time);
	FOUT;
	}
static void creat_RCAP_Intr()
	{
	//double RCAP_time=0.025000; 
	FIN (creat_RCAP_Intr ());
	RCAP_intr_handle=op_intrpt_schedule_self(op_sim_time()+ RCAP_time,RCAP_intr_Code);
	//printf("CAP_intr=%2g\n",op_sim_time()+ CAP_time);
	FOUT;
	}
static void creat_ACAP_Intr()
	{
	//double ACAP_time=0.005000; 
	FIN (creat_ACAP_Intr ());
	RCAP_intr_handle=op_intrpt_schedule_self(op_sim_time()+ ACAP_time,ACAP_intr_Code);
	//printf("CAP_intr=%2g\n",op_sim_time()+ CAP_time);
	FOUT;
	}

static void creat_MCTA_Intr()
	{
	//double MCTA_time=0.005000;
	FIN (creat_MCTA_Intr ());
	MCTA_intr_handle=op_intrpt_schedule_self(op_sim_time()+ MCTA_time,MCTA_intr_Code);
	//printf("MCTA_intr=%2g\n",op_sim_time()+ MCTA_time);
	FOUT;
	}

static void creat_CTA_Intr()
	{
	//double CTA_time=0.030000;
	FIN (creat_CTA_Intr ());
	CTA_intr_handle=op_intrpt_schedule_self(op_sim_time()+ CTA_time,CTA_intr_Code);
	//printf("CTA_intr=%2g\n",op_sim_time()+ CTA_time);
	FOUT;
	}
static void creat_Star_of_specialCTA_Intr()
	{
	//
	FIN (creat_Star_of_specialCTA_Intr ());
	Star_of_specialCTA_intr_handle=op_intrpt_schedule_self(op_sim_time()+ CTA_location,Star_of_specialCTA_intr_Code);
	//;
	FOUT;
	}
static void creat_End_of_specialCTA_Intr()
	{
	//
	FIN (creat_End_of_specialCTA_Intr ());
	End_of_specialCTA_intr_handle=op_intrpt_schedule_self(op_sim_time()+ CTA_location+CTA_Duration,End_of_specialCTA_intr_Code);
	//;
	FOUT;
	}

static void get_beacon_pkt()
	{
		int frame_type,pksize,length0,DestID,i,pkID;
		Packet *enc_IE_body,*enc_CTA_block,*enc_CTA_blocks;
		FIN (get_beacon_pkt());

		beacon_pkptr=op_pk_get(op_intrpt_strm());
		op_pk_nfd_get (beacon_pkptr, "frame_type", &frame_type); //acording to the standard, type value of beacon is zero
		pkID=op_pk_id (beacon_pkptr);
		//printf("\nbeacon node:frame_type=%d,pkID=%d",frame_type,pkID);
		if (frame_type==0)
		{
		op_pk_nfd_get (beacon_pkptr, "superframe_duration",&superframe_duration);
		op_pk_nfd_get (beacon_pkptr, "CAP_end_time",&CAP_end_time);
		op_pk_nfd_get (beacon_pkptr, "IE_body", &enc_IE_body);
		pksize=op_pk_total_size_get (beacon_pkptr);
		//printf("pksize=%d\n",pksize);
		
		if (pksize>760)
			{
			op_pk_nfd_get (enc_IE_body, "length0", &length0);
			op_pk_nfd_get (enc_IE_body, "IE_payload0", &enc_CTA_blocks);
			for(i=1;i<=length0/7;i++)
				{
				op_pk_fd_get (enc_CTA_blocks,i-1, &enc_CTA_block);//CTA_block0
				op_pk_nfd_get (enc_CTA_block,"DestID", &DestID);
				//printf("DestID=%d\n",DestID);
				if (node_id==DestID)
					{
						op_pk_nfd_get (enc_CTA_block,"CTA duration", &CTA_Duration);
						op_pk_nfd_get (enc_CTA_block,"CTA location", &CTA_location);
						CTA_assigned=1;
					}
				}
			}
		
		
		//op_pk_nfd_get_int32 (beacon_pkptr, "PNC_address",1);
		//printf("CTA_location=%f,CAP_end_time=%f,CTA_Duration=%f\n",CTA_location,CAP_end_time,CTA_Duration);

		}
		
		
		FOUT;
		
	}
static void Q_CAPFrame()
	{ 
	FIN (Q_CAPFrame ());
	op_subq_pk_insert (0, op_pk_get(op_intrpt_strm()), OPC_QPOS_TAIL);
	FOUT;
	}
static void Q_CTAFrame()
	{ 
	FIN (Q_CTAFrame ());
	op_subq_pk_insert (1, op_pk_get(op_intrpt_strm()), OPC_QPOS_TAIL);
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
	void MAC_PROC_OrdinaryNode (OP_SIM_CONTEXT_ARG_OPT);
	VosT_Obtype _op_MAC_PROC_OrdinaryNode_init (int * init_block_ptr);
	void _op_MAC_PROC_OrdinaryNode_diag (OP_SIM_CONTEXT_ARG_OPT);
	void _op_MAC_PROC_OrdinaryNode_terminate (OP_SIM_CONTEXT_ARG_OPT);
	VosT_Address _op_MAC_PROC_OrdinaryNode_alloc (VosT_Obtype, int);
	void _op_MAC_PROC_OrdinaryNode_svar (void *, const char *, void **);


#if defined (__cplusplus)
} /* end of 'extern "C"' */
#endif




/* Process model interrupt handling procedure */


void
MAC_PROC_OrdinaryNode (OP_SIM_CONTEXT_ARG_OPT)
	{
#if !defined (VOSD_NO_FIN)
	int _op_block_origin = 0;
#endif
	FIN_MT (MAC_PROC_OrdinaryNode ());

		{


		FSM_ENTER ("MAC_PROC_OrdinaryNode")

		FSM_BLOCK_SWITCH
			{
			/*---------------------------------------------------------*/
			/** state (init) enter executives **/
			FSM_STATE_ENTER_FORCED_NOLABEL (0, "init", "MAC_PROC_OrdinaryNode [init enter execs]")
				FSM_PROFILE_SECTION_IN ("MAC_PROC_OrdinaryNode [init enter execs]", state0_enter_exec)
				{
				/* Create child process(es) */
				
				beacon_proc = op_pro_create ("beacon_proc", OPC_NIL);
				en_beacon=0;
				
				RCAP_process = op_pro_create ("RCAP_process", OPC_NIL);
				en_RCAP=0;
				
				ACAP_proc = op_pro_create ("ACAP_proc", OPC_NIL);
				en_ACAP=0;
				
				MCTA_proc = op_pro_create ("MCTA_proc", OPC_NIL);
				en_MCTA=0;
				
				CTA_proc = op_pro_create ("CTA_proc", OPC_NIL);
				en_CTA=0;
				
				
				tx_state=0;
				
				node_id=op_id_self();
				CTA_assigned=0;
				
				}
				FSM_PROFILE_SECTION_OUT (state0_enter_exec)

			/** state (init) exit executives **/
			FSM_STATE_EXIT_FORCED (0, "init", "MAC_PROC_OrdinaryNode [init exit execs]")


			/** state (init) transition processing **/
			FSM_TRANSIT_ONLY ((PowerUp), 1, state1_enter_exec, ;, init, "PowerUp", "", "init", "beacon_proc", "tr_35", "MAC_PROC_OrdinaryNode [init -> beacon_proc : PowerUp / ]")
				/*---------------------------------------------------------*/



			/** state (beacon_proc) enter executives **/
			FSM_STATE_ENTER_FORCED (1, "beacon_proc", state1_enter_exec, "MAC_PROC_OrdinaryNode [beacon_proc enter execs]")

			/** state (beacon_proc) exit executives **/
			FSM_STATE_EXIT_FORCED (1, "beacon_proc", "MAC_PROC_OrdinaryNode [beacon_proc exit execs]")
				FSM_PROFILE_SECTION_IN ("MAC_PROC_OrdinaryNode [beacon_proc exit execs]", state1_exit_exec)
				{
				//MY_STRUCT* someData;
				en_beacon=1;
				//op_intrpt_port_register (OPC_PORT_TYPE_STRM, 0, beacon_proc);
				//op_intrpt_port_register (OPC_PORT_TYPE_STRM, 1, beacon_proc);
				//op_pro_invoke (beacon_proc, OPC_NIL);//OPC_NIL
				creat_beacon_Intr();
				
				}
				FSM_PROFILE_SECTION_OUT (state1_exit_exec)


			/** state (beacon_proc) transition processing **/
			FSM_TRANSIT_FORCE (5, state5_enter_exec, ;, "default", "", "beacon_proc", "beacon_duration", "tr_23", "MAC_PROC_OrdinaryNode [beacon_proc -> beacon_duration : default / ]")
				/*---------------------------------------------------------*/



			/** state (RCAP_proc) enter executives **/
			FSM_STATE_ENTER_FORCED (2, "RCAP_proc", state2_enter_exec, "MAC_PROC_OrdinaryNode [RCAP_proc enter execs]")
				FSM_PROFILE_SECTION_IN ("MAC_PROC_OrdinaryNode [RCAP_proc enter execs]", state2_enter_exec)
				{
				en_beacon=0;
				//op_pk_destroy(beacon_pkptr);
				en_RCAP=1;
				creat_RCAP_Intr();
				
				RCAP_start_time=op_sim_time ();
				op_pro_invoke (RCAP_process, OPC_NIL);
				op_intrpt_port_register (OPC_PORT_TYPE_STRM, 0, RCAP_process);
				op_intrpt_port_register (OPC_PORT_TYPE_STRM, 1, RCAP_process);
				//op_intrpt_port_register (OPC_PORT_TYPE_STRM, 2, RCAP_process);
				}
				FSM_PROFILE_SECTION_OUT (state2_enter_exec)

			/** state (RCAP_proc) exit executives **/
			FSM_STATE_EXIT_FORCED (2, "RCAP_proc", "MAC_PROC_OrdinaryNode [RCAP_proc exit execs]")


			/** state (RCAP_proc) transition processing **/
			FSM_TRANSIT_FORCE (6, state6_enter_exec, ;, "default", "", "RCAP_proc", "RCAP_duration", "tr_25", "MAC_PROC_OrdinaryNode [RCAP_proc -> RCAP_duration : default / ]")
				/*---------------------------------------------------------*/



			/** state (MCTA_proc) enter executives **/
			FSM_STATE_ENTER_FORCED (3, "MCTA_proc", state3_enter_exec, "MAC_PROC_OrdinaryNode [MCTA_proc enter execs]")
				FSM_PROFILE_SECTION_IN ("MAC_PROC_OrdinaryNode [MCTA_proc enter execs]", state3_enter_exec)
				{
				en_ACAP=0;
				op_intrpt_port_deregister (OPC_PORT_TYPE_STRM, 0);
				op_intrpt_port_deregister (OPC_PORT_TYPE_STRM, 2);
				req_sended=0;
				en_MCTA=1;
				op_pro_invoke (MCTA_proc, OPC_NIL);
				creat_MCTA_Intr();
				}
				FSM_PROFILE_SECTION_OUT (state3_enter_exec)

			/** state (MCTA_proc) exit executives **/
			FSM_STATE_EXIT_FORCED (3, "MCTA_proc", "MAC_PROC_OrdinaryNode [MCTA_proc exit execs]")


			/** state (MCTA_proc) transition processing **/
			FSM_TRANSIT_FORCE (7, state7_enter_exec, ;, "default", "", "MCTA_proc", "MCTA_duration", "tr_27", "MAC_PROC_OrdinaryNode [MCTA_proc -> MCTA_duration : default / ]")
				/*---------------------------------------------------------*/



			/** state (CTA_proc) enter executives **/
			FSM_STATE_ENTER_FORCED (4, "CTA_proc", state4_enter_exec, "MAC_PROC_OrdinaryNode [CTA_proc enter execs]")

			/** state (CTA_proc) exit executives **/
			FSM_STATE_EXIT_FORCED (4, "CTA_proc", "MAC_PROC_OrdinaryNode [CTA_proc exit execs]")
				FSM_PROFILE_SECTION_IN ("MAC_PROC_OrdinaryNode [CTA_proc exit execs]", state4_exit_exec)
				{
				en_MCTA=0;
				creat_CTA_Intr();
				creat_Star_of_specialCTA_Intr();
				creat_End_of_specialCTA_Intr();
				
				}
				FSM_PROFILE_SECTION_OUT (state4_exit_exec)


			/** state (CTA_proc) transition processing **/
			FSM_TRANSIT_FORCE (12, state12_enter_exec, ;, "default", "", "CTA_proc", "Wait2startOfCTA", "tr_53", "MAC_PROC_OrdinaryNode [CTA_proc -> Wait2startOfCTA : default / ]")
				/*---------------------------------------------------------*/



			/** state (beacon_duration) enter executives **/
			FSM_STATE_ENTER_UNFORCED (5, "beacon_duration", state5_enter_exec, "MAC_PROC_OrdinaryNode [beacon_duration enter execs]")

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (11,"MAC_PROC_OrdinaryNode")


			/** state (beacon_duration) exit executives **/
			FSM_STATE_EXIT_UNFORCED (5, "beacon_duration", "MAC_PROC_OrdinaryNode [beacon_duration exit execs]")


			/** state (beacon_duration) transition processing **/
			FSM_PROFILE_SECTION_IN ("MAC_PROC_OrdinaryNode [beacon_duration trans conditions]", state5_trans_conds)
			FSM_INIT_COND (beacon_intr)
			FSM_TEST_COND (StrFromLinkArrival)
			FSM_TEST_COND (CTAFrameArrival)
			FSM_TEST_COND (CAPFrameArrival)
			FSM_DFLT_COND
			FSM_TEST_LOGIC ("beacon_duration")
			FSM_PROFILE_SECTION_OUT (state5_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 2, state2_enter_exec, ;, "beacon_intr", "", "beacon_duration", "RCAP_proc", "tr_24", "MAC_PROC_OrdinaryNode [beacon_duration -> RCAP_proc : beacon_intr / ]")
				FSM_CASE_TRANSIT (1, 9, state9_enter_exec, ;, "StrFromLinkArrival", "", "beacon_duration", "st_10", "tr_36", "MAC_PROC_OrdinaryNode [beacon_duration -> st_10 : StrFromLinkArrival / ]")
				FSM_CASE_TRANSIT (2, 5, state5_enter_exec, Q_CTAFrame();, "CTAFrameArrival", "Q_CTAFrame()", "beacon_duration", "beacon_duration", "tr_48", "MAC_PROC_OrdinaryNode [beacon_duration -> beacon_duration : CTAFrameArrival / Q_CTAFrame()]")
				FSM_CASE_TRANSIT (3, 5, state5_enter_exec, Q_CAPFrame();, "CAPFrameArrival", "Q_CAPFrame()", "beacon_duration", "beacon_duration", "tr_49", "MAC_PROC_OrdinaryNode [beacon_duration -> beacon_duration : CAPFrameArrival / Q_CAPFrame()]")
				FSM_CASE_TRANSIT (4, 5, state5_enter_exec, ;, "default", "", "beacon_duration", "beacon_duration", "tr_19", "MAC_PROC_OrdinaryNode [beacon_duration -> beacon_duration : default / ]")
				}
				/*---------------------------------------------------------*/



			/** state (RCAP_duration) enter executives **/
			FSM_STATE_ENTER_UNFORCED (6, "RCAP_duration", state6_enter_exec, "MAC_PROC_OrdinaryNode [RCAP_duration enter execs]")

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (13,"MAC_PROC_OrdinaryNode")


			/** state (RCAP_duration) exit executives **/
			FSM_STATE_EXIT_UNFORCED (6, "RCAP_duration", "MAC_PROC_OrdinaryNode [RCAP_duration exit execs]")


			/** state (RCAP_duration) transition processing **/
			FSM_PROFILE_SECTION_IN ("MAC_PROC_OrdinaryNode [RCAP_duration trans conditions]", state6_trans_conds)
			FSM_INIT_COND (RCAP_intr)
			FSM_TEST_COND (CTAFrameArrival)
			FSM_DFLT_COND
			FSM_TEST_LOGIC ("RCAP_duration")
			FSM_PROFILE_SECTION_OUT (state6_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 10, state10_enter_exec, ;, "RCAP_intr", "", "RCAP_duration", "ACAP-proc", "tr_43", "MAC_PROC_OrdinaryNode [RCAP_duration -> ACAP-proc : RCAP_intr / ]")
				FSM_CASE_TRANSIT (1, 6, state6_enter_exec, Q_CTAFrame();, "CTAFrameArrival", "Q_CTAFrame()", "RCAP_duration", "RCAP_duration", "tr_47", "MAC_PROC_OrdinaryNode [RCAP_duration -> RCAP_duration : CTAFrameArrival / Q_CTAFrame()]")
				FSM_CASE_TRANSIT (2, 6, state6_enter_exec, ;, "default", "", "RCAP_duration", "RCAP_duration", "tr_20", "MAC_PROC_OrdinaryNode [RCAP_duration -> RCAP_duration : default / ]")
				}
				/*---------------------------------------------------------*/



			/** state (MCTA_duration) enter executives **/
			FSM_STATE_ENTER_UNFORCED (7, "MCTA_duration", state7_enter_exec, "MAC_PROC_OrdinaryNode [MCTA_duration enter execs]")

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (15,"MAC_PROC_OrdinaryNode")


			/** state (MCTA_duration) exit executives **/
			FSM_STATE_EXIT_UNFORCED (7, "MCTA_duration", "MAC_PROC_OrdinaryNode [MCTA_duration exit execs]")


			/** state (MCTA_duration) transition processing **/
			FSM_PROFILE_SECTION_IN ("MAC_PROC_OrdinaryNode [MCTA_duration trans conditions]", state7_trans_conds)
			FSM_INIT_COND (MCTA_intr)
			FSM_TEST_COND (CAPFrameArrival)
			FSM_DFLT_COND
			FSM_TEST_LOGIC ("MCTA_duration")
			FSM_PROFILE_SECTION_OUT (state7_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 4, state4_enter_exec, ;, "MCTA_intr", "", "MCTA_duration", "CTA_proc", "tr_28", "MAC_PROC_OrdinaryNode [MCTA_duration -> CTA_proc : MCTA_intr / ]")
				FSM_CASE_TRANSIT (1, 7, state7_enter_exec, Q_CAPFrame();, "CAPFrameArrival", "Q_CAPFrame()", "MCTA_duration", "MCTA_duration", "tr_58", "MAC_PROC_OrdinaryNode [MCTA_duration -> MCTA_duration : CAPFrameArrival / Q_CAPFrame()]")
				FSM_CASE_TRANSIT (2, 7, state7_enter_exec, ;, "default", "", "MCTA_duration", "MCTA_duration", "tr_21", "MAC_PROC_OrdinaryNode [MCTA_duration -> MCTA_duration : default / ]")
				}
				/*---------------------------------------------------------*/



			/** state (Wait2EndOfCTAP) enter executives **/
			FSM_STATE_ENTER_UNFORCED (8, "Wait2EndOfCTAP", state8_enter_exec, "MAC_PROC_OrdinaryNode [Wait2EndOfCTAP enter execs]")

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (17,"MAC_PROC_OrdinaryNode")


			/** state (Wait2EndOfCTAP) exit executives **/
			FSM_STATE_EXIT_UNFORCED (8, "Wait2EndOfCTAP", "MAC_PROC_OrdinaryNode [Wait2EndOfCTAP exit execs]")


			/** state (Wait2EndOfCTAP) transition processing **/
			FSM_PROFILE_SECTION_IN ("MAC_PROC_OrdinaryNode [Wait2EndOfCTAP trans conditions]", state8_trans_conds)
			FSM_INIT_COND (CAPFrameArrival)
			FSM_TEST_COND (CTAFrameArrival)
			FSM_TEST_COND (CTA_intr)
			FSM_TEST_COND (StrFromLinkArrival)
			FSM_DFLT_COND
			FSM_TEST_LOGIC ("Wait2EndOfCTAP")
			FSM_PROFILE_SECTION_OUT (state8_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 8, state8_enter_exec, Q_CAPFrame();, "CAPFrameArrival", "Q_CAPFrame()", "Wait2EndOfCTAP", "Wait2EndOfCTAP", "tr_61", "MAC_PROC_OrdinaryNode [Wait2EndOfCTAP -> Wait2EndOfCTAP : CAPFrameArrival / Q_CAPFrame()]")
				FSM_CASE_TRANSIT (1, 8, state8_enter_exec, Q_CTAFrame();, "CTAFrameArrival", "Q_CTAFrame()", "Wait2EndOfCTAP", "Wait2EndOfCTAP", "tr_63", "MAC_PROC_OrdinaryNode [Wait2EndOfCTAP -> Wait2EndOfCTAP : CTAFrameArrival / Q_CTAFrame()]")
				FSM_CASE_TRANSIT (2, 1, state1_enter_exec, ;, "CTA_intr", "", "Wait2EndOfCTAP", "beacon_proc", "tr_66", "MAC_PROC_OrdinaryNode [Wait2EndOfCTAP -> beacon_proc : CTA_intr / ]")
				FSM_CASE_TRANSIT (3, 17, state17_enter_exec, ;, "StrFromLinkArrival", "", "Wait2EndOfCTAP", "del_extrapkt2", "tr_78", "MAC_PROC_OrdinaryNode [Wait2EndOfCTAP -> del_extrapkt2 : StrFromLinkArrival / ]")
				FSM_CASE_TRANSIT (4, 8, state8_enter_exec, ;, "default", "", "Wait2EndOfCTAP", "Wait2EndOfCTAP", "tr_22", "MAC_PROC_OrdinaryNode [Wait2EndOfCTAP -> Wait2EndOfCTAP : default / ]")
				}
				/*---------------------------------------------------------*/



			/** state (st_10) enter executives **/
			FSM_STATE_ENTER_FORCED (9, "st_10", state9_enter_exec, "MAC_PROC_OrdinaryNode [st_10 enter execs]")
				FSM_PROFILE_SECTION_IN ("MAC_PROC_OrdinaryNode [st_10 enter execs]", state9_enter_exec)
				{
				get_beacon_pkt();
				}
				FSM_PROFILE_SECTION_OUT (state9_enter_exec)

			/** state (st_10) exit executives **/
			FSM_STATE_EXIT_FORCED (9, "st_10", "MAC_PROC_OrdinaryNode [st_10 exit execs]")


			/** state (st_10) transition processing **/
			FSM_TRANSIT_FORCE (5, state5_enter_exec, ;, "default", "", "st_10", "beacon_duration", "tr_37", "MAC_PROC_OrdinaryNode [st_10 -> beacon_duration : default / ]")
				/*---------------------------------------------------------*/



			/** state (ACAP-proc) enter executives **/
			FSM_STATE_ENTER_FORCED (10, "ACAP-proc", state10_enter_exec, "MAC_PROC_OrdinaryNode [ACAP-proc enter execs]")
				FSM_PROFILE_SECTION_IN ("MAC_PROC_OrdinaryNode [ACAP-proc enter execs]", state10_enter_exec)
				{
				en_RCAP=0;
				op_intrpt_port_deregister (OPC_PORT_TYPE_STRM, 1);
					
				en_ACAP=1;
				req_sended=0;
				creat_ACAP_Intr();
				ACAP_start_time=op_sim_time ();
				op_pro_invoke (ACAP_proc, OPC_NIL);
				op_intrpt_port_register (OPC_PORT_TYPE_STRM, 0, ACAP_proc);
				op_intrpt_port_register (OPC_PORT_TYPE_STRM, 2, ACAP_proc);
				//op_intrpt_type_register (OPC_INTRPT_SELF, ACAP_proc);
				
				}
				FSM_PROFILE_SECTION_OUT (state10_enter_exec)

			/** state (ACAP-proc) exit executives **/
			FSM_STATE_EXIT_FORCED (10, "ACAP-proc", "MAC_PROC_OrdinaryNode [ACAP-proc exit execs]")


			/** state (ACAP-proc) transition processing **/
			FSM_TRANSIT_FORCE (11, state11_enter_exec, ;, "default", "", "ACAP-proc", "ACAP_duration", "tr_44", "MAC_PROC_OrdinaryNode [ACAP-proc -> ACAP_duration : default / ]")
				/*---------------------------------------------------------*/



			/** state (ACAP_duration) enter executives **/
			FSM_STATE_ENTER_UNFORCED (11, "ACAP_duration", state11_enter_exec, "MAC_PROC_OrdinaryNode [ACAP_duration enter execs]")

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (23,"MAC_PROC_OrdinaryNode")


			/** state (ACAP_duration) exit executives **/
			FSM_STATE_EXIT_UNFORCED (11, "ACAP_duration", "MAC_PROC_OrdinaryNode [ACAP_duration exit execs]")


			/** state (ACAP_duration) transition processing **/
			FSM_PROFILE_SECTION_IN ("MAC_PROC_OrdinaryNode [ACAP_duration trans conditions]", state11_trans_conds)
			FSM_INIT_COND (ACAP_intr)
			FSM_TEST_COND (CAPFrameArrival)
			FSM_DFLT_COND
			FSM_TEST_LOGIC ("ACAP_duration")
			FSM_PROFILE_SECTION_OUT (state11_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 3, state3_enter_exec, ;, "ACAP_intr", "", "ACAP_duration", "MCTA_proc", "tr_46", "MAC_PROC_OrdinaryNode [ACAP_duration -> MCTA_proc : ACAP_intr / ]")
				FSM_CASE_TRANSIT (1, 11, state11_enter_exec, Q_CAPFrame();, "CAPFrameArrival", "Q_CAPFrame()", "ACAP_duration", "ACAP_duration", "tr_52", "MAC_PROC_OrdinaryNode [ACAP_duration -> ACAP_duration : CAPFrameArrival / Q_CAPFrame()]")
				FSM_CASE_TRANSIT (2, 11, state11_enter_exec, ;, "default", "", "ACAP_duration", "ACAP_duration", "tr_45", "MAC_PROC_OrdinaryNode [ACAP_duration -> ACAP_duration : default / ]")
				}
				/*---------------------------------------------------------*/



			/** state (Wait2startOfCTA) enter executives **/
			FSM_STATE_ENTER_UNFORCED (12, "Wait2startOfCTA", state12_enter_exec, "MAC_PROC_OrdinaryNode [Wait2startOfCTA enter execs]")

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (25,"MAC_PROC_OrdinaryNode")


			/** state (Wait2startOfCTA) exit executives **/
			FSM_STATE_EXIT_UNFORCED (12, "Wait2startOfCTA", "MAC_PROC_OrdinaryNode [Wait2startOfCTA exit execs]")


			/** state (Wait2startOfCTA) transition processing **/
			FSM_PROFILE_SECTION_IN ("MAC_PROC_OrdinaryNode [Wait2startOfCTA trans conditions]", state12_trans_conds)
			FSM_INIT_COND (CAPFrameArrival)
			FSM_TEST_COND (CTAFrameArrival)
			FSM_TEST_COND (Start_of_specialCTA_intr)
			FSM_TEST_COND (StrFromLinkArrival)
			FSM_DFLT_COND
			FSM_TEST_LOGIC ("Wait2startOfCTA")
			FSM_PROFILE_SECTION_OUT (state12_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 12, state12_enter_exec, Q_CAPFrame();, "CAPFrameArrival", "Q_CAPFrame()", "Wait2startOfCTA", "Wait2startOfCTA", "tr_59", "MAC_PROC_OrdinaryNode [Wait2startOfCTA -> Wait2startOfCTA : CAPFrameArrival / Q_CAPFrame()]")
				FSM_CASE_TRANSIT (1, 12, state12_enter_exec, Q_CTAFrame();, "CTAFrameArrival", "Q_CTAFrame()", "Wait2startOfCTA", "Wait2startOfCTA", "tr_62", "MAC_PROC_OrdinaryNode [Wait2startOfCTA -> Wait2startOfCTA : CTAFrameArrival / Q_CTAFrame()]")
				FSM_CASE_TRANSIT (2, 14, state14_enter_exec, ;, "Start_of_specialCTA_intr", "", "Wait2startOfCTA", "register_intr", "tr_69", "MAC_PROC_OrdinaryNode [Wait2startOfCTA -> register_intr : Start_of_specialCTA_intr / ]")
				FSM_CASE_TRANSIT (3, 16, state16_enter_exec, ;, "StrFromLinkArrival", "", "Wait2startOfCTA", "del_extrapkt1", "tr_76", "MAC_PROC_OrdinaryNode [Wait2startOfCTA -> del_extrapkt1 : StrFromLinkArrival / ]")
				FSM_CASE_TRANSIT (4, 12, state12_enter_exec, ;, "default", "", "Wait2startOfCTA", "Wait2startOfCTA", "tr_54", "MAC_PROC_OrdinaryNode [Wait2startOfCTA -> Wait2startOfCTA : default / ]")
				}
				/*---------------------------------------------------------*/



			/** state (CTA_duration) enter executives **/
			FSM_STATE_ENTER_UNFORCED (13, "CTA_duration", state13_enter_exec, "MAC_PROC_OrdinaryNode [CTA_duration enter execs]")

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (27,"MAC_PROC_OrdinaryNode")


			/** state (CTA_duration) exit executives **/
			FSM_STATE_EXIT_UNFORCED (13, "CTA_duration", "MAC_PROC_OrdinaryNode [CTA_duration exit execs]")


			/** state (CTA_duration) transition processing **/
			FSM_PROFILE_SECTION_IN ("MAC_PROC_OrdinaryNode [CTA_duration trans conditions]", state13_trans_conds)
			FSM_INIT_COND (CAPFrameArrival)
			FSM_TEST_COND (End_of_specialCTA_intr)
			FSM_TEST_COND (CTA_intr)
			FSM_DFLT_COND
			FSM_TEST_LOGIC ("CTA_duration")
			FSM_PROFILE_SECTION_OUT (state13_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 13, state13_enter_exec, Q_CAPFrame();, "CAPFrameArrival", "Q_CAPFrame()", "CTA_duration", "CTA_duration", "tr_60", "MAC_PROC_OrdinaryNode [CTA_duration -> CTA_duration : CAPFrameArrival / Q_CAPFrame()]")
				FSM_CASE_TRANSIT (1, 15, state15_enter_exec, ;, "End_of_specialCTA_intr", "", "CTA_duration", "deregister_intr", "tr_71", "MAC_PROC_OrdinaryNode [CTA_duration -> deregister_intr : End_of_specialCTA_intr / ]")
				FSM_CASE_TRANSIT (2, 18, state18_enter_exec, ;, "CTA_intr", "", "CTA_duration", "deregister_intr1", "tr_80", "MAC_PROC_OrdinaryNode [CTA_duration -> deregister_intr1 : CTA_intr / ]")
				FSM_CASE_TRANSIT (3, 13, state13_enter_exec, ;, "default", "", "CTA_duration", "CTA_duration", "tr_57", "MAC_PROC_OrdinaryNode [CTA_duration -> CTA_duration : default / ]")
				}
				/*---------------------------------------------------------*/



			/** state (register_intr) enter executives **/
			FSM_STATE_ENTER_FORCED (14, "register_intr", state14_enter_exec, "MAC_PROC_OrdinaryNode [register_intr enter execs]")
				FSM_PROFILE_SECTION_IN ("MAC_PROC_OrdinaryNode [register_intr enter execs]", state14_enter_exec)
				{
				en_CTA=1;
				op_pro_invoke (CTA_proc, OPC_NIL);
				op_intrpt_port_register (OPC_PORT_TYPE_STRM, 0, CTA_proc);
				op_intrpt_port_register (OPC_PORT_TYPE_STRM, 2, CTA_proc);
				op_intrpt_port_register (OPC_INTRPT_SELF, 5 , CTA_proc);//start_of_specialCTA_intr
				//op_intrpt_port_register (OPC_INTRPT_SELF, 6 , CTA_proc);//End_of_specialCTA_intr
				CTA_start_time=op_sim_time ();
				
				}
				FSM_PROFILE_SECTION_OUT (state14_enter_exec)

			/** state (register_intr) exit executives **/
			FSM_STATE_EXIT_FORCED (14, "register_intr", "MAC_PROC_OrdinaryNode [register_intr exit execs]")


			/** state (register_intr) transition processing **/
			FSM_TRANSIT_FORCE (13, state13_enter_exec, ;, "default", "", "register_intr", "CTA_duration", "tr_70", "MAC_PROC_OrdinaryNode [register_intr -> CTA_duration : default / ]")
				/*---------------------------------------------------------*/



			/** state (deregister_intr) enter executives **/
			FSM_STATE_ENTER_FORCED (15, "deregister_intr", state15_enter_exec, "MAC_PROC_OrdinaryNode [deregister_intr enter execs]")
				FSM_PROFILE_SECTION_IN ("MAC_PROC_OrdinaryNode [deregister_intr enter execs]", state15_enter_exec)
				{
				en_CTA=0;
				op_intrpt_port_deregister (OPC_PORT_TYPE_STRM, 0);
				op_intrpt_port_deregister (OPC_PORT_TYPE_STRM, 1);
				op_intrpt_port_deregister (OPC_PORT_TYPE_STRM, 2);
				op_intrpt_port_deregister (OPC_INTRPT_SELF, 5);
				//op_intrpt_port_deregister (OPC_INTRPT_SELF, 6);
				}
				FSM_PROFILE_SECTION_OUT (state15_enter_exec)

			/** state (deregister_intr) exit executives **/
			FSM_STATE_EXIT_FORCED (15, "deregister_intr", "MAC_PROC_OrdinaryNode [deregister_intr exit execs]")


			/** state (deregister_intr) transition processing **/
			FSM_TRANSIT_FORCE (8, state8_enter_exec, ;, "default", "", "deregister_intr", "Wait2EndOfCTAP", "tr_72", "MAC_PROC_OrdinaryNode [deregister_intr -> Wait2EndOfCTAP : default / ]")
				/*---------------------------------------------------------*/



			/** state (del_extrapkt1) enter executives **/
			FSM_STATE_ENTER_FORCED (16, "del_extrapkt1", state16_enter_exec, "MAC_PROC_OrdinaryNode [del_extrapkt1 enter execs]")
				FSM_PROFILE_SECTION_IN ("MAC_PROC_OrdinaryNode [del_extrapkt1 enter execs]", state16_enter_exec)
				{
				extra_pkptr=op_pk_get(op_intrpt_strm());
				//printf("extra packet witd id packet =%d deleted\n",op_pk_id (extra_pkptr));
				op_pk_destroy(extra_pkptr);
				}
				FSM_PROFILE_SECTION_OUT (state16_enter_exec)

			/** state (del_extrapkt1) exit executives **/
			FSM_STATE_EXIT_FORCED (16, "del_extrapkt1", "MAC_PROC_OrdinaryNode [del_extrapkt1 exit execs]")


			/** state (del_extrapkt1) transition processing **/
			FSM_TRANSIT_FORCE (12, state12_enter_exec, ;, "default", "", "del_extrapkt1", "Wait2startOfCTA", "tr_77", "MAC_PROC_OrdinaryNode [del_extrapkt1 -> Wait2startOfCTA : default / ]")
				/*---------------------------------------------------------*/



			/** state (del_extrapkt2) enter executives **/
			FSM_STATE_ENTER_FORCED (17, "del_extrapkt2", state17_enter_exec, "MAC_PROC_OrdinaryNode [del_extrapkt2 enter execs]")
				FSM_PROFILE_SECTION_IN ("MAC_PROC_OrdinaryNode [del_extrapkt2 enter execs]", state17_enter_exec)
				{
				extra_pkptr=op_pk_get(op_intrpt_strm());
				//printf("extra packet witd id packet =%d deleted\n",op_pk_id (extra_pkptr));
				op_pk_destroy(extra_pkptr);
				}
				FSM_PROFILE_SECTION_OUT (state17_enter_exec)

			/** state (del_extrapkt2) exit executives **/
			FSM_STATE_EXIT_FORCED (17, "del_extrapkt2", "MAC_PROC_OrdinaryNode [del_extrapkt2 exit execs]")


			/** state (del_extrapkt2) transition processing **/
			FSM_TRANSIT_FORCE (8, state8_enter_exec, ;, "default", "", "del_extrapkt2", "Wait2EndOfCTAP", "tr_79", "MAC_PROC_OrdinaryNode [del_extrapkt2 -> Wait2EndOfCTAP : default / ]")
				/*---------------------------------------------------------*/



			/** state (deregister_intr1) enter executives **/
			FSM_STATE_ENTER_FORCED (18, "deregister_intr1", state18_enter_exec, "MAC_PROC_OrdinaryNode [deregister_intr1 enter execs]")
				FSM_PROFILE_SECTION_IN ("MAC_PROC_OrdinaryNode [deregister_intr1 enter execs]", state18_enter_exec)
				{
				en_CTA=0;
				op_intrpt_port_deregister (OPC_PORT_TYPE_STRM, 0);
				op_intrpt_port_deregister (OPC_PORT_TYPE_STRM, 1);
				op_intrpt_port_deregister (OPC_PORT_TYPE_STRM, 2);
				op_intrpt_port_deregister (OPC_INTRPT_SELF, 5);
				//op_intrpt_port_deregister (OPC_INTRPT_SELF, 6);
				}
				FSM_PROFILE_SECTION_OUT (state18_enter_exec)

			/** state (deregister_intr1) exit executives **/
			FSM_STATE_EXIT_FORCED (18, "deregister_intr1", "MAC_PROC_OrdinaryNode [deregister_intr1 exit execs]")


			/** state (deregister_intr1) transition processing **/
			FSM_TRANSIT_FORCE (1, state1_enter_exec, ;, "default", "", "deregister_intr1", "beacon_proc", "tr_81", "MAC_PROC_OrdinaryNode [deregister_intr1 -> beacon_proc : default / ]")
				/*---------------------------------------------------------*/



			}


		FSM_EXIT (0,"MAC_PROC_OrdinaryNode")
		}
	}




void
_op_MAC_PROC_OrdinaryNode_diag (OP_SIM_CONTEXT_ARG_OPT)
	{
	/* No Diagnostic Block */
	}




void
_op_MAC_PROC_OrdinaryNode_terminate (OP_SIM_CONTEXT_ARG_OPT)
	{

	FIN_MT (_op_MAC_PROC_OrdinaryNode_terminate ())


	/* No Termination Block */

	Vos_Poolmem_Dealloc (op_sv_ptr);

	FOUT
	}


/* Undefine shortcuts to state variables to avoid */
/* syntax error in direct access to fields of */
/* local variable prs_ptr in _op_MAC_PROC_OrdinaryNode_svar function. */
#undef beacon_proc
#undef RCAP_process
#undef MCTA_proc
#undef CTA_proc
#undef RCAP_intr_handle
#undef MCTA_intr_handle
#undef CTA_intr_handle
#undef beacon_intr_handle
#undef beacon_pkptr
#undef tx_state
#undef ACAP_intr_handle
#undef ACAP_proc
#undef node_id
#undef Star_of_specialCTA_intr_handle
#undef End_of_specialCTA_intr_handle
#undef CTA_location
#undef CTA_assigned
#undef CTA_start_time
#undef extra_pkptr

#undef FIN_PREAMBLE_DEC
#undef FIN_PREAMBLE_CODE

#define FIN_PREAMBLE_DEC
#define FIN_PREAMBLE_CODE

VosT_Obtype
_op_MAC_PROC_OrdinaryNode_init (int * init_block_ptr)
	{
	VosT_Obtype obtype = OPC_NIL;
	FIN_MT (_op_MAC_PROC_OrdinaryNode_init (init_block_ptr))

	obtype = Vos_Define_Object_Prstate ("proc state vars (MAC_PROC_OrdinaryNode)",
		sizeof (MAC_PROC_OrdinaryNode_state));
	*init_block_ptr = 0;

	FRET (obtype)
	}

VosT_Address
_op_MAC_PROC_OrdinaryNode_alloc (VosT_Obtype obtype, int init_block)
	{
#if !defined (VOSD_NO_FIN)
	int _op_block_origin = 0;
#endif
	MAC_PROC_OrdinaryNode_state * ptr;
	FIN_MT (_op_MAC_PROC_OrdinaryNode_alloc (obtype))

	ptr = (MAC_PROC_OrdinaryNode_state *)Vos_Alloc_Object (obtype);
	if (ptr != OPC_NIL)
		{
		ptr->_op_current_block = init_block;
#if defined (OPD_ALLOW_ODB)
		ptr->_op_current_state = "MAC_PROC_OrdinaryNode [init enter execs]";
#endif
		}
	FRET ((VosT_Address)ptr)
	}



void
_op_MAC_PROC_OrdinaryNode_svar (void * gen_ptr, const char * var_name, void ** var_p_ptr)
	{
	MAC_PROC_OrdinaryNode_state		*prs_ptr;

	FIN_MT (_op_MAC_PROC_OrdinaryNode_svar (gen_ptr, var_name, var_p_ptr))

	if (var_name == OPC_NIL)
		{
		*var_p_ptr = (void *)OPC_NIL;
		FOUT
		}
	prs_ptr = (MAC_PROC_OrdinaryNode_state *)gen_ptr;

	if (strcmp ("beacon_proc" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->beacon_proc);
		FOUT
		}
	if (strcmp ("RCAP_process" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->RCAP_process);
		FOUT
		}
	if (strcmp ("MCTA_proc" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->MCTA_proc);
		FOUT
		}
	if (strcmp ("CTA_proc" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->CTA_proc);
		FOUT
		}
	if (strcmp ("RCAP_intr_handle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->RCAP_intr_handle);
		FOUT
		}
	if (strcmp ("MCTA_intr_handle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->MCTA_intr_handle);
		FOUT
		}
	if (strcmp ("CTA_intr_handle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->CTA_intr_handle);
		FOUT
		}
	if (strcmp ("beacon_intr_handle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->beacon_intr_handle);
		FOUT
		}
	if (strcmp ("beacon_pkptr" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->beacon_pkptr);
		FOUT
		}
	if (strcmp ("tx_state" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->tx_state);
		FOUT
		}
	if (strcmp ("ACAP_intr_handle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->ACAP_intr_handle);
		FOUT
		}
	if (strcmp ("ACAP_proc" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->ACAP_proc);
		FOUT
		}
	if (strcmp ("node_id" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->node_id);
		FOUT
		}
	if (strcmp ("Star_of_specialCTA_intr_handle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->Star_of_specialCTA_intr_handle);
		FOUT
		}
	if (strcmp ("End_of_specialCTA_intr_handle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->End_of_specialCTA_intr_handle);
		FOUT
		}
	if (strcmp ("CTA_location" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->CTA_location);
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
	if (strcmp ("extra_pkptr" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->extra_pkptr);
		FOUT
		}
	*var_p_ptr = (void *)OPC_NIL;

	FOUT
	}

