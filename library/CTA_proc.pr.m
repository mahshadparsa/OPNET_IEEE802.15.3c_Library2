MIL_3_Tfile_Hdr_ 145A 140A modeler 9 58451B8A 59100EEF 42 DESKTOP-FFL2R3R user 0 0 none none 0 0 none 2BA0007 3897 0 0 0 0 0 0 1e80 8                                                                                                                                                                                                                                                                                                                                                                                           ??g?      @   D   H        6s  6w  6{  6  6?  6?  6?             	   begsim intrpt             ????      doc file            	nd_module      endsim intrpt             ????      failure intrpts            disabled      intrpt interval         ԲI?%??}????      priority              ????      recovery intrpts            disabled      subqueue                     count    ???   
   ????   
      list   	???   
          
      super priority             ????              Evhandle	\Start_CTA_intr_handle;       Packet *	\copyPkt;       int	\res_timeOK;       int	\ack_correct;       Objid	\node_id;       int	\CTA_assigned;       double	\CTA_start_time;       (Evhandle	\End_of_specialCTA_intr_handle;       Evhandle	\SIFS_intr_handle;           0   /* packet stream definitions */   #define StreamFromSrcCTA  2   #define StreamFromSrcCAP  1   #define StreamFromLink 0   #define StreamToLink   0           ?#define PowerUp         (op_intrpt_type() == OPC_INTRPT_BEGSIM)   e#define CAPFrameArrival (op_intrpt_type() == OPC_INTRPT_STRM && op_intrpt_strm() == StreamFromSrcCAP)   e#define CTAFrameArrival (op_intrpt_type() == OPC_INTRPT_STRM && op_intrpt_strm() == StreamFromSrcCTA)   g#define StrFromLinkArrival  (op_intrpt_type() == OPC_INTRPT_STRM && op_intrpt_strm() == StreamFromLink)       #define ACAP_Timeout_Code 0   k#define ACAP_Timeout         (op_intrpt_type() == OPC_INTRPT_SELF && op_intrpt_code() == ACAP_Timeout_Code)       #define ACAP_BIFS_intr_Code 1   m#define ACAP_BIFS_intr       (op_intrpt_type() == OPC_INTRPT_SELF && op_intrpt_code() == ACAP_BIFS_intr_Code)       #define ACAP_RIFS_intr_Code 2   m#define ACAP_RIFS_intr       (op_intrpt_type() == OPC_INTRPT_SELF && op_intrpt_code() == ACAP_RIFS_intr_Code)       ##define ACAP_PacketSent_intr_code 3   s#define ACAP_PacketSent_intr (op_intrpt_type() == OPC_INTRPT_SELF && op_intrpt_code() == ACAP_PacketSent_intr_code)       #define Start_CTA_intr_Code 4   m#define Start_CTA_intr       (op_intrpt_type() == OPC_INTRPT_SELF && op_intrpt_code() == Start_CTA_intr_Code)       '#define Start_of_specialCTA_intr_Code 5   ?#define Start_of_specialCTA_intr       (op_intrpt_type() == OPC_INTRPT_SELF && op_intrpt_code() == Start_of_specialCTA_intr_Code)       %#define End_of_specialCTA_intr_Code 6   }#define End_of_specialCTA_intr       (op_intrpt_type() == OPC_INTRPT_SELF && op_intrpt_code() == End_of_specialCTA_intr_Code)       #define SIFS_intr_Code 7   c#define SIFS_intr       (op_intrpt_type() == OPC_INTRPT_SELF && op_intrpt_code() == SIFS_intr_Code)               ?#define ACAP_QEmpty 	(op_subq_stat(1, OPC_QSTAT_PKSIZE) == 0.0)   double SIFS_time;           int en_CTA;   //double CTA_start_time;   double CTA_location;   double CTA_Duration;   double service_rate=900000000;       G   *static void creat_End_of_specialCTA_Intr()   	{   	//   '	FIN (creat_End_of_specialCTA_Intr ());   o	End_of_specialCTA_intr_handle=op_intrpt_schedule_self(op_sim_time()+CTA_Duration,End_of_specialCTA_intr_Code);   K	//printf("CTA_Duration=%2g,op_sim_time()=%2g",CTA_Duration,op_sim_time());   	FOUT;   	}   X/**************************************************************************************/   X/**************************************************************************************/   static void creat_SIFS_Intr()   	{   	FIN(creat_SIFS_Intr());   T	SIFS_intr_handle=op_intrpt_schedule_self(op_sim_time()+2*SIFS_time,SIFS_intr_Code);   	FOUT;   	}   X/**************************************************************************************/   X/**************************************************************************************/   static void Qframe()   	{    	FIN (Qframe ());   C	op_subq_pk_insert (1, op_pk_get(op_intrpt_strm()), OPC_QPOS_TAIL);   	FOUT;   	}   X/**************************************************************************************/   X/**************************************************************************************/       static void check_ack()   	{   	int dest_ID;   	int frame_type;   	Packet* ack_pk;   	FIN (check_ack());   $	ack_pk=op_pk_get(op_intrpt_strm());   r	op_pk_nfd_get_int32 (ack_pk, "frame_type",&frame_type ); //acording to the standard, type value of beacon is zero   2	op_pk_nfd_get_int32 (ack_pk, "dest_ID",&dest_ID);   '	if (frame_type==1 && dest_ID==node_id)   		{   		ack_correct=1;   		creat_SIFS_Intr();   		}   	else   		{   		ack_correct=0;   v		printf("\n CTA_Proc: dest_ID=%d,node_id=%d,frame_type=%d,pkt_id=%d\n",dest_ID,node_id,frame_type,op_pk_id (ack_pk));   		}   		op_pk_destroy(ack_pk);   	FOUT;   	   	}   X/**************************************************************************************/   X/**************************************************************************************/   static void check_res_time()   	{   	double ACK_svc_time;   	double pk_svc_time;   	FIN (check_res_time());   	ACK_svc_time=0;   	pk_svc_time=720/service_rate;   W	if (CTA_Duration-op_sim_time()+ CTA_start_time< pk_svc_time +2*SIFS_time+ACK_svc_time)   		{   		res_timeOK=0;   		}   	else   		{   		res_timeOK=1;   		}   ?	//printf("CTA_start_time=%2g,CTA_Duration=%2g,pk_svc_time=%2g,SIFS_time=%2g,res_timeOK=%d\n",CTA_start_time,CTA_Duration,pk_svc_time,SIFS_time,res_timeOK);   		   	FOUT;   	}                                         ?  J          
   init   
       
      b//Start_CTA_intr_handle=op_intrpt_schedule_self( op_sim_time ()+CTA_location,Start_CTA_intr_Code);   
                     
   ????   
          pr_state          J          
   Idle   
                                       ????             pr_state        :  ?          
   srv_Qpkt   
       
   
   check_res_time();   if (res_timeOK==1)   	{   =	copyPkt = op_pk_copy (op_subq_pk_remove (1, OPC_QPOS_HEAD));   	node_id=op_id_self();		   1	op_pk_nfd_set_int32 (copyPkt, "src_ID",node_id);   ,	op_pk_nfd_set_int32 (copyPkt, "dest_ID",0);   c	op_pk_nfd_set_int32 (copyPkt, "frame_type", 4);//acording to the standard, type value of data is 4       	}   
                     
   ????   
          pr_state        :   ?          
   WaitforData   
                                       ????             pr_state        :            
   srv_pkt   
       
   
   check_res_time();   if (res_timeOK==1)   	{   4	copyPkt = op_pk_copy (op_pk_get(op_intrpt_strm()));   	node_id=op_id_self();		   1	op_pk_nfd_set_int32 (copyPkt, "src_ID",node_id);   ,	op_pk_nfd_set_int32 (copyPkt, "dest_ID",0);   c	op_pk_nfd_set_int32 (copyPkt, "frame_type", 4);//acording to the standard, type value of data is 4       	}   
                     
   ????   
          pr_state        f            
   wait_ack   
                                       ????             pr_state      	              
   	check_ack   
       
      check_ack();   
                     
   ????   
          pr_state      
  ?   Z          
   
set_param1   
       
      CTA_start_time=op_sim_time();   creat_End_of_specialCTA_Intr();   
                     
   ????   
          pr_state        ?  ?          
   
set_param2   
                     
      CTA_start_time=op_sim_time();   creat_End_of_specialCTA_Intr();   
       
   ????   
          pr_state        ?  :          
   
set_param3   
       
      	Qframe();   CTA_start_time=op_sim_time();   creat_End_of_specialCTA_Intr();   
                     
   ????   
          pr_state        *   ?          
   wait_2SIFS1   
                                       ????             pr_state        *  ?          
   wait_2SIFS2   
                                       ????             pr_state                     ?  I      ?  H    J          
   tr_3   
       
   default   
       ????          
    ????   
          ????                       pr_transition              9   \     .   ?     i  T   i  J   ?          
   tr_19   
       
   default   
       ????          
    ????   
          ????                       pr_transition              ?   ?     t   ?  ?   ?  ?    }            
   tr_24   
       
   default   
       ????          
    ????   
          ????                       pr_transition      %         ?  ?      ?  ]   ?  ?  )  ?    \          
   tr_37   
       
   ,CTAFrameArrival && !Start_of_specialCTA_intr   
       
   	Qframe();   
       
    ????   
          ????                       pr_transition      '           ?     L        >   ?  _            
   tr_39   
       
   CTAFrameArrival   
       
   	Qframe();   
       
    ????   
          ????                       pr_transition      +        O       
  5  )    T  2    =          
   tr_43   
       
   default   
       ????          
    ????   
          ????                       pr_transition      .        c   ?     7   ?  :            
   tr_46   
       
   *CTAFrameArrival && !End_of_specialCTA_intr   
       ????          
    ????   
          ????                       pr_transition      2        ?       F    M            
   tr_50   
       
   
res_timeOK   
       
   #op_pk_send (copyPkt,StreamToLink );   
       
    ????   
          ????                       pr_transition      3        ?  D     J  ?  T            
   tr_51   
       
   
res_timeOK   
       
   #op_pk_send (copyPkt,StreamToLink );   
       
    ????   
          ????                       pr_transition      8        ?  -     #    ?  :  '  A          
   tr_56   
       
   !res_timeOK   
       ????          
    ????   
          ????                       pr_transition      9           ?     ;  ?  ?  Q  *  I          
   tr_57   
       
   !res_timeOK   
       ????          
    ????   
          ????                       pr_transition      <      	  ?  =     h    ?  '              
   tr_60   
       
   StrFromLinkArrival   
       ????          
    ????   
          ????                       pr_transition      =   	     ?           x            
   tr_61   
       
   default   
       ????          
    ????   
          ????                       pr_transition      @        g   ?     6   ?  e   ?     ?        5          
   tr_64   
       
   End_of_specialCTA_intr   
       ????          
    ????   
          ????                       pr_transition      A        ?   ?     -   ?  ?   ?  "       -    6          
   tr_65   
       
   )End_of_specialCTA_intr && CTAFrameArrival   
       
   	Qframe();   
       
    ????   
          ????                       pr_transition      B      
     ?       C   ?   ?  t   `          
   tr_66   
       
   <Start_of_specialCTA_intr  && ACAP_QEmpty && !CTAFrameArrival   
       ????          
    ????   
          ????                       pr_transition      C   
     ?   p     ?   V  *   ?          
   tr_67   
       ????          ????          
    ????   
          ????                       pr_transition      D        ?  |        F  ?  ?          
   tr_68   
       
   )Start_of_specialCTA_intr && !ACAP_QEmpty    
       ????          
    ????   
          ????                       pr_transition      E          ?     ?  ?  <  ?          
   tr_69   
       ????          ????          
    ????   
          ????                       pr_transition      H        =  ?     !  O  q  ?  ?    ?  2          
   tr_72   
       
   ,Start_of_specialCTA_intr &&  CTAFrameArrival   
       ????          
    ????   
          ????                       pr_transition      I        ?       ?  ,    
  1  ?          
   tr_73   
       ????          ????          
    ????   
          ????                       pr_transition      L   	        ?        ?  ?   ?  4   ?          
   tr_76   
       
    ack_correct==1 && ACAP_QEmpty    
       ????          
    ????   
          ????                       pr_transition      M        ?   ?        ?  N   ?          
   tr_77   
       
   	SIFS_intr   
       ????          
    ????   
          ????                       pr_transition      N   	     ?  ?         ?  ?  6  ?          
   tr_78   
       
   ack_correct==1 && !ACAP_QEmpty    
       ????          
    ????   
          ????                       pr_transition      O        ?  ?     !  ?  E  ?          
   tr_79   
       
   	SIFS_intr   
       ????          
    ????   
          ????                       pr_transition      P        ?   ?        ?  ?   ?     ?  (   ?          
   tr_80   
       
   default   
       ????          
    ????   
          ????                       pr_transition      Q        )  y     8  ?  A  ?  	  ?  %  ?          
   tr_81   
       
   default   
       ????          
    ????   
          ????                       pr_transition      R        5   T     7   ~  :   ]     ^  #   ?          
   tr_82   
       
   CTAFrameArrival   
       
   	Qframe();   
       
    ????   
          ????                       pr_transition      S        7       2  ?  I              ?          
   tr_83   
       
   CTAFrameArrival   
       
   	Qframe();   
       
    ????   
          ????                       pr_transition         T                                    