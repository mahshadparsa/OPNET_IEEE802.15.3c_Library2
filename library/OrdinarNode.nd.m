MIL_3_Tfile_Hdr_ 145A 140A modeler 6 5823FF5F 5946B20C 89 DESKTOP-FFL2R3R user 0 0 none none 0 0 none 5530DD8F 1F37 0 0 0 0 0 0 1e80 8                                                                                                                                                                                                                                                                                                                                                                                          ??g?      8   <   n   r   v   z  ;  ?  ?  ?  #  '       
fixed_comm   mobile_comm   sat_comm????                       
TIM source            none      altitude                 ????      altitude modeling            relative to subnet-platform      	condition            ????      financial cost            0.00      hostname         ????      minimized icon            circle/#708090      phase                 ????      priority              ????      role         ????      user id              ????               ?  R          
   radio_Tx   
       
            count    ???   
   ????   
      list   	???   
            	data rate   ???   
A??t?   ????   
      packet formats   ???   
   CTA_req_pkt,data_pkt   
      	bandwidth   ???   
A@??    ????   
      min frequency   ???   
@?L     ????   
      power   ???   
@@     ????   
   
   
          bpsk          
   wpan_802153_dra_rxgroup   
       
   wpan_802153_dra_txdel   
       
   NONE   
       
   dra_chanmatch   
          
dra_tagain             dra_propdel             ra_tx                       nd_radio_transmitter      
      ?  ?   
       
   
Tx_antenna   
          	isotropic                  ????          @f?     ????                  ????                  ????                  ????             antenna                       
nd_antenna      	     ?  R          
   radio_Rx   
       
            count    ???   
   ????   
      list   	???   
            	data rate   ???   
A??t?   ????   
      packet formats   ???   
   "beacon_pkt,CTA_res_pkt,Imm_ACK_pkt   
      	bandwidth   ???   
A@??    ????   
      min frequency   ???   
@?L     ????   
   
   
          bpsk          ??      ????                  ????             
dra_ragain          
   wpan_802153_dra_power1   
          dra_bkgnoise          
   wpan_802153_dra_inoise   
          dra_snr             dra_ber             	dra_error          
   dra_ecc   
          ra_rx                       nd_radio_receiver          !   ?   ?          
   src_RCAP   
       
   simple_source   
          	processor                   Packet Format   ???   
   data_pkt   
      Packet Interarrival Time   ???   J   exponential (.01)   J      Packet Size   ???   
   constant (1000)   
      
Start Time   ???   
?A???n?????   
   
   $  ?  ?   
       
   
Rx_antenna   
          	isotropic                  ????          @f?     ????                  ????                  ????                  ????             antenna                       
nd_antenna         &  $   ?          
   data_generator   
       
   MAC_PROC_OrdinaryNode   
          queue                   begsim intrpt   ???   
   ????   
      subqueue   ???   
            count    ???   
   ????   
      list   	???   
                bit capacity   ???   
T?I?%??}????   
      pk capacity   ???   
T?I?%??}????   
   
   
       (  R   ?          
   src_CTA   
       
   simple_source   
          	processor                   Packet Format   ???   
   data_pkt   
      Packet Interarrival Time   ???   
   constant (500)   
      Packet Size   ???   
   constant (720)   
      
Start Time   ???   
        ????   
          '            ?  T   ?  ?   
       
   strm_11   
       
   src stream [0]   
       
   dest stream [0]   
          ????                  ????             ????                                           nd_packet_stream          )   $          ?    R   
       
   strm_12   
       
   src stream [0]   
       
   dest stream [0]   
          ????                  ????             ????                                           nd_packet_stream          .   !   &      ?   ?  $   ?   
       
   strm_13   
       
   src stream [0]   
       
   dest stream [1]   
          ????                  ????             ????                                           nd_packet_stream          /   &           ?   ?  N   
       
   strm_14   
       
   src stream [0]   
       
   dest stream [0]   
          ????                  ????             ????                                           nd_packet_stream          1      &     ?  S  #   ?   
       
   strm_15   
       
   src stream [0]   
       
   dest stream [0]   
          ????                  ????             ????                                           nd_packet_stream          4   (   &     P   ?  )   ?   
       
   strm_16   
       
   src stream [0]   
       
   dest stream [2]   
          ????                  ????             ????                                           nd_packet_stream         8      &     }  R  &   ?          
   stat_0   
       
   channel [0]   
       
   busy   
       
   
instat [0]   
          ????                  ????             ????             ????              ????              ????          ԲI?%??}????          ԲI?%??}????             0????                                           nd_statistic_wire         9      &      ?  I     ?          
   stat_1   
       
   channel [0]   
       
   queue size (packets)   
       
   
instat [1]   
          ????                  ????             ????             ????              ????              ????          ԲI?%??}????          ԲI?%??}????             0????                                           nd_statistic_wire      )   :      0data_generator.subqueue [0].queue size (packets)   queue size (packets)           queue   !bucket/default total/time average   linear   queue   /data_generator.subqueue [0].queuing delay (sec)   queuing delay (sec)           queue    bucket/default total/sample mean   linear   queue   &radio_Tx.channel [0].queue size (bits)   queue size (bits)           radio transmitter   !bucket/default total/time average   linear   radio transmitter   (radio_Tx.channel [0].queuing delay (sec)   queuing delay (sec)           radio transmitter    bucket/default total/sample mean   linear   radio transmitter   radio_Rx.channel [0].busy   busy           radio receiver   
all values   square-wave   radio receiver   %radio_Rx.channel [0].collision status   collision status           radio receiver   
all values   square-wave   radio receiver   *radio_Rx.channel [0].throughput (bits/sec)   throughput (bits/sec)           radio receiver   bucket/default total/sum_time   linear   radio receiver   radio_Tx.channel [0].busy   busy           radio transmitter   
all values   square-wave   radio transmitter   *radio_Tx.channel [0].throughput (bits/sec)   throughput (bits/sec)           radio transmitter   bucket/default total/sum_time   linear   radio transmitter    src_RCAP.Traffic Sent (bits/sec)   Traffic Sent (bits/sec)           	Generator   bucket/default total/sum_time   linear   	Generator   Aradio_Tx.channel [0].# of receiver channels passing closure stage   ,# of receiver channels passing closure stage           radio transmitter   
all values   sample-hold   radio transmitter                    