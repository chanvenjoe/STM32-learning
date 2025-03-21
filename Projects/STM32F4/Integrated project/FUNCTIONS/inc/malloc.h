#ifndef __MALLOC_H
#define __MALLOC_H
#include "stm32f4xx.h"

#ifndef NULL
#define NULL 0
#endif

//定义三个内存池
#define SRAMIN	 0		//内部内存池
#define SRAMEX   1		//外部内存池
#define SRAMCCM  2		//CCM内存池(此部分SRAM仅仅CPU可以访问!!!)


#define SRAMBANK 	3	//定义支持的SRAM块数.	


//mem1内存参数设定.mem1完全处于内部SRAM里面.
#define MEM1_BLOCK_SIZE			32  	  						//内存块大小为32字节
#define MEM1_MAX_SIZE			100*1024  						//最大管理内存 100K
#define MEM1_ALLOC_TABLE_SIZE	MEM1_MAX_SIZE/MEM1_BLOCK_SIZE 	//内存表大小

//mem2内存参数设定.mem2的内存池处于外部SRAM里面
#define MEM2_BLOCK_SIZE			32  	  						//内存块大小为32字节
#define MEM2_MAX_SIZE			960 *1024  						//最大管理内存960K
#define MEM2_ALLOC_TABLE_SIZE	MEM2_MAX_SIZE/MEM2_BLOCK_SIZE 	//内存表大小
		 
//mem3内存参数设定.mem3处于CCM,用于管理CCM(特别注意,这部分SRAM,仅CPU可以访问!!)
#define MEM3_BLOCK_SIZE			32  	  						//内存块大小为32字节
#define MEM3_MAX_SIZE			60 *1024  						//最大管理内存60K
#define MEM3_ALLOC_TABLE_SIZE	MEM3_MAX_SIZE/MEM3_BLOCK_SIZE 	//内存表大小

//内存管理控制器
struct _m_malloc_dev
{
	void (*init)(u8);					//初始化
	u8 (*perused)(u8);		  	    	//内存使用率
	u8 	*membase[SRAMBANK];				//内存池 管理SRAMBANK个区域的内存
	u16 *memmap[SRAMBANK]; 				//内存管理状态表
	u8  memrdy[SRAMBANK]; 				//内存管理是否就绪
};

extern struct _m_malloc_dev malloc_dev;

void mymemset(void *s, u8 cont, u32 count);
void mymemcpy(void *dest, void *src, u32 n);
void my_mem_init(u8 memx);
u32 my_mem_malloc(u8 memx, u32 size);
u8 my_mem_free(u8 memx, u32 offset);
u8 my_mem_perused(u8 memx);

//User API
void myfree(u8 memx, void *ptr);
void *mymalloc(u8 memx, u32 size);
void *myrealloc(u8 memx, void *ptr, u32 size);

#endif
