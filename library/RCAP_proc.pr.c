/* Process model C form file: CAP_PROC.pr.c */
/* Portions of this file copyright 1986-2008 by OPNET Technologies, Inc. */



/* This variable carries the header into the object file */
const char CAP_PROC_pr_c [] = "MIL_3_Tfile_Hdr_ 145A 30A op_runsim 7 58451E5D 58451E5D 1 Parsa-Rasuli Parsa 0 0 none none 0 0 none 0 0 0 0 0 0 0 0 1e80 8                                                                                                                                                                                                                                                                                                                                                                                                     ";
#include <string.h>



/* OPNET system definitions */
#include <opnet.h>



/* Header Block */

/* packet stream definitions */
#define StreamFromApp  1
#define StreamFromLink 0
#define StreamToLink   0


#define PowerUp         (op_intrpt_type() == OPC_INTRPT_BEGSIM)
#define FrameArrival    (op_intrpt_type() == OPC_INTRPT_STRM && op_intrpt_strm() == StreamFromApp)
#define ACK 		    (op_intrpt_type() == OPC_INTRPT_STRM && op_intrpt_strm() == StreamFromLink)

#define Timeout_Code 0
#define Timeout         (op_intrpt_type() == OPC_INTRPT_SELF && op_intrpt_code() == Timeout_Code)

#define BIFS_intr_Code 1
#define BIFS_intr       (op_intrpt_type() == OPC_INTRPT_SELF && op_intrpt_code() == BIFS_intr_Code)

#define RIFS_intr_Code 2
#define RIFS_intr       (op_intrpt_type() == OPC_INTRPT_SELF && op_intrpt_code() == RIFS_intr_Code)

#define PacketSent_intr_code 0
#define PacketSent_intr (op_intrpt_type() == OPC_INTRPT_SELF && op_intrpt_code() == PacketSent_intr_code)

#define LinkFail 	    (op_intrpt_type() == OPC_INTRPT_FAIL)
#define LinkUp		    (op_intrpt_type() == OPC_INTRPT_RECOVER)
#define QEmpty 		    (op_subq_stat(0, OPC_QSTAT_PKSIZE) == 0.0)

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
	int	                    		channel_busy                                    ;
	double	                 		service_rate                                    ;
	Evhandle	               		BIFS_intr_handle                                ;
	Evhandle	               		RIFS_intr_handle                                ;
	Evhandle	               		Timeout_handle                                  ;
	Evhandle	               		PacketSent_intr_handle                          ;
	Stathandle	             		ete_gsh                                         ;
	Objid	                  		node_id                                         ;
	Distribution *	         		address_dist                                    ;
	Packet *	               		Data_pkptr                                      ;
	} CAP_PROC_state;

#define channel_busy            		op_sv_ptr->channel_busy
#define service_rate            		op_sv_ptr->service_rate
#define BIFS_intr_handle        		op_sv_ptr->BIFS_intr_handle
#define RIFS_intr_handle        		op_sv_ptr->RIFS_intr_handle
#define Timeout_handle          		op_sv_ptr->Timeout_handle
#define PacketSent_intr_handle  		op_sv_ptr->PacketSent_intr_handle
#define ete_gsh                 		op_sv_ptr->ete_gsh
#define node_id                 		op_sv_ptr->node_id
#define address_dist            		op_sv_ptr->address_dist
#define Data_pkptr              		op_sv_ptr->Data_pkptr

/* These macro definitions will define a local variable called	*/
/* "op_sv_ptr" in each function containing a FIN statement.	*/
/* This variable points to the state variable data structure,	*/
/* and can be used from a C debugger to display their values.	*/
#undef FIN_PREAMBLE_DEC
#undef FIN_PREAMBLE_CODE
#define FIN_PREAMBLE_DEC	CAP_PROC_state *op_sv_ptr;
#define FIN_PREAMBLE_CODE	\
		op_sv_ptr = ((CAP_PROC_state *)(OP_SIM_CONTEXT_PTR->_op_mod_state_ptr));


/* Function Block */

#if !defined (VOSD_NO_FIN)
enum { _op_block_origin = __LINE__ + 2};
#endif

static void backoff (int BackoffWindow,int BackoffExponent)
	{		
	FIN (backoff (BackoffWindow,BackoffExponent));
	BackoffWindow=(2<<BackoffExponent)-1;
	FOUT;
	}
static void creat_BIFS_Intr()
	{
	double BIFS_time=0.0000065;//0.00000065; // 6.5us
	FIN (creat_BIFS_Intr ());
	BIFS_intr_handle=op_intrpt_schedule_self(op_sim_time()+ BIFS_time,BIFS_intr_Code);
	FOUT;
	}
static void creat_RIFS_Intr()
	{
	double RIFS_time=0.000009;//0.0000009; // 6.5us
	FIN (creat_BIFS_Intr ());
	RIFS_intr_handle=op_intrpt_schedule_self(op_sim_time()+ RIFS_time,RIFS_intr_Code);
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
	void CAP_PROC (OP_SIM_CONTEXT_ARG_OPT);
	VosT_Obtype _op_CAP_PROC_init (int * init_block_ptr);
	void _op_CAP_PROC_diag (OP_SIM_CONTEXT_ARG_OPT);
	void _op_CAP_PROC_terminate (OP_SIM_CONTEXT_ARG_OPT);
	VosT_Address _op_CAP_PROC_alloc (VosT_Obtype, int);
	void _op_CAP_PROC_svar (void *, const char *, void **);


#if defined (__cplusplus)
} /* end of 'extern "C"' */
#endif




/* Process model interrupt handling procedure */


void
CAP_PROC (OP_SIM_CONTEXT_ARG_OPT)
	{
#if !defined (VOSD_NO_FIN)
	int _op_block_origin = 0;
#endif
	FIN_MT (CAP_PROC ());

		{
		/* Temporary Variables */
		int BackoffExponent=3;
		int RetryCount=0;
		int BWRandom=0;
		int max_RetryCount=3;
		int BackoffWindow=7;
		
		//double ete_delay;
		//double creation_time;
		//double current_time;
		double pk_svc_time;
		
		int    insert_ok;
		int stream_index;
		//Packet* Data_pkptr;
		Packet* copyPkt;
		OpT_Packet_Size  pk_len;
		
		/* End of Temporary Variables */


		FSM_ENTER ("CAP_PROC")

		FSM_BLOCK_SWITCH
			{
			/*---------------------------------------------------------*/
			/** state (init) enter executives **/
			FSM_STATE_ENTER_FORCED_NOLABEL (0, "init", "CAP_PROC [init enter execs]")

			/** state (init) exit executives **/
			FSM_STATE_EXIT_FORCED (0, "init", "CAP_PROC [init exit execs]")
				FSM_PROFILE_SECTION_IN ("CAP_PROC [init exit execs]", state0_exit_exec)
				{
				/* Register handle for statistic collection. */
				address_dist = op_dist_load ("uniform_int", 0, 0);
				ete_gsh = op_stat_reg("ETE Delay", OPC_STAT_INDEX_NONE, OPC_STAT_GLOBAL);
				BackoffExponent=3;
				RetryCount=0;
				max_RetryCount=3;
				
				channel_busy=0;
				service_rate=1610000;
				}
				FSM_PROFILE_SECTION_OUT (state0_exit_exec)


			/** state (init) transition processing **/
			FSM_TRANSIT_ONLY ((PowerUp), 1, state1_enter_exec, ;, init, "PowerUp", "", "init", "Idle", "tr_0", "CAP_PROC [init -> Idle : PowerUp / ]")
				/*---------------------------------------------------------*/



			/** state (Idle) enter executives **/
			FSM_STATE_ENTER_UNFORCED (1, "Idle", state1_enter_exec, "CAP_PROC [Idle enter execs]")

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (3,"CAP_PROC")


			/** state (Idle) exit executives **/
			FSM_STATE_EXIT_UNFORCED (1, "Idle", "CAP_PROC [Idle exit execs]")


			/** state (Idle) transition processing **/
			FSM_PROFILE_SECTION_IN ("CAP_PROC [Idle trans conditions]", state1_trans_conds)
			FSM_INIT_COND (FrameArrival)
			FSM_DFLT_COND
			FSM_TEST_LOGIC ("Idle")
			FSM_PROFILE_SECTION_OUT (state1_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 2, state2_enter_exec, ;, "FrameArrival", "", "Idle", "svc_packet", "tr_1", "CAP_PROC [Idle -> svc_packet : FrameArrival / ]")
				FSM_CASE_TRANSIT (1, 1, state1_enter_exec, ;, "default", "", "Idle", "Idle", "tr_24", "CAP_PROC [Idle -> Idle : default / ]")
				}
				/*---------------------------------------------------------*/



			/** state (svc_packet) enter executives **/
			FSM_STATE_ENTER_FORCED (2, "svc_packet", state2_enter_exec, "CAP_PROC [svc_packet enter execs]")

			/** state (svc_packet) exit executives **/
			FSM_STATE_EXIT_FORCED (2, "svc_packet", "CAP_PROC [svc_packet exit execs]")
				FSM_PROFILE_SECTION_IN ("CAP_PROC [svc_packet exit execs]", state2_exit_exec)
				{
				//op_subq_pk_insert (0, op_pk_get(op_intrpt_strm()), OPC_QPOS_TAIL);
				/* Get stream index number.              */
				stream_index=op_intrpt_strm();
				
				/* Get pointer to packet from stream index. */
				Data_pkptr=op_pk_get(stream_index);
				
				//if (op_subq_pk_insert(0,Data_pkptr,OPC_QPOS_TAIL)!=OPC_QINS_OK)
					//{
					/* the insertion failed (due to a 	 */
					/* full queue) 						 */
					//op_pk_destroy(Data_pkptr);
					/* set flag indicating insertion fail*/
					/* this flag is used to determine	 */
					/* transition out of this state		 */
					//insert_ok= 0;
					//}
				//else{
					/* insertion was successful			 */
					insert_ok= 1;
					//}
				}
				FSM_PROFILE_SECTION_OUT (state2_exit_exec)


			/** state (svc_packet) transition processing **/
			FSM_PROFILE_SECTION_IN ("CAP_PROC [svc_packet trans conditions]", state2_trans_conds)
			FSM_INIT_COND (insert_ok)
			FSM_DFLT_COND
			FSM_TEST_LOGIC ("svc_packet")
			FSM_PROFILE_SECTION_OUT (state2_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 3, state3_enter_exec, ;, "insert_ok", "", "svc_packet", "backoff", "tr_3", "CAP_PROC [svc_packet -> backoff : insert_ok / ]")
				FSM_CASE_TRANSIT (1, 1, state1_enter_exec, ;, "default", "", "svc_packet", "Idle", "tr_2", "CAP_PROC [svc_packet -> Idle : default / ]")
				}
				/*---------------------------------------------------------*/



			/** state (backoff) enter executives **/
			FSM_STATE_ENTER_FORCED (3, "backoff", state3_enter_exec, "CAP_PROC [backoff enter execs]")
				FSM_PROFILE_SECTION_IN ("CAP_PROC [backoff enter execs]", state3_enter_exec)
				{
				
				
				backoff (BackoffWindow,BackoffExponent);
				
				creat_BIFS_Intr();
					
				}
				FSM_PROFILE_SECTION_OUT (state3_enter_exec)

			/** state (backoff) exit executives **/
			FSM_STATE_EXIT_FORCED (3, "backoff", "CAP_PROC [backoff exit execs]")


			/** state (backoff) transition processing **/
			FSM_TRANSIT_FORCE (9, state9_enter_exec, ;, "default", "", "backoff", "wait_BIFS", "tr_19", "CAP_PROC [backoff -> wait_BIFS : default / ]")
				/*---------------------------------------------------------*/



			/** state (check_channel) enter executives **/
			FSM_STATE_ENTER_FORCED (4, "check_channel", state4_enter_exec, "CAP_PROC [check_channel enter execs]")
				FSM_PROFILE_SECTION_IN ("CAP_PROC [check_channel enter execs]", state4_enter_exec)
				{
				//if (channel_busy==1 )
				//	{
				//	creat_BIFS_Intr();
				//	}
				//else 
					if(BWRandom!=0)
					{
					BWRandom=BWRandom-1;
					creat_BIFS_Intr();
					}
				else
					{
					/* get a handle on packet at head of subqueue 0 */
					/* (this does not remove the packet)			*/
					
					//Data_pkptr = op_pk_get (StreamFromApp);
					//Data_pkptr=op_subq_pk_access (0,OPC_QPOS_HEAD);
					
					copyPkt = op_pk_copy (Data_pkptr);
				
					//op_pk_nfd_set_int32 (Data_pkptr, "dest_address",(int)op_dist_outcome (address_dist));	
					op_pk_send (Data_pkptr,StreamToLink );
				
					/* determine the packets length (in bits)		*/
					pk_len= op_pk_total_size_get(Data_pkptr);
				
					/* determine the time required to complete 		*/
					/* service of the packet						*/
					pk_svc_time=pk_len/ service_rate;
				
					/* schedule an interrupt for this process		*/
					/* at the time where service ends.				*/
					PacketSent_intr_handle=op_intrpt_schedule_self(op_sim_time ()+ pk_svc_time,PacketSent_intr_code);
					Timeout_handle=op_intrpt_schedule_self(op_sim_time ()+ pk_svc_time+1,PacketSent_intr_code);
					
					/* the channel is now busy.						*/
					channel_busy= 1;
					}
				}
				FSM_PROFILE_SECTION_OUT (state4_enter_exec)

			/** state (check_channel) exit executives **/
			FSM_STATE_EXIT_FORCED (4, "check_channel", "CAP_PROC [check_channel exit execs]")


			/** state (check_channel) transition processing **/
			FSM_TRANSIT_FORCE (9, state9_enter_exec, ;, "default", "", "check_channel", "wait_BIFS", "tr_21", "CAP_PROC [check_channel -> wait_BIFS : default / ]")
				/*---------------------------------------------------------*/



			/** state (wait_ACK) enter executives **/
			FSM_STATE_ENTER_UNFORCED (5, "wait_ACK", state5_enter_exec, "CAP_PROC [wait_ACK enter execs]")

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (11,"CAP_PROC")


			/** state (wait_ACK) exit executives **/
			FSM_STATE_EXIT_UNFORCED (5, "wait_ACK", "CAP_PROC [wait_ACK exit execs]")


			/** state (wait_ACK) transition processing **/
			FSM_PROFILE_SECTION_IN ("CAP_PROC [wait_ACK trans conditions]", state5_trans_conds)
			FSM_INIT_COND (ACK)
			FSM_TEST_COND (Timeout && (RetryCount+1)<=max_RetryCount)
			FSM_TEST_COND (Timeout&& (RetryCount+1)>max_RetryCount)
			FSM_DFLT_COND
			FSM_TEST_LOGIC ("wait_ACK")
			FSM_PROFILE_SECTION_OUT (state5_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 6, state6_enter_exec, ;, "ACK", "", "wait_ACK", "PKTSending_success", "tr_11", "CAP_PROC [wait_ACK -> PKTSending_success : ACK / ]")
				FSM_CASE_TRANSIT (1, 7, state7_enter_exec, ;, "Timeout && (RetryCount+1)<=max_RetryCount", "", "wait_ACK", "PKTSending_faild", "tr_13", "CAP_PROC [wait_ACK -> PKTSending_faild : Timeout && (RetryCount+1)<=max_RetryCount / ]")
				FSM_CASE_TRANSIT (2, 6, state6_enter_exec, ;, "Timeout&& (RetryCount+1)>max_RetryCount", "", "wait_ACK", "PKTSending_success", "tr_18", "CAP_PROC [wait_ACK -> PKTSending_success : Timeout&& (RetryCount+1)>max_RetryCount / ]")
				FSM_CASE_TRANSIT (3, 5, state5_enter_exec, ;, "default", "", "wait_ACK", "wait_ACK", "tr_25", "CAP_PROC [wait_ACK -> wait_ACK : default / ]")
				}
				/*---------------------------------------------------------*/



			/** state (PKTSending_success) enter executives **/
			FSM_STATE_ENTER_FORCED (6, "PKTSending_success", state6_enter_exec, "CAP_PROC [PKTSending_success enter execs]")
				FSM_PROFILE_SECTION_IN ("CAP_PROC [PKTSending_success enter execs]", state6_enter_exec)
				{
				RetryCount=0;
				BackoffExponent=3;
				}
				FSM_PROFILE_SECTION_OUT (state6_enter_exec)

			/** state (PKTSending_success) exit executives **/
			FSM_STATE_EXIT_FORCED (6, "PKTSending_success", "CAP_PROC [PKTSending_success exit execs]")


			/** state (PKTSending_success) transition processing **/
			FSM_TRANSIT_FORCE (1, state1_enter_exec, ;, "default", "", "PKTSending_success", "Idle", "tr_12", "CAP_PROC [PKTSending_success -> Idle : default / ]")
				/*---------------------------------------------------------*/



			/** state (PKTSending_faild) enter executives **/
			FSM_STATE_ENTER_FORCED (7, "PKTSending_faild", state7_enter_exec, "CAP_PROC [PKTSending_faild enter execs]")
				FSM_PROFILE_SECTION_IN ("CAP_PROC [PKTSending_faild enter execs]", state7_enter_exec)
				{
				RetryCount=RetryCount+1;
				BackoffExponent=BackoffExponent+1;
				
				creat_RIFS_Intr();
				}
				FSM_PROFILE_SECTION_OUT (state7_enter_exec)

			/** state (PKTSending_faild) exit executives **/
			FSM_STATE_EXIT_FORCED (7, "PKTSending_faild", "CAP_PROC [PKTSending_faild exit execs]")


			/** state (PKTSending_faild) transition processing **/
			FSM_TRANSIT_FORCE (8, state8_enter_exec, ;, "default", "", "PKTSending_faild", "wait_RIFS", "tr_15", "CAP_PROC [PKTSending_faild -> wait_RIFS : default / ]")
				/*---------------------------------------------------------*/



			/** state (wait_RIFS) enter executives **/
			FSM_STATE_ENTER_UNFORCED (8, "wait_RIFS", state8_enter_exec, "CAP_PROC [wait_RIFS enter execs]")

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (17,"CAP_PROC")


			/** state (wait_RIFS) exit executives **/
			FSM_STATE_EXIT_UNFORCED (8, "wait_RIFS", "CAP_PROC [wait_RIFS exit execs]")


			/** state (wait_RIFS) transition processing **/
			FSM_PROFILE_SECTION_IN ("CAP_PROC [wait_RIFS trans conditions]", state8_trans_conds)
			FSM_INIT_COND (RIFS_intr)
			FSM_DFLT_COND
			FSM_TEST_LOGIC ("wait_RIFS")
			FSM_PROFILE_SECTION_OUT (state8_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 3, state3_enter_exec, ;, "RIFS_intr", "", "wait_RIFS", "backoff", "tr_17", "CAP_PROC [wait_RIFS -> backoff : RIFS_intr / ]")
				FSM_CASE_TRANSIT (1, 8, state8_enter_exec, ;, "default", "", "wait_RIFS", "wait_RIFS", "tr_27", "CAP_PROC [wait_RIFS -> wait_RIFS : default / ]")
				}
				/*---------------------------------------------------------*/



			/** state (wait_BIFS) enter executives **/
			FSM_STATE_ENTER_UNFORCED (9, "wait_BIFS", state9_enter_exec, "CAP_PROC [wait_BIFS enter execs]")

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (19,"CAP_PROC")


			/** state (wait_BIFS) exit executives **/
			FSM_STATE_EXIT_UNFORCED (9, "wait_BIFS", "CAP_PROC [wait_BIFS exit execs]")


			/** state (wait_BIFS) transition processing **/
			FSM_PROFILE_SECTION_IN ("CAP_PROC [wait_BIFS trans conditions]", state9_trans_conds)
			FSM_INIT_COND (BIFS_intr)
			FSM_TEST_COND (PacketSent_intr)
			FSM_DFLT_COND
			FSM_TEST_LOGIC ("wait_BIFS")
			FSM_PROFILE_SECTION_OUT (state9_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 4, state4_enter_exec, ;, "BIFS_intr", "", "wait_BIFS", "check_channel", "tr_20", "CAP_PROC [wait_BIFS -> check_channel : BIFS_intr / ]")
				FSM_CASE_TRANSIT (1, 5, state5_enter_exec, ;, "PacketSent_intr", "", "wait_BIFS", "wait_ACK", "tr_22", "CAP_PROC [wait_BIFS -> wait_ACK : PacketSent_intr / ]")
				FSM_CASE_TRANSIT (2, 9, state9_enter_exec, ;, "default", "", "wait_BIFS", "wait_BIFS", "tr_26", "CAP_PROC [wait_BIFS -> wait_BIFS : default / ]")
				}
				/*---------------------------------------------------------*/



			}


		FSM_EXIT (0,"CAP_PROC")
		}
	}




void
_op_CAP_PROC_diag (OP_SIM_CONTEXT_ARG_OPT)
	{
	/* No Diagnostic Block */
	}




void
_op_CAP_PROC_terminate (OP_SIM_CONTEXT_ARG_OPT)
	{

	FIN_MT (_op_CAP_PROC_terminate ())


	/* No Termination Block */

	Vos_Poolmem_Dealloc (op_sv_ptr);

	FOUT
	}


/* Undefine shortcuts to state variables to avoid */
/* syntax error in direct access to fields of */
/* local variable prs_ptr in _op_CAP_PROC_svar function. */
#undef channel_busy
#undef service_rate
#undef BIFS_intr_handle
#undef RIFS_intr_handle
#undef Timeout_handle
#undef PacketSent_intr_handle
#undef ete_gsh
#undef node_id
#undef address_dist
#undef Data_pkptr

#undef FIN_PREAMBLE_DEC
#undef FIN_PREAMBLE_CODE

#define FIN_PREAMBLE_DEC
#define FIN_PREAMBLE_CODE

VosT_Obtype
_op_CAP_PROC_init (int * init_block_ptr)
	{
	VosT_Obtype obtype = OPC_NIL;
	FIN_MT (_op_CAP_PROC_init (init_block_ptr))

	obtype = Vos_Define_Object_Prstate ("proc state vars (CAP_PROC)",
		sizeof (CAP_PROC_state));
	*init_block_ptr = 0;

	FRET (obtype)
	}

VosT_Address
_op_CAP_PROC_alloc (VosT_Obtype obtype, int init_block)
	{
#if !defined (VOSD_NO_FIN)
	int _op_block_origin = 0;
#endif
	CAP_PROC_state * ptr;
	FIN_MT (_op_CAP_PROC_alloc (obtype))

	ptr = (CAP_PROC_state *)Vos_Alloc_Object (obtype);
	if (ptr != OPC_NIL)
		{
		ptr->_op_current_block = init_block;
#if defined (OPD_ALLOW_ODB)
		ptr->_op_current_state = "CAP_PROC [init enter execs]";
#endif
		}
	FRET ((VosT_Address)ptr)
	}



void
_op_CAP_PROC_svar (void * gen_ptr, const char * var_name, void ** var_p_ptr)
	{
	CAP_PROC_state		*prs_ptr;

	FIN_MT (_op_CAP_PROC_svar (gen_ptr, var_name, var_p_ptr))

	if (var_name == OPC_NIL)
		{
		*var_p_ptr = (void *)OPC_NIL;
		FOUT
		}
	prs_ptr = (CAP_PROC_state *)gen_ptr;

	if (strcmp ("channel_busy" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->channel_busy);
		FOUT
		}
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
	if (strcmp ("PacketSent_intr_handle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->PacketSent_intr_handle);
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
	*var_p_ptr = (void *)OPC_NIL;

	FOUT
	}

