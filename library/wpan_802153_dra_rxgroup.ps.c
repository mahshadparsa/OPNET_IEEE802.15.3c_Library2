/* LZ_wpan_dra_rxgroup.ps.c */                                                       
/* Modified receiver group model for IEEE802.15.3 			*/
/* This routine is part of radio link Transceiver Pipeline 	*/					

/****************************************/
/*		 Copyright (c) 1993-2006		*/
/*		by OPNET Technologies, Inc.		*/
/*		 (A Delaware Corporation)		*/
/*	   7255 Woodmont Av., Suite 250  	*/
/*      Bethesda, MD 20814, U.S.A.      */
/*		   All Rights Reserved.			*/
/****************************************/

#include "opnet.h"
#include "dra.h"

#if defined (__cplusplus)
extern "C"
#endif
	
int
wpan_802153_dra_rxgroup (Objid tx_obid, Objid rx_obid)
	{
	Objid       tx_node_obid, rx_node_obid;
	DraT_Rxch_State_Info*	rxch_state_ptr;

	/** Determine the potential for communication between	**/
	/** given transmitter and receiver channel objects.		**/
	/** Also create and initialize the receiver channel's	**/
	/** state information to be used by other pipeline		**/
	/** stages during the simulation.						**/
	FIN_MT (wpan_802153_dra_rxgroup (tx_obid, rx_obid));

	/*Obtain the Objid of the node where tx/rx is located	**/
	tx_node_obid =  op_topo_parent (op_topo_parent (op_topo_parent (tx_obid))); 
	rx_node_obid =  op_topo_parent (op_topo_parent (op_topo_parent (rx_obid)));
	
	/* A Station shall not hear its own transmission.	*/
	if (tx_node_obid == rx_node_obid)
		{
		FRET (OPC_FALSE);
    	}
	
	/* Unless it is already done, initialize the receiver	*/
	/* channel's state information.							*/
	if (op_ima_obj_state_get (rx_obid) == OPC_NIL)
		{
#if defined (OPD_PARALLEL)
		/* Channel state information doesn't exist. Lock	*/
		/* the global mutex before continuing.				*/
		op_prg_mt_global_lock ();
		
		/* Check again since another thread may have		*/
		/* already set up the state information.			*/
		if (op_ima_obj_state_get (rx_obid) == OPC_NIL)
			{
#endif /* OPD_PARALLEL */
			/* Create and set the initial state information	*/
			/* for the receiver channel. State information	*/
			/* is used by other pipeline stages to			*/
			/* access/update channel specific data			*/
			/* efficiently.									*/
			rxch_state_ptr = (DraT_Rxch_State_Info *) 
				op_prg_mem_alloc (sizeof (DraT_Rxch_State_Info));
			rxch_state_ptr->signal_lock = OPC_FALSE;
			op_ima_obj_state_set (rx_obid, rxch_state_ptr);
#if defined (OPD_PARALLEL)
			}
		
		/* Unlock the global mutex.							*/
		op_prg_mt_global_unlock ();
#endif /* OPD_PARALLEL */
		}
		
	/* By default, all receivers are considered as			*/
	/* potential destinations.								*/
	FRET (OPC_TRUE)
	}                
