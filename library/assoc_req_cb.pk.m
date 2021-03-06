MIL_3_Tfile_Hdr_ 145A 94B modeler 40 48A19F19 58A4B003 4 DESKTOP-FFL2R3R user 0 0 none none 0 0 none BCD966AA 2415 0 0 0 0 0 0 1e80 8                                                                                                                                                                                                                                                                                                                                                                                                        ????             0x0             NONE                                    (              J   Preamble   J                        integer             signed, big endian          
   384   
       ????             set             NONE             *????          J   1   J                    pk_field         ?              J   
PHY_header   J                        integer             signed, big endian          
   16   
       ????             set             NONE             *????          J   2   J                    pk_field         ?              J   protocol_version   J                        integer          
   unsigned, little endian   
       
   3   
       ????             set             NONE          
@???????   
       J   3   J             P   ?              J   
frame_type   J       
      @Frame Type field indicates the type of frame that is being sent.   
          integer          
   unsigned, little endian   
       
   3   
       ????             set             NONE          
@???????   
       J   4   J             ?   ?              J   SEC   J                        integer          
   unsigned, little endian   
       
   1   
       ????             set             NONE          
@???????   
       J   5   J             ?   ?              J   
ACK_policy   J       
      ACK Policy field is used to indicate the type of acknowledgement procedure that the addressed recipient is required to perform.   
          integer          
   unsigned, little endian   
       
   2   
       ????             set             NONE          
@ ??????   
       J   6   J               ?              J   retry   J       
      ?The Retry bit shall be set to one in any data or command frame that is a retransmission of an earlier frame. It shall be set to zero in all other frames.   
          integer          
   unsigned, little endian   
       
   1   
       ????             set             NONE          
@???????   
       J   7   J            @   ?              J   	more_data   J       
     pMore Data bit shall be set to zero if the DEV will not use the rest of the channel time in that CTA. It shall be set to zero in the last frame of an extended beacon and in a beacon frame that is not part of an extended beacon. In all other cases the More Data bit shall be set to one. This includes frames, other than the last one, that are part of an extended beacon.   
          integer          
   unsigned, little endian   
       
   1   
       ????             set             NONE          
@ ??????   
       J   8   J            |   ?              J   imp_ACK_request   J                        integer          
   unsigned, little endian   
       
   1   
       ????             set             NONE          
@???????   
       J   9   J            ?   ?              J   imp_ACK_NAK   J       
      ]Imp-ACK NAK field shall be set to one by a DEV when all of the following conditions are true:   ?? The DEV is responding to a frame addressed to the DEV for which it has successfully received the MAC header with an ACK policy of Imp-ACK.   )? The FCS checkfor the frame body failed.   6The Imp-ACK NAK field shall be set to zero otherwise.    
          integer          
   unsigned, little endian   
       
   1   
       ????             set             NONE          
@???????   
       J   10   J            ?   ?              J   CTA_relinquish   J       
      ?CTA Relinquish field shall be set to one when the DEV relinquishes CTA ownership to another DEV. It shall be set to zero otherwise.    
          integer          
   unsigned, little endian   
       
   1   
       ????             set             NONE          
@? ?????   
       J   11   J            0   ?              J   	reserved0   J                        integer          
   unsigned, little endian   
       
   2   
       ????             set             NONE          
@???????   
       J   12   J                             J   PNID   J       
      FPNID field contains the unique identifier for the piconet. It normally   ?remains constant during the current instantiation of the piconet and may be persistent for multiple sequential instantiations of the piconet by the same PNC.    
          integer          
   unsigned, little endian   
       
   16   
       ????             set             NONE          
@ ??????   
       J   13   J               T              J   dest_ID   J       
      7DestID field is used to indicate the destination DEVID.   
          integer          
   unsigned, little endian   
       
   8   
       ????             set             NONE          
@ ??????   
       J   14   J             d  T              J   src_ID   J       
      2src_ID field is used to indicate the source DEVID.   
          integer          
   unsigned, little endian   
       
   8   
       ????             set             NONE          
@ ?@????   
       J   15   J               ?              J   MSDU_number   J       
      YThe MSDU Number field indicates the sequence number of the current MSDU or command frame.   JEach MSDU number counter shall be set to zero when the DEV is initialized.   
          integer          
   unsigned, little endian   
       
   9   
       ????             set             NONE          
@?? ????   
       J   16   J             n  ?              J   fragment_number   J       
      ?The Fragment Number field indicates the order of the current fragment within the current MSDU. The Fragment Number field shall be set to zero in all unfragmented frames.   
          integer          
   unsigned, little endian   
       
   7   
       ????             set             NONE          
@?? ????   
       J   17   J             ?  ?              J   last_fragment_number   J       
      ?The Last Fragment Number field indicates the total number of fragments within the current MSDU. The value of this field is equal to one less than the number of fragments.   ?The Last Fragment Number field shall be the same for every fragment of a fragmented MSDU and shall be set to zero for all unfragmented MSDUs.   
          integer          
   unsigned, little endian   
       
   7   
       ????             set             NONE          
@?  ????   
       J   18   J             ?  ?              J   	reserved1   J                        integer          
   unsigned, little endian   
       
   1   
       ????             set             NONE          
@?@@????   
       J   19   J               ?              J   stream_index   J                        integer          
   unsigned, little endian   
       
   8   
       ????             set             NONE          
@??@????   
       J   20   J               &              
   command_type   
                        integer             signed, big endian          
   16   
       ????             set             NONE             *????          J   21   J               b              
   length   
                        integer             signed, big endian          
   16   
       ????             set             NONE             *????          J   22   J               ?              
   DEV_address   
                        integer             signed, big endian          
   64   
       ????             set             NONE             *????          J   23   J                    pk_field        ?              
   Overall_capabilities   
                     
   information   
          signed, big endian          
   56   
       ????             set             NONE             *????          J   24   J                    pk_field                      
   ATP   
                     
   floating point   
       
   
big endian   
       
   16   
       ????             set             NONE             *????          J   25   J                    pk_field        >              
   DEV_utility   
                     
   information   
          signed, big endian          
   8   
       ????             set             NONE             *????          J   26   J                    pk_field   