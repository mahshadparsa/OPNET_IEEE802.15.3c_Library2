MIL_3_Tfile_Hdr_ 145A 140A modeler 9 58347759 590F69D6 73 DESKTOP-FFL2R3R user 0 0 none none 0 0 none F5B58D19 1EF2 0 0 0 0 0 0 1e80 8                                                                                                                                                                                                                                                                                                                                                                                          ??g?      @   D   H      ?  ?  ?  ?  ?  ?  ?  ?  ?           	   begsim intrpt             ????      doc file            	nd_module      endsim intrpt             ????      failure intrpts            disabled      intrpt interval         ԲI?%??}????      priority              ????      recovery intrpts            disabled      subqueue                     count    ???   
   ????   
      list   	???   
          
      super priority             ????             char	\format_str [64];       Evhandle	\beacon_intr_handle;       double	\tx_state;              #/* Node configuration constants.	*/   #define StreamFromLink 0   #define StreamToLink   0   ?#define PowerUp         (op_intrpt_type() == OPC_INTRPT_BEGSIM)       c#define FrameArrival    (op_intrpt_type() == OPC_INTRPT_STRM && op_intrpt_strm() == StreamFromLink)       #define beacon_intr_Code 0   g#define beacon_intr       (op_intrpt_type() == OPC_INTRPT_SELF && op_intrpt_code() == beacon_intr_Code)       #define ch_busy 0   5#define channel_busy (op_stat_local_read(ch_busy)==1)   int num_CTA_req;   int nodeID_CTA_req[32];   s   &static void send_ack (Packet *Data_pk)   	{		   	Packet * ack_pk;   	int data_ID;   	double	pksize=496;   	FIN (send_ack (Data_pk));   -	op_pk_nfd_get (Data_pk, "src_ID",&data_ID);	    	//printf("data_ID=%d",data_ID);   	   1	/* Create a packet with the specified format.	*/   +	ack_pk = op_pk_create_fmt ("Imm_ACK_pkt");   (	op_pk_total_size_set (ack_pk, pksize);    h	op_pk_nfd_set_int32 (ack_pk, "frame_type", 1); //acording to the standard, type value of beacon is zero   1	op_pk_nfd_set_int32 (ack_pk, "dest_ID",data_ID);       (	//op_pk_total_size_set (pkptr, pksize);   *	//printf("channel_busy=%d",channel_busy);   	//if (channel_busy==0)   $		op_pk_send (ack_pk, StreamToLink);   	   	FOUT;   	}           static void send_beacon ()   	{		   ?	Packet *beacon_pk,*enc_IE_body,*enc_CTA_block,*enc_CTA_blocks;   *	int	pksize=696,pksize1;//whithout IE_body   	double  SD=0.065535;   	double  CAPD=0.030000;   *	double  CTAPD=0.030000;//SD-CAPD-BeaconD;   	double  BeaconD=0.000535;   	int i;   	   	FIN (send_beacon ());   	   1	/* Create a packet with the specified format.	*/   .	beacon_pk = op_pk_create_fmt ("beacon_pkt");    ,	enc_IE_body = op_pk_create_fmt ("IE_body");   	   @	/*************************************************************/   (	//printf("num_CTA_req=%d",num_CTA_req);   	if (num_CTA_req>0)   		{   3		enc_CTA_blocks = op_pk_create_fmt ("CTA_blocks");   		for (i=1;i<=num_CTA_req;i++)   			{   3			enc_CTA_block = op_pk_create_fmt ("CTA_block");	   W			//printf("\n nodeID_CTA_req[%d]=%d,num_CTA_req=%d",i,nodeID_CTA_req[i],num_CTA_req);   >			op_pk_nfd_set (enc_CTA_block, "DestID", nodeID_CTA_req[i]);   -			op_pk_nfd_set (enc_CTA_block, "SrcID", 0);   D			op_pk_nfd_set (enc_CTA_block, "CTA duration", CTAPD/num_CTA_req);   J			op_pk_nfd_set (enc_CTA_block, "CTA location", (i-1)*CTAPD/num_CTA_req);   X			op_pk_fd_set (enc_CTA_blocks,i-1,OPC_FIELD_TYPE_PACKET, enc_CTA_block,-1);//CTA_block   			}       J		op_pk_fd_set (enc_IE_body,0,OPC_FIELD_TYPE_INTEGER, 0,8);//"element_ID0"   S		op_pk_fd_set (enc_IE_body,1,OPC_FIELD_TYPE_INTEGER, num_CTA_req*7,8);//"length0"    X		op_pk_fd_set (enc_IE_body,2,OPC_FIELD_TYPE_PACKET , enc_CTA_blocks,-1);//"IE_payload0"   		   @		//op_pk_bulk_size_set (enc_IE_body,8+8+56*num_CTA_req+8+8+48);   *		pksize=pksize+8+8+56*num_CTA_req+8+8+48;   		}   	else   		{   -		//op_pk_bulk_size_set (enc_IE_body,8+8+48);   		pksize=pksize+8+8+48;   		}   		   	//printf("pksize=%d",pksize);   @	/*************************************************************/   	   3	op_pk_nfd_set (beacon_pk, "IE_body", enc_IE_body);   	   k	op_pk_nfd_set_int32 (beacon_pk, "frame_type", 0); //acording to the standard, type value of beacon is zero   5	op_pk_nfd_set (beacon_pk, "superframe_duration",SD);   0	op_pk_nfd_set (beacon_pk, "CAP_end_time",CAPD);   2	op_pk_nfd_set_int32 (beacon_pk, "PNC_address",1);   	   )	op_pk_total_size_set (beacon_pk,pksize);   )	pksize1=op_pk_total_size_get(beacon_pk);   "	//printf("pksize1=%d\n",pksize1);   &	op_pk_send (beacon_pk, StreamToLink);   	FOUT;   	}       +static void send_assoc_res(Packet *Data_pk)   	{		   	Packet *assoc_res_pk;   	double	pksize=720;    	FIN (send_assoc_res (Data_pk));   	   1	/* Create a packet with the specified format.	*/   2	assoc_res_pk = op_pk_create_fmt ("assoc_res_cb");   .	op_pk_total_size_set (assoc_res_pk, pksize);    	//set_beacon_pk_fields   	   8	op_pk_nfd_set_int32 (assoc_res_pk, "command_type", 1);    )	op_pk_send (assoc_res_pk, StreamToLink);   	   	FOUT;   	}       static void creat_beacon_Intr()   	{   %	double superframe_duration=0.065535;   	FIN (creat_beacon_Intr ());   a	beacon_intr_handle=op_intrpt_schedule_self(op_sim_time()+ superframe_duration,beacon_intr_Code);   W	//printf("op_sim_time()+ superframe_duration=%2g",op_sim_time()+ superframe_duration);   	FOUT;   	}                                                        ?   ?          
   idle   
                                       ????             pr_state        ?            
   send_responce   
       J      Packet *pkt;   int frame_type;   !pkt = op_pk_get (StreamFromLink);   /op_pk_nfd_get (pkt, "frame_type",&frame_type);	   /if (frame_type==3)//that frame is command frame   	{   	send_ack (pkt);   (	//send_assoc_res(pkt);//creat response    	num_CTA_req=num_CTA_req+1;   I	op_pk_nfd_get (pkt, "CTRqB_Target_IDlist",&nodeID_CTA_req[num_CTA_req]);   K	//printf("nodeID_CTA_req[%d]=%d",num_CTA_req,nodeID_CTA_req[num_CTA_req]);   	}   1else if (frame_type==4)//that frame is data frame   	send_ack (pkt);   J                     
   ????   
          pr_state        ?   ?          
   send_beacon   
       
      send_beacon ();   creat_beacon_Intr();   num_CTA_req=0;   
                     
   ????   
          pr_state           ?          
   init   
                     
      send_beacon ();   creat_beacon_Intr();   num_CTA_req=0;   tx_state=0;   
       
   ????   
          pr_state                     ?   G     |   ?  k   ^  ?   \  ?   ?          
   tr_0   
       
   default   
       ????          
    ????   
          ????                       pr_transition              ?   p     ?   ?  ?   z     ?          
   tr_4   
       
   beacon_intr    
       ????          
    ????   
          ????                       pr_transition              ?   ?     ?   ?  ?   ?  ?   ?          
   tr_5   
       ????          ????          
    ????   
          ????                       pr_transition              N   ?        ?  ?   ?          
   tr_8   
       ????          ????          
    ????   
          ????                       pr_transition              ?   ?     ?   ?  ?   ?  ?            
   tr_14   
       
   FrameArrival   
       ????          
    ????   
          ????                       pr_transition              z   ?     ?    `   ?  |   ?          
   tr_15   
       ????          ????          
    ????   
          ????                       pr_transition                                             