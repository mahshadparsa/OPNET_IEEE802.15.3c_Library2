/* Process model C form file: MCTA_proc.pr.c */
/* Portions of this file copyright 1986-2008 by OPNET Technologies, Inc. */



/* This variable carries the header into the object file */
const char MCTA_proc_pr_c [] = "MIL_3_Tfile_Hdr_ 145A 30A modeler 7 5846FBAD 5846FBAD 1 DESKTOP-FFL2R3R user 0 0 none none 0 0 none 0 0 0 0 0 0 0 0 1e80 8                                                                                                                                                                                                                                                                                                                                                                                                     ";
#include <string.h>



/* OPNET system definitions */
#include <opnet.h>

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
	} MCTA_proc_state;


/* These macro definitions will define a local variable called	*/
/* "op_sv_ptr" in each function containing a FIN statement.	*/
/* This variable points to the state variable data structure,	*/
/* and can be used from a C debugger to display their values.	*/
#undef FIN_PREAMBLE_DEC
#undef FIN_PREAMBLE_CODE
#define FIN_PREAMBLE_DEC	MCTA_proc_state *op_sv_ptr;
#define FIN_PREAMBLE_CODE	\
		op_sv_ptr = ((MCTA_proc_state *)(OP_SIM_CONTEXT_PTR->_op_mod_state_ptr));


/* No Function Block */

#if !defined (VOSD_NO_FIN)
enum { _op_block_origin = __LINE__ };
#endif

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
	void MCTA_proc (OP_SIM_CONTEXT_ARG_OPT);
	VosT_Obtype _op_MCTA_proc_init (int * init_block_ptr);
	void _op_MCTA_proc_diag (OP_SIM_CONTEXT_ARG_OPT);
	void _op_MCTA_proc_terminate (OP_SIM_CONTEXT_ARG_OPT);
	VosT_Address _op_MCTA_proc_alloc (VosT_Obtype, int);
	void _op_MCTA_proc_svar (void *, const char *, void **);


#if defined (__cplusplus)
} /* end of 'extern "C"' */
#endif




/* Process model interrupt handling procedure */


void
MCTA_proc (OP_SIM_CONTEXT_ARG_OPT)
	{
#if !defined (VOSD_NO_FIN)
	int _op_block_origin = 0;
#endif
	FIN_MT (MCTA_proc ());

		{


		FSM_ENTER_NO_VARS ("MCTA_proc")

		FSM_BLOCK_SWITCH
			{
			/*---------------------------------------------------------*/
			/** state (init) enter executives **/
			FSM_STATE_ENTER_UNFORCED (0, "init", state0_enter_exec, "MCTA_proc [init enter execs]")

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (1,"MCTA_proc")


			/** state (init) exit executives **/
			FSM_STATE_EXIT_UNFORCED (0, "init", "MCTA_proc [init exit execs]")


			/** state (init) transition processing **/
			FSM_TRANSIT_FORCE (0, state0_enter_exec, ;, "default", "", "init", "init", "tr_0", "MCTA_proc [init -> init : default / ]")
				/*---------------------------------------------------------*/



			}


		FSM_EXIT (0,"MCTA_proc")
		}
	}




void
_op_MCTA_proc_diag (OP_SIM_CONTEXT_ARG_OPT)
	{
	/* No Diagnostic Block */
	}




void
_op_MCTA_proc_terminate (OP_SIM_CONTEXT_ARG_OPT)
	{

	FIN_MT (_op_MCTA_proc_terminate ())


	/* No Termination Block */

	Vos_Poolmem_Dealloc (op_sv_ptr);

	FOUT
	}


#undef FIN_PREAMBLE_DEC
#undef FIN_PREAMBLE_CODE

#define FIN_PREAMBLE_DEC
#define FIN_PREAMBLE_CODE

VosT_Obtype
_op_MCTA_proc_init (int * init_block_ptr)
	{
	VosT_Obtype obtype = OPC_NIL;
	FIN_MT (_op_MCTA_proc_init (init_block_ptr))

	obtype = Vos_Define_Object_Prstate ("proc state vars (MCTA_proc)",
		sizeof (MCTA_proc_state));
	*init_block_ptr = 0;

	FRET (obtype)
	}

VosT_Address
_op_MCTA_proc_alloc (VosT_Obtype obtype, int init_block)
	{
#if !defined (VOSD_NO_FIN)
	int _op_block_origin = 0;
#endif
	MCTA_proc_state * ptr;
	FIN_MT (_op_MCTA_proc_alloc (obtype))

	ptr = (MCTA_proc_state *)Vos_Alloc_Object (obtype);
	if (ptr != OPC_NIL)
		{
		ptr->_op_current_block = init_block;
#if defined (OPD_ALLOW_ODB)
		ptr->_op_current_state = "MCTA_proc [init enter execs]";
#endif
		}
	FRET ((VosT_Address)ptr)
	}



void
_op_MCTA_proc_svar (void * gen_ptr, const char * var_name, void ** var_p_ptr)
	{

	FIN_MT (_op_MCTA_proc_svar (gen_ptr, var_name, var_p_ptr))

	*var_p_ptr = (void *)OPC_NIL;

	FOUT
	}

