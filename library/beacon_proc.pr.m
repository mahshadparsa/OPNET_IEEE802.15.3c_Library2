MIL_3_Tfile_Hdr_ 145A 140A modeler 9 58451B48 5868EC08 11 DESKTOP-FFL2R3R user 0 0 none none 0 0 none B4FD280E B92 0 0 0 0 0 0 1e80 8                                                                                                                                                                                                                                                                                                                                                                                           ??g?      @   D   H        	n  	r  	v  	z  	?  	?  	?             	   begsim intrpt             ????      doc file            	nd_module      endsim intrpt             ????      failure intrpts            disabled      intrpt interval         ԲI?%??}????      priority              ????      recovery intrpts            disabled      subqueue                     count    ???   
   ????   
      list   	???   
          
      super priority             ????             Packet *	\beacon_pkptr;       double	\CAP_end_time;       double	\superframe_duration;              #define StreamFromLink 0   d#define beaconArrival    (op_intrpt_type() == OPC_INTRPT_STRM && op_intrpt_strm() == StreamFromLink)   int frame_type;   double superframe_duration;   double CAP_end_time;      static void get_beacon_pkt()   	{   		int stream_index;   		FIN (get_beacon_pkt());   		    		stream_index=op_intrpt_strm();   '		beacon_pkptr=op_pk_get(stream_index);   		   s		op_pk_nfd_get (beacon_pkptr, "frame_type", &frame_type); //acording to the standard, type value of beacon is zero   K		op_pk_nfd_get (beacon_pkptr, "superframe_duration",&superframe_duration);   =		op_pk_nfd_get (beacon_pkptr, "CAP_end_time",&CAP_end_time);   8		//op_pk_nfd_get_int32 (beacon_pkptr, "PNC_address",1);   %		printf("frame_type=%d",frame_type);   7		printf("superframe_duration=%f",superframe_duration);   )		printf("CAP_end_time=%f",CAP_end_time);   		   		   		FOUT;   		   	}                                         ?   ?          
   init   
                                   
    ????   
          pr_state         ?            
   st_2   
       
      get_beacon_pkt();   
                     
   ????   
          pr_state                     ?   w      ?   ?     j     ?   ?   ?          
   tr_1   
       
   default   
       ????          
    ????   
          ????                       pr_transition                 ?      ?   ?   ?   ?   ?            
   tr_2   
       
   beaconArrival   
       ????          
    ????   
          ????                       pr_transition               ?   ?      ?   ?   ?   ?   ?   ?          
   tr_3   
       ????          ????          
    ????   
          ????                       pr_transition                                             