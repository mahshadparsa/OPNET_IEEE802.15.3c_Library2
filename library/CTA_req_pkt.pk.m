MIL_3_Tfile_Hdr_ 145A 94B modeler 40 48A19F19 58B54134 5 DESKTOP-FFL2R3R user 0 0 none none 0 0 none F719A8C4 2B22 0 0 0 0 0 0 1e80 8                                                                                                                                                                                                                                                                                                                                                                                                        ????             0x0             NONE                                    (              
   Preamble   
                        integer             signed, big endian          
   384   
       ????             set             NONE             *????          J   1   J                ?              
   
PHY_header   
                        integer             signed, big endian          
   16   
       ????             set             NONE             *????          J   2   J                ?              
   protocol_version   
                        integer          
   unsigned, little endian   
       
   3   
       ????             set             NONE          
@???????   
       J   3   J                    pk_field      P   ?              
   
frame_type   
       
      @Frame Type field indicates the type of frame that is being sent.   
          integer          
   unsigned, little endian   
       
   3   
       ????             set             NONE          
@???????   
       J   4   J                    pk_field      ?   ?              
   SEC   
                        integer          
   unsigned, little endian   
       
   1   
       ????             set             NONE          
@???????   
       J   5   J                    pk_field      ?   ?              
   
ACK_policy   
       
      ACK Policy field is used to indicate the type of acknowledgement procedure that the addressed recipient is required to perform.   
          integer          
   unsigned, little endian   
       
   2   
       ????             set             NONE          
@ ??????   
       J   6   J                    pk_field        ?              
   retry   
       
      ?The Retry bit shall be set to one in any data or command frame that is a retransmission of an earlier frame. It shall be set to zero in all other frames.   
          integer          
   unsigned, little endian   
       
   1   
       ????             set             NONE          
@???????   
       J   7   J                    pk_field     @   ?              
   	more_data   
       
     pMore Data bit shall be set to zero if the DEV will not use the rest of the channel time in that CTA. It shall be set to zero in the last frame of an extended beacon and in a beacon frame that is not part of an extended beacon. In all other cases the More Data bit shall be set to one. This includes frames, other than the last one, that are part of an extended beacon.   
          integer          
   unsigned, little endian   
       
   1   
       ????             set             NONE          
@ ??????   
       J   8   J                    pk_field     |   ?              
   imp_ACK_request   
                        integer          
   unsigned, little endian   
       
   1   
       ????             set             NONE          
@???????   
       J   9   J                    pk_field     ?   ?              
   imp_ACK_NAK   
       
      ]Imp-ACK NAK field shall be set to one by a DEV when all of the following conditions are true:   ?? The DEV is responding to a frame addressed to the DEV for which it has successfully received the MAC header with an ACK policy of Imp-ACK.   )? The FCS checkfor the frame body failed.   6The Imp-ACK NAK field shall be set to zero otherwise.    
          integer          
   unsigned, little endian   
       
   1   
       ????             set             NONE          
@???????   
       J   10   J                    pk_field     ?   ?              
   CTA_relinquish   
       
      ?CTA Relinquish field shall be set to one when the DEV relinquishes CTA ownership to another DEV. It shall be set to zero otherwise.    
          integer          
   unsigned, little endian   
       
   1   
       ????             set             NONE          
@? ?????   
       J   11   J                    pk_field     0   ?              
   	reserved0   
                        integer          
   unsigned, little endian   
       
   2   
       ????             set             NONE          
@???????   
       J   12   J                    pk_field                      
   PNID   
       
      FPNID field contains the unique identifier for the piconet. It normally   ?remains constant during the current instantiation of the piconet and may be persistent for multiple sequential instantiations of the piconet by the same PNC.    
          integer          
   unsigned, little endian   
       
   16   
       ????             set             NONE          
@ ??????   
       J   13   J                    pk_field        T              
   dest_ID   
       
      7DestID field is used to indicate the destination DEVID.   
          integer          
   unsigned, little endian   
       
   8   
       ????             set             NONE          
@ ??????   
       J   14   J                    pk_field      d  T              
   src_ID   
       
      2src_ID field is used to indicate the source DEVID.   
          integer          
   unsigned, little endian   
       
   8   
       ????             set             NONE          
@ ?@????   
       J   15   J                    pk_field        ?              
   MSDU_number   
       
      YThe MSDU Number field indicates the sequence number of the current MSDU or command frame.   JEach MSDU number counter shall be set to zero when the DEV is initialized.   
          integer          
   unsigned, little endian   
       
   9   
       ????             set             NONE          
@?? ????   
       J   16   J                    pk_field      n  ?              
   fragment_number   
       
      ?The Fragment Number field indicates the order of the current fragment within the current MSDU. The Fragment Number field shall be set to zero in all unfragmented frames.   
          integer          
   unsigned, little endian   
       
   7   
       ????             set             NONE          
@?? ????   
       J   17   J                    pk_field      ?  ?              
   last_fragment_number   
       
      ?The Last Fragment Number field indicates the total number of fragments within the current MSDU. The value of this field is equal to one less than the number of fragments.   ?The Last Fragment Number field shall be the same for every fragment of a fragmented MSDU and shall be set to zero for all unfragmented MSDUs.   
          integer          
   unsigned, little endian   
       
   7   
       ????             set             NONE          
@?  ????   
       J   18   J                    pk_field      ?  ?              
   	reserved1   
                        integer          
   unsigned, little endian   
       
   1   
       ????             set             NONE          
@?@@????   
       J   19   J                    pk_field        ?              
   stream_index   
                        integer          
   unsigned, little endian   
       
   8   
       ????             set             NONE          
@??@????   
       J   20   J                    pk_field        &              
   command_type   
                        integer             signed, big endian          
   16   
       ????             set             NONE             *????          J   21   J                    pk_field        b              
   length   
                        integer             signed, big endian          
   16   
       ????             set             NONE             *????          J   22   J                    pk_field      
  ?              J   CTRqB_Desired_NumberOfTUs   J                        integer             signed, big endian          J   8   J       ????             set             NONE             *????          J   23   J                    pk_field      ?  ?              J   CTRqB_Min_NumberOfTUs   J                        integer             signed, big endian          J   8   J       ????             set             NONE             *????          J   24   J                    pk_field      ?  ?              J   CTRqB_CTRqTU   J                        integer             signed, big endian          J   16   J       ????             set             NONE             *????          J   25   J                    pk_field     ?  ?              J   CTRqB_CTA_rate_factor   J                        integer             signed, big endian          J   16   J       ????             set             NONE             *????          J   26   J                    pk_field     N  ?              J   CTRqB_CTRqcontrol   J                        integer             signed, big endian          J   8   J       ????             set             NONE             *????          J   27   J                    pk_field     ?  ?              J   CTRqB_Stream_requestID   J                        integer             signed, big endian          J   8   J       ????             set             NONE             *????          J   28   J                    pk_field     H  ?              J   CTRqB_DSPsetindex   J                        integer             signed, big endian          J   8   J       ????             set             NONE             *????          J   29   J                    pk_field     ?  ?              J   CTRqB_Target_IDlist   J                        integer             signed, big endian          J   8   J       ????             set             NONE             *????          J   30   J                    pk_field     $  ?              J   CTRqB_Num_targets   J                        integer             signed, big endian          J   8   J       ????             set             NONE             *????          J   31   J                    pk_field   