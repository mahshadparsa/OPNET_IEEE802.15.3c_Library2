MIL_3_Tfile_Hdr_ 145A 140A modeler 9 58451A63 58451A63 1 Parsa-Rasuli Parsa 0 0 none none 0 0 none C4FFE19B 299B 0 0 0 0 0 0 1e80 8                                                                                                                                                                                                                                                                                                                                                                                             ??g?      @   D   H      ?  '@  'D  'H  '?  '?  '?  '?  ?           	   begsim intrpt             ????      doc file            	nd_module      endsim intrpt             ????      failure intrpts            disabled      intrpt interval         ԲI?%??}????      priority              ????      recovery intrpts            disabled      subqueue                     count    ???   
   ????   
      list   	???   
          
      super priority             ????             int	\channel_busy;       double	\service_rate;       Evhandle	\BIFS_intr_handle;       Evhandle	\RIFS_intr_handle;       Evhandle	\Timeout_handle;       !Evhandle	\PacketSent_intr_handle;       Stathandle	\ete_gsh;       Objid	\node_id;       Distribution *	\address_dist;       Packet *	\Data_pkptr;          int BackoffExponent=3;   int RetryCount=0;   int BWRandom=0;   int max_RetryCount=3;   int BackoffWindow=7;       //double ete_delay;   //double creation_time;   //double current_time;   double pk_svc_time;       int    insert_ok;   int stream_index;   //Packet* Data_pkptr;   Packet* copyPkt;   OpT_Packet_Size  pk_len;          /* packet stream definitions */   #define StreamFromApp  1   #define StreamFromLink 0   #define StreamToLink   0           ?#define PowerUp         (op_intrpt_type() == OPC_INTRPT_BEGSIM)   b#define FrameArrival    (op_intrpt_type() == OPC_INTRPT_STRM && op_intrpt_strm() == StreamFromApp)   ]#define ACK 		    (op_intrpt_type() == OPC_INTRPT_STRM && op_intrpt_strm() == StreamFromLink)       #define Timeout_Code 0   a#define Timeout         (op_intrpt_type() == OPC_INTRPT_SELF && op_intrpt_code() == Timeout_Code)       #define BIFS_intr_Code 1   c#define BIFS_intr       (op_intrpt_type() == OPC_INTRPT_SELF && op_intrpt_code() == BIFS_intr_Code)       #define RIFS_intr_Code 2   c#define RIFS_intr       (op_intrpt_type() == OPC_INTRPT_SELF && op_intrpt_code() == RIFS_intr_Code)       #define PacketSent_intr_code 0   i#define PacketSent_intr (op_intrpt_type() == OPC_INTRPT_SELF && op_intrpt_code() == PacketSent_intr_code)       ;#define LinkFail 	    (op_intrpt_type() == OPC_INTRPT_FAIL)   <#define LinkUp		    (op_intrpt_type() == OPC_INTRPT_RECOVER)   ?#define QEmpty 		    (op_subq_stat(0, OPC_QSTAT_PKSIZE) == 0.0)      ;static void backoff (int BackoffWindow,int BackoffExponent)   	{		   /	FIN (backoff (BackoffWindow,BackoffExponent));   &	BackoffWindow=(2<<BackoffExponent)-1;   	FOUT;   	}   static void creat_BIFS_Intr()   	{   2	double BIFS_time=0.0000065;//0.00000065; // 6.5us   	FIN (creat_BIFS_Intr ());   S	BIFS_intr_handle=op_intrpt_schedule_self(op_sim_time()+ BIFS_time,BIFS_intr_Code);   	FOUT;   	}   static void creat_RIFS_Intr()   	{   0	double RIFS_time=0.000009;//0.0000009; // 6.5us   	FIN (creat_BIFS_Intr ());   S	RIFS_intr_handle=op_intrpt_schedule_self(op_sim_time()+ RIFS_time,RIFS_intr_Code);   	FOUT;   	}                                   
       ?            
   init   
                     
   	   //* Register handle for statistic collection. */   2address_dist = op_dist_load ("uniform_int", 0, 0);   Iete_gsh = op_stat_reg("ETE Delay", OPC_STAT_INDEX_NONE, OPC_STAT_GLOBAL);   BackoffExponent=3;   RetryCount=0;   max_RetryCount=3;       channel_busy=0;   service_rate=1610000;   
       
   ????   
          pr_state        ?            
   Idle   
                                       ????             pr_state        ?             
   
svc_packet   
                     
      D//op_subq_pk_insert (0, op_pk_get(op_intrpt_strm()), OPC_QPOS_TAIL);   +/* Get stream index number.              */   stream_index=op_intrpt_strm();       ./* Get pointer to packet from stream index. */   #Data_pkptr=op_pk_get(stream_index);       A//if (op_subq_pk_insert(0,Data_pkptr,OPC_QPOS_TAIL)!=OPC_QINS_OK)   	//{   '	/* the insertion failed (due to a 	 */   	/* full queue) 						 */   	//op_pk_destroy(Data_pkptr);   (	/* set flag indicating insertion fail*/   &	/* this flag is used to determine	 */   %	/* transition out of this state		 */   	//insert_ok= 0;   	//}   //else{   "	/* insertion was successful			 */   	insert_ok= 1;   	//}   
       
   ????   
          pr_state        :             
   backoff   
       
              (backoff (BackoffWindow,BackoffExponent);       creat_BIFS_Intr();   	   
                     
   ????   
          pr_state        :            
   check_channel   
       
   &   //if (channel_busy==1 )   //	{   //	creat_BIFS_Intr();   //	}   //else    	if(BWRandom!=0)   	{   	BWRandom=BWRandom-1;   	creat_BIFS_Intr();   	}   else   	{   3	/* get a handle on packet at head of subqueue 0 */   *	/* (this does not remove the packet)			*/   	   *	//Data_pkptr = op_pk_get (StreamFromApp);   2	//Data_pkptr=op_subq_pk_access (0,OPC_QPOS_HEAD);   	   #	copyPkt = op_pk_copy (Data_pkptr);       Y	//op_pk_nfd_set_int32 (Data_pkptr, "dest_address",(int)op_dist_outcome (address_dist));	   '	op_pk_send (Data_pkptr,StreamToLink );       .	/* determine the packets length (in bits)		*/   *	pk_len= op_pk_total_size_get(Data_pkptr);       0	/* determine the time required to complete 		*/   !	/* service of the packet						*/   "	pk_svc_time=pk_len/ service_rate;       .	/* schedule an interrupt for this process		*/   )	/* at the time where service ends.				*/   b	PacketSent_intr_handle=op_intrpt_schedule_self(op_sim_time ()+ pk_svc_time,PacketSent_intr_code);   \	Timeout_handle=op_intrpt_schedule_self(op_sim_time ()+ pk_svc_time+1,PacketSent_intr_code);   	   $	/* the channel is now busy.						*/   	channel_busy= 1;   	}   
                     
   ????   
          pr_state        ?  ?          
   wait_ACK   
                                       ????             pr_state          ?          
   PKTSending_success   
       
      RetryCount=0;   BackoffExponent=3;   
                     
   ????   
          pr_state        v  ?          
   PKTSending_faild   
       
      RetryCount=RetryCount+1;   "BackoffExponent=BackoffExponent+1;       creat_RIFS_Intr();   
                     
   ????   
          pr_state        *  ?          
   	wait_RIFS   
                                       ????             pr_state      	  :   ?          
   	wait_BIFS   
                                       ????             pr_state                                ?    m            
   tr_0   
       
   PowerUp   
       ????          
    ????   
          ????                       pr_transition              _   ?     ?   ?  ?   -          
   tr_1   
       
   FrameArrival   
       ????          
    ????   
          ????                       pr_transition              ?   ?     ?   +  ?   ?  ?   ?          
   tr_2   
       
   default   
       ????          
    ????   
          ????                       pr_transition              ?        ?     #             
   tr_3   
       
   	insert_ok   
       ????          
    ????   
          ????                       pr_transition              H  v     ?  ?   ?  ?          
   tr_11   
       
   ACK   
       ????          
    ????   
          ????                       pr_transition              >  H       r  o            
   tr_12   
       ????          ????          
    ????   
          ????                       pr_transition                ?     ?  ?  ?  ?  ]  ?          
   tr_13   
       
   )Timeout && (RetryCount+1)<=max_RetryCount   
       ????          
    ????   
          ????                       pr_transition              ?  w     ?  ?  Y  ?  Y  ?    ?          
   tr_15   
       
????   
       ????          
    ????   
          ????                       pr_transition              ?         ?  ?   ?  T             
   tr_17   
       
   	RIFS_intr   
       ????          
    ????   
          ????                       pr_transition              ]  ?     ?  ?  U  ?    ?          
   tr_18   
       
   'Timeout&& (RetryCount+1)>max_RetryCount   
       ????          
    ????   
          ????                       pr_transition            	  7   ^     :   9  :   y  6   ?          
   tr_19   
       ????          ????          
    ????   
          ????                       pr_transition         	     a   ?     <   ?  ;             
   tr_20   
       
   	BIFS_intr   
       ????          
    ????   
          ????                       pr_transition            	  $   ?     /       ?  (   ?          
   tr_21   
       ????          ????          
    ????   
          ????                       pr_transition         	     ?  ;     %   ?  ?    ?  p          
   tr_22   
       
   PacketSent_intr   
       ????          
    ????   
          ????                       pr_transition              Y   ?     v    H   ?  f   ?  ?            
   tr_24   
       
   default   
       ????          
    ????   
          ????                       pr_transition              n  P     {  }  `  g    Q  ?  |          
   tr_25   
       
   default   
       ????          
    ????   
          ????                       pr_transition         	   	     ?     #   ?  ?   {     a  0   ?          
   tr_26   
       
   default   
       ????          
    ????   
          ????                       pr_transition              z  ?     3  ?  R  ?  i  ?  4  ~          
   tr_27   
       
   default   
       ????          
    ????   
          ????                       pr_transition      
                 	ETE Delay        ????????   linear        ԲI?%??}                        