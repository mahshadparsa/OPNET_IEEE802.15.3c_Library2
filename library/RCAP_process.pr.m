MIL_3_Tfile_Hdr_ 145A 140A modeler 9 58452124 5950B6A1 D6 DESKTOP-FFL2R3R user 0 0 none none 0 0 none 96A69FD4 5C93 0 0 0 0 0 0 1e80 8                                                                                                                                                                                                                                                                                                                                                                                          ??g?      @   D   H      #s  Z8  Z<  Z@  Z{  Z?  Z?  Z?  #g           	   begsim intrpt             ????      doc file            	nd_module      endsim intrpt             ????      failure intrpts            disabled      intrpt interval         ԲI?%??}????      priority              ????      recovery intrpts            disabled      subqueue                     count    ???   
   ????   
      list   	???   
          
      super priority             ????          0   double	\service_rate;       Evhandle	\BIFS_intr_handle;       Evhandle	\RIFS_intr_handle;       Evhandle	\Timeout_handle;       Evhandle	\NextSD_intr_handle;       Stathandle	\ete_gsh;       Objid	\node_id;       Distribution *	\address_dist;       Packet *	\Data_pkptr;       int	\BackoffWindow;       int	\BackoffExponent;       int	\ack_correct;       Packet *	\ack_pk;       int	\RetryCount;       Packet *	\copyPkt;       Packet *	\extra_pkptr;       double	\tx_state;       "Evhandle	\invoke_ACAP_intr_handle;       Prohandle	\ACAP_proc;       int	\WaitForSD;       double	\pk_svc_time;       Packet *	\Data_pkptrQ;       Evhandle	\SIFS_intr_handle;       double	\NSendPKT;          //int RetryCount=0;   int BWRandom=0;   int max_RetryCount=3;       <   /* packet stream definitions */   //#define StreamFromSrcCTA  2   #define StreamFromSrcCAP  1   #define StreamFromLink 0   #define StreamToLink   0           ?#define PowerUp         (op_intrpt_type() == OPC_INTRPT_BEGSIM)   e#define CAPFrameArrival (op_intrpt_type() == OPC_INTRPT_STRM && op_intrpt_strm() == StreamFromSrcCAP)   g//#define CTAFrameArrival (op_intrpt_type() == OPC_INTRPT_STRM && op_intrpt_strm() == StreamFromSrcCTA)   g#define StrFromLinkArrival  (op_intrpt_type() == OPC_INTRPT_STRM && op_intrpt_strm() == StreamFromLink)       #define Timeout_Code 1   a#define Timeout         (op_intrpt_type() == OPC_INTRPT_SELF && op_intrpt_code() == Timeout_Code)       #define BIFS_intr_Code 2   c#define BIFS_intr       (op_intrpt_type() == OPC_INTRPT_SELF && op_intrpt_code() == BIFS_intr_Code)       #define RIFS_intr_Code 3   c#define RIFS_intr       (op_intrpt_type() == OPC_INTRPT_SELF && op_intrpt_code() == RIFS_intr_Code)       #define NextSD_intr_code 4   a#define NextSD_intr (op_intrpt_type() == OPC_INTRPT_SELF && op_intrpt_code() == NextSD_intr_code)       #define invoke_ACAP_intr_code 5   k#define invoke_ACAP_intr (op_intrpt_type() == OPC_INTRPT_SELF && op_intrpt_code() == invoke_ACAP_intr_code)       #define SIFS_intr_Code 6   c#define SIFS_intr       (op_intrpt_type() == OPC_INTRPT_SELF && op_intrpt_code() == SIFS_intr_Code)           #define ch_busy 0   5#define channel_busy (op_stat_local_read(ch_busy)==1)           ;#define LinkFail 	    (op_intrpt_type() == OPC_INTRPT_FAIL)   <#define LinkUp		    (op_intrpt_type() == OPC_INTRPT_RECOVER)   ?#define QEmpty 		    (op_subq_stat(0, OPC_QSTAT_PKSIZE) == 0.0)   ?#define ACAP_QEmpty 	(op_subq_stat(1, OPC_QSTAT_PKSIZE) == 0.0)   -int en_beacon,en_RCAP,en_ACAP,en_MCTA,en_CTA;   double beacon_time;   double RCAP_time;   double ACAP_time;   double MCTA_time;   double CTA_time;   double SD_time;   double beacon_start_time;   double RCAP_start_time;   double ACAP_start_time;   double MCTA_start_time;   double CTA_start_time;       double SIFS_time;   double ACK_svc_time;   int req_sended;       //int BackoffExponent;   //int BackoffWindow;   //int channel_busy;   //int ack_correct;   ?   #include<stdio.h>   X/**************************************************************************************/   X/**************************************************************************************/   static void backoff ()   	{		   	FIN (backoff ());   0	//printf("BackoffExponent=%d",BackoffExponent);   W	BackoffWindow=op_dist_outcome (op_dist_load ("uniform_int",0,(2<<BackoffExponent)-1));   ,	//printf("backoffWindow=%d",BackoffWindow);   	   	FOUT;   	}   X/**************************************************************************************/   X/**************************************************************************************/   static void creat_BIFS_Intr()   	{   *	double BIFS_time=0.0000065;//0.00000065;    	   	FIN (creat_BIFS_Intr ());   S	if (RCAP_time-op_sim_time ()+RCAP_start_time<pk_svc_time+2*SIFS_time+ACK_svc_time)   		{   ?		//printf("CAP_start_time=%2g,RCAP_start_time+SD_time+ BIFS_time,BIFS_intr_Code=%2g\n",RCAP_start_time,RCAP_start_time+SD_time+ BIFS_time,BIFS_intr_Code);   ^		BIFS_intr_handle=op_intrpt_schedule_self(RCAP_start_time+SD_time+ BIFS_time,BIFS_intr_Code);   		}   	else   T		BIFS_intr_handle=op_intrpt_schedule_self(op_sim_time()+ BIFS_time,BIFS_intr_Code);   ,	//printf("backoffWindow=%d",BackoffWindow);   *	//printf("channel_busy=%d",channel_busy);   	FOUT;   	}       X/**************************************************************************************/   X/**************************************************************************************/   #static void CheckTime_ForSend_pkt()   	{   	    	FIN (CheckTime_ForSend_pkt ());   		   S	if (RCAP_time-op_sim_time ()+RCAP_start_time<pk_svc_time+2*SIFS_time+ACK_svc_time)   		{   X 		NextSD_intr_handle=op_intrpt_schedule_self(RCAP_start_time+SD_time,NextSD_intr_code);   		WaitForSD=1;   		}   	else   		WaitForSD=0;   		   	FOUT;   	}   X/**************************************************************************************/   X/**************************************************************************************/   static void creat_RIFS_Intr()   	{   (	double RIFS_time=0.000009;//0.0000009;    	FIN (creat_BIFS_Intr ());   S	RIFS_intr_handle=op_intrpt_schedule_self(op_sim_time()+ RIFS_time,RIFS_intr_Code);   	FOUT;   	}   X/**************************************************************************************/   X/**************************************************************************************/   static void creat_SIFS_Intr()   	{   	FIN(creat_SIFS_Intr());   T	SIFS_intr_handle=op_intrpt_schedule_self(op_sim_time()+2*SIFS_time,SIFS_intr_Code);   	FOUT;   	}   X/**************************************************************************************/   X/**************************************************************************************/   static void Qframe()   	{    	FIN (Qframe ());   C	op_subq_pk_insert (0, op_pk_get(op_intrpt_strm()), OPC_QPOS_TAIL);   	FOUT;   	}   X/**************************************************************************************/   X/**************************************************************************************/   static void check_channel()   	{		   		FIN (check_channel ());   						   		if (channel_busy==1 )   		{   			creat_BIFS_Intr();			   		}   		else if(BackoffWindow>0)   		{   !			BackoffWindow=BackoffWindow-1;   			creat_BIFS_Intr();   		}       		FOUT;       	}       X/**************************************************************************************/   X/**************************************************************************************/   static void check_ack()   	{   	int dest_ID;   	int frame_type;   	double throughput;   	FIN (check_ack());   $	ack_pk=op_pk_get(op_intrpt_strm());   	op_pk_total_size_get(ack_pk);   r	op_pk_nfd_get_int32 (ack_pk, "frame_type",&frame_type ); //acording to the standard, type value of beacon is zero   2	op_pk_nfd_get_int32 (ack_pk, "dest_ID",&dest_ID);   	   '	if (frame_type==1 && dest_ID==node_id)   		{   		ack_correct=1;   		creat_SIFS_Intr();   		NSendPKT=NSendPKT+1;   2		//printf("NSendPKT[%d]=%f \n",node_id,NSendPKT);   		if (op_sim_time()>300)   			{   !				throughput=NSendPKT*1000/300;   K				printf("throughput[%d]=%f,NSendPKT=%f \n",node_id,throughput,NSendPKT);   			}   		}   	else   		{   		ack_correct=0;   [		//printf("\n RCAP_Proc: dest_ID=%d,node_id=%d,frame_type=%d",dest_ID,node_id,frame_type);   J		//printf("pkID=%d,op_sim_time()=%2g\n",op_pk_id (ack_pk),op_sim_time());   		}   	op_pk_destroy(ack_pk);   	FOUT;   	   	}   static void invoke_ACAP()   	{   	FIN (invoke_ACAP());   	en_RCAP=0;   	en_ACAP=1;   	req_sended=0;    	ACAP_start_time=op_sim_time ();   (	//printf("ACAP_QEmpty=%d",ACAP_QEmpty);   $	op_pro_invoke (ACAP_proc, OPC_NIL);   <	op_intrpt_port_register (OPC_PORT_TYPE_STRM, 0, ACAP_proc);   <	op_intrpt_port_register (OPC_PORT_TYPE_STRM, 2, ACAP_proc);   `	invoke_ACAP_intr_handle=op_intrpt_schedule_self (op_sim_time()+SD_time, invoke_ACAP_intr_code);   	FOUT;   	}   static void send_packet()   	{   	OpT_Packet_Size  pk_len;   	FIN (send_packet());	   '	pk_len= op_pk_total_size_get(copyPkt);   E	/* determine the time required to complete 	service of the packet	*/   "	pk_svc_time=pk_len/ service_rate;    	ACK_svc_time=496/ service_rate;       $	op_pk_send (copyPkt,StreamToLink );       k	Timeout_handle=op_intrpt_schedule_self(op_sim_time ()+ pk_svc_time+2*SIFS_time+ACK_svc_time,Timeout_Code);   	FOUT;   	}                                            ?  J          
   Idle   
       
      //* Register handle for statistic collection. */   Iete_gsh = op_stat_reg("ETE Delay", OPC_STAT_INDEX_NONE, OPC_STAT_GLOBAL);   BackoffExponent=3;   RetryCount=0;       
                         ????             pr_state        ?   Z          
   
svc_packet   
                     
      ./* Get pointer to packet from stream index. */   'Data_pkptr=op_pk_get(op_intrpt_strm());   ;pk_svc_time=op_pk_total_size_get (Data_pkptr)/service_rate;   "copyPkt = op_pk_copy (Data_pkptr);   node_id=op_id_self();   !//printf("node_id=%d",node_id);		   3op_pk_nfd_set_int32 (Data_pkptr, "src_ID",node_id);   .op_pk_nfd_set_int32 (Data_pkptr, "dest_ID",0);   0op_pk_nfd_set_int32 (copyPkt, "src_ID",node_id);   +op_pk_nfd_set_int32 (copyPkt, "dest_ID",0);   bop_pk_nfd_set_int32 (copyPkt, "frame_type", 4);//acording to the standard, type value of data is 4           
       
   ????   
          pr_state        ?   Z          
   backoff   
       
      backoff ();   creat_BIFS_Intr();   
                     
   ????   
          pr_state        ?  J          
   check_channel   
       
      check_channel();   
                     
   ????   
          pr_state        ?  v          
   wait_ACK   
                                       ????             pr_state        J  v          
   PKTSending_success   
       
      RetryCount=0;   BackoffExponent=3;   
                     
   ????   
          pr_state        f  v          
   PKTSending_faild   
       
      RetryCount=RetryCount+1;   "BackoffExponent=BackoffExponent+1;   "copyPkt = op_pk_copy (Data_pkptr);   0op_pk_nfd_set_int32 (copyPkt, "src_ID",node_id);   +op_pk_nfd_set_int32 (copyPkt, "dest_ID",0);   bop_pk_nfd_set_int32 (copyPkt, "frame_type", 4);//acording to the standard, type value of data is 4       creat_RIFS_Intr();   
                     
   ????   
          pr_state          v          
   	wait_RIFS   
                                       ????             pr_state      	  ?   ?          
   	wait_BIFS   
                                       ????             pr_state      
  ?  *          
   	check_ack   
                     
      check_ack();   
       
   ????   
          pr_state           ?          
   del_extra_rcv_packet   
       
      (extra_pkptr=op_pk_get(op_intrpt_strm());   M//printf("extra packet witd id packet =%d deleted\n",op_pk_id (extra_pkptr));   op_pk_destroy(extra_pkptr);           
                     
   ????   
          pr_state        ?  ?          
   del_extra_rcv_packet1   
                     
      (extra_pkptr=op_pk_get(op_intrpt_strm());   op_pk_destroy(extra_pkptr);   
       
   ????   
          pr_state        ?   Z          
   svc_Q   
       
      1Data_pkptrQ=op_subq_pk_remove (0, OPC_QPOS_HEAD);   #copyPkt = op_pk_copy (Data_pkptrQ);   <pk_svc_time=op_pk_total_size_get (Data_pkptrQ)/service_rate;   node_id=op_id_self();   !//printf("node_id=%d",node_id);		   0op_pk_nfd_set_int32 (copyPkt, "src_ID",node_id);   +op_pk_nfd_set_int32 (copyPkt, "dest_ID",0);   bop_pk_nfd_set_int32 (copyPkt, "frame_type", 4);//acording to the standard, type value of data is 4   
                     
   ????   
          pr_state        ?  ?          
   
check_time   
       
      CheckTime_ForSend_pkt();   
                     
   ????   
          pr_state        ?  ?          
   WaitFor_nextSD   
                                       ????             pr_state        v  ?          
   send_packet   
       
      send_packet();   
                     
   ????   
          pr_state        ?  J          
   init   
       
      max_RetryCount=3;   service_rate=900000000;   BackoffExponent=3;   RetryCount=0;   NSendPKT=0;       
                     
   ????   
          pr_state        J  *          
   wait_2*SIFS   
                                       ????             pr_state        ?  ?          
   del_extra_rcv_packet3   
       
      (extra_pkptr=op_pk_get(op_intrpt_strm());   op_pk_destroy(extra_pkptr);   
                     
   ????   
          pr_state        ?  ?          
   del_extra_rcv_packet4   
                                   
   ????   
          pr_state        ?  ?          
   del_extra_rcv_packet5   
       
      (extra_pkptr=op_pk_get(op_intrpt_strm());   M//printf("extra packet witd id packet =%d deleted\n",op_pk_id (extra_pkptr));   op_pk_destroy(extra_pkptr);           
                     
   ????   
          pr_state         .           3   ?       3  ?   i          
   tr_1   
       
   &CAPFrameArrival && en_RCAP==1&& QEmpty   
       ????          
    ????   
          ????                       pr_transition              ?  ?     I  b  ?  Y          
   tr_12   
       
   QEmpty   
       ????          
    ????   
          ????                       pr_transition              ?  ?       u  ?  u  M  v          
   tr_13   
       
   )Timeout && (RetryCount+1)<=max_RetryCount   
       ????          
    ????   
          ????                       pr_transition              ?  g     ?  v  I  u  I  u     u          
   tr_15   
       
????   
       ????          
    ????   
          ????                       pr_transition              ?  ?     ?  ?  ?  ?  Q  ?          
   tr_18   
       
   'Timeout&& (RetryCount+1)>max_RetryCount   
       ????          
    ????   
          ????                       pr_transition            	  ?   ?     ?   u  ?   ?  ?   ?          
   tr_19   
       ????          ????          
    ????   
          ????                       pr_transition         	     ?       ?   ?  ?  <          
   tr_20   
       
   1BIFS_intr && (BackoffWindow>0 || channel_busy==1)   
       ????          
    ????   
          ????                       pr_transition            	  ?  /     ?  A  ?    ?   ?          
   tr_21   
       ????          ????          
    ????   
          ????                       pr_transition              ?       ?  D  ?  1  ?    ?  H          
   tr_24   
       
   default   
       ????          
    ????   
          ????                       pr_transition              ?  @     ?  m  ?  W  ?  A  ?  l          
   tr_25   
       
   default   
       ????          
    ????   
          ????                       pr_transition              j  ?     #  ?  B  ?  Y  w  $  n          
   tr_27   
       
   default   
       ????          
    ????   
          ????                       pr_transition      $      
  ?  ?       x  ?            
   tr_36   
       
   StrFromLinkArrival   
       ????          
    ????   
          ????                       pr_transition      %   
     9  ?       &     ?  ?  z          
   tr_37   
       
   default   
       ????          
    ????   
          ????                       pr_transition      &   	     ?   ?     ?   ?  ?   ?     ?          
   tr_38   
       
   StrFromLinkArrival   
       ????          
    ????   
          ????                       pr_transition      (      	  ?   ?     
   ?  ?   ?          
   tr_40   
       ????          ????          
    ????   
          ????                       pr_transition      )        ?  ?       L    ~    ?          
   tr_41   
       
   StrFromLinkArrival   
       ????          
    ????   
          ????                       pr_transition      *        ?  ?     ?  ?  ?  ?  ?  Z          
   tr_42   
       ????          ????          
    ????   
          ????                       pr_transition      ,        u  ]       d  B  J  I  e    n          
   tr_44   
       
   CAPFrameArrival   
       
   	Qframe();   
       
    ????   
          ????                       pr_transition      -   	   	  ?   ?     ?   ?  ?   ?  ?    ?   ?          
   tr_45   
       
   CAPFrameArrival   
       
   	Qframe();   
       
    ????   
          ????                       pr_transition      .        ?  5       m  ?  Z  ?  :    `          
   tr_46   
       
   CAPFrameArrival   
       
   	Qframe();   
       
    ????   
          ????                       pr_transition      3           3     ?   M  ?     ?   I          
   tr_51   
       ????          ????          
    ????   
          ????                       pr_transition      4        e         >  1    =  Q    N          
   tr_52   
       
   CAPFrameArrival && en_RCAP==0   
       
   	Qframe();   
       
    ????   
          ????                       pr_transition      6        ?   Z        Y  ?   [          
   tr_54   
       ????          ????          
    ????   
          ????                       pr_transition      7   	     
  V     ?   ?  c  !  ?  r          
   tr_55   
       
   0BIFS_intr && BackoffWindow==0 && channel_busy==0   
       ????          
    ????   
          ????                       pr_transition      9   	   	  y   ?     ?   ?  m   ?  ?   ?  ?   ?          
   tr_57   
       
   default   
       ????          
    ????   
          ????                       pr_transition      @        Z  +       f  ?  +  ?   ?  ?   T          
   tr_64   
       
   	RIFS_intr   
       ????          
    ????   
          ????                       pr_transition      D        ?  ?     ?  ?  w  ?          
   tr_68   
       
   WaitForSD==0   
       ????          
    ????   
          ????                       pr_transition      E        ?  ?     ?  ?  ?  ?          
   tr_69   
       
   WaitForSD==1   
       ????          
    ????   
          ????                       pr_transition      F        ?       ?  ?  ?  ?          
   tr_70   
       
   NextSD_intr   
       ????          
    ????   
          ????                       pr_transition      G        O       s  ?  +  &    `          
   tr_71   
       ????          ????          
    ????   
          ????                       pr_transition      H        ?  A     ?    ?  3  ?  3  ?            
   tr_72   
       
   default   
       ????          
    ????   
          ????                       pr_transition      L        ?  P     ?  J  ?  J          
   tr_76   
       
   default   
       ????          
    ????   
          ????                       pr_transition      M          w     A  b   ?  p  }   b          
   tr_77   
       
   !QEmpty   
       ????          
    ????   
          ????                       pr_transition      O        ?   ?     ?  8  ?   ?  ?   d          
   tr_79   
       
   en_RCAP==1 && !QEmpty   
       ????          
    ????   
          ????                       pr_transition      Q        ?   ?     ?  <  ?   j          
   tr_81   
       
   en_RCAP==1 && !QEmpty   
       ????          
    ????   
          ????                       pr_transition      T        %       D      ?  7  }          
   tr_84   
       
   	SIFS_intr   
       ????          
    ????   
          ????                       pr_transition      U   
     ?  D     ?  /  ?  4  _  )          
   tr_85   
       
   ack_correct==1   
       ????          
    ????   
          ????                       pr_transition      V        w  ?     K    T  ?  y    Y            
   tr_86   
       
   default   
       ????          
    ????   
          ????                       pr_transition      W           h     >  <  %  b    0  =  -          
   tr_87   
       
   CAPFrameArrival   
       
   	Qframe();   
       
    ????   
          ????                       pr_transition      X        ?  f     W  2  w  R  w  R  ?  ?          
   tr_88   
       
   StrFromLinkArrival   
       ????          
    ????   
          ????                       pr_transition      Y        f  ?     u  ?  W  ?  O  8          
   tr_89   
       ????          ????          
    ????   
          ????                       pr_transition      Z        ?  ?       ?  ?  ?  ?  ?          
   tr_90   
       
   StrFromLinkArrival   
       ????          
    ????   
          ????                       pr_transition      [        ?  ?     ?  ?    ?          
   tr_91   
       ????          ????          
    ????   
          ????                       pr_transition      \        G  ?     ?  ?  &  ?  ?  ?          
   tr_92   
       
   StrFromLinkArrival   
       ????          
    ????   
          ????                       pr_transition      ]        k  ?     ?  ?  ?  ?  ?  ?          
   tr_93   
       ????          ????          
    ????   
          ????                       pr_transition      ^        e       ?  ?  >     &  .  ?            
   tr_94   
       
   CAPFrameArrival   
       
   	Qframe();   
       
    ????   
          ????                       pr_transition         _              	ETE Delay        ????????   linear        ԲI?%??}                        