MIL_3_Tfile_Hdr_ 145A 140A modeler 9 58A47804 59100F59 5F DESKTOP-FFL2R3R user 0 0 none none 0 0 none FFAF67DE 4B77 0 0 0 0 0 0 1e80 8                                                                                                                                                                                                                                                                                                                                                                                          ??g?      @   D   H       F  IS  IW  I[  I_  Ik  Io  Is   :           	   begsim intrpt             ????      doc file            	nd_module      endsim intrpt             ????      failure intrpts            disabled      intrpt interval         ԲI?%??}????      priority              ????      recovery intrpts            disabled      subqueue                     count    ???   
   ????   
      list   	???   
          
      super priority             ????          &   double	\service_rate;        Evhandle	\ACAP_BIFS_intr_handle;        Evhandle	\ACAP_RIFS_intr_handle;       Evhandle	\ACAP_Timeout_handle;       &Evhandle	\ACAP_PacketSent_intr_handle;       Stathandle	\ete_gsh;       Objid	\node_id;       Evhandle	\Req_intr_handle;       int	\ACAP_BackoffWindow;       int	\ACAP_BackoffExponent;       int	\ACAP_ack_correct;       Packet *	\ACAP_ack_pk;       int	\ACAP_RetryCount;       Packet *	\ACAP_copyPkt;       Packet *	\ACAP_extra_pkptr;       double	\ACAP_tx_state;       Packet *	\CTA_req_pk;       Evhandle	\CheckR_intr_handle;       int	\req_sended;          //int RetryCount=0;   int BWRandom=0;   int max_RetryCount=3;       =   /* packet stream definitions */   #define StreamFromSrcCTA  2   #define StreamFromSrcCAP  1   #define StreamFromLink 0   #define StreamToLink   0           ?#define PowerUp         (op_intrpt_type() == OPC_INTRPT_BEGSIM)   e#define CAPFrameArrival (op_intrpt_type() == OPC_INTRPT_STRM && op_intrpt_strm() == StreamFromSrcCAP)   e#define CTAFrameArrival (op_intrpt_type() == OPC_INTRPT_STRM && op_intrpt_strm() == StreamFromSrcCTA)   g#define StrFromLinkArrival  (op_intrpt_type() == OPC_INTRPT_STRM && op_intrpt_strm() == StreamFromLink)       #define ACAP_Timeout_Code 0   k#define ACAP_Timeout         (op_intrpt_type() == OPC_INTRPT_SELF && op_intrpt_code() == ACAP_Timeout_Code)       #define ACAP_BIFS_intr_Code 4   m#define ACAP_BIFS_intr       (op_intrpt_type() == OPC_INTRPT_SELF && op_intrpt_code() == ACAP_BIFS_intr_Code)       #define ACAP_RIFS_intr_Code 2   m#define ACAP_RIFS_intr       (op_intrpt_type() == OPC_INTRPT_SELF && op_intrpt_code() == ACAP_RIFS_intr_Code)       ##define ACAP_PacketSent_intr_code 3   s#define ACAP_PacketSent_intr (op_intrpt_type() == OPC_INTRPT_SELF && op_intrpt_code() == ACAP_PacketSent_intr_code)       #define Req_intr_Code 5   a#define Req_intr       (op_intrpt_type() == OPC_INTRPT_SELF && op_intrpt_code() == Req_intr_Code)       #define CheckR_intr_Code 6   g#define CheckR_intr       (op_intrpt_type() == OPC_INTRPT_SELF && op_intrpt_code() == CheckR_intr_Code)       #define ch_busy 0   5#define channel_busy (op_stat_local_read(ch_busy)==1)           ;#define LinkFail 	    (op_intrpt_type() == OPC_INTRPT_FAIL)   <#define LinkUp		    (op_intrpt_type() == OPC_INTRPT_RECOVER)   ?#define ACAP_QEmpty 	(op_subq_stat(1, OPC_QSTAT_PKSIZE) == 0.0)   %int en_beacon,en_ACAP,en_MCTA,en_CTA;   double beacon_time;   double RCAP_time;   double ACAP_time;   double MCTA_time;   double CTA_time;   double SD_time;   double beacon_start_time;   double ACAP_start_time;   double MCTA_start_time;   double CTA_start_time;       double SIFS_time;       double ACAP_pk_svc_time;   double ACAP_ACK_svc_time;       int associated;   //int req_sended;       //int BackoffExponent;   //int BackoffWindow;   //int channel_busy;   //int ack_correct;   {   #include<stdio.h>   X/**************************************************************************************/   X/**************************************************************************************/   static void backoff ()   	{		   	FIN (backoff ());   0	//printf("BackoffExponent=%d",BackoffExponent);   a	ACAP_BackoffWindow=op_dist_outcome (op_dist_load ("uniform_int",0,(2<<ACAP_BackoffExponent)-1));   6	printf("ACAP_BackoffWindow=%d\n",ACAP_BackoffWindow);   	   	FOUT;   	}   X/**************************************************************************************/   X/**************************************************************************************/   static void creat_BIFS_Intr()   	{   *	double BIFS_time=0.0000065;//0.00000065;    	   	FIN (creat_BIFS_Intr ());   ]	if (ACAP_time-op_sim_time ()+ACAP_start_time<ACAP_pk_svc_time+2*SIFS_time+ACAP_ACK_svc_time)   		{   }		//printf("CAP_start_time=%2g,ACAP_start_time+SD_time+ BIFS_time=%2g\n",ACAP_start_time,ACAP_start_time+SD_time+ BIFS_time);   h		ACAP_BIFS_intr_handle=op_intrpt_schedule_self(ACAP_start_time+SD_time+ BIFS_time,ACAP_BIFS_intr_Code);   		}   	else   ^		ACAP_BIFS_intr_handle=op_intrpt_schedule_self(op_sim_time()+ BIFS_time,ACAP_BIFS_intr_Code);   ,	//printf("backoffWindow=%d",BackoffWindow);   *	//printf("channel_busy=%d",channel_busy);   	FOUT;   	}   X/**************************************************************************************/   X/**************************************************************************************/   !static void creat_Sent_pkt_Intr()   	{   	OpT_Packet_Size  pk_len=720;   	   	FIN (creat_Sent_pkt_Intr ());   	   E	/* determine the time required to complete 	service of the packet	*/   	   ,	pk_len= op_pk_total_size_get(ACAP_copyPkt);   '	ACAP_pk_svc_time=pk_len/ service_rate;   %	ACAP_ACK_svc_time=496/ service_rate;   ?	//printf("op_sim_time ()=%2g, ACAP_pk_svc_time=%2g,2*SIFS_time=%2g,ACAP_ACK_svc_time=%2g,ACAP_start_time=%2g",op_sim_time (), ACAP_pk_svc_time,2*SIFS_time,ACAP_ACK_svc_time,ACAP_start_time);	   ]	if (ACAP_time-op_sim_time ()+ACAP_start_time<ACAP_pk_svc_time+2*SIFS_time+ACAP_ACK_svc_time)   		{   		   m		//PacketSent_intr_handle=op_intrpt_schedule_self(CAP_start_time+SD_time,PacketSent_intr_code);//pk_svc_time   ?		ACAP_Timeout_handle=op_intrpt_schedule_self(ACAP_start_time+SD_time+ ACAP_pk_svc_time+2*SIFS_time+ACAP_ACK_svc_time,ACAP_Timeout_Code); 		   		}   		else   		{   e		//PacketSent_intr_handle=op_intrpt_schedule_self(op_sim_time (),PacketSent_intr_code);//pk_svc_time   ?		ACAP_Timeout_handle=op_intrpt_schedule_self(op_sim_time ()+ ACAP_pk_svc_time+2*SIFS_time+ACAP_ACK_svc_time,ACAP_Timeout_Code);   		   		}   	FOUT;   	}   X/**************************************************************************************/   X/**************************************************************************************/   static void creat_RIFS_Intr()   	{   (	double RIFS_time=0.000009;//0.0000009;    	FIN (creat_BIFS_Intr ());   ]	ACAP_RIFS_intr_handle=op_intrpt_schedule_self(op_sim_time()+ RIFS_time,ACAP_RIFS_intr_Code);   	FOUT;   	}   X/**************************************************************************************/   X/**************************************************************************************/   static void Qframe()   	{    	FIN (Qframe ());   C	op_subq_pk_insert (1, op_pk_get(op_intrpt_strm()), OPC_QPOS_TAIL);   	FOUT;   	}   X/**************************************************************************************/   X/**************************************************************************************/   static void check_channel()   	{		   		FIN (check_channel ());   						   		if (channel_busy==1 )   		{   			creat_BIFS_Intr();			   		}   		else if(ACAP_BackoffWindow>0)   		{   +			ACAP_BackoffWindow=ACAP_BackoffWindow-1;   			creat_BIFS_Intr();   		}       		FOUT;       	}       X/**************************************************************************************/   X/**************************************************************************************/   static void check_ack()   	{   	int dest_ID;   	int frame_type;   	FIN (check_ack());   )	ACAP_ack_pk=op_pk_get(op_intrpt_strm());   w	op_pk_nfd_get_int32 (ACAP_ack_pk, "frame_type",&frame_type ); //acording to the standard, type value of beacon is zero   7	op_pk_nfd_get_int32 (ACAP_ack_pk, "dest_ID",&dest_ID);   '	if (frame_type==1 && dest_ID==node_id)   		{   		ACAP_ack_correct=1;   		req_sended=1;   		}   	else   		{   		ACAP_ack_correct=0;   @		printf("\n ACAP_Proc: dest_ID=%d,node_id=%d",dest_ID,node_id);   -		printf("pkID=%d\n",op_pk_id (ACAP_ack_pk));   		}   	op_pk_destroy(ACAP_ack_pk);   	FOUT;   	   	}                                                     ?  ?          
   Idle   
       
      //* Register handle for statistic collection. */   Iete_gsh = op_stat_reg("ETE Delay", OPC_STAT_INDEX_NONE, OPC_STAT_GLOBAL);   ACAP_BackoffExponent=3;   ACAP_RetryCount=0;   max_RetryCount=3;   service_rate=900000000;   
                         ????             pr_state        ?   ?          
   
svc_packet   
                     
      ./* Get pointer to packet from stream index. */   .CTA_req_pk = op_pk_create_fmt ("CTA_req_pkt");   (op_pk_total_size_set (CTA_req_pk, 720);    'ACAP_copyPkt = op_pk_copy (CTA_req_pk);   node_id=op_id_self();   3op_pk_nfd_set_int32 (CTA_req_pk, "src_ID",node_id);   .op_pk_nfd_set_int32 (CTA_req_pk, "dest_ID",0);   top_pk_nfd_set_int32 (CTA_req_pk, "frame_type", 3);//acording to the standard, type value of association request is 3   Aop_pk_nfd_set_int32 (CTA_req_pk, "CTRqB_Target_IDlist", node_id);       5op_pk_nfd_set_int32 (ACAP_copyPkt, "src_ID",node_id);   0op_pk_nfd_set_int32 (ACAP_copyPkt, "dest_ID",0);   vop_pk_nfd_set_int32 (ACAP_copyPkt, "frame_type", 3);//acording to the standard, type value of association request is 3   Cop_pk_nfd_set_int32 (ACAP_copyPkt, "CTRqB_Target_IDlist", node_id);           
       
   ????   
          pr_state        *   ?          
   backoff   
       
      backoff ();   creat_BIFS_Intr();   
                     
   ????   
          pr_state        *  ?          
   check_channel   
       
      check_channel();   
                     
   ????   
          pr_state        ?  v          
   wait_ACK   
                                       ????             pr_state         ?  v          
   PKTSending_success   
       
      ACAP_RetryCount=0;   ACAP_BackoffExponent=3;   
                     
   ????   
          pr_state        *  v          
   PKTSending_faild   
       
      "ACAP_RetryCount=ACAP_RetryCount+1;   ,ACAP_BackoffExponent=ACAP_BackoffExponent+1;   'ACAP_copyPkt = op_pk_copy (CTA_req_pk);   5op_pk_nfd_set_int32 (ACAP_copyPkt, "src_ID",node_id);   0op_pk_nfd_set_int32 (ACAP_copyPkt, "dest_ID",0);   vop_pk_nfd_set_int32 (ACAP_copyPkt, "frame_type", 3);//acording to the standard, type value of association request is 3   Cop_pk_nfd_set_int32 (ACAP_copyPkt, "CTRqB_Target_IDlist", node_id);   creat_RIFS_Intr();   
                     
   ????   
          pr_state        ?  v          
   	wait_RIFS   
                                       ????             pr_state        *            
   	wait_BIFS   
                                       ????             pr_state      	  ?  *          
   	check_ack   
                     
      check_ack();   
       
   ????   
          pr_state      
  ?            
   del_extra_rcv_packet   
       
      -ACAP_extra_pkptr=op_pk_get(op_intrpt_strm());   P//printf("extra packet witd id packet =%d deleted",op_pk_id (ACAP_extra_pkptr));    op_pk_destroy(ACAP_extra_pkptr);   
                     
   ????   
          pr_state        ?  ?          
   del_extra_rcv_packet1   
                     
      -ACAP_extra_pkptr=op_pk_get(op_intrpt_strm());       
       
   ????   
          pr_state        v  ?          
   send_pkt   
       
      creat_Sent_pkt_Intr();   (op_pk_send (ACAP_copyPkt,StreamToLink );   
                     
   ????   
          pr_state        J  ?          
   init   
       
      /if (req_sended==0 && en_ACAP==1&& !ACAP_QEmpty)   R	Req_intr_handle=op_intrpt_schedule_self( op_sim_time ()+.0000030,Req_intr_Code);	   else   Z    CheckR_intr_handle=op_intrpt_schedule_self( op_sim_time ()+.0000065,CheckR_intr_Code);   		   
                     
   ????   
          pr_state        ?  ?          
   del_extra_rcv_packet2   
       
      -ACAP_extra_pkptr=op_pk_get(op_intrpt_strm());   
                     
   ????   
          pr_state                      "  ?      ?  b  ?  ?          
   tr_12   
       
   req_sended==1   
       ????          
    ????   
          ????                       pr_transition              p  ?     ?  u  ?  u    v          
   tr_13   
       
   3ACAP_Timeout && (ACAP_RetryCount+1)<=max_RetryCount   
       ????          
    ????   
          ????                       pr_transition              )  v     E  v    u    u  ?  u          
   tr_15   
       
????   
       ????          
    ????   
          ????                       pr_transition              ?  ?     ?  r    n  D   ?          
   tr_17   
       
   ACAP_RIFS_intr   
       ????          
    ????   
          ????                       pr_transition              5  s     ?  ?  ^  |   ?  ?          
   tr_18   
       
   1ACAP_Timeout&& (ACAP_RetryCount+1)>max_RetryCount   
       ????          
    ????   
          ????                       pr_transition              *   ?     *   ?  *   ?  &            
   tr_19   
       ????          ????          
    ????   
          ????                       pr_transition              ?  {     ,    +  ?          
   tr_20   
       
   ;ACAP_BIFS_intr && (ACAP_BackoffWindow>0 || channel_busy==1)   
       ????          
    ????   
          ????                       pr_transition                ?       ?    V              
   tr_21   
       ????          ????          
    ????   
          ????                       pr_transition      	          ?  ]     ?  ?  ?  m  ?  Q  ?  ?          
   tr_24   
       
   default   
       ????          
    ????   
          ????                       pr_transition      
        ?  B     ?  m  ?  W  ?  A  ?  l          
   tr_25   
       
   default   
       ????          
    ????   
          ????                       pr_transition              ?   ?         ?   ?     ?      ?          
   tr_26   
       
   default   
       ????          
    ????   
          ????                       pr_transition              (  ?     ?  ?    ?    w  ?  n          
   tr_27   
       
   default   
       ????          
    ????   
          ????                       pr_transition         	     '  ?     ?  5   ?  ?          
   tr_35   
       
   ACAP_ack_correct==1   
       ????          
    ????   
          ????                       pr_transition            	  ?  ?     ?  x  ?            
   tr_36   
       
   StrFromLinkArrival   
       ????          
    ????   
          ????                       pr_transition         	     ?  ?     ?  &  ?  ?  ?  z          
   tr_37   
       
   default   
       ????          
    ????   
          ????                       pr_transition            
  j   ?     6    r   ?  ?            
   tr_38   
       
   StrFromLinkArrival   
       ????          
    ????   
          ????                       pr_transition         
     d       ?    5            
   tr_40   
       ????          ????          
    ????   
          ????                       pr_transition               ?  ?     ?  ?  ?  ?  ?            
   tr_41   
       
    StrFromLinkArrival && !Req_intr    
       ????          
    ????   
          ????                       pr_transition               ?  ?     ?  ?  ?  ?  ?  ?          
   tr_42   
       ????          ????          
    ????   
          ????                       pr_transition              ?  5     ?  d    J    e  ?  n          
   tr_44   
       
   CTAFrameArrival   
       
   	Qframe();   
       
    ????   
          ????                       pr_transition              x  H     >    q  1  Q  L  8  $          
   tr_45   
       
   CTAFrameArrival   
       
   	Qframe();   
       
    ????   
          ????                       pr_transition              ?  >     ?  m  ?  Z  ?  :  ?  `          
   tr_46   
       
   CTAFrameArrival   
       
   	Qframe();   
       
    ????   
          ????                       pr_transition                (  k     ?  z  ?  Y    ?  ?  ?          
   tr_52   
       
   CTAFrameArrival   
       
   	Qframe();   
       
    ????   
          ????                       pr_transition              y   ?     ?   ?      ?          
   tr_54   
       ????          ????          
    ????   
          ????                       pr_transition              w  5         ?  N  v  ?          
   tr_55   
       
   :ACAP_BIFS_intr && ACAP_BackoffWindow==0 && channel_busy==0   
       ????          
    ????   
          ????                       pr_transition              !       y  ?  ?  `          
   tr_56   
       ????          ????          
    ????   
          ????                       pr_transition               ?  ?      ?  r   ?  ?  ?   ?          
   tr_30   
       
   req_sended==0 && en_ACAP==1   
       ????          
    ????   
          ????                       pr_transition      "         ?  ?     I  ?  ?  ?          
   tr_34   
       
????   
       ????          
    ????   
          ????                       pr_transition      $         ?       ?  }  ?   ?          
   tr_36_0   
       
   Req_intr   
       ????          
    ????   
          ????                       pr_transition      %         ?  ?     ?  ?  ?  ?  [  ?          
   tr_37_0   
       
   CheckR_intr   
       ????          
    ????   
          ????                       pr_transition      &        ?  ?     ?  ?  ?  ?  ?  ?          
   tr_38_0   
       
   StrFromLinkArrival   
       ????          
    ????   
          ????                       pr_transition      '        ?  ?     ?  ?  ?  ?  ?  |          
   tr_38_1   
       ????          ????          
    ????   
          ????                       pr_transition         (                                    