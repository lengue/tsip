NetTelMain.h   
  #ifndef   NetTelMainH   
  #define   NetTelMainH   
  //---------------------------------------------------------------------------   
  #include   <Classes.hpp>   
  #include   <Controls.hpp>   
  #include   <StdCtrls.hpp>   
  #include   <Forms.hpp>   
  #include   <ComCtrls.hpp>   
  #include   <ExtCtrls.hpp>   
  #include   <Menus.hpp>   
  #include   <ScktComp.hpp>   
  #include<math.h>   
  #include<MMSystem.h>   
  #include<Buttons.hpp>   
  #include   <NMSTRM.hpp>   
  #include   <Psock.hpp>   
  #include<ToolWin.hpp>   
    
  #define   NETTEL_PORT   2001   
  //---------------------------------------------------------------------------   
  class   TNetTelForm   :   public   TForm   
  {   
  __published: //   IDE-managed   Components   
                  TServerSocket   *ServerSocket1;   
                  TClientSocket   *ClientSocket1;   
                  TMainMenu   *MainMenu1;   
                  TLabel   *Label_IP;   
                  TEdit   *Edit_IP;   
                  TPanel   *Panel1;   
                  TGroupBox   *VolumeGroupBox;   
                  TTrackBar   *VolumeBar;   
                  TStatusBar   *StatusBar1;   
                  TMenuItem   *N1;   
                  TMenuItem   *N2;   
                  TMenuItem   *N3;   
                  TMenuItem   *N4;   
                  TMenuItem   *N5;   
                  TMenuItem   *N6;   
                  TMenuItem   *N7;   
                  void   __fastcall   ServerSocket1ClientRead(TObject   *Sender,   
                      TCustomWinSocket   *Socket);   
                  void   __fastcall   N2Click(TObject   *Sender);   
                  void   __fastcall   N3Click(TObject   *Sender);   
                  void   __fastcall   N4Click(TObject   *Sender);   
                  void   __fastcall   N5Click(TObject   *Sender);   
                  void   __fastcall   N6Click(TObject   *Sender);   
                  void   __fastcall   VolumeBarChange(TObject   *Sender);   
  private:   
              WAVEFORMATEX   pcmWaveFormat;   
              HWAVEIN   phwi;   
              LPWAVEHDR   lpInWaveHdr[2];   
              bool   IsNeedSwap;   
              HWAVEOUT   pwho;   
              LPWAVEHDR   lpOutWaveHdr[2];   
              DWORD   Volume;   
              void   _fastcall   InitializeOutputDevice();   
              void   _fastcall   CallBackPlay(TMessage*Message);   
              void   _fastcall   InitializeInputDevice();   
    
  public:   
                  __fastcall   TNetTelForm(TComponent*   Owner);   
                  BEGIN_MESSAGE_MAP   
                    MESSAGE_HANDLER(MM_WIM_DATA,TMessage*   ,CallBackPlay)   
                END_MESSAGE_MAP(TComponent)   
  };   
  //---------------------------------------------------------------------------   
  extern   PACKAGE   TNetTelForm   *NetTelForm;   
  //---------------------------------------------------------------------------   
  #endif   
    
    
    
  nettelmain.cpp   
    
  #include   <vcl.h>   
  #pragma   hdrstop   
    
  #include   "NetTelMain.h"   
  #include   "WaveInput.h"   
  #include   "WaveOutPut.h"   
  //---------------------------------------------------------------------------   
  #pragma   package(smart_init)   
  #pragma   resource   "*.dfm"   
  TNetTelForm   *NetTelForm;   
  //---------------------------------------------------------------------------   
  __fastcall   TNetTelForm::TNetTelForm(TComponent*   Owner)   
                  :   TForm(Owner)   
  {   
  }   
  //---------------------------------------------------------------------------   
    void   _fastcall   TNetTelForm::InitializeInputDevice()   
  {   
        //set   up   pcm   wave   format   for   11   kHz   8-bit   mono.   
  pcmWaveFormat.wFormatTag=WAVE_FORMAT_PCM;   
  pcmWaveFormat.nChannels=1;   
  pcmWaveFormat.nSamplesPerSec=11025L;   
  pcmWaveFormat.nAvgBytesPerSec=11025L;   
  pcmWaveFormat.nBlockAlign=1;   
  pcmWaveFormat.wBitsPerSample=8;   
  pcmWaveFormat.cbSize=0;   
  //open   the   output   device   
    
  switch(waveInOpen(&phwi,WAVE_MAPPER,&pcmWaveFormat,(unsigned   long)Handle,0,CALLBACK_WINDOW))   
  {   case   MMSYSERR_ALLOCATED:   
                  ShowMessage("MMSYSERR_ALLOCTEC");return;   
    case   MMSYSERR_BADDEVICEID:   
                  ShowMessage("MMSYSERR_BADDEVICEID");return;   
  case   MMSYSERR_NODRIVER:   
                  ShowMessage("MMSYSERR_NODRIVER");return;   
  case   WAVERR_STILLPLAYING:   
                  ShowMessage("WAVERR_STILLPLAYING");return;   
  case   WAVERR_BADFORMAT:   
                  ShowMessage("WAVERR_BADFORMAT");return;   
                  }   
  //   ALLOCATE   MEMORY   AND   PERPARE   FOR   RECORDING   
  for(int   i=0;i<=1;i++)   
  {lpInWaveHdr[i]=AllocateInputLPWAVEHDRMemory(4*1024);   
    switch(waveInPrepareHeader(phwi,lpInWaveHdr[i],sizeof(WAVEHDR)))   
    {case   MMSYSERR_INVALHANDLE:   
                ShowMessage("MMSYSERR_INVALHANDLE");return;   
                case   MMSYSERR_NODRIVER:   
                ShowMessage("MMSYSERR_NODRIVER");return;   
                case   MMSYSERR_NOMEM:   
                ShowMessage("MMSYSERR_NOMEM");return;   
                                                                      }   
                                                                      }   
  switch(vaeinAddBuffer(phwi,lpinWaveHdr[!IsNeedSwap],sizeof(WAVEHDR)))   
  case   WAVERR_UNPREPAPERD:   
                ShowMessage("WAVE_UNPREPAPERD");return;   
  case   MMSYSERR_INVALHANDLE:   
                ShowMessage("MMSYSERR_INVALHANDLE");return;   
  case   MMSYSERR_NODRIVER:   
                ShowMessage("MMSYSERR_NODRIVER");return;   
  case   MMSYSERR_NOMEM:   
                ShowMessage("MMSYSERR_NOMEM");return;   
                                                                      }   
    
                                                                      }   
  //------------------------------------------------   
  void   _fastcall   TNetTelForm::CallBackPlay(TMessage   *Message)   
  {switch(vaeinAddBuffer(phwi,lpinWaveHdr[!IsNeedSwap],sizeof(WAVEHDR)))   
  case   WAVERR_UNPREPAPERD:   
                ShowMessage("WAVE_UNPREPAPERD");return;   
  case   MMSYSERR_INVALHANDLE:   
                ShowMessage("MMSYSERR_INVALHANDLE");return;   
  case   MMSYSERR_NODRIVER:   
                ShowMessage("MMSYSERR_NODRIVER");return;   
  case   MMSYSERR_NOMEM:   
                ShowMessage("MMSYSERR_NOMEM");return;   
                                                                      }   
  ClientSocket1->SendBuf(lpInWaveHdr[!IsNeedSwap]->lpData,lpInWaveHdr[!IsNeedSwap]->dwBufferLength);   
  IsNeedSwap=!IsNeedSwap;   
  }   
  //------------------------   
  void   _fastcall   TNetTelForm::InitalizeOutputDevice()   
  {if(waveOutOpen(&phwo,WAVE_MAPPER,&pcmWaveFormat,(unsigned   log)Handle,0,CALLBACK_WINDOW)!=MMSYSERR_NOERROR)   
  ::MessageBox(0,"out   Open   error","",MB_OK);   
  for(int   i=0,i<=1;i++)lpOutWaveHdr[i]=AlocateOutputLPWAVEHDRMemory(phwo,4*1024);   
  Volume=Volume&0x00000000;   
  Volume=Volume^((DWORD)0xFFFF/2);   
  Volume=Volume^((DWORD)0xFFFF/2)<<16);   
  if(waveOutSetVolume(phwo,Volume)!=MMSYSTERR_NOERROR)   
  ShowMessage("waveOutSetVolume   error!");   
  }   
    
  void   __fastcall   TNetTelForm::ServerSocket1ClientRead(TObject   *Sender,   
              TCustomWinSocket   *Socket)   
  {   
  if(OutputDeviceUnOpend)   
  {   InitializeOutputDevice();   
      OutputDeviceUnOpend=false;   
      }   
      try{ReceiveLength=Socket->receiveLength();   
      Socket->ReceiveBuf(lpOutWaveHdr[WhichtoPlay]->lpData,ReceiveLength):   
      StatusBar1->Panels->Items[1]->Text=AnsiString("Packages:")+MsgNumber++;   
      if(WaveOutWrite(phwo,lpOutWaveHdr[WhichToPlay],sizeof(WAVEHDR))==MMSYERR_NOERROR0)   
      StatusBar1->Panels->Items[0]->Text=AnsiString("Status:")+(int)WhichToPlay;   
      StatusBar1->panels->Items[2]->Text=AnsiString("PSize:")+ReceiveLength;   
      WhichToPlay=!WhichToPlay;}   
      catch(...)   
      {StatusBar1->Panels->Items[0]->Text="WaveOut   ERRor";}   
  }   
  //---------------------------------------------------------------------------   
    
  void   __fastcall   TNetTelForm::N2Click(TObject   *Sender)   
  {   
  ClientSocket1->Active=false;   
  ClientSocket1->Address=Edit_Ip->Text;   
  ClientSocket1->Port=2001;   
  ClientSocket1->Active=true;   
  }   
  //---------------------------------------------------------------------------   
    
  void   __fastcall   TNetTelForm::N3Click(TObject   *Sender)   
  {   
  IsPaused=false;   
  waveInStart(phwi);                   
  }   
  //---------------------------------------------------------------------------   
    
  void   __fastcall   TNetTelForm::N4Click(TObject   *Sender)   
  {   
  if(!IsPaused0   
  waveInStop(phwi);                   
  }   
  //---------------------------------------------------------------------------   
    
  void   __fastcall   TNetTelForm::N5Click(TObject   *Sender)   
  {   
  waveInStart(phwi);                   
  }   
  //---------------------------------------------------------------------------   
    
  void   __fastcall   TNetTelForm::N6Click(TObject   *Sender)   
  {   
  ServerSocket1->Active=false;   
  ClientSocket1->Active=false;   
  IsPaused=true;   
  waveInReset(phwi);   
  waveInStop(phwi);   
  for(int   i=0;i<=1;i++)   FreeInPutBuffer(phwi,lpInWaveHdr[i]);   
  waveInClose(phwi);   
  waveOutReset(phwo);   
  waveOutBreakLoop(phwo);   
  for(int   i=0;i<=1;i++)FreeOutPutBuffer(phwo,lpOutWaveHdr[i]);   
  waveOutClose(phwo);   
  StatusBar1->Panels->Items[0]->Text="Is   Stoped";   
  }   
  //---------------------------------------------------------------------------   
    
  void   __fastcall   TNetTelForm::VolumeBarChange(TObject   *Sender)   
  {   
  Volume=Volume&0x00000000;   
  Volume=Volume^((DWORD)VolumeBar->Position*0xFFFF/100);   
  Volume=Volume^((DWoRD)VolumeBar->Position*0xFFFF/100<<16);   
  if(waveOutSetVolume(phwo,Volume)!=MMSYSTERR_NOERROR)   
  ShowMessage("waveoutsetvolumeerror");                   
  }   
    
    
  WaveInput.H   
    
    
  //---------------------------------------------------------------------------   
  #ifndef   WaveInputH   
  #define   WaveInputH   
  #include<MMSystem.h>   
  //----------   
    
  LPWAVEHDR   AllocateInputLPWWAVEHDRMemory(UINT   size);   
  void   FreeInPutBuffer(HWAVEIN   hwi,LPWAVEHDR   pwh);   
  //------------------   
  #endif   
    
    
  WaveInput.cpp   
    
  #include   <vcl.h>   
  #pragma   hdrstop   
  #include   "WaveInput.h"   
  //-------------------------------------------------------------------   
  LPWAVEHDR   AllocateInputLPWAVEHDRMemory(UINT   size)   
  {HPSTR   sublpData;   
    HGLOBAL   lpmemory;   
  LPWAVEHDR   lpInWaveHdr;   
  lpmemory=GloblAlloc(GMEM_MOVEABLE|GMEM_SHARE,size);   
  if(lpmemory==NULL)   
        ShowMessage("Failed   to   GloabalAlloc   memory   for   data   sublpData");   
    if((sublpData=(HPSTR)GlobalLock(lpmemory))=NULL)   
  {::MessageBox(0,"Failed   to   lock   memory   for   data   sublpData",NULL,MB_OK|MB_ICONEXCLAMATION);   
  }   
  lpmemory=GlobalAlloc(GMEM_MOVEABLE|GMEM_SHARE,(DWORD)sizeof(WAVEHDR));   
  if(lpmemory==NULL)ShowMessage("Failed   to   globalAlloc   memory   for   data   LPWAVEHDR");   
  lpInWaveHdr=(LPWAVEHDR)GlobalLock(lpmemory);   
  if(lpInWaveHdr==NULL)   
  {::MessageBox(0,"failed   to   lock   memory   for   header:lplpInWaveHdr",NULL,MB_OK|MB_ICONEXCLAMATION);   
  }   
  lpInWaveHdr->lpData=SublpData;   
  lpInWaveHdr->dwBufferLength=size;   
  lpIWaveHdr->dwLoops=0L;   
  lpInWaveHdr->dwFlags=0L;   
  sublpData=NULL;   
  lpmemory=NULL;   
  return(lpInWaveHdr);   
  }   
  //--------------------------   
  void   FreeInPutBuffer(HWAVEIN   hwi,LPWAVEHDR   pwh)   
  {   
    if(waveInUnperpareHeader(hwi,pwh,sizeof(WAVEHDR))!=MMSYSERR_NOERROR)   
      ShowMessage("WaveInUnperpareHeader   ERROR!");   
      try   
      {   
          delete   pwh->lpData;   
          delete   pwh;   
          }   
          catch(...)   
          {ShowMessage("delete   pwh   error!");   
          }   
          }   
          //----------------------------------------   
    
    
  Waveout.h   
    
    
  #ifndef   WaveOutPutH   
  #define   WaveOutPutH   
  #include<MMSystem.h>   
  //-----------------------   
  LPWAVEHDR   AllocateOutPutLPWAVEHDRMemory(HWAVEOUT   phwo,UINT   size);   
  void   FreeOutPutBuffer(HWAVEOUT   hwo,LPWAVEHDR   pwh);   
  //----------------------------   
  #endif   
    
    
  Waveout.cpp   
  #include<vcl.>   
  #pragam   hdrstop   
  #include   "WaveOutPut.h"   
  //------------------------   
  LPWAVEHDR   AllocateOutPutLPWAVEHDRMemory(HWAVEOUT   phwo,UINT   size)   
  {   
      LPWAVEHDR   lpOutWaveHdr;   
      lpOutWaveHdr=(LPWAVEHDR)GlobalLock(GlobalAlloc(GMEM_MOVEABLE|GMEM_SHARE,sizeof(WAVEHDR)));   
      if(lpOutWaveHdr==NULL)   
      ::MessageBox(0,"Failed   to   lock   memory   for   header",NULL   ,MB_OK|MB_ICONEXCLAMATION);   
      if((lpOutWaveHdr->lpData=(HPSTR)GlobalLock(GlobalAlloc(GMEM_MOVEABLE|GMEM_SHARE,size)))==NULL)   
      ::MessageBox(0,"Failed   to   lock   memory   for   data   chumk",NUL,MB_OK|MB_ICONEXCLAMATION);   
      lpOutWaveHdr->dwBufferLength=size;   
      lpOutWaveHdr->dwFlags=0L;   
      lpOutWaveHdr->dwLoops=0L;   
      waveOutPrepareHeader(phwo,lpOutwaveHdr,sizeof(WAVEHDR));   
      return(lpOutWaveHdr);   
      }   
  //---------------------------------------------------------------------   
  void   FreeOutPutBuffer(HWABEOUT   hwo,LPWAVEHDR   pwh)   
  {   
      switch(waveOutUnprepareHeader(hwo,pwh,sizeof(WAVEHDR)))   
      {case   MMSYSERR_INVALHANDLE:   
                    ShowMessage("MMSYSERR_INVALHANDLE");return;   
        case   MMSYSERR_NODRIVE:   
                    ShowMessage("MMSYSERR_NODRIVE");return;   
      case   MMSYSERR_NOMEM:   
                    ShowMessage("MMSYSERR_NOMEM");return;   
      case   WAVERR_STILLPLAYING:   
                    ShowMessage("WAVERR_STILLPLAYING");return;   
                    }   
    try{   
    delete   pwh->lpData;   
    delete   pwh;   
    }   
    catch(...)   
    {ShowMessage("delete   'pwh'error!");   
    }   
    }   