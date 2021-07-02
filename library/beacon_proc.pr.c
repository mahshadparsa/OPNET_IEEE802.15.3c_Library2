/* Process model C form file: beacon_proc.pr.c */
/* Portions of this file copyright 1986-2008 by OPNET Technologies, Inc. */



/* This variable carries the header into the object file */
const char beacon_proc_pr_c [] = "MIL_3_Tfile_Hdr_ 145A 30A op_runsim 7 5868EE15 5868EE15 1 DESKTOP-FFL2R3R user 0 0 none none 0 0 none 0 0 0 0 0 0 0 0 1e80 8                                                                                                                                                                                                                                                                                                                                                                                                   ";
#include <string.h>



/* OPNET system definitions */
#include <opnet.h>



/* Header Block */

#define StreamFromLink 0
#define beaconArrival    (op_intrpt_type() == OPC_INTRPT_STRM && op_intrpt_strm() == StreamFromLink)
int frame_type;
double superframe_duration;
double CAP_end_time;

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
	Packet *	               		beacon_pkptr                                    ;
	double	                 		CAP_end_time                                    ;
	double	                 		superframe_duration                             ;
	} beacon_proc_state;

#define beacon_pkptr            		op_sv_ptr->beacon_pkptr
#define CAP_end_time            		op_sv_ptr->CAP_end_time
#define superframe_duration     		op_sv_ptr->superframe_duration

/* These macro definitions will define a local variable called	*/
/* "op_sv_ptr" in each function containing a FIN statement.	*/
/* This variable points to the state variable data structure,	*/
/* and can be used from a C debugger to display their values.	*/
#undef FIN_PREAMBLE_DEC
#undef FIN_PREAMBLE_CODE
#define FIN_PREAMBLE_DEC	beacon_proc_state *op_sv_ptr;
#define FIN_PREAMBLE_CODE	\
		op_sv_ptr = ((beacon_proc_state *)(OP_SIM_CONTEXT_PTR->_op_mod_state_ptr));


/* Function Block */

#if !defined (VOSD_NO_FIN)
enum { _op_block_origin = __LINE__ + 2};
#endif

static void get_beacon_pkt()
	{
		int stream_index;
		FIN (get_beacon_pkt());
		
		stream_index=op_intrpt_strm();
		beacon_pkptr=op_pk_get(stream_index);
		
		op_pk_nfd_get (beacon_pkptr, "frame_type", &frame_type); //acording to the standard, type value of beacon is zero
		op_pk_nfd_get (beacon_pkptr, "superframe_duration",&superframe_duration);
		op_pk_nfd_get (beacon_pkptr, "CAP_end_time",&CAP_end_time);
		//op_pk_nfd_get_int32 (beacon_pkptr, "PNC_address",1);
		printf("frame_type=%d",frame_type);
		printf("superframe_duration=%f",superframe_duration);
		printf("CAP_end_time=%f",CAP_end_time);
		
		
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
	void beacon_proc (OP_SIM_CONTEXT_ARG_OPT);
	VosT_Obtype _op_beacon_proc_init (int * init_block_ptr);
	void _op_beacon_proc_diag (OP_SIM_CONTEXT_ARG_OPT);
	void _op_beacon_proc_terminate (OP_SIM_CONTEXT_ARG_OPT);
	VosT_Address _op_beacon_proc_alloc (VosT_Obtype, int);
	void _op_beacon_proc_svar (void *, const char *, void **);


#if defined (__cplusplus)
} /* end of 'extern "C"' */
#endif




/* Process model interrupt handling procedure */


void
beacon_proc (OP_SIM_CONTEXT_ARG_OPT)
	{
#if !defined (VOSD_NO_FIN)
	int _op_block_origin = 0;
#endif
	FIN_MT (beacon_proc ());

		{


		FSM_ENTER ("beacon_proc")

		FSM_BLOCK_SWITCH
			{
			/*---------------------------------------------------------*/
			/** state (init) enter executives **/
			FSM_STATE_ENTER_UNFORCED (0, "init", state0_enter_exec, "beacon_proc [init enter execs]")

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (1,"beacon_proc")


			/** state (init) exit executives **/
			FSM_STATE_EXIT_UNFORCED (0, "init", "beacon_proc [init exit execs]")


			/** state (init) transition processing **/
			FSM_PROFILE_SECTION_IN ("beacon_proc [init trans conditions]", state0_trans_conds)
			FSM_INIT_COND (beaconArrival)
			FSM_DFLT_COND
			FSM_TEST_LOGIC ("init")
			FSM_PROFILE_SECTION_OUT (state0_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 1, state1_enter_exec, ;, "beaconArrival", "", "init", "st_2", "tr_2", "beacon_proc [init -> st_2 : beaconArrival / ]")
				FSM_CASE_TRANSIT (1, 0, state0_enter_exec, ;, "default", "", "init", "init", "tr_1", "beacon_proc [init -> init : default / ]")
				}
				/*---------------------------------------------------------*/



			/** state (st_2) enter executives **/
			FSM_STATE_ENTER_FORCED (1, "st_2", state1_enter_exec, "beacon_proc [st_2 enter execs]")
				FSM_PROFILE_SECTION_IN ("beacon_proc [st_2 enter execs]", state1_enter_exec)
				{
				get_beacon_pkt();
				}
				FSM_PROFILE_SECTION_OUT (state1_enter_exec)

			/** state (st_2) exit executives **/
			FSM_STATE_EXIT_FORCED (1, "st_2", "beacon_proc [st_2 exit execs]")


			/** state (st_2) transition processing **/
			FSM_TRANSIT_FORCE (0, state0_enter_exec, ;, "default", "", "st_2", "init", "tr_3", "beacon_proc [st_2 -> init : default / ]")
				/*---------------------------------------------------------*/



			}


		FSM_EXIT (0,"beacon_proc")
		}
	}




void
_op_beacon_proc_diag (OP_SIM_CONTEXT_ARG_OPT)
	{
	/* No Diagnostic Block */
	}




void
_op_beacon_proc_terminate (OP_SIM_CONTEXT_ARG_OPT)
	{

	FIN_MT (_op_beacon_proc_terminate ())


	/* No Termination Block */

	Vos_Poolmem_Dealloc (op_sv_ptr);

	FOUT
	}


/* Undefine shortcuts to state variables to avoid */
/* syntax error in direct access to fields of */
/* local variable prs_ptr in _op_beacon_proc_svar function. */
#undef beacon_pkptr
#undef CAP_end_time
#undef superframe_duration

#undef FIN_PREAMBLE_DEC
#undef FIN_PREAMBLE_CODE

#define FIN_PREAMBLE_DEC
#define FIN_PREAMBLE_CODE

VosT_Obtype
_op_beacon_proc_init (int * init_block_ptr)
	{
	VosT_Obtype obtype = OPC_NIL;
	FIN_MT (_op_beacon_proc_init (init_block_ptr))

	obtype = Vos_Define_Object_Prstate ("proc state vars (beacon_proc)",
		sizeof (beacon_proc_state));
	*init_block_ptr = 0;

	FRET (obtype)
	}

VosT_Address
_op_beacon_proc_alloc (VosT_Obtype obtype, int init_block)
	{
#if !defined (VOSD_NO_FIN)
	int _op_block_origin = 0;
#endif
	beacon_proc_state * ptr;
	FIN_MT (_op_beacon_proc_alloc (obtype))

	ptr = (beacon_proc_state *)Vos_Alloc_Object (obtype);
	if (ptr != OPC_NIL)
		{
		ptr->_op_current_block = init_block;
#if defined (OPD_ALLOW_ODB)
		ptr->_op_current_state = "beacon_proc [init enter execs]";
#endif
		}
	FRET ((VosT_Address)ptr)
	}



void
_op_beacon_proc_svar (void * gen_ptr, const char * var_name, void ** var_p_ptr)
	{
	beacon_proc_state		*prs_ptr;

	FIN_MT (_op_beacon_proc_svar (gen_ptr, var_name, var_p_ptr))

	if (var_name == OPC_NIL)
		{
		*var_p_ptr = (void *)OPC_NIL;
		FOUT
		}
	prs_ptr = (beacon_proc_state *)gen_ptr;

	if (strcmp ("beacon_pkptr" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->beacon_pkptr);
		FOUT
		}
	if (strcmp ("CAP_end_time" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->CAP_end_time);
		FOUT
		}
	if (strcmp ("superframe_duration" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->superframe_duration);
		FOUT
		}
	*var_p_ptr = (void *)OPC_NIL;

	FOUT
	}

