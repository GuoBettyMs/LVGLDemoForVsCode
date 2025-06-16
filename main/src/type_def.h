

#ifndef __TYPE_DEF_H__
#define __TYPE_DEF_H__

#pragma GCC diagnostic error "-Wsign-compare"
//---------------------------------------------------------------------------
typedef unsigned char       INT8U;
typedef char                INT8S;
typedef unsigned short      INT16U;
typedef signed short        INT16S;
typedef unsigned int        INT32U;
typedef signed int          INT32S;
typedef unsigned long long  INT64U;
typedef signed long long    INT64S;
typedef unsigned char       BOOL;

//typedef unsigned char       uint8_t;
//typedef char                int8_t;
//typedef unsigned short      uint16_t;
//typedef signed short        int16_t;
//typedef unsigned int        uint32_t;
//typedef signed int          int32_t;
//typedef unsigned long long  uint64_t;
//typedef signed long long    int64_t;

//#ifndef __cplusplus
//typedef     unsigned char   bool;
//#endif
typedef     unsigned char   u08;
typedef     signed char     s08;
typedef     unsigned short  u16;
typedef     signed short    s16;
typedef     unsigned int    u32;
typedef     unsigned long long  u64;
typedef     signed long long  s64;
typedef     signed int      s32;
typedef     float           fp32;
typedef     double          fp64;
//---------------------------------------------------------------------------


#define  BIT0      (1ul<<0)    
#define  BIT1      (1ul<<1)
#define  BIT2      (1ul<<2)
#define  BIT3      (1ul<<3)
#define  BIT4      (1ul<<4)
#define  BIT5      (1ul<<5)
#define  BIT6      (1ul<<6)
#define  BIT7      (1ul<<7)
#define  BIT8      (1ul<<8)
#define  BIT9      (1ul<<9)
#define  BIT10     (1ul<<10)
#define  BIT11     (1ul<<11)    
#define  BIT12     (1ul<<12)
#define  BIT13     (1ul<<13)
#define  BIT14     (1ul<<14)
#define  BIT15     (1ul<<15)
#define  BIT16     (1ul<<16)
#define  BIT17     (1ul<<17)
#define  BIT18     (1ul<<18)
#define  BIT19     (1ul<<19)
#define  BIT20     (1ul<<20)
#define  BIT21     (1ul<<21)
#define  BIT22     (1ul<<22)
#define  BIT23     (1ul<<23)
#define  BIT24     (1ul<<24)
#define  BIT25     (1ul<<25)
#define  BIT26     (1ul<<26)
#define  BIT27     (1ul<<27)
#define  BIT28     (1ul<<28)
#define  BIT29     (1ul<<29)
#define  BIT30     (1ul<<30)
#define  BIT31     (1ul<<31)

#ifndef TRUE
    #define TRUE    1
#endif

#ifndef FALSE
    #define FALSE   0
#endif

#ifndef NULL
    #define NULL    0
#endif

//---------------------------------------------------------------------------
#endif

