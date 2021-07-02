/* Process model C form file: RCAP_PROC_PNC.pr.c */
/* Portions of this file copyright 1986-2008 by OPNET Technologies, Inc. */



/* This variable carries the header into the object file */
const char RCAP_PROC_PNC_pr_c [] = "MIL_3_Tfile_Hdr_ 145A 30A op_runsim 7 590F6A3B 590F6A3B 1 DESKTOP-FFL2R3R user 0 0 none none 0 0 none 0 0 0 0 0 0 0 0 1e80 8                                                                                                                                                                                                                                                                                                                                                                                                   ";
#include <string.h>



/* OPNET system definitions */
#include <opnet.h>



/* Header Block */

/* Node configuration constants.	*/
#define StreamFromLink 0
#define StreamToLink   0
#define PowerUp         (op_intrpt_type() == OPC_INTRPT_BEGSIM)

#define FrameArrival    (op_intrpt_type() == OPC_INTRPT_STRM && op_intrpt_strm() == StreamFromLink)

#define beacon_intr_Code 0
#define beacon_intr       (op_intrpt_type() == OPC_INTRPT_SELF && op_intrpt_code() == beacon_intr_Code)

#define ch_busy 0
#define channel_busy (op_stat_local_read(ch_busy)==1)
int num_CTA_req;
int nodeID_CTA_req[32];

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
	char	                   		format_str [64]                                 ;
	Evhandle	               		beacon_intr_handle                              ;
	double	                 		tx_state                                        ;
	} RCAP_PROC_PNC_state;

#define format_str              		op_sv_ptr->format_str
#define beacon_intr_handle      		op_sv_ptr->beacon_intr_handle
#define tx_state                		op_sv_ptr->tx_state

/* These macro definitions will define a local variable called	*/
/* "op_sv_ptr" in each function containing a FIN statement.	*/
/* This variable points to the state variable data structure,	*/
/* and can be used from a C debugger to display their values.	*/
#undef FIN_PREAMBLE_DEC
#undef FIN_PREAMBLE_CODE
#define FIN_PREAMBLE_DEC	RCAP_PROC_PNC_state *op_sv_ptr;
#define FIN_PREAMBLE_CODE	\
		op_sv_ptr = ((RCAP_PROC_PNC_state *)(OP_SIM_CONTEXT_PTR->_op_mod_state_ptr));


/* Function Block */

#if !defined (VOSD_NO_FIN)
enum { _op_block_origin = __LINE__ + 2};
#endif

static void send_ack (Packet *Data_pk)
	{		
	Packet * ack_pk;
	int data_ID;
	double	pksize=496;
	FIN (send_ack (Data_pk));
	op_pk_nfd_get (Data_pk, "src_ID",&data_ID);	
	//printf("data_ID=%d",data_ID);
	
	/* Create a packet with the specified format.	*/
	ack_pk = op_pk_create_fmt ("Imm_ACK_pkt");
	op_pk_total_size_set (ack_pk, pksize); 
	op_pk_nfd_set_int32 (ack_pk, "frame_type", 1); //acording to the standard, type value of beacon is zero
	op_pk_nfd_set_int32 (ack_pk, "dest_ID",data_ID);

	//op_pk_total_size_set (pkptr, pksize);
	//printf("channel_busy=%d",channel_busy);
	//if (channel_busy==0)
		op_pk_send (ack_pk, StreamToLink);
	
	FOUT;
	}


static void send_beacon ()
	{		
	Packet *beacon_pk,*enc_IE_body,*enc_CTA_block,*enc_CTA_blocks;
	int	pksize=696,pksize1;//whithout IE_body
	double  SD=0.065535;
	double  CAPD=0.030000;
	double  CTAPD=0.030000;//SD-CAPD-BeaconD;
	double  BeaconD=0.000535;
	int i;
	
	FIN (send_beacon ());
	
	/* Create a packet with the specified format.	*/
	beacon_pk = op_pk_create_fmt ("beacon_pkt"); 
	enc_IE_body = op_pk_create_fmt ("IE_body");
	
	/*************************************************************/
	//printf("num_CTA_req=%d",num_CTA_req);
	if (num_CTA_req>0)
		{
		enc_CTA_blocks = op_pk_create_fmt ("CTA_blocks");
		for (i=1;i<=num_CTA_req;i++)
			{
			enc_CTA_block = op_pk_create_fmt ("CTA_block");	
			//printf("\n nodeID_CTA_req[%d]=%d,num_CTA_req=%d",i,nodeID_CTA_req[i],num_CTA_req);
			op_pk_nfd_set (enc_CTA_block, "DestID", nodeID_CTA_req[i]);
			op_pk_nfd_set (enc_CTA_block, "SrcID", 0);
			op_pk_nfd_set (enc_CTA_block, "CTA duration", CTAPD/num_CTA_req);
			op_pk_nfd_set (enc_CTA_block, "CTA location", (i-1)*CTAPD/num_CTA_req);
			op_pk_fd_set (enc_CTA_blocks,i-1,OPC_FIELD_TYPE_PACKET, enc_CTA_block,-1);//CTA_block
			}

		op_pk_fd_set (enc_IE_body,0,OPC_FIELD_TYPE_INTEGER, 0,8);//"element_ID0"
		op_pk_fd_set (enc_IE_body,1,OPC_FIELD_TYPE_INTEGER, num_CTA_req*7,8);//"length0" 
		op_pk_fd_set (enc_IE_body,2,OPC_FIELD_TYPE_PACKET , enc_CTA_blocks,-1);//"IE_payload0"
		
		//op_pk_bulk_size_set (enc_IE_body,8+8+56*num_CTA_req+8+8+48);
		pksize=pksize+8+8+56*num_CTA_req+8+8+48;
		}
	else
		{
		//op_pk_bulk_size_set (enc_IE_body,8+8+48);
		pksize=pksize+8+8+48;
		}
		
	//printf("pksize=%d",pksize);
	/*************************************************************/
	
	op_pk_nfd_set (beacon_pk, "IE_body", enc_IE_body);
	
	op_pk_nfd_set_int32 (beacon_pk, "frame_type", 0); //acording to the standard, type value of beacon is zero
	op_pk_nfd_set (beacon_pk, "superframe_duration",SD);
	op_pk_nfd_set (beacon_pk, "CAP_end_time",CAPD);
	op_pk_nfd_set_int32 (beacon_pk, "PNC_address",1);
	
	op_pk_total_size_set (beacon_pk,pksize);
	pksize1=op_pk_total_size_get(beacon_pk);
	//printf("pksize1=%d\n",pksize1);
	op_pk_send (beacon_pk, StreamToLink);
	FOUT;
	}

static void send_assoc_res(Packet *Data_pk)
	{		
	Packet *assoc_res_pk;
	double	pksize=720;
	FIN (send_assoc_res (Data_pk));
	
	/* Create a packet with the specified format.	*/
	assoc_res_pk = op_pk_create_fmt ("assoc_res_cb");
	op_pk_total_size_set (assoc_res_pk, pksize); 
	//set_beacon_pk_fields
	
	op_pk_nfd_set_int32 (assoc_res_pk, "command_type", 1); 
	op_pk_send (assoc_res_pk, StreamToLink);
	
	FOUT;
	}

static void creat_beacon_Intr()
	{
	double superframe_duration=0.065535;
	FIN (creat_beacon_Intr ());
	beacon_intr_handle=op_intrpt_schedule_self(op_sim_time()+ superframe_duration,beacon_intr_Code);
	//printf("op_sim_time()+ superframe_duration=%2g",op_sim_time()+ superframe_duration);
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
	void RCAP_PROC_PNC (OP_SIM_CONTEXT_ARG_OPT);
	VosT_Obtype _op_RCAP_PROC_PNC_init (int * init_block_ptr);
	void _op_RCAP_PROC_PNC_diag (OP_SIM_CONTEXT_ARG_OPT);
	void _op_RCAP_PROC_PNC_terminate (OP_SIM_CONTEXT_ARG_OPT);
	VosT_Address _op_RCAP_PROC_PNC_alloc (VosT_Obtype, int);
	void _op_RCAP_PROC_PNC_svar (void *, const char *, void **);


#if defined (__cplusplus)
} /* end of 'extern "C"' */
#endif




/* Process model interrupt handling procedure */


void
RCAP_PROC_PNC (OP_SIM_CONTEXT_ARG_OPT)
	{
#if !defined (VOSD_NO_FIN)
	int _op_block_origin = 0;
#endif
	FIN_MT (RCAP_PROC_PNC ());

		{


		FSM_ENTER ("RCAP_PROC_PNC")

		FSM_BLOCK_SWITCH
			{
			/*---------------------------------------------------------*/
			/** state (idle) enter executives **/
			FSM_STATE_ENTER_UNFORCED (0, "idle", state0_enter_exec, "RCAP_PROC_PNC [idle enter execs]")

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (1,"RCAP_PROC_PNC")


			/** state (idle) exit executives **/
			FSM_STATE_EXIT_UNFORCED (0, "idle", "RCAP_PROC_PNC [idle exit execs]")


			/** state (idle) transition processing **/
			FSM_PROFILE_SECTION_IN ("RCAP_PROC_PNC [idle trans conditions]", state0_trans_conds)
			FSM_INIT_COND (beacon_intr )
			FSM_TEST_COND (FrameArrival)
			FSM_DFLT_COND
			FSM_TEST_LOGIC ("idle")
			FSM_PROFILE_SECTION_OUT (state0_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 2, state2_enter_exec, ;, "beacon_intr ", "", "idle", "send_beacon", "tr_4", "RCAP_PROC_PNC [idle -> send_beacon : beacon_intr  / ]")
				FSM_CASE_TRANSIT (1, 1, state1_enter_exec, ;, "FrameArrival", "", "idle", "send_responce", "tr_14", "RCAP_PROC_PNC [idle -> send_responce : FrameArrival / ]")
				FSM_CASE_TRANSIT (2, 0, state0_enter_exec, ;, "default", "", "idle", "idle", "tr_0", "RCAP_PROC_PNC [idle -> idle : default / ]")
				}
				/*---------------------------------------------------------*/



			/** state (send_responce) enter executives **/
			FSM_STATE_ENTER_FORCED (1, "send_responce", state1_enter_exec, "RCAP_PROC_PNC [send_responce enter execs]")
				FSM_PROFILE_SECTION_IN ("RCAP_PROC_PNC [send_responce enter execs]", state1_enter_exec)
				{
				Packet *pkt;
				int frame_type;
				pkt = op_pk_get (StreamFromLink);
				op_pk_nfd_get (pkt, "frame_type",&frame_type);	
				if (frame_type==3)//that frame is command frame
					{
					send_ack (pkt);
					//send_assoc_res(pkt);//creat response 
					num_CTA_req=num_CTA_req+1;
					op_pk_nfd_get (pkt, "CTRqB_Target_IDlist",&nodeID_CTA_req[num_CTA_req]);
					//printf("nodeID_CTA_req[%d]=%d",num_CTA_req,nodeID_CTA_req[num_CTA_req]);
					}
				else if (frame_type==4)//that frame is data frame
					send_ack (pkt);
				}
				FSM_PROFILE_SECTION_OUT (state1_enter_exec)

			/** state (send_responce) exit executives **/
			FSM_STATE_EXIT_FORCED (1, "send_responce", "RCAP_PROC_PNC [send_responce exit execs]")


			/** state (send_responce) transition processing **/
			FSM_TRANSIT_FORCE (0, state0_enter_exec, ;, "default", "", "send_responce", "idle", "tr_15", "RCAP_PROC_PNC [send_responce -> idle : default / ]")
				/*---------------------------------------------------------*/



			/** state (send_beacon) enter executives **/
			FSM_STATE_ENTER_FORCED (2, "send_beacon", state2_enter_exec, "RCAP_PROC_PNC [send_beacon enter execs]")
				FSM_PROFILE_SECTION_IN ("RCAP_PROC_PNC [send_beacon enter execs]", state2_enter_exec)
				{
				send_beacon ();
				creat_beacon_Intr();
				num_CTA_req=0;
				}
				FSM_PROFILE_SECTION_OUT (state2_enter_exec)

			/** state (send_beacon) exit executives **/
			FSM_STATE_EXIT_FORCED (2, "send_beacon", "RCAP_PROC_PNC [send_beacon exit execs]")


			/** state (send_beacon) transition processing **/
			FSM_TRANSIT_FORCE (0, state0_enter_exec, ;, "default", "", "send_beacon", "idle", "tr_5", "RCAP_PROC_PNC [send_beacon -> idle : default / ]")
				/*---------------------------------------------------------*/



			/** state (init) enter executives **/
			FSM_STATE_ENTER_FORCED_NOLABEL (3, "init", "RCAP_PROC_PNC [init enter execs]")

			/** state (init) exit executives **/
			FSM_STATE_EXIT_FORCED (3, "init", "RCAP_PROC_PNC [init exit execs]")
				FSM_PROFILE_SECTION_IN ("RCAP_PROC_PNC [init exit execs]", state3_exit_exec)
				{
				send_beacon ();
				creat_beacon_Intr();
				num_CTA_req=0;
				tx_state=0;
				}
				FSM_PROFILE_SECTION_OUT (state3_exit_exec)


			/** state (init) transition processing **/
			FSM_TRANSIT_FORCE (0, state0_enter_exec, ;, "default", "", "init", "idle", "tr_8", "RCAP_PROC_PNC [init -> idle : default / ]")
				/*---------------------------------------------------------*/



			}


		FSM_EXIT (3,"RCAP_PROC_PNC")
		}
	}




void
_op_RCAP_PROC_PNC_diag (OP_SIM_CONTEXT_ARG_OPT)
	{
	/* No Diagnostic Block */
	}




void
_op_RCAP_PROC_PNC_terminate (OP_SIM_CONTEXT_ARG_OPT)
	{

	FIN_MT (_op_RCAP_PROC_PNC_terminate ())


	/* No Termination Block */

	Vos_Poolmem_Dealloc (op_sv_ptr);

	FOUT
	}


/* Undefine shortcuts to state variables to avoid */
/* syntax error in direct access to fields of */
/* local variable prs_ptr in _op_RCAP_PROC_PNC_svar function. */
#undef format_str
#undef beacon_intr_handle
#undef tx_state

#undef FIN_PREAMBLE_DEC
#undef FIN_PREAMBLE_CODE

#define FIN_PREAMBLE_DEC
#define FIN_PREAMBLE_CODE

VosT_Obtype
_op_RCAP_PROC_PNC_init (int * init_block_ptr)
	{
	VosT_Obtype obtype = OPC_NIL;
	FIN_MT (_op_RCAP_PROC_PNC_init (init_block_ptr))

	obtype = Vos_Define_Object_Prstate ("proc state vars (RCAP_PROC_PNC)",
		sizeof (RCAP_PROC_PNC_state));
	*init_block_ptr = 6;

	FRET (obtype)
	}

VosT_Address
_op_RCAP_PROC_PNC_alloc (VosT_Obtype obtype, int init_block)
	{
#if !defined (VOSD_NO_FIN)
	int _op_block_origin = 0;
#endif
	RCAP_PROC_PNC_state * ptr;
	FIN_MT (_op_RCAP_PROC_PNC_alloc (obtype))

	ptr = (RCAP_PROC_PNC_state *)Vos_Alloc_Object (obtype);
	if (ptr != OPC_NIL)
		{
		ptr->_op_current_block = init_block;
#if defined (OPD_ALLOW_ODB)
		ptr->_op_current_state = "RCAP_PROC_PNC [init enter execs]";
#endif
		}
	FRET ((VosT_Address)ptr)
	}



void
_op_RCAP_PROC_PNC_svar (void * gen_ptr, const char * var_name, void ** var_p_ptr)
	{
	RCAP_PROC_PNC_state		*prs_ptr;

	FIN_MT (_op_RCAP_PROC_PNC_svar (gen_ptr, var_name, var_p_ptr))

	if (var_name == OPC_NIL)
		{
		*var_p_ptr = (void *)OPC_NIL;
		FOUT
		}
	prs_ptr = (RCAP_PROC_PNC_state *)gen_ptr;

	if (strcmp ("format_str" , var_name) == 0)
		{
		*var_p_ptr = (void *) (prs_ptr->format_str);
		FOUT
		}
	if (strcmp ("beacon_intr_handle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->beacon_intr_handle);
		FOUT
		}
	if (strcmp ("tx_state" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->tx_state);
		FOUT
		}
	*var_p_ptr = (void *)OPC_NIL;

	FOUT
	}

