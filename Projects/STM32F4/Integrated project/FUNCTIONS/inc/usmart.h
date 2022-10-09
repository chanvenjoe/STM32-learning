#ifndef __USMART_H
#define __USMART_H
#include "sys.h"
#include "usmart_str.h"
/********************USER PARAMETER*****************/
#define USMART_ENTIMX_SCAN 	1
#define MAX_FNAME_LEN 		30	//the max length of function name									   
#define MAX_PARM 			10	//10 parameters top, usmart_exe related
#define PARM_LEN 			200	//All parameter<PARM_LEN, be aware of usart receiving part

#define USMART_USE_HELP		1//TAKE 700 bytes
#define USMART_USE_WRFUNS	1
/*****************************************************/

#define USMART_OK			0//ERROR FREE
#define USMART_FUNCERR		1
#define USMART_PARMERR		2
#define USMART_PARMOVER		3
#define USMART_NOFUNCFIND	4

#define SP_TYPE_DEC			0
#define SP_TYPE_HEX			1

struct _m_usmart_nametab
{
	void* func;		//function pointer
	const u8* name; //function name
};
struct _m_usmart_dev
{
	struct _m_usmart_nametab *funs;
	
	void(*init)(u8);
	u8(*cmd_rec)(u8*str);
	void(*exe)(void);
	void (*scan)(void);
	u8 fnum;
	u8 pnum;
	u8 id;
	u8 sptype;
	u16 parmtype;
	u8 plentbl[MAX_PARM];  //Parameter lenth temperary list
	u8 parm[PARM_LEN];
	u8 runtimeflag;
	u32 runtime;
};

extern struct _m_usmart_dev usmart_dev;
extern struct _m_usmart_nametab usmart_nametab[];
	
void usmart_init(u8 sysclk);	//initialization
u8 usmart_cmd_rec(u8*str);		//recognize
void usmart_exe(void);			//excecute
void usmart_scan(void);			//scan
u32 read_addr(u32 addr);
void write_addr(u32 addr,u32 val);
void usmart_reset_runtime(void);
u32 usmart_get_runtime(void);

#endif
