MIL_3_Tfile_Hdr_ 145A 140A modeler 9 5845091D 59103A39 AC DESKTOP-FFL2R3R user 0 0 none none 0 0 none F6FA7288 528B 0 0 0 0 0 0 1e80 8                                                                                                                                                                                                                                                                                                                                                                                          ��g�      @   D   H      �  P"  Pk  Po  Ps  P  P�  P�  �           	   begsim intrpt             ����      doc file            	nd_module      endsim intrpt             ����      failure intrpts            disabled      intrpt interval         ԲI�%��}����      priority              ����      recovery intrpts            disabled      subqueue                     count    ���   
   ����   
      list   	���   
          
      super priority             ����          &   Prohandle	\beacon_proc;       Prohandle	\RCAP_process;       Prohandle	\MCTA_proc;       Prohandle	\CTA_proc;       Evhandle	\RCAP_intr_handle;       Evhandle	\MCTA_intr_handle;       Evhandle	\CTA_intr_handle;       Evhandle	\beacon_intr_handle;       Packet *	\beacon_pkptr;       double	\tx_state;       Evhandle	\ACAP_intr_handle;       Prohandle	\ACAP_proc;       Objid	\node_id;       )Evhandle	\Star_of_specialCTA_intr_handle;       (Evhandle	\End_of_specialCTA_intr_handle;       double	\CTA_location;       int	\CTA_assigned;       double	\CTA_start_time;       Packet *	\extra_pkptr;           <   /* packet stream definitions */   #define StreamFromSrcCTA  2   #define StreamFromSrcCAP  1   #define StreamFromLink 0   #define StreamToLink   0           ?#define PowerUp         (op_intrpt_type() == OPC_INTRPT_BEGSIM)   e#define CAPFrameArrival (op_intrpt_type() == OPC_INTRPT_STRM && op_intrpt_strm() == StreamFromSrcCAP)   e#define CTAFrameArrival (op_intrpt_type() == OPC_INTRPT_STRM && op_intrpt_strm() == StreamFromSrcCTA)   h#define StrFromLinkArrival   (op_intrpt_type() == OPC_INTRPT_STRM && op_intrpt_strm() == StreamFromLink)       #define beacon_intr_Code 0   g#define beacon_intr       (op_intrpt_type() == OPC_INTRPT_SELF && op_intrpt_code() == beacon_intr_Code)       #define RCAP_intr_Code 1   c#define RCAP_intr       (op_intrpt_type() == OPC_INTRPT_SELF && op_intrpt_code() == RCAP_intr_Code)       #define ACAP_intr_Code 2   c#define ACAP_intr       (op_intrpt_type() == OPC_INTRPT_SELF && op_intrpt_code() == ACAP_intr_Code)       #define MCTA_intr_Code 3   c#define MCTA_intr       (op_intrpt_type() == OPC_INTRPT_SELF && op_intrpt_code() == MCTA_intr_Code)       #define CTA_intr_Code 4   a#define CTA_intr       (op_intrpt_type() == OPC_INTRPT_SELF && op_intrpt_code() == CTA_intr_Code)       &#define Star_of_specialCTA_intr_Code 5   �#define Start_of_specialCTA_intr       (op_intrpt_type() == OPC_INTRPT_SELF && op_intrpt_code() == Star_of_specialCTA_intr_Code)       %#define End_of_specialCTA_intr_Code 6   }#define End_of_specialCTA_intr       (op_intrpt_type() == OPC_INTRPT_SELF && op_intrpt_code() == End_of_specialCTA_intr_Code)       #define ch_busy 0   5#define channel_busy (op_stat_local_read(ch_busy)==1)           -int en_beacon,en_RCAP,en_ACAP,en_MCTA,en_CTA;       double beacon_start_time;   double RCAP_start_time;   double ACAP_start_time;   double MCTA_start_time;   //double CTA_start_time;       double beacon_time=0.000535;   double RCAP_time=0.025000;   double ACAP_time=0.005000;   double MCTA_time=0.005000;   double CTA_time=0.030000;   double SD_time=0.065535;       double SIFS_time=0.0000025;           double superframe_duration;   double CAP_end_time;   double CTA_Duration;   //double CTA_location;   int req_sended;   u   static void creat_beacon_Intr()   	{   	//double beacon_time=0.000535;   	FIN (creat_beacon_Intr ());   Y	beacon_intr_handle=op_intrpt_schedule_self(op_sim_time()+ beacon_time,beacon_intr_Code);   :	//printf("beacon_intr=%2g\n",op_sim_time()+ beacon_time);   	FOUT;   	}   static void creat_RCAP_Intr()   	{   	//double RCAP_time=0.025000;    	FIN (creat_RCAP_Intr ());   S	RCAP_intr_handle=op_intrpt_schedule_self(op_sim_time()+ RCAP_time,RCAP_intr_Code);   4	//printf("CAP_intr=%2g\n",op_sim_time()+ CAP_time);   	FOUT;   	}   static void creat_ACAP_Intr()   	{   	//double ACAP_time=0.005000;    	FIN (creat_ACAP_Intr ());   S	RCAP_intr_handle=op_intrpt_schedule_self(op_sim_time()+ ACAP_time,ACAP_intr_Code);   4	//printf("CAP_intr=%2g\n",op_sim_time()+ CAP_time);   	FOUT;   	}       static void creat_MCTA_Intr()   	{   	//double MCTA_time=0.005000;   	FIN (creat_MCTA_Intr ());   S	MCTA_intr_handle=op_intrpt_schedule_self(op_sim_time()+ MCTA_time,MCTA_intr_Code);   6	//printf("MCTA_intr=%2g\n",op_sim_time()+ MCTA_time);   	FOUT;   	}       static void creat_CTA_Intr()   	{   	//double CTA_time=0.030000;   	FIN (creat_CTA_Intr ());   P	CTA_intr_handle=op_intrpt_schedule_self(op_sim_time()+ CTA_time,CTA_intr_Code);   4	//printf("CTA_intr=%2g\n",op_sim_time()+ CTA_time);   	FOUT;   	}   +static void creat_Star_of_specialCTA_Intr()   	{   	//   (	FIN (creat_Star_of_specialCTA_Intr ());   r	Star_of_specialCTA_intr_handle=op_intrpt_schedule_self(op_sim_time()+ CTA_location,Star_of_specialCTA_intr_Code);   	//;   	FOUT;   	}   *static void creat_End_of_specialCTA_Intr()   	{   	//   '	FIN (creat_End_of_specialCTA_Intr ());   }	End_of_specialCTA_intr_handle=op_intrpt_schedule_self(op_sim_time()+ CTA_location+CTA_Duration,End_of_specialCTA_intr_Code);   	//;   	FOUT;   	}       static void get_beacon_pkt()   	{   .		int frame_type,pksize,length0,DestID,i,pkID;   5		Packet *enc_IE_body,*enc_CTA_block,*enc_CTA_blocks;   		FIN (get_beacon_pkt());       +		beacon_pkptr=op_pk_get(op_intrpt_strm());   s		op_pk_nfd_get (beacon_pkptr, "frame_type", &frame_type); //acording to the standard, type value of beacon is zero   		pkID=op_pk_id (beacon_pkptr);   B		//printf("\nbeacon node:frame_type=%d,pkID=%d",frame_type,pkID);   		if (frame_type==0)   		{   K		op_pk_nfd_get (beacon_pkptr, "superframe_duration",&superframe_duration);   =		op_pk_nfd_get (beacon_pkptr, "CAP_end_time",&CAP_end_time);   8		op_pk_nfd_get (beacon_pkptr, "IE_body", &enc_IE_body);   -		pksize=op_pk_total_size_get (beacon_pkptr);   !		//printf("pksize=%d\n",pksize);   		   		if (pksize>760)   			{   4			op_pk_nfd_get (enc_IE_body, "length0", &length0);   ?			op_pk_nfd_get (enc_IE_body, "IE_payload0", &enc_CTA_blocks);   			for(i=1;i<=length0/7;i++)   				{   B				op_pk_fd_get (enc_CTA_blocks,i-1, &enc_CTA_block);//CTA_block0   4				op_pk_nfd_get (enc_CTA_block,"DestID", &DestID);   #				//printf("DestID=%d\n",DestID);   				if (node_id==DestID)   					{   B						op_pk_nfd_get (enc_CTA_block,"CTA duration", &CTA_Duration);   B						op_pk_nfd_get (enc_CTA_block,"CTA location", &CTA_location);   						CTA_assigned=1;   					}   				}   			}   		   		   8		//op_pk_nfd_get_int32 (beacon_pkptr, "PNC_address",1);   g		//printf("CTA_location=%f,CAP_end_time=%f,CTA_Duration=%f\n",CTA_location,CAP_end_time,CTA_Duration);       		}   		   		   		FOUT;   		   	}   static void Q_CAPFrame()   	{    	FIN (Q_CAPFrame ());   C	op_subq_pk_insert (0, op_pk_get(op_intrpt_strm()), OPC_QPOS_TAIL);   	FOUT;   	}   static void Q_CTAFrame()   	{    	FIN (Q_CTAFrame ());   C	op_subq_pk_insert (1, op_pk_get(op_intrpt_strm()), OPC_QPOS_TAIL);   	FOUT;   	}                                          �   Z          
   init   
       
      /* Create child process(es) */       5beacon_proc = op_pro_create ("beacon_proc", OPC_NIL);   en_beacon=0;       7RCAP_process = op_pro_create ("RCAP_process", OPC_NIL);   
en_RCAP=0;       1ACAP_proc = op_pro_create ("ACAP_proc", OPC_NIL);   
en_ACAP=0;       1MCTA_proc = op_pro_create ("MCTA_proc", OPC_NIL);   
en_MCTA=0;       /CTA_proc = op_pro_create ("CTA_proc", OPC_NIL);   	en_CTA=0;           tx_state=0;       node_id=op_id_self();   CTA_assigned=0;       
                     
   ����   
          pr_state        �   Z          
   beacon_proc   
                     
      //MY_STRUCT* someData;   en_beacon=1;   ?//op_intrpt_port_register (OPC_PORT_TYPE_STRM, 0, beacon_proc);   ?//op_intrpt_port_register (OPC_PORT_TYPE_STRM, 1, beacon_proc);   0//op_pro_invoke (beacon_proc, OPC_NIL);//OPC_NIL   creat_beacon_Intr();       
       
   ����   
          pr_state        �            
   	RCAP_proc   
       
   
   en_beacon=0;   //op_pk_destroy(beacon_pkptr);   
en_RCAP=1;   creat_RCAP_Intr();       RCAP_start_time=op_sim_time ();   &op_pro_invoke (RCAP_process, OPC_NIL);   >op_intrpt_port_register (OPC_PORT_TYPE_STRM, 0, RCAP_process);   >op_intrpt_port_register (OPC_PORT_TYPE_STRM, 1, RCAP_process);   @//op_intrpt_port_register (OPC_PORT_TYPE_STRM, 2, RCAP_process);   
                     
   ����   
          pr_state        �  :          
   	MCTA_proc   
       
      
en_ACAP=0;   2op_intrpt_port_deregister (OPC_PORT_TYPE_STRM, 0);   2op_intrpt_port_deregister (OPC_PORT_TYPE_STRM, 2);   req_sended=0;   
en_MCTA=1;   #op_pro_invoke (MCTA_proc, OPC_NIL);   creat_MCTA_Intr();   
       
       
       
   ����   
          pr_state        �  �          
   CTA_proc   
                     
      
en_MCTA=0;   creat_CTA_Intr();    creat_Star_of_specialCTA_Intr();   creat_End_of_specialCTA_Intr();       
       
   ����   
          pr_state        �   �          
   beacon_duration   
                                       ����             pr_state        �  J          
   RCAP_duration   
                                       ����             pr_state        �  v          
   MCTA_duration   
                                       ����             pr_state      	  �  �          
   Wait2EndOfCTAP   
                                       ����             pr_state      
  f   �          
   st_10   
       
      get_beacon_pkt();   
                     
   ����   
          pr_state        �  �          
   	ACAP-proc   
       
      
en_RCAP=0;   2op_intrpt_port_deregister (OPC_PORT_TYPE_STRM, 1);   	   
en_ACAP=1;   req_sended=0;   creat_ACAP_Intr();   ACAP_start_time=op_sim_time ();   #op_pro_invoke (ACAP_proc, OPC_NIL);   ;op_intrpt_port_register (OPC_PORT_TYPE_STRM, 0, ACAP_proc);   ;op_intrpt_port_register (OPC_PORT_TYPE_STRM, 2, ACAP_proc);   7//op_intrpt_type_register (OPC_INTRPT_SELF, ACAP_proc);       
                     
   ����   
          pr_state        �  �          
   ACAP_duration   
                                       ����             pr_state        �  �          
   Wait2startOfCTA   
                                       ����             pr_state          *          
   CTA_duration   
                                       ����             pr_state        �  �          
   register_intr   
       
      	en_CTA=1;   "op_pro_invoke (CTA_proc, OPC_NIL);   :op_intrpt_port_register (OPC_PORT_TYPE_STRM, 0, CTA_proc);   :op_intrpt_port_register (OPC_PORT_TYPE_STRM, 2, CTA_proc);   Rop_intrpt_port_register (OPC_INTRPT_SELF, 5 , CTA_proc);//start_of_specialCTA_intr   R//op_intrpt_port_register (OPC_INTRPT_SELF, 6 , CTA_proc);//End_of_specialCTA_intr   CTA_start_time=op_sim_time ();       
                     
   ����   
          pr_state        �  f          
   deregister_intr   
       
      	en_CTA=0;   2op_intrpt_port_deregister (OPC_PORT_TYPE_STRM, 0);   2op_intrpt_port_deregister (OPC_PORT_TYPE_STRM, 1);   2op_intrpt_port_deregister (OPC_PORT_TYPE_STRM, 2);   /op_intrpt_port_deregister (OPC_INTRPT_SELF, 5);   1//op_intrpt_port_deregister (OPC_INTRPT_SELF, 6);   
                     
   ����   
          pr_state        f  �          
   del_extrapkt1   
       
      (extra_pkptr=op_pk_get(op_intrpt_strm());   M//printf("extra packet witd id packet =%d deleted\n",op_pk_id (extra_pkptr));   op_pk_destroy(extra_pkptr);   
                     
   ����   
          pr_state        *  �          
   del_extrapkt2   
       
      (extra_pkptr=op_pk_get(op_intrpt_strm());   M//printf("extra packet witd id packet =%d deleted\n",op_pk_id (extra_pkptr));   op_pk_destroy(extra_pkptr);   
                     
   ����   
          pr_state        �  *          
   deregister_intr1   
       
      	en_CTA=0;   2op_intrpt_port_deregister (OPC_PORT_TYPE_STRM, 0);   2op_intrpt_port_deregister (OPC_PORT_TYPE_STRM, 1);   2op_intrpt_port_deregister (OPC_PORT_TYPE_STRM, 2);   /op_intrpt_port_deregister (OPC_INTRPT_SELF, 5);   1//op_intrpt_port_deregister (OPC_INTRPT_SELF, 6);   
                     
   ����   
          pr_state          (           �   @     �   {  �   E  �   w  �   �          
   tr_19   
       
   default   
       ����          
    ����   
          ����                       pr_transition              �  J     �  <  �  /  �  e  �  Q          
   tr_20   
       
   default   
       ����          
    ����   
          ����                       pr_transition              �  h     �  o  �  X  �  �  �  u          
   tr_21   
       
   default   
       ����          
    ����   
          ����                       pr_transition         	   	  W  �     �  �  e  �  �  �  �  �          
   tr_22   
       
   default   
       ����          
    ����   
          ����                       pr_transition              ]   u        X  �   �          
   tr_23   
       ����          ����          
    ����   
          ����                       pr_transition              L   �     �   �              
   tr_24   
       
   beacon_intr   
       ����          
    ����   
          ����                       pr_transition              S  ,         �  =          
   tr_25   
       ����          ����          
    ����   
          ����                       pr_transition              Y  Y       <  �  s          
   tr_27   
       ����          ����          
    ����   
          ����                       pr_transition              W  �     �  �    �          
   tr_28   
       
   	MCTA_intr   
       ����          
    ����   
          ����                       pr_transition      #         Z   I      �   T  �   [          
   tr_35   
       
   PowerUp   
       ����          
    ����   
          ����                       pr_transition      $      
     t     �   �     y  ]   �          
   tr_36   
       
   StrFromLinkArrival   
       ����          
    ����   
          ����                       pr_transition      %   
     5   �     R   �     �  �   �          
   tr_37   
       ����          ����          
    ����   
          ����                       pr_transition      +        V  `     �  :  �  �          
   tr_43   
       
   	RCAP_intr   
       ����          
    ����   
          ����                       pr_transition      ,        ^  �       �  �  �          
   tr_44   
       ����          ����          
    ����   
          ����                       pr_transition      -          �     �  �  �  �  �  �  �  �          
   tr_45   
       
   default   
       ����          
    ����   
          ����                       pr_transition      .        X  �     �  �    2          
   tr_46   
       
   	ACAP_intr   
       ����          
    ����   
          ����                       pr_transition      /        �       �  B  �    �    �  ?          
   tr_47   
       
   CTAFrameArrival   
       
   Q_CTAFrame()   
       
    ����   
          ����                       pr_transition      0        �   �     �   �  �   �  �   �  �   �          
   tr_48   
       
   CTAFrameArrival   
       
   Q_CTAFrame()   
       
    ����   
          ����                       pr_transition      1        v   �     �   �  r     r   �  �   �          
   tr_49   
       
   CAPFrameArrival   
       
   Q_CAPFrame()   
       
    ����   
          ����                       pr_transition      4        �       �  �  �    �     �  �          
   tr_52   
       
   CAPFrameArrival   
       
   Q_CAPFrame()   
       
    ����   
          ����                       pr_transition      5        X  �       �  �  �          
   tr_53   
       ����          ����          
    ����   
          ����                       pr_transition      6        `        �  �  �  �  l  �  �    �  �          
   tr_54   
       
   default   
       ����          
    ����   
          ����                       pr_transition      9        `  .     *    H    F  N  '  2          
   tr_57   
       
   default   
       ����          
    ����   
          ����                       pr_transition      :        �  9     �  c  �  B  �  C  �  f          
   tr_58   
       
   CAPFrameArrival   
       
   Q_CAPFrame()   
       
    ����   
          ����                       pr_transition      ;        �  �     �  �  �  �  �  �  �  �          
   tr_59   
       
   CAPFrameArrival   
       
   Q_CAPFrame()   
       
    ����   
          ����                       pr_transition      <        :  �         �  �  .  �              
   tr_60   
       
   CAPFrameArrival   
       
   Q_CAPFrame()   
       
    ����   
          ����                       pr_transition      =   	   	  �  b     �  �  �  q  �  p  �  �          
   tr_61   
       
   CAPFrameArrival   
       
   Q_CAPFrame()   
       
    ����   
          ����                       pr_transition      >        �  *     �  �  �    �    �  �          
   tr_62   
       
   CTAFrameArrival   
       
   Q_CTAFrame()   
       
    ����   
          ����                       pr_transition      ?   	   	  �  �     �  �  �  �  �  �  �  �          
   tr_63   
       
   CTAFrameArrival   
       
   Q_CTAFrame()   
       
    ����   
          ����                       pr_transition      B   	     ,  �     �  �  �  �  >  �  �   �  �   f          
   tr_66   
       
   CTA_intr   
       ����          
    ����   
          ����                       pr_transition      E        3  �     �  �  �    �  �          
   tr_69   
       
   Start_of_specialCTA_intr   
       ����          
    ����   
          ����                       pr_transition      F        �  �     �  �  �  �               
   tr_70   
       ����          ����          
    ����   
          ����                       pr_transition      G        �  [       9  �  a  �  i          
   tr_71   
       
   End_of_specialCTA_intr   
       ����          
    ����   
          ����                       pr_transition      H      	  I  m     �  f     s  �  �          
   tr_72   
       ����          ����          
    ����   
          ����                       pr_transition      L          �     �  �  �  �  P  �          
   tr_76   
       
   StrFromLinkArrival   
       ����          
    ����   
          ����                       pr_transition      M        .  �     f  �  �  �  �  �  �  �          
   tr_77   
       ����          ����          
    ����   
          ����                       pr_transition      N   	       �     �  �  �  �  $  �          
   tr_78   
       
   StrFromLinkArrival   
       ����          
    ����   
          ����                       pr_transition      O      	    �       �  �  �  �  �          
   tr_79   
       ����          ����          
    ����   
          ����                       pr_transition      P        b  6       (  �  D    5          
   tr_80   
       
   CTA_intr   
       ����          
    ����   
          ����                       pr_transition      Q        �       �  !  �  �  �   �  �   s          
   tr_81   
       ����          ����          
    ����   
          ����                       pr_transition         R      	ACAP_proc   beacon_proc   CTA_proc   	MCTA_proc   RCAP_process                                