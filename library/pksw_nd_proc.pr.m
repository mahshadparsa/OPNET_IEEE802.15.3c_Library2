MIL_3_Tfile_Hdr_ 145A 140A modeler 9 44C91988 46AFC7A8 2 WTN11040 OPNET 0 0 none none 0 0 none 3AD219C8 D59 0 0 0 0 0 0 1e80 8                                                                                                                                                                                                                                                                                                                                                                                                  ??g?      @   D   H        
?  
?  
?  3  M  Q  U  ?           	   begsim intrpt         
   ????   
   doc file            	nd_module      endsim intrpt             ????      failure intrpts            disabled      intrpt interval         ԲI?%??}????      priority              ????      recovery intrpts            disabled      subqueue                     count    ???   
   ????   
      list   	???   
          
      super priority             ????             Distribution *	\address_dist;       Stathandle	\ete_gsh;              /* packet stream definitions */   #define RCV_IN_STRM 0   #define SRC_IN_STRM 1   #define XMT_OUT_STRM 0       /* transition macros */   )#define SRC_ARRVL (op_intrpt_type () == \   5	OPC_INTRPT_STRM && op_intrpt_strm () == SRC_IN_STRM)       )#define RCV_ARRVL (op_intrpt_type () == \   5	OPC_INTRPT_STRM && op_intrpt_strm () == RCV_IN_STRM)      static void xmt (void)   	{   	Packet * pkptr;   	   	FIN (xmt ());   !	pkptr = op_pk_get (SRC_IN_STRM);   ,	op_pk_nfd_set_int32 (pkptr, "dest_address",   '		(int)op_dist_outcome (address_dist));   "	op_pk_send (pkptr, XMT_OUT_STRM);   	FOUT;   	}       static void rcv (void)   	{   	Packet * pkptr;   	double ete_delay;   	   	FIN (rcv ());   !	pkptr = op_pk_get (RCV_IN_STRM);   	ete_delay = op_sim_time () -   "		op_pk_creation_time_get (pkptr);   $	op_stat_write (ete_gsh, ete_delay);   	op_pk_destroy (pkptr);   	FOUT;   	}                                       Z   ?          
   init   
       
      2address_dist = op_dist_load ("uniform_int", 0, 3);   8ete_gsh = op_stat_reg ("ETE Delay", OPC_STAT_INDEX_NONE,   	OPC_STAT_GLOBAL);   
                     
   ????   
          pr_state           ?             idle                                          ????             pr_state                     ?   ?      p   ?     ?             tr_0          ????          ????              ????             ????                       pr_transition              u   ?     %   ?  O   w  O   ?  (   ?             tr_3          
   default   
       ????              ????             ????                       pr_transition                 -        ?  @   B   ?   B     ?          
   tr_7   
       
   	SRC_ARRVL   
       
   xmt ()   
       
    ????   
          ????                       pr_transition                 ?        ?  C   ?   ?   ?   ?   ?          
   tr_8   
       
   	RCV_ARRVL   
       
   rcv()   
       
    ????   
          ????                       pr_transition         	              	ETE Delay          VCalculates ETE delay by subtracting packet creation time from current simulation time.????????????        ԲI?%??}      
link_delay                    