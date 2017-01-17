#ifndef _KRNLTYPE
#define _KRNLTYPE

typedef unsigned long  DWORD;
typedef unsigned char  BYTE;
typedef unsigned short WORD;

//Константы, используемые при определении структур
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

//Флаги поля Options
#define O_DEBIT      0x00000001
#define O_CREDIT     0x00000002
#define O_CASH       0x00000004
#define O_TRANSFER   0x00000008
#define O_SERVICE    0x00000010
#define O_INFO       0x00000020
#define O_BANKTERM   0x00000040
#define O_TRADETERM  0x00000080
#define O_SPECIAL    0x00000100

//Флаги поля Flags
#define F_HAVE_DOCUMENT   0x00000001
#define F_CRITICAL        0x00000002
#define F_HAVE_MONMSG     0x00000004

#define SBER_OWN_CARD     'S'

//значения аргумента amount для запуска запросов к серверу мониторинга через функцию 17
//f.e., for call helios monitor
//sprintf(CmdLineBuf, "c:\\work\\gate.dll %d %d", 47, CMD_MONIT_H);
#define CMD_MONIT_H  1  //отправка запроса на мониторинг helios
#define CMD_MONIT_P  2	//RFU - отправка запроса на мониторинг pserverDB

#pragma pack(1) //Выравнивание на границу байта
//Структура для хранения паспортных данных
typedef struct tagTPassportData{
  char sFIO[MAX_FIO_N];    //ФИО
  char sAddr[58];   //Адрес
  short  sRes;      //Резидент/нерезидент
  char sDocType[21];//Тип документа
  char sSer[11];    //Серия
  char sNum[16];    //Номер
  char sIssuer[58]; //Кем выдан
  char sWhen[13];   //Когда выдан
  char sValid[13];  //Срок действия
  char sEmit[42];   //Банк - эмитент карты
  char Agent[42];   //Банк, выдавший карту
} TPassportData;

//Структура для хранения данных о транзакциях на карте клиента (СБЕРКАРТ)
typedef struct tagTClientTrxData{
  short TSN;              //Номер операции на карте клиента
  char  Date[6];          //Дата операции
  char  Description[17];  //Описание
  DWORD Amount;           //Сумма операции
  WORD  Batch;            //Номер пакета карты терминала. Заполняется для операци списания средств.
  BYTE  BatchRSN;         //Номер транзакции в пакете карты терминала. Заполняется для операци списания средств.
  DWORD BalanceBefore;    //Баланс ЗСК карты до операции. Заполняется для операций перевода средств.
} TClientTrxData;

//Структура для хранения данных о транзакциях в пакете терминала
typedef struct tagTBatchRecord{
  BYTE    TrxType;
  DWORD   AmountClear; // сумма операции без учета комиссии / скидки
  DWORD   Amount;      // сумма операции с учетом комиссии / скидки
  char    CardName[MAX_NAME+1];       // название карты (Visa, Maestro и т.д.)
  ::BYTE  CardType;  //тип карты
  char    TrxDate[MAX_DATE+1];        // дата операции  (ДД.ММ.ГГГГ)
  char    TrxTime[MAX_TIME+1];        // время операции (ЧЧ:ММ:СС)
  char    AuthCode[MAX_AUTH_CODE+1];  // код авторизации
  char    RRN[MAX_RRN+1];             //номер ссылки
  ::WORD  MerchantTSN;                // номер транзакции в пакете терминала
  ::WORD  MerchantBatchNum;           // номер пакета терминала по магн.картам
  char    ClientCard[MAX_CARD_NUM+1];   // номер карты клиента
  char    ClientExpiryDate[MAX_DATE+1]; // срок действия карты клиента
  ::BYTE  Hash[MAX_HASH];
  int     NextRecId;
} TBatchRecord;

//Структура для передачи товарной информации
typedef struct tagTGoodsData{
  DWORD   Price;       //Цена за единицу, exp 2
  DWORD   Volume;      //Количество exp 3
  char    Name[MAX_GOODS_NAME+1];      //наименование
  char    Code[MAX_GOODS_CODE+1];      //Внутренний код учетной системы вызывающей программы
}TGoodsData;


//Параметры call_sb_kernel
typedef struct tagInArg{
  void	*Reserved;
  void 	*in_struct;
} InArg;

typedef struct tagOutArg{
  DWORD  ErrorCode;        //код завершения операции
  DWORD  Flags;            //флаги последующей обработки
  void   *Reserved;
  void   *out_struct;
} OutArg;

//----------------------- Параметры функций класса 0 { ---------------------------

//Функция 0
typedef struct tag_struct_in0{
  DWORD  AppVersion;  		//минимальная требуемая версия ядра
  void   *UIData;               //параметры библиотеки пользов. интерфейса
  void   *Reserved;
} struct_in0;

typedef struct tag_struct_out0{
  DWORD  LibVersion;  		//установленная версия ядра
  void   *Reserved;
}struct_out0;

//Функция 1

//Функция 2
typedef struct tag_struct_out2{
  DWORD  Count;                 //кол-во поддерживаемых операций
}struct_out2;

//Функция 3
typedef struct tag_EnumStruct{
  DWORD  FuncID;    	 //номер функции библиотеки
  DWORD  Options;        //флаги, описывающие содержание операции
  char   Name[64];       //название карточной операции
}EnumStruct;

typedef struct tag_struct_out3{
  EnumStruct  *Buffer;          //буфер размера Count * sizeof(EnumStruct)
                       		// заполняется структурами EnumStruct (см. ниже)
} struct_out3;

//Функция 4
typedef struct tag_struct_out4{
  DWORD  Size;		 //размер образа документа в байтах
}struct_out4;

//Функция 5
typedef struct tag_struct_out5{
  void  *Buffer;         //буфер, в который помещается образ документа
} struct_out5;

//Функция 6
typedef struct tag_struct_out6{
  DWORD  Count; 	//число используемых валют
} struct_out6;

//Функция 7
typedef struct tagCurrDesc{
  DWORD Currency;	  //Код валюты: 810 - рубли, 840 - доллары США
  char  Name[4]; 	  //Сокращенное название валюты, напр. "Руб", "USD"
  char  FullName[64]; //Полное название валюты
} CurrDesc;

typedef struct tag_struct_out7{
  CurrDesc  *Buffer; //Буфер, заполняемый структурами CurrDescr
}struct_out7;

//Функция 8
typedef struct tag_struct_in8{
  void	*Settings; //Указатель на буфер с настроечными параметрами
  int   Version;
} struct_in8;

//only c++ compilation allowed because we have some empty struct
#ifdef __cplusplus
typedef struct tag_struct_out8{
} struct_out8;
#endif /*__cplusplus*/

//Функция 9
typedef struct tag_struct_in9{
  char UserName[MAX_OPER_NAME+1]; //Имя пользователя
} struct_in9;

//only c++ compilation allowed because we have some empty struct
#ifdef __cplusplus
typedef struct tag_struct_out9{
} struct_out9;
#endif /*__cplusplus*/



//Функция 11
typedef struct tag_struct_in11{
  DWORD hWindow;
  DWORD Message;
} struct_in11;


//Функция 13
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

//Функция 14
typedef struct tag_struct_in14{
  DWORD dwErrorCode;
} struct_in14;

typedef struct tag_struct_out14{
  char ErrorDescription[256];
} struct_out14;

//Функция 15
typedef struct tag_struct_out15{
  char TerminalID[ 9];
} struct_out15;


typedef struct tag_struct_out_reserved15{
  DWORD size;
  char MerchantID[16];
} struct_out_reserved15;

//Функция 22
typedef struct tag_struct_in22{
  int bErase;   //удалить полученное сообщение
} struct_in22;

typedef struct tag_struct_out22{
  int mesType;     //тип сообщения. 0 - обычное. должно выводиться только если текст был изменен, 1 - срочное. выводится всегда
  int mesState;    //состояние процессинга. 0 - стабильное, 1 - предупреждение, возможны отдельные отказы и замедление проведения операции, 2 - все плохо.
  int mesStateEx;  //RFU
  char msg[196];   //сообщение
} struct_out22;


//----------------------- } Параметры функций класса 0 ---------------------------


//----------------------struct_in_clX  & struct_out_clX {-------------------------

//Класс 1
typedef struct tag_struct_in_cl1{
  DWORD  Amount; //сумма операции; если 0 - запрашивается библиотекой
} struct_in_cl1;

typedef struct tag_struct_out_cl1{
  DWORD    AmountClear;        //сумма операции без комиссии/скидки
  DWORD    Amount;             // сумма операции с учетом комиссии
  char     TrxDate[MAX_DATE+1];// дата операции  (ДД.ММ.ГГГГ)
  char     TrxTime[MAX_TIME+1];// время операции (ЧЧ:ММ:СС)
  char     MerchantCard[MAX_CARD_NUM+1]; // номер карты терминала
  char     MerchantName[MAX_NAME+1];     // название карты терминала
  char     MerchantCert[MAX_CERT+1];     // терминальный сертификат транзакции
  ::WORD     MerchantTSN;                  // номер транзакции в пакете терминала
  ::WORD     MerchantBatchNum;             // номер пакета терминала
  char     MerchantBatchDate[MAX_DATE+1];// дата пакета терминала (ДД.ММ.ГГГГ)
  char     ClientCard[MAX_CARD_NUM+1];   // номер карты клиента
  char     ClientBranch[MAX_BRANCH_NUM+1];  // код банка клиента
  char     ClientAccount[MAX_ACCOUNT_NUM+1];// номер счета клиента
  char     ClientName[MAX_NAME+1]; // фамилия клиента
  char     ClientCert[MAX_CERT+1]; // клиентский сертификат транзакции
  ::WORD     ClientTSN;              // номер транзакции по клиентской карте
  char     ClientExpiryDate[MAX_DATE+1];// срок действия карты клиента
} struct_out_cl1;

//Класс 2
//only c++ compilation allowed because we have some empty struct
#ifdef __cplusplus
typedef struct tag_struct_in_cl2{
}struct_in_cl2;
#endif /*__cplusplus*/

typedef struct tag_struct_out_cl2{
  char  TrxDate[MAX_DATE+1];// дата операции  (ДД.ММ.ГГГГ)
  char  TrxTime[MAX_TIME+1];// время операции (ЧЧ:ММ:СС)
  char  ClientCard[MAX_CARD_NUM+1];   // номер карты клиента
  char  ClientBranch[MAX_BRANCH_NUM+1]; // код банка клиента
  char  ClientAccount[MAX_ACCOUNT_NUM+1];// номер счета клиента
  char  ClientName[MAX_NAME+1];       // фамилия клиента
  char  ClientCert[MAX_CERT+1];       // клиентский сертификат транзакции
  char  ServerCert[MAX_CERT+1];       // серверный сертификат транзакции
  ::WORD  ClientTSN;                    // номер транзакции по клиентской карте
  char  ClientExpiryDate [MAX_DATE+1];// срок действия карты клиента
} struct_out_cl2;

//Класс 3
//only c++ compilation allowed because we have some empty struct
#ifdef __cplusplus
typedef struct tag_struct_in_cl3{
} struct_in_cl3;
#endif /*__cplusplus*/

typedef struct tag_struct_out_cl3{
  char  TrxDate[MAX_DATE+1];		 // дата операции  (ДД.ММ.ГГГГ)
  char  TrxTime[MAX_TIME+1];		 // время операции (ЧЧ:ММ:СС)
  char  MerchantCard[MAX_CARD_NUM+1]; // номер карты терминала
  char  MerchantName[MAX_NAME+1];     // название карты терминала
  char  MerchantCert[MAX_CERT+1];     // терминальный сертификат транзакции
  ::WORD  MerchantBatchNum;             // номер пакета терминала
  char  MerchantBatchDate[MAX_DATE+1];// дата пакета терминала (ДД.ММ.ГГГГ)
  DWORD BatchAmount;                  // сумма пакета
  ::WORD  BatchTrx;                      // число операций в пакете
  char  ServerCert[MAX_CERT+1];       // серверный сертификат транзакции
}struct_out_cl3;

//Класс 4
typedef struct tag_struct_in_cl4{
  DWORD Amount;    //сумма операции, если 0 - запрашивается библиотекой
  char  Track2[MAX_TRACK2+1];//2-я дорожка карты, если пустая - считывается библиотекой
  ::BYTE  CardType;  //тип карты   0 - выбор из меню
} struct_in_cl4;               //1 - Visa
                               //2 - Eurocard/Mastercard
                               //3 - Cirrus/Maestro

typedef struct tag_struct_out_cl4{
  DWORD AmountClear; // сумма операции без учета комиссии / скидки
  DWORD Amount;      // сумма операции с учетом комиссии / скидки
  char  CardName[MAX_NAME+1];       // название карты (Visa, Maestro и т.д.)
  ::BYTE  CardType;  //тип карты
  char  TrxDate[MAX_DATE+1];        // дата операции  (ДД.ММ.ГГГГ)
  char  TrxTime[MAX_TIME+1];        // время операции (ЧЧ:ММ:СС)
  char  TermNum[MAX_TERM+1];        // номер терминала
  char  MerchNum[MAX_MERCHANT_LN+1];   // номер мерчанта
  char  AuthCode[MAX_AUTH_CODE+1];  // код авторизации
  char  RRN[MAX_RRN+1];             //номер ссылки
  ::WORD  MerchantTSN;                // номер транзакции в пакете терминала
  ::WORD  MerchantBatchNum;           // номер пакета терминала по магн.картам
  char  ClientCard[MAX_CARD_NUM+1];   // номер карты клиента
  char  ClientExpiryDate[MAX_DATE+1]; // срок действия карты клиента
} struct_out_cl4;

//Класс 5
typedef struct tag_struct_in_cl5{
  char  Track2[MAX_TRACK2+1];//2-я дорожка карты, если пустая - считывается библиотекой
  ::BYTE  CardType;  //тип карты
} struct_in_cl5;

typedef struct tag_struct_out_cl5{
  char  CardName[MAX_NAME+1];         // название карты
  ::BYTE  CardType;                     //тип карты
  char  TrxDate[MAX_DATE+1];          // дата операции  (ДД.ММ.ГГГГ)
  char  TrxTime[MAX_TIME+1];          // время операции (ЧЧ:ММ:СС)
  char  TermNum[MAX_TERM+1];          // номер терминала
  char  ClientCard[MAX_CARD_NUM+1];   // номер карты клиента
  char  ClientExpiryDate [MAX_DATE+1];// срок действия карты клиента
} struct_out_cl5;

//Класс 6
typedef struct tag_struct_in_cl6{
  DWORD  Currency;			// код валюты (810, 840 и т.д.) или номер терминала
} struct_in_cl6;

typedef struct tag_struct_out_cl6{
  char  TrxDate[MAX_DATE+1];	 	//дата операции  (ДД.ММ.ГГГГ)
  char  TrxTime[MAX_TIME+1];	 	//время операции (ЧЧ:ММ:СС)
  char  TermNum[MAX_TERM+1];    	//номер терминала
  char  MerchNum[MAX_MERCHANT_LN+1];	//номер мерчанта
  ::WORD  MerchantBatchNum;        	//номер пакета терминала по магн.картам
  DWORD DebitNumber;     // число операций выдачи наличных/оплаты покупки
  DWORD DebitTotal;       // сумма операций выдачи наличных/оплаты покупки
  DWORD DebitClearTotal;  // сумма операций выдачи наличных/оплаты покупки
                          // без учета комиссии/скидки
  ::WORD  ReturnNumber;    //число операций возврата/безналичного перевода
  DWORD ReturnTotal;     //сумма кредитовых операций
  DWORD ReturnClearTotal;//сумма кредитовых операций без учета комиссии
} struct_out_cl6;


//Класс 7
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
  DWORD size;          //Размер структуры.
  void  *Reserved1;    //Зарезервировано.
  ::BYTE  KeyVersion;    //Версия ключа для формирования сертификата
                       //Используется функцией 0
} struct_in_reserved0;

//class 1
typedef struct tag_struct_in_reserved1{
  DWORD size;          //Размер структуры.
  void  *Reserved1;    //Зарезервировано.
  DWORD Rand;          //Случайное число для включения в сертификат (0<=Rand<100000)
  ::WORD  ColNum;        //Номер колонки при оплате бензина
  TPassportData PassportData;//Паспортные данные
  DWORD Department; //Номер отдела (DEPARTMENT_NOT_SPECIFIED - выбор из меню)
} struct_in_reserved1;

#define USE_BINARY_HASH   0x01    // for HashFlags[0]
#define NO_PIN_BYPASS     0x02    //
#define KILL_SECOND_COPY  0x04    //
#define CLIENT_SKIPPED_PIN 0x08   //

//class 4
typedef struct tag_struct_in_reserved4{
  DWORD size;          //Размер структуры.
  void  *Reserved1;    //Зарезервировано.
  char  RRN[MAX_RRN+1];//RRN
  DWORD RequestID;     //Случайное число для включения в сертификат (0<=Rand<100000)
  DWORD Currency;      //Код валюты. Если 0, выбирается из меню
  char  RecvCard[MAX_PAN_N+1]; //Номер карты получателя платежа
  BYTE  BinHash[20];
  BYTE  HashFlags[5];   
  TPassportData PassportData; //Паспортные данные
  char  AuthCode[MAX_AUTH_CODE+1];//Код авторизации для операции ввода слипа по Amex
  char  RecvValidDate[MAX_CARD_DATE+1];//Срок действия карты получателя
  DWORD Department; //Номер отдела (DEPARTMENT_NOT_SPECIFIED - выбор из меню)
  char  PaymentTag[MAX_PAYMENT_TAG+1];
  char  TagValue  [MAX_PAYMENT_VALUE+1];
  char  CashierFIO[MAX_FIO_N+1];
  char  TextMessage[MAX_TEXT_MESSAGE];  //текстовое сообщение для 5005/5006 (на дисплей) и 4011/4012 (на чек) операций
  TGoodsData GoodsData;
} struct_in_reserved4;

#define TVM_FULL  0x01
#define TVM_NEXT  0x02
#define TVM_PREV  0x04

#define TVM_NEED_TRACK1  0x08

//class 5
typedef struct tag_struct_in_reserved5{
  DWORD size;          //Размер структуры.
  BYTE  bEditMode;
  char  TextMessage[MAX_TEXT_MESSAGE];
  char  CashierFIO[MAX_FIO_N+1];
} struct_in_reserved5;

//class 6
typedef struct tag_struct_in_reserved6{
  DWORD size;          //Размер структуры.
  void  *Reserved1;    //Зарезервировано.
  char  CashierCard[MAX_TRACK2+1];  // Карта кассира
  WORD  MerchantTSN;                // номер транзакции в пакете терминала
  WORD  MerchantBatchNum;           // номер пакета терминала по магн.картам
  DWORD dwAmount;                   // Сумма для suspend/commit/rollback
  char  AuthCode[MAX_AUTH_CODE+1];  // Код авторизации для suspend/commit/rollback
  char  AmountCashTotal[MAX_CASH_STATS+1];  // сумма оплат наличными, значение в копейках
  char  CountCashTotal[MAX_CASH_STATS+1];   // количество оплат наличными
  char  ShiftIndexTotal[MAX_RRN+1];   // идентификатор смены
} struct_in_reserved6;

//class 7
typedef struct tag_struct_in_reserved7{
  DWORD  size;
  int    RecordID;              //номер записи в журнале. 0,struct_out_reserved7::TBatchRecord::NextRecId,... пока не вернется ошибка ERR_PIL_NO_FILES
} struct_in_reserved7;

//---------------------}struct  for InArg.Reserved-------------------------------


//---------------------struct  for OutArg.Reserved{-------------------------------
//class 1
typedef struct tag_struct_out_reserved1{
  DWORD size;            //Размер структуры.
  void  *Reserved1;      //Зарезервировано.
  ::BYTE  Cert[128];     //Сертификат операции.
  WORD  PurchCode;       //Код завершения операции
  DWORD PurchTrace;      //Трассировка завершения операции
  TPassportData PassportData; //Паспортные данные
} struct_out_reserved1;

//class 2
typedef struct tag_struct_out_reserved2{
  DWORD size;                //Размер структуры.
  void  *Reserved1;          //Зарезервировано.
  DWORD SCABefore;           //SCA до операции
  DWORD HSSSBefore;          //HSSS до операции
  DWORD SCAAfter;            //SCA после операции
  DWORD HSSSAfter;           //HSSS после операции
  TClientTrxData TrxData[20];//Данные об операциях с карты клиента
} struct_out_reserved2;

//class 3
typedef struct tag_struct_out_reserved3{
  DWORD size;                //Размер структуры.
  void  *Reserved1;          //Зарезервировано.
  DWORD ClearTotal;           //SCA до операции
} struct_out_reserved3;


//class 4
typedef struct tag_struct_out_reserved4{
  DWORD size;          //Размер структуры.
  void  *Reserved1;    //Зарезервировано.
  ::BYTE  Cert[128];   //Сертификат операции.Здесь же возвращается Track2.
  TPassportData PassportData; //Паспортные данные
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
  DWORD size;          //Размер структуры.
  void  *Reserved1;    //Зарезервировано.
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
  char          TermNum[MAX_TERM+1];        // номер терминала
  char          MerchNum[MAX_MERCHANT_LN+1];   // номер мерчанта
  TBatchRecord  Record;                      //операция из журнала. функция 7006
} struct_out_reserved7;


//---------------------}struct  for OutArg.Reserved-------------------------------
#pragma pack()

typedef DWORD (*Gate_Call_Sb_Kernel)(DWORD  func,void   *in_arg,void   *out_arg);

#endif //_KRNLTYPE
