#ifdef    TRACE_CAINFO_ENABLE
#define  MGCARD_DRIVER_DEBUG
#endif

/******************************************************************************
*
* File : MgCardDrv.C
*
* Description : SoftCell driver
*
*
* NOTES :
*
*
* Author :
*
* Status : 
*
* History : 0.0   2004-06-07  Start coding
*           
*
* Copyright: Changhong 2004 (c)
*
*****************************************************************************/
/*****************************************************************************
 *BASIC DEFINITIONS
 *****************************************************************************/
#include  "MgCardDrv.h"



/*****************************************************************************
 *interface description
 *****************************************************************************/
/*****************************************************************************
 *Function name: MGDDICardGetMGReaders
 *            
 *
 *Description: This function returns the list of handles of all MG card
 *             readers.
 *
 *Prototype:
 *      TMGDDIStatus  MGDDICardGetMGReaders    
 *                    ( TMGSysSCRdrHandle* lhReaders, ubyte*  nReaders )
 *
 *input:
 *      lhReaders:  Address of the list of MG card reader handle.
 *      nReaders:   Address of the number of MG card readers(maximum 255)
 *
 *output:
 *            
 *            
 *
 *return value
 *      MGDDIOK:         The list of card readers has been filled      
 *      MGDDIBadParam:   The nReaders parameter is incorrect       
 *      MGDDINoResource: The size of the memory block is insufficient      
 *      MGDDIError:      The list of MG card readers is unavailable      
 *************************************************************************/
TMGDDIStatus MGDDICardGetMGReaders(TMGSysSCRdrHandle* lhReaders, ubyte* nReaders)
{
      TMGDDIStatus     StatusMgDdi;

      StatusMgDdi =  (TMGDDIStatus)CHCA_GetMGCardReaders((TCHCASysSCRdrHandle *)lhReaders,(CHCA_UCHAR*)nReaders);

      return (StatusMgDdi);	  	

}


/*******************************************************************************
 *Function name: MGDDICardOpen
 * 
 *
 *Description: This function opens the MG card reader identified by the hReader 
 *                   handle.  
 *
 *Prototype:
 *      TMGDDICardReaderHandle  MGDDICardOpen( TMGSysSCRdrHandle  hReader )             
 *
 *input:
 *      hReader:   MG card reader system handle
 * 
 *
 *output:
 *
 *Return Value:
 *     Handle of a DDI card reader instance if not null. 
 *     NULL,if the MG card reader system handle is not valid
 *
 *Comments:
 *     1) Exclusivity of the MG card reader does not prevent the use of this function.
 *     2) The fact is that two applications using the same MG card reader must not 
 *        have the same DDI card reader instance handle for this MG card reader.
 *******************************************************************************/
TMGDDICardReaderHandle  MGDDICardOpen( TMGSysSCRdrHandle  hReader )
{
        TMGDDICardReaderHandle     iCardReaderHandle = NULL;

        iCardReaderHandle = (TMGDDICardReaderHandle)CHCA_CardOpen((TCHCASysSCRdrHandle)hReader);  
		   
        return (iCardReaderHandle);
}





/*******************************************************************************
 *Function name: MGDDICardClose
 *
 *
 *Description: this function closes the logical channel to a MG card reader 
 *             identified by the hCard handle
 *
 *Prototype
 *      TMGDDIStatus  MGDDICardClose( TMGDDICardReaderHandle  hCard)
 *
 *
 *input:
 *      hCard:  DDI card reader instance handle.
 * 
 * 
 *
 *output:
 *
 *Return value:
 *      MGDDIOk :               DDI card reader instance close. 
 *      MGDDIBadParam:     DDI card reader instance handle unknown
 *      MGDDIError:             Other error.
 *
 * 
 *
 *Comment:
 *       1) if this MG card reader instance has locked temporarily the MG card 
 *          reader or have been granted exclusivity to it,the locking or 
 *          exclusivity is automatically canceled
 *       2)Exclusivity of the MG card reader does not prevent the use of this function
 *******************************************************************************/
 TMGDDIStatus  MGDDICardClose( TMGDDICardReaderHandle  hCard)
 {
         TMGDDIStatus                 StatusMgDdi;

         StatusMgDdi = (TMGDDIStatus)CHCA_CardClose((TCHCADDICardReaderHandle)hCard);

         return(StatusMgDdi);
 
 }


/*********************************************************************
 *Function name: MGDDICardSubscribe
 *            
 *
 *Description:This function is used to subscribe to the different 
 *            unexpected events that can be generated by the MG
 *            card reader associated to the hCard handle. 
 *Prototype:
 *      TMGDDIEventSubscriptionHandle  MGDDICardSubscribe    
 *           (TMGDDICardReaderHandle  hCard, CALLBACK hCallback)        
 *
 *input:
 *      hCard:     DDI card reader instance handle.
 *      hCallback: Callback function handle.
 *
 *output:
 *            
 *            
 *
 *return value
 *      Event subscription handle, if not null.      
 *      NULL, if a problem has occurred when recording.      
 *            - Unknown DDI card reader instance handle. 
 *            - Null callback function address
 *            
 *comment:
 *      1) Exclusivity of the MG card reader dones not prevent the use of 
 *         this function
 *
 *
 *      2) these events are as follows
 *         MGDDIEvCardInsert:       insertion of a smart card in the reader
 *         MGDDIEvCardExtract:      extraction of a smart card from the reader
 *         MGDDIEvCardResetRequest: request to (re)initialize the reader smart card
 *         MGDDIEvCardReset:        (re)initialization of the reader smart card 
 *         MGDDIEvCardProtocol:     change of smart card protocol.
 ***********************************************************************/
 TMGDDIEventSubscriptionHandle  MGDDICardSubscribe
	 (TMGDDICardReaderHandle  hCard, CALLBACK hCallback)
 {
          TMGDDIEventSubscriptionHandle        SubscriptionHandle; 

	   CHCA_UCHAR                                   SubHandle[4];	  
          WORD2BYTE                                      iSubTemp;
		  
	    if(CHCA_CardSubscribe((TCHCADDICardReaderHandle)hCard,hCallback,SubHandle))
	    {
                 return NULL;
	    }
	    else
	    {
                 iSubTemp.byte.ucByte0=SubHandle[0];
		   iSubTemp.byte.ucByte1=SubHandle[1];
		   iSubTemp.byte.ucByte2=SubHandle[2];
		   iSubTemp.byte.ucByte3=SubHandle[3];
		
                SubscriptionHandle = (TMGDDIEventSubscriptionHandle)iSubTemp.uiWord32;
	    }

		 
          /* SubscriptionHandle = (TMGDDIEventSubscriptionHandle)CHCA_CardSubscribe((TCHCADDICardReaderHandle)hCard,hCallback);*/
		  
          return  SubscriptionHandle;
	  
 }                           


/*******************************************************************************
 *Function name: MGDDICardIsEmpty
 * 
 *
 *Description: This function indicates whether the MG card reader associated to 
 *             the hCard handle has a smart card inserted in it, or not. 
 *
 *Prototype:
 *     TMGDDIStatus MGDDICardIsEmpty( TMGDDICardReaderHandle hCard)
 *
 *input:
 *     hCard:   DDI card reader instance handle.
 * 
 *
 *output:
 *
 *Return Value:
 *      MGDDIOK:              A smart card is present in the MG card reader.      
 *      MGDDIBadParam:    The DDI card reader instance handle is unknown.       
 *      MGDDINotFound:     There is no smart card in the MG card reader.      
 *      MGDDIError:           Other error      
 *
 *
 *Comments:
 *      Exclusivity of the MG card reader does not prevent the use of this 
 *      function 
 *******************************************************************************/
 TMGDDIStatus MGDDICardIsEmpty( TMGDDICardReaderHandle hCard)
 {
         TMGDDIStatus                 StatusMgDdi;

         StatusMgDdi = (TMGDDIStatus)CHCA_CardEmpty((TCHCADDICardReaderHandle)hCard);

	  return (StatusMgDdi);
    
 }






/*******************************************************************************
 *Function name: MGDDICardSetExclusive
 *
 *
 *Description: This function requests the exclusivity of the MG card reader related
 *             to the hCard handle.The exclusivity is granted if no application has 
 *             been granted it.    
 *
 *             if smart card reader exclusivity is granted,no other application than 
 *             the one using the hCard handle will be able to use the reader
 *
 *        
 *Prototype
 *     TMGDDIStatus  MGDDICardSetExclusive( TMGDDICardReaderHandle hCard )
 *
 *
 *input:
 *     hCard:   DDI card reader instance handle.
 * 
 * 
 *
 *output:
 *
 *Return value:
 *    MGDDIOk:               The exclusivity has been granted. 
 *    MGDDIBadParam:    The DDI card reader instance handle is unknown.
 *    MGDDINoResource:  Exclusive use of the smart card reader has already been 
 *                                  granted.   
 *    MGDDIError:       Other error.
 * 
 *******************************************************************************/
 TMGDDIStatus  MGDDICardSetExclusive( TMGDDICardReaderHandle  hCard )
 {
        TMGDDIStatus                 StatusMgDdi;
		
         StatusMgDdi = (TMGDDIStatus)CHCA_CardSetExclusive((TCHCADDICardReaderHandle)hCard);
		 
         return  (StatusMgDdi);
 }


/*********************************************************************************
 *Function name: MGDDICardClearExclusive
 *            
 *
 *Description: This function cancels the exclusivity of the MG card reader related 
 *             to the hCard handle. The exclusivity must have been granted previously  
 *             by the mean of MGDDICardSetExclusive interface. 
 *   
 *Prototype:
 *     TMGDDIStatus  MGDDICardClearExclusive( TMGDDICardReaderHandle  hCard)     
 *          
 *
 *input:
 *
 *output:
 *            
 *            
 *
 *return value
 *       MGDDIOk:                   The exclusivity has been canceled.     
 *       MGDDIBadParam:        The DDI card reader instance handle is unknown.    
 *       MGDDINoResource:      Exclusive use of the reader has been granted to 
 *                                          another application.
 *       MGDDIError:        Other error(reader is not locked for exclusivity)     
 *********************************************************************************/
TMGDDIStatus  MGDDICardClearExclusive( TMGDDICardReaderHandle  hCard)
{
        TMGDDIStatus                 StatusMgDdi;
	 StatusMgDdi = (TMGDDIStatus)CHCA_CardClearExclusive((TCHCADDICardReaderHandle)hCard);	
	 return (StatusMgDdi);	

}






/*******************************************************************************
 *Function name: MGDDICardReset
 * 
 *
 *Description: This function is used to request reset of a smart card inserted in
 *             the MG card reader related to the hCard handle.Correct or incorrect
 *             execution of reset procedure is notified later to the subscribing 
 *             instances. This notification comprises MGDDIEvCardResetRequest and  
 *             MGDDIEvCardReset events. If possible, the latter event give the 
 *             entire card ATR and, failing this,the card historic if the ATR is not
 *             available for a setup reason
 *  
 *Prototype:
 *     TMGDDIStatus  MGDDICardReset( TMGDDICardReaderHandle  hCard)
 *
 *input:
 *     hCard:   DDI card reader instance handle.
 * 
 *
 *output:
 *
 *Return Value:
 *     MGDDIOk:              The smart card reset request has been accepted
 *     MGDDIBadParam:   The DDI card reader instance handle is unknown.
 *     MGDDINoResource: Exclusive use of the reader has been granted to 
 *                                  another application[2]. 
 *     MGDDINotFound:    Empty MG card reader or smart card insert not ready
 *     MGDDIError:          interface execution error
 *
 *Comments:
 *     1) These events must be generated, even for the instance calling this function
 *     2) Exclusivity of the MG card can prevent the use of this function
 *
 *******************************************************************************/
 TMGDDIStatus  MGDDICardReset( TMGDDICardReaderHandle  hCard)
 {
        TMGDDIStatus                 StatusMgDdi = MGDDIError;

	 StatusMgDdi = (TMGDDIStatus)CHCA_CardReset((TCHCADDICardReaderHandle)hCard);	 
     
	 return(StatusMgDdi);
 }


/*******************************************************************************
 *Function name: MGDDICardGetCaps
 *
 *
 *Description: This function indicates the possibilities of the smart card inserted  
 *             in the MG card reader related to the hCard handle.The information is 
 *             placed in the information structure provided at the address given in  
 *             the pCaps parameter.
 *
 *Prototype
 *     TMGDDIStatus  MGDDICardGetCaps
 *            ( TMGDDICardReaderHandle hCard, TMGDDICardCaps* pCaps)
 *
 *input:
 *     hCard:    DDI card reader instance handle 
 *     pCaps:    Address of information structure to be filled
 * 
 *
 *output:
 *
 *Return value:
 *     MGDDIOk:        The protocol information has been recovered.
 *     MGDDIBadParam:  DDI card reader instance handle unknown or parameter incorrect.
 *     MGDDINotFound:  Empty MG card reader or smart card inserted not ready.
 *     MGDDIError:     Information unavailable.
 * 
 *Comments: 
 *     1) Exclusivity of the MG card reader does not prevent the use of this function
 * 
 *******************************************************************************/
 TMGDDIStatus  MGDDICardGetCaps
        ( TMGDDICardReaderHandle hCard, TMGDDICardCaps* pCaps)
 {
         TMGDDIStatus                      StatusMgDdi;
         TCHCADDICardCaps             TempCaps;
		 
         StatusMgDdi = (TMGDDIStatus)CHCA_CardGetCaps((TCHCADDICardReaderHandle)hCard,&TempCaps); 

	  if(StatusMgDdi == MGDDIOK)	 
	  {
                pCaps->SupportedPtcl = (BitField32)TempCaps.SupportedISOProtocols;
		  pCaps->Rate = (udword)TempCaps.MaxBaudRate;
	  }
   
         return (StatusMgDdi);
 } 		


/*********************************************************************
 *Function name: MGDDICardGetProtocol
 *            
 *
 *Description: This function indicates the protocol used by the smart card
 *             inserted in the smart card reader associated to the hCard 
 *             handle.The protocol information used is placed in the protocol
 *             information structure provided at the address given in the 
 *             pProtocol parameter.
 *
 *
 *Prototype:
 *     TMGDDIStatus  MGDDICardGetProtocol     
 *           ( TMGDDICardReaderHandle  hCard, TMGDDICardProtocol* pProtocol)    
 *
 *input:
 *     hCard:       DDI card reader instance handle
 *     pProtocol:   Address of protocol information structure to be filled.
 *
 *output:
 *
 *return value
 *     MGDDIOk:         The protocol information has been recovered.                
 *     MGDDIBadParam:   DDI card reader instance handle unknown or parameter incorrect      
 *     MGDDINotFound:   Empty card reader or smart card inserted not ready.       
 *     MGDDIError:      Protocol information unavailable.       
 *            
 *Commtents:            
 *     1) In the Type field of the MGDDICardProtocol,only one bit is set,       
 *         corresponding to the protocol currently used.     
 *     2) Exclusivity of the MG card reader does not prevent the use of this function       
 ***********************************************************************/
 TMGDDIStatus  MGDDICardGetProtocol
       ( TMGDDICardReaderHandle  hCard, TMGDDICardProtocol* pProtocol)
 {
         TMGDDIStatus                                  StatusMgDdi;
	  TCHCADDICardProtocol                     TempProtocol ;	 
		 
	  StatusMgDdi = (TMGDDIStatus)CHCA_CardGetProtocol((TCHCADDICardReaderHandle)hCard,&TempProtocol);

	  if(StatusMgDdi==MGDDIOK)
	  {
	        pProtocol->Type = (TMGDDICardTxPtcl)TempProtocol.SupportedProtocolTypes;
               pProtocol->Rate = (udword)TempProtocol.CurrentBaudRate;
	  }
	  
         return (StatusMgDdi);		
 }


/*******************************************************************************
 *Function name: MGDDICardChangeProtocol
 * 
 *
 *Description: This function is usded to modify the dialog protocol with the smart
 *             card inserted in the MG card reader related to the hCard handle.This  
 *             protocol is specified by the protocol information structure provided 
 *             at the address given by bProtocol parameter.The change in the protocol
 *             is notified to the subscribing instances via th MGDDIEvProtocol event.
 *         
 *Prototype:
 *     TMGDDIStatus  MGDDICardChangeProtocol
 *            ( TMGDDICardReaderHandle hCard, TMGDDICardProtocol* pProtocol)
 *
 *input:
 *     hCard:         DDI card reader instance handle.
 *     pProtocol:     Address of information structure of protocol to be used
 *
 *output:
 *
 *Return Value:
 *     MGDDIOk:          The protocol change request will be executed
 *     MGDDIBadParam:    Parameter(s) incorrect.
 *     MGDDINoResource:  Exclusive use of the reader has been granted to another application[3]
 *     MGDDILocked:      The reader is temporarily locked by another application[4]
 *     MGDDIBusy:        The reader is currently processing a command[4]
 *     MGDDIProtocol:    Protocol unknown.
 *     MGDDINotFound:    Empty MG card reader or smart card inserted not ready 
 *     MGDDIError:       Interface execution error.
 *
 *   
 *Comments:
 *    1) This event must be generated even for the instance that initiated the change in the protocol. 
 *    2) The Type field of the MGDDICardProtocol structure must contain only a single bit,
 *       corresponding to the protocol currently used. 
 *    3) Exclusivity of the MG card reader can prevent the use of this function
 *    4) A locking sequence, a smart card exchange or a protocol change in progress may delay this 
 *       request until the end. This interface returns MGDDIOk in this case
 *
 *******************************************************************************/
 TMGDDIStatus  MGDDICardChangeProtocol
          ( TMGDDICardReaderHandle hCard, TMGDDICardProtocol* pProtocol)
 {
          TMGDDIStatus                    StatusMgDdi;
          TCHCADDICardProtocol      TempProtocol;    

          if(pProtocol == NULL)
          {
                StatusMgDdi =  MGDDIBadParam;
                 return StatusMgDdi;
	   }

          TempProtocol.CurrentBaudRate = (CHCA_UINT)pProtocol->Rate;
	   TempProtocol.SupportedProtocolTypes = (CHCA_UCHAR)pProtocol->Type; 	  

           StatusMgDdi = (TMGDDIStatus)CHCA_CardChangeProtocol((TCHCADDICardReaderHandle)hCard,&TempProtocol);
		  
          return (StatusMgDdi); 
 }





/*******************************************************************************
 *Function name: MGDDICardSend
 *
 *
 *Description: This function is used to send a command to the smart card inserted 
 *             in the smart card reader related to the hCard handle.The command is  
 *             contained in a memory block provided at the address given in the 
 *             pMsg parameter.The size this memory block is defined by the Size  
 *             parameter.The message report is notified later,by means of the  
 *             notification function provided at the address given in the hCallback 
 *             parameter,and via the MGDDIEvCardReport event.The processing command
 *             can be cancelled before completion,via the MGDDICardAbort interface,
 *             by the application handling the hCard handle.
 *
 *             No assumptions have been made as to the protocol used by the smart card.       
 *             This interface must enable a command of variable length to be sent.If the
 *             Smart card recognizes the command,it returns an acknowledgement of variable
 *             length to be sent.If the smart card recognizes the command,it returns an 
 *             acknowledgement of variable length that is notified immediately on reception.
 *
 *            
 *
 *Prototype
 *     TMGDDIStatus MGDDICardSend
 *          ( TMGDDICardReaderHandle  hCard,
 *            CALLBACK  hCallback, ubyte* pMsg,uword  Size)
 *
 *input:
 *    hCard:          DDI card reader instance handle.    
 *    hCallback:     Callback function address
 *    pMsg:            Address of memory block providing message to be sent.  
 *    Size:             Message size.
 * 
 *
 *output:
 *
 *Return value:
 *    MGDDIOK:         The message sending request will be executed.
 *    MGDDIBadParam:   DDI card reader instance handle unknown or parameter incorrect.
 *    MGDDINoResource: Exclusive use of the reader has been granted to another application
 *    MGDDILocked:     The reader is temporarily locked by another application
 *    MGDDIBusy:       The reader is currently processing a command
 *    MGDDINotFound:   Empty MG card reader or smart card inserted not ready.
 *    MGDDIError:      Interface execution error. 
 * 
 * 
 *Comments:
 *    1) Exclusivity of the MG card reader can prevent the use of this function
 *    2) A locking sequence, a smart card exchange or a protocol change in progress may 
 *       delay this request until the end.This interface returns MGDDIOK in this case
 * 
 * 
 *******************************************************************************/
 TMGDDIStatus MGDDICardSend
	  ( TMGDDICardReaderHandle  hCard,
	    CALLBACK  hCallback, ubyte* pMsg,uword  Size)
 {
          TMGDDIStatus                 StatusMgDdi;

          StatusMgDdi =(TMGDDIStatus)CHCA_CardSend((TCHCADDICardReaderHandle)hCard,hCallback,pMsg,Size);
           
           return  (StatusMgDdi);
 }


/*********************************************************************
 *Function name: MGDDICardAbort
 *            
 *
 *Description: this interface cancels the exchange processed by the 
 *             smart card inserted in the MG card reader related to the             
 *             hCard handle.If the MG card reader is locked by the DDI
 *             card reader instance aborting,the locking sequence is  
 *             cancelled.
 * 
 *Prototype:
 *     TMGDDIStatus  MGDDICardAbort( TMGDDICardReaderHandle  hCard)     
 *          
 *
 *input:
 *     hCard:    DDI card reader instance handle
 *
 *output:
 *            
 *return value
 *     MGDDIOk:          The exchange processed by the reader smart card is cancelled       
 *     MGDDIBadParam:    DDI card reader instance handle unknown.      
 *     MGDDINoResource:  Exclusive use of the reader has been granted to another       
 *                       application 
 *     MGDDILocked:      The reader is temporarily locked by another application       
 *     MGDDIBusy:        The reader is currently processing a command of another        
 *                       application 
 *     MGDDINotFound:    Empty reader or smart card inserted not ready.       
 *     MGDDIError:       Interface execution error(no exchange currently processed).
 *
 *comments:
 *     Exclusivity of the MG card reader can prevent the use of this fucntion  
 *
 ***********************************************************************/
 TMGDDIStatus  MGDDICardAbort( TMGDDICardReaderHandle  hCard)
 {
          TMGDDIStatus                 StatusMgDdi;

           StatusMgDdi =(TMGDDIStatus)CHCA_CardAbort((TCHCADDICardReaderHandle)hCard);		 
		  
	   return (StatusMgDdi);
 }


/*******************************************************************************
 *Function name: MGDDICardLock
 * 
 *
 *Description: This interface is used to send a continuous sequence of commands
 *             to the card. The group of commands to be strung together begin
 *             with a MGDDICardLock call and end with a MGDDICardUnlock call.
 *             The LockTime parameter indicates the time during which access to
 *             The MG card reader must be locked out temporarily for the 
 *             application that handles the hCard handle.This time is expressed in ms. 
 *
 *             While locked out,only the application that handles the hCard handles 
 *             can retransmit through this MG card reader.Temporary locking is 
 *             automatically cancelled by the device driver if no new command is  
 *             sent to the card after a timeout defined by the LockTime parameter on 
 *             completion of reception of the response of the last chained command. 
 *             This automatic cancellation is notified by the callback function provided 
 *             at the address given in the hCallback parameter via the MGDDIEvCardLockTimeout
 *             event.The locking sequence can be cancelled by the lock owner at any time 
 *             with MGDDICardUnlock interface.
 *
 *
 *Prototype:
 *     TMGDDIStatus  MGDDICardLock
 *           ( TMGDDICardReaderHandle  hCard, CALLBACK  hCallback, udword  LockTime)
 *
 *input:
 *     hCard:          DDI card reader instance handle.       
 *     hCallback:      Callback function address.
 *     LockTime:       Maximum duration of MG card reader locking.
 *
 *
 *output:
 *
 *Return Value:
 *     MGDDIOk:          Access to the smart card reader is unlocked.
 *     MGDDIBadParam:    Smart card reader handle unknown
 *     MGDDINoResource:  Exclusive use of the reader has been granted to another application
 *     MGDDILocked:      The reader is temporary locked by another application
 *     MGDDIBusy:        The reader is currently processing a command.
 *     MGDDINotFound:    Empty reader or smart card inserted not ready.
 *     MGDDIError:       Interface execution error.
 *
 *
 *
 *Comments:
 *     1) Exclusivity of the MG card reader can prevent the use of this function 
 *     2) A locking sequence, a smart card exchange or a protocol change in progress may
 *        delay this request until their completion.This interface returns MGDDIOk in 
 *        this case
 *
 *******************************************************************************/
 TMGDDIStatus   MGDDICardLock
	  ( TMGDDICardReaderHandle  hCard, CALLBACK  hCallback, udword  LockTime)  
 {
          TMGDDIStatus                 StatusMgDdi;

	   /*do_report(severity_info,"\n[MGDDICardLock]\n");	  */

	   StatusMgDdi	 = (TMGDDIStatus)CHCA_CardLock((TCHCADDICardReaderHandle)hCard,hCallback,LockTime); 

	   return (StatusMgDdi);
 }






/*******************************************************************************
 *Function name:  MGDDICardUnlock
 *
 *
 *Description: this interface is used to cancel temporary locking of access to the
 *             card present in the card reader related to the hCard handle.If the
 *             exchange initiating the request has not been completed,it must terminate
 *             normally              
 *
 *Prototype
 *     TMGDDIStatus  MGDDICardUnlock( TMGDDICardReaderHandle  hCard)
 *
 *
 *input:
 *     hCard:   DDI card reader instance handle.
 * 
 * 
 *
 *output:
 *
 *Return value:
 *     MGDDIOk:          Access to the smart card reader is unlocked.
 *     MGDDIBadParam:    DDI card reader instance handle unknown.
 *     MGDDINoResource:  Exclusive use of the reader has been granted to another application
 *     MGDDILocked:      The reader is temporary locked by another application
 *     MGDDINotFound:    Empty reader or smart card inserted not ready.
 *     MGDDIError:       Interface execution error.
 * 
 * 
 *******************************************************************************/
 TMGDDIStatus  MGDDICardUnlock( TMGDDICardReaderHandle  hCard)
 {
         TMGDDIStatus                 StatusMgDdi;
	  /*do_report(severity_info,"\n[MGDDICardUnlock]\n");	  */

         StatusMgDdi = (TMGDDIStatus)CHCA_CardUnlock((TCHCADDICardReaderHandle)hCard); 
		  
	  return(StatusMgDdi);	   
 }



