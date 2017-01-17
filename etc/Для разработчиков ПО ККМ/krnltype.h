#ifndef _KRNLTYPE
#define _KRNLTYPE

typedef unsigned long  DWORD;
typedef unsigned char  BYTE;
typedef unsigned short WORD;

//���������, ������������ ��� ����������� ��������
#define MAX_CARD_NUM      30
#define MAX_ACCOUNT_NUM   11
#define MAX_BRANCH_NUM    11
#define MAX_CERT          16
#define MAX_NAME          38
#define MAX_DATE          10
#define MAX_TIME           8
#define MAX_AUTH_CODE      6
#define MAX_TERM           8
#define MAX_MERCHANT_LN   15
#define MAX_RRN           12
#define MAX_OPER_NAME     64
#define MAX_PAN_N         19
#define MAX_TRACK2        40
#define MAX_HASH          20
#define MAX_CARD_DATE     4
#define MAX_PASSPORT      512
#define MAX_PAYMENT_TAG   32
#define MAX_PAYMENT_VALUE 200
#define MAX_CARD_LS_DATA  800
#define MAX_TEXT_MESSAGE  700
#define MAX_CLIENT_NAME    80
#define MAX_GOODS_NAME     25
#define MAX_GOODS_CODE     24
#define MAX_CASH_STATS     24

#define MAX_FIO_N            58

//����� ���� Options
#define O_DEBIT      0x00000001
#define O_CREDIT     0x00000002
#define O_CASH       0x00000004
#define O_TRANSFER   0x00000008
#define O_SERVICE    0x00000010
#define O_INFO       0x00000020
#define O_BANKTERM   0x00000040
#define O_TRADETERM  0x00000080
#define O_SPECIAL    0x00000100

//����� ���� Flags
#define F_HAVE_DOCUMENT   0x00000001
#define F_CRITICAL        0x00000002
#define F_HAVE_MONMSG     0x00000004

#define SBER_OWN_CARD     'S'

//�������� ��������� amount ��� ������� �������� � ������� ����������� ����� ������� 17
//f.e., for call helios monitor
//sprintf(CmdLineBuf, "c:\\work\\gate.dll %d %d", 47, CMD_MONIT_H);
#define CMD_MONIT_H  1  //�������� ������� �� ���������� helios
#define CMD_MONIT_P  2	//RFU - �������� ������� �� ���������� pserverDB

#pragma pack(1) //������������ �� ������� �����
//��������� ��� �������� ���������� ������
typedef struct tagTPassportData{
  char sFIO[MAX_FIO_N];    //���
  char sAddr[58];   //�����
  short  sRes;      //��������/����������
  char sDocType[21];//��� ���������
  char sSer[11];    //�����
  char sNum[16];    //�����
  char sIssuer[58]; //��� �����
  char sWhen[13];   //����� �����
  char sValid[13];  //���� ��������
  char sEmit[42];   //���� - ������� �����
  char Agent[42];   //����, �������� �����
} TPassportData;

//��������� ��� �������� ������ � ����������� �� ����� ������� (��������)
typedef struct tagTClientTrxData{
  short TSN;              //����� �������� �� ����� �������
  char  Date[6];          //���� ��������
  char  Description[17];  //��������
  DWORD Amount;           //����� ��������
  WORD  Batch;            //����� ������ ����� ���������. ����������� ��� ������� �������� �������.
  BYTE  BatchRSN;         //����� ���������� � ������ ����� ���������. ����������� ��� ������� �������� �������.
  DWORD BalanceBefore;    //������ ��� ����� �� ��������. ����������� ��� �������� �������� �������.
} TClientTrxData;

//��������� ��� �������� ������ � ����������� � ������ ���������
typedef struct tagTBatchRecord{
  BYTE    TrxType;
  DWORD   AmountClear; // ����� �������� ��� ����� �������� / ������
  DWORD   Amount;      // ����� �������� � ������ �������� / ������
  char    CardName[MAX_NAME+1];       // �������� ����� (Visa, Maestro � �.�.)
  ::BYTE  CardType;  //��� �����
  char    TrxDate[MAX_DATE+1];        // ���� ��������  (��.��.����)
  char    TrxTime[MAX_TIME+1];        // ����� �������� (��:��:��)
  char    AuthCode[MAX_AUTH_CODE+1];  // ��� �����������
  char    RRN[MAX_RRN+1];             //����� ������
  ::WORD  MerchantTSN;                // ����� ���������� � ������ ���������
  ::WORD  MerchantBatchNum;           // ����� ������ ��������� �� ����.������
  char    ClientCard[MAX_CARD_NUM+1];   // ����� ����� �������
  char    ClientExpiryDate[MAX_DATE+1]; // ���� �������� ����� �������
  ::BYTE  Hash[MAX_HASH];
  int     NextRecId;
} TBatchRecord;

//��������� ��� �������� �������� ����������
typedef struct tagTGoodsData{
  DWORD   Price;       //���� �� �������, exp 2
  DWORD   Volume;      //���������� exp 3
  char    Name[MAX_GOODS_NAME+1];      //������������
  char    Code[MAX_GOODS_CODE+1];      //���������� ��� ������� ������� ���������� ���������
}TGoodsData;


//��������� call_sb_kernel
typedef struct tagInArg{
  void	*Reserved;
  void 	*in_struct;
} InArg;

typedef struct tagOutArg{
  DWORD  ErrorCode;        //��� ���������� ��������
  DWORD  Flags;            //����� ����������� ���������
  void   *Reserved;
  void   *out_struct;
} OutArg;

//----------------------- ��������� ������� ������ 0 { ---------------------------

//������� 0
typedef struct tag_struct_in0{
  DWORD  AppVersion;  		//����������� ��������� ������ ����
  void   *UIData;               //��������� ���������� �������. ����������
  void   *Reserved;
} struct_in0;

typedef struct tag_struct_out0{
  DWORD  LibVersion;  		//������������� ������ ����
  void   *Reserved;
}struct_out0;

//������� 1

//������� 2
typedef struct tag_struct_out2{
  DWORD  Count;                 //���-�� �������������� ��������
}struct_out2;

//������� 3
typedef struct tag_EnumStruct{
  DWORD  FuncID;    	 //����� ������� ����������
  DWORD  Options;        //�����, ����������� ���������� ��������
  char   Name[64];       //�������� ��������� ��������
}EnumStruct;

typedef struct tag_struct_out3{
  EnumStruct  *Buffer;          //����� ������� Count * sizeof(EnumStruct)
                       		// ����������� ����������� EnumStruct (��. ����)
} struct_out3;

//������� 4
typedef struct tag_struct_out4{
  DWORD  Size;		 //������ ������ ��������� � ������
}struct_out4;

//������� 5
typedef struct tag_struct_out5{
  void  *Buffer;         //�����, � ������� ���������� ����� ���������
} struct_out5;

//������� 6
typedef struct tag_struct_out6{
  DWORD  Count; 	//����� ������������ �����
} struct_out6;

//������� 7
typedef struct tagCurrDesc{
  DWORD Currency;	  //��� ������: 810 - �����, 840 - ������� ���
  char  Name[4]; 	  //����������� �������� ������, ����. "���", "USD"
  char  FullName[64]; //������ �������� ������
} CurrDesc;

typedef struct tag_struct_out7{
  CurrDesc  *Buffer; //�����, ����������� ����������� CurrDescr
}struct_out7;

//������� 8
typedef struct tag_struct_in8{
  void	*Settings; //��������� �� ����� � ������������ �����������
  int   Version;
} struct_in8;

//only c++ compilation allowed because we have some empty struct
#ifdef __cplusplus
typedef struct tag_struct_out8{
} struct_out8;
#endif /*__cplusplus*/

//������� 9
typedef struct tag_struct_in9{
  char UserName[MAX_OPER_NAME+1]; //��� ������������
} struct_in9;

//only c++ compilation allowed because we have some empty struct
#ifdef __cplusplus
typedef struct tag_struct_out9{
} struct_out9;
#endif /*__cplusplus*/



//������� 11
typedef struct tag_struct_in11{
  DWORD hWindow;
  DWORD Message;
} struct_in11;


//������� 13
typedef struct tag_struct_in13{
  DWORD ScrId;	//screen id for width and height
  int   DlgNum;  //dlg id for width and height
  BYTE  RFU[56];
} struct_in13;

typedef struct tag_struct_out13{
  BYTE  Model;
  BYTE  Version;
  BYTE  Release;
  BYTE  Build;
  BYTE  HasCtls;
  BYTE  bPPScreenWidth;
  BYTE  bPPScreenHeight;
  char  SN[12+1];
} struct_out13;

//������� 14
typedef struct tag_struct_in14{
  DWORD dwErrorCode;
} struct_in14;

typedef struct tag_struct_out14{
  char ErrorDescription[256];
} struct_out14;

//������� 15
typedef struct tag_struct_out15{
  char TerminalID[ 9];
} struct_out15;


typedef struct tag_struct_out_reserved15{
  DWORD size;
  char MerchantID[16];
} struct_out_reserved15;

//������� 22
typedef struct tag_struct_in22{
  int bErase;   //������� ���������� ���������
} struct_in22;

typedef struct tag_struct_out22{
  int mesType;     //��� ���������. 0 - �������. ������ ���������� ������ ���� ����� ��� �������, 1 - �������. ��������� ������
  int mesState;    //��������� �����������. 0 - ����������, 1 - ��������������, �������� ��������� ������ � ���������� ���������� ��������, 2 - ��� �����.
  int mesStateEx;  //RFU
  char msg[196];   //���������
} struct_out22;


//----------------------- } ��������� ������� ������ 0 ---------------------------


//----------------------struct_in_clX  & struct_out_clX {-------------------------

//����� 1
typedef struct tag_struct_in_cl1{
  DWORD  Amount; //����� ��������; ���� 0 - ������������� �����������
} struct_in_cl1;

typedef struct tag_struct_out_cl1{
  DWORD    AmountClear;        //����� �������� ��� ��������/������
  DWORD    Amount;             // ����� �������� � ������ ��������
  char     TrxDate[MAX_DATE+1];// ���� ��������  (��.��.����)
  char     TrxTime[MAX_TIME+1];// ����� �������� (��:��:��)
  char     MerchantCard[MAX_CARD_NUM+1]; // ����� ����� ���������
  char     MerchantName[MAX_NAME+1];     // �������� ����� ���������
  char     MerchantCert[MAX_CERT+1];     // ������������ ���������� ����������
  ::WORD     MerchantTSN;                  // ����� ���������� � ������ ���������
  ::WORD     MerchantBatchNum;             // ����� ������ ���������
  char     MerchantBatchDate[MAX_DATE+1];// ���� ������ ��������� (��.��.����)
  char     ClientCard[MAX_CARD_NUM+1];   // ����� ����� �������
  char     ClientBranch[MAX_BRANCH_NUM+1];  // ��� ����� �������
  char     ClientAccount[MAX_ACCOUNT_NUM+1];// ����� ����� �������
  char     ClientName[MAX_NAME+1]; // ������� �������
  char     ClientCert[MAX_CERT+1]; // ���������� ���������� ����������
  ::WORD     ClientTSN;              // ����� ���������� �� ���������� �����
  char     ClientExpiryDate[MAX_DATE+1];// ���� �������� ����� �������
} struct_out_cl1;

//����� 2
//only c++ compilation allowed because we have some empty struct
#ifdef __cplusplus
typedef struct tag_struct_in_cl2{
}struct_in_cl2;
#endif /*__cplusplus*/

typedef struct tag_struct_out_cl2{
  char  TrxDate[MAX_DATE+1];// ���� ��������  (��.��.����)
  char  TrxTime[MAX_TIME+1];// ����� �������� (��:��:��)
  char  ClientCard[MAX_CARD_NUM+1];   // ����� ����� �������
  char  ClientBranch[MAX_BRANCH_NUM+1]; // ��� ����� �������
  char  ClientAccount[MAX_ACCOUNT_NUM+1];// ����� ����� �������
  char  ClientName[MAX_NAME+1];       // ������� �������
  char  ClientCert[MAX_CERT+1];       // ���������� ���������� ����������
  char  ServerCert[MAX_CERT+1];       // ��������� ���������� ����������
  ::WORD  ClientTSN;                    // ����� ���������� �� ���������� �����
  char  ClientExpiryDate [MAX_DATE+1];// ���� �������� ����� �������
} struct_out_cl2;

//����� 3
//only c++ compilation allowed because we have some empty struct
#ifdef __cplusplus
typedef struct tag_struct_in_cl3{
} struct_in_cl3;
#endif /*__cplusplus*/

typedef struct tag_struct_out_cl3{
  char  TrxDate[MAX_DATE+1];		 // ���� ��������  (��.��.����)
  char  TrxTime[MAX_TIME+1];		 // ����� �������� (��:��:��)
  char  MerchantCard[MAX_CARD_NUM+1]; // ����� ����� ���������
  char  MerchantName[MAX_NAME+1];     // �������� ����� ���������
  char  MerchantCert[MAX_CERT+1];     // ������������ ���������� ����������
  ::WORD  MerchantBatchNum;             // ����� ������ ���������
  char  MerchantBatchDate[MAX_DATE+1];// ���� ������ ��������� (��.��.����)
  DWORD BatchAmount;                  // ����� ������
  ::WORD  BatchTrx;                      // ����� �������� � ������
  char  ServerCert[MAX_CERT+1];       // ��������� ���������� ����������
}struct_out_cl3;

//����� 4
typedef struct tag_struct_in_cl4{
  DWORD Amount;    //����� ��������, ���� 0 - ������������� �����������
  char  Track2[MAX_TRACK2+1];//2-� ������� �����, ���� ������ - ����������� �����������
  ::BYTE  CardType;  //��� �����   0 - ����� �� ����
} struct_in_cl4;               //1 - Visa
                               //2 - Eurocard/Mastercard
                               //3 - Cirrus/Maestro

typedef struct tag_struct_out_cl4{
  DWORD AmountClear; // ����� �������� ��� ����� �������� / ������
  DWORD Amount;      // ����� �������� � ������ �������� / ������
  char  CardName[MAX_NAME+1];       // �������� ����� (Visa, Maestro � �.�.)
  ::BYTE  CardType;  //��� �����
  char  TrxDate[MAX_DATE+1];        // ���� ��������  (��.��.����)
  char  TrxTime[MAX_TIME+1];        // ����� �������� (��:��:��)
  char  TermNum[MAX_TERM+1];        // ����� ���������
  char  MerchNum[MAX_MERCHANT_LN+1];   // ����� ��������
  char  AuthCode[MAX_AUTH_CODE+1];  // ��� �����������
  char  RRN[MAX_RRN+1];             //����� ������
  ::WORD  MerchantTSN;                // ����� ���������� � ������ ���������
  ::WORD  MerchantBatchNum;           // ����� ������ ��������� �� ����.������
  char  ClientCard[MAX_CARD_NUM+1];   // ����� ����� �������
  char  ClientExpiryDate[MAX_DATE+1]; // ���� �������� ����� �������
} struct_out_cl4;

//����� 5
typedef struct tag_struct_in_cl5{
  char  Track2[MAX_TRACK2+1];//2-� ������� �����, ���� ������ - ����������� �����������
  ::BYTE  CardType;  //��� �����
} struct_in_cl5;

typedef struct tag_struct_out_cl5{
  char  CardName[MAX_NAME+1];         // �������� �����
  ::BYTE  CardType;                     //��� �����
  char  TrxDate[MAX_DATE+1];          // ���� ��������  (��.��.����)
  char  TrxTime[MAX_TIME+1];          // ����� �������� (��:��:��)
  char  TermNum[MAX_TERM+1];          // ����� ���������
  char  ClientCard[MAX_CARD_NUM+1];   // ����� ����� �������
  char  ClientExpiryDate [MAX_DATE+1];// ���� �������� ����� �������
} struct_out_cl5;

//����� 6
typedef struct tag_struct_in_cl6{
  DWORD  Currency;			// ��� ������ (810, 840 � �.�.) ��� ����� ���������
} struct_in_cl6;

typedef struct tag_struct_out_cl6{
  char  TrxDate[MAX_DATE+1];	 	//���� ��������  (��.��.����)
  char  TrxTime[MAX_TIME+1];	 	//����� �������� (��:��:��)
  char  TermNum[MAX_TERM+1];    	//����� ���������
  char  MerchNum[MAX_MERCHANT_LN+1];	//����� ��������
  ::WORD  MerchantBatchNum;        	//����� ������ ��������� �� ����.������
  DWORD DebitNumber;     // ����� �������� ������ ��������/������ �������
  DWORD DebitTotal;       // ����� �������� ������ ��������/������ �������
  DWORD DebitClearTotal;  // ����� �������� ������ ��������/������ �������
                          // ��� ����� ��������/������
  ::WORD  ReturnNumber;    //����� �������� ��������/������������ ��������
  DWORD ReturnTotal;     //����� ���������� ��������
  DWORD ReturnClearTotal;//����� ���������� �������� ��� ����� ��������
} struct_out_cl6;


//����� 7
//only c++ compilation allowed because we have some empty struct
#ifdef __cplusplus
typedef struct tag_struct_in_cl7{
} struct_in_cl7;

typedef struct tag_struct_out_cl7{
} struct_out_cl7;
#endif /*__cplusplus*/

//----------------------}struct_in_clX  & struct_out_clX -------------------------

//---------------------struct  for InArg.Reserved{-------------------------------
//class 0
typedef struct tag_struct_in_reserved0{
  DWORD size;          //������ ���������.
  void  *Reserved1;    //���������������.
  ::BYTE  KeyVersion;    //������ ����� ��� ������������ �����������
                       //������������ �������� 0
} struct_in_reserved0;

//class 1
typedef struct tag_struct_in_reserved1{
  DWORD size;          //������ ���������.
  void  *Reserved1;    //���������������.
  DWORD Rand;          //��������� ����� ��� ��������� � ���������� (0<=Rand<100000)
  ::WORD  ColNum;        //����� ������� ��� ������ �������
  TPassportData PassportData;//���������� ������
  DWORD Department; //����� ������ (DEPARTMENT_NOT_SPECIFIED - ����� �� ����)
} struct_in_reserved1;

#define USE_BINARY_HASH   0x01    // for HashFlags[0]
#define NO_PIN_BYPASS     0x02    //
#define KILL_SECOND_COPY  0x04    //
#define CLIENT_SKIPPED_PIN 0x08   //

//class 4
typedef struct tag_struct_in_reserved4{
  DWORD size;          //������ ���������.
  void  *Reserved1;    //���������������.
  char  RRN[MAX_RRN+1];//RRN
  DWORD RequestID;     //��������� ����� ��� ��������� � ���������� (0<=Rand<100000)
  DWORD Currency;      //��� ������. ���� 0, ���������� �� ����
  char  RecvCard[MAX_PAN_N+1]; //����� ����� ���������� �������
  BYTE  BinHash[20];
  BYTE  HashFlags[5];   
  TPassportData PassportData; //���������� ������
  char  AuthCode[MAX_AUTH_CODE+1];//��� ����������� ��� �������� ����� ����� �� Amex
  char  RecvValidDate[MAX_CARD_DATE+1];//���� �������� ����� ����������
  DWORD Department; //����� ������ (DEPARTMENT_NOT_SPECIFIED - ����� �� ����)
  char  PaymentTag[MAX_PAYMENT_TAG+1];
  char  TagValue  [MAX_PAYMENT_VALUE+1];
  char  CashierFIO[MAX_FIO_N+1];
  char  TextMessage[MAX_TEXT_MESSAGE];  //��������� ��������� ��� 5005/5006 (�� �������) � 4011/4012 (�� ���) ��������
  TGoodsData GoodsData;
} struct_in_reserved4;

#define TVM_FULL  0x01
#define TVM_NEXT  0x02
#define TVM_PREV  0x04

#define TVM_NEED_TRACK1  0x08

//class 5
typedef struct tag_struct_in_reserved5{
  DWORD size;          //������ ���������.
  BYTE  bEditMode;
  char  TextMessage[MAX_TEXT_MESSAGE];
  char  CashierFIO[MAX_FIO_N+1];
} struct_in_reserved5;

//class 6
typedef struct tag_struct_in_reserved6{
  DWORD size;          //������ ���������.
  void  *Reserved1;    //���������������.
  char  CashierCard[MAX_TRACK2+1];  // ����� �������
  WORD  MerchantTSN;                // ����� ���������� � ������ ���������
  WORD  MerchantBatchNum;           // ����� ������ ��������� �� ����.������
  DWORD dwAmount;                   // ����� ��� suspend/commit/rollback
  char  AuthCode[MAX_AUTH_CODE+1];  // ��� ����������� ��� suspend/commit/rollback
  char  AmountCashTotal[MAX_CASH_STATS+1];  // ����� ����� ���������, �������� � ��������
  char  CountCashTotal[MAX_CASH_STATS+1];   // ���������� ����� ���������
  char  ShiftIndexTotal[MAX_RRN+1];   // ������������� �����
} struct_in_reserved6;

//class 7
typedef struct tag_struct_in_reserved7{
  DWORD  size;
  int    RecordID;              //����� ������ � �������. 0,struct_out_reserved7::TBatchRecord::NextRecId,... ���� �� �������� ������ ERR_PIL_NO_FILES
} struct_in_reserved7;

//---------------------}struct  for InArg.Reserved-------------------------------


//---------------------struct  for OutArg.Reserved{-------------------------------
//class 1
typedef struct tag_struct_out_reserved1{
  DWORD size;            //������ ���������.
  void  *Reserved1;      //���������������.
  ::BYTE  Cert[128];     //���������� ��������.
  WORD  PurchCode;       //��� ���������� ��������
  DWORD PurchTrace;      //����������� ���������� ��������
  TPassportData PassportData; //���������� ������
} struct_out_reserved1;

//class 2
typedef struct tag_struct_out_reserved2{
  DWORD size;                //������ ���������.
  void  *Reserved1;          //���������������.
  DWORD SCABefore;           //SCA �� ��������
  DWORD HSSSBefore;          //HSSS �� ��������
  DWORD SCAAfter;            //SCA ����� ��������
  DWORD HSSSAfter;           //HSSS ����� ��������
  TClientTrxData TrxData[20];//������ �� ��������� � ����� �������
} struct_out_reserved2;

//class 3
typedef struct tag_struct_out_reserved3{
  DWORD size;                //������ ���������.
  void  *Reserved1;          //���������������.
  DWORD ClearTotal;           //SCA �� ��������
} struct_out_reserved3;


//class 4
typedef struct tag_struct_out_reserved4{
  DWORD size;          //������ ���������.
  void  *Reserved1;    //���������������.
  ::BYTE  Cert[128];   //���������� ��������.����� �� ������������ Track2.
  TPassportData PassportData; //���������� ������
  BYTE  IsOwn;
  DWORD Currency;
  DWORD TrxFlags;
  DWORD RequestID;
  char  CardEntryMode;
  char  AID[33];
  BYTE  LltID;	//loyalty program identifier
} struct_out_reserved4;

//class 5
typedef struct tag_struct_out_reserved5{
  DWORD size;          //������ ���������.
  void  *Reserved1;    //���������������.
  BYTE  Hash    [MAX_HASH];
  BYTE  CardData[MAX_TRACK2];

  DWORD Balance;
  DWORD Currency;

  DWORD CardLCDataLen;
  BYTE  CardLCData[MAX_CARD_LS_DATA];
  char  AuthCode[6+1];
  char  ClientName[MAX_CLIENT_NAME];
  BYTE  IsOwn;
  BYTE  LltID;	//loyalty program identifier
} struct_out_reserved5;

typedef struct tag_struct_out_reserved7{
  DWORD         size;
  char          TermNum[MAX_TERM+1];        // ����� ���������
  char          MerchNum[MAX_MERCHANT_LN+1];   // ����� ��������
  TBatchRecord  Record;                      //�������� �� �������. ������� 7006
} struct_out_reserved7;


//---------------------}struct  for OutArg.Reserved-------------------------------
#pragma pack()

typedef DWORD (*Gate_Call_Sb_Kernel)(DWORD  func,void   *in_arg,void   *out_arg);

#endif //_KRNLTYPE
