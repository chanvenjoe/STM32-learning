#include "malloc.h"

//storage pool 32bits alignment
__align(32) u8 mem1base[MEM1_MAX_SIZE];
__align(32) u8 mem2base[MEM2_MAX_SIZE] __attribute__((at(0x68000000)));//External SRAM
__align(32) u8 mem3base[MEM3_MAX_SIZE] __attribute__((at(0x10000000)));//1 addr -> 8bit = 1 byte

u16 mem1mapbase[MEM1_ALLOC_TABLE_SIZE];
u16 mem2mapbase[MEM2_ALLOC_TABLE_SIZE] __attribute__((at(0x68000000+MEM2_MAX_SIZE)));
u16 mem3mapbase[MEM3_ALLOC_TABLE_SIZE] __attribute__((at(0x10000000+MEM3_MAX_SIZE)));

const u32 memtblsize[SRAMBANK]={MEM1_ALLOC_TABLE_SIZE,MEM2_ALLOC_TABLE_SIZE,MEM3_ALLOC_TABLE_SIZE};	//内存表大小
const u32 memblksize[SRAMBANK]={MEM1_BLOCK_SIZE,MEM2_BLOCK_SIZE,MEM3_BLOCK_SIZE};					//内存分块大小
const u32 memsize[SRAMBANK]={MEM1_MAX_SIZE,MEM2_MAX_SIZE,MEM3_MAX_SIZE};

struct _m_malloc_dev malloc_dev=
{
	my_mem_init,//function name it self means the addr of this function, like arry name
	my_mem_perused,
	mem1base,mem2base,mem3base,
	mem1mapbase,mem2mapbase,mem3mapbase,
	0,0,0,
};

void mymemcpy(void *des, void *src, u32 n)
{
	u8 *xdes=des;//declar a pointer and then  = des
	u8 *xsrc=src;
	while(n--)
		*xdes++=*xsrc++;//addr++ and then aceess
}

void mymemset(void *saddr, u8 cont, u32 count)
{
	u8 *xs = saddr;
	while(count--)
		*xs++=cont;
}
//Clear all the memory table and memory bank
void my_mem_init(u8 memx)
{
	mymemset(malloc_dev.memmap[memx], 0, memtblsize[memx]*2);//++ is prior to *
	mymemcpy(malloc_dev.membase[memx], 0, memsize[memx]);
	malloc_dev.memrdy[memx]=1;
}

u8 my_mem_perused(u8 memx)
{
	u32 used = 0;
	u32 i;
	for(i=0;i<memtblsize[memx];i++)
	{
		if(malloc_dev.memmap[memx][i])
			used++;
	}
	return (used)*100/memtblsize[memx];
}
//size: bytes needed
u32 my_mem_malloc(u8 memx, u32 size)
{
	signed long offset=0;
	u32 nmemb;
	u32 cmemb=0;// comsecutive memory bank number
	u32 i;
	if(!malloc_dev.memrdy[memx])
		malloc_dev.init(memx);
	if(size==0)
		return 0xffffffff;
	nmemb=size/memblksize[memx];//Can be replaced by 3yuan operation 
	if(size%memblksize[memx])//how many block needed
		nmemb+=1;
	for(offset=memtblsize[memx]-1;offset>=0;offset--)//since array start from 0, so -1
	{
		if(!malloc_dev.memmap[memx][offset])//0 means available
			cmemb++;
		else
			cmemb=0;
		if(cmemb==nmemb)
		{
			for(i=0;i<nmemb;i++)//Mart these memory block is not empty(with the block number)
			{
				malloc_dev.memmap[memx][offset+i]=nmemb;
			}
			return (offset*memblksize[memx]);//memblksize[memx] = 32
		}
	}
	return 0xffffffff;
}

u8 my_mem_free(u8 memx, u32 offset)
{
	int i;
	if(!malloc_dev.memrdy[memx])
	{
		malloc_dev.init(memx);
		return 1;
	}
	if(offset<memsize[memx])
	{
		int index=offset/memblksize[memx];
		int nmemb=malloc_dev.memmap[memx][index];
		for(i=0;i<nmemb;i++)
		{
			malloc_dev.memmap[memx][index+i]=0;
		}
		return 0;
	}
	else return 2;
}

void myfree(u8 memx, void *ptr)
{
	u32 offset;
	if(ptr==NULL)
		return;
	offset=(u32)ptr-(u32)malloc_dev.membase[memx];// 1 addr offset is 1 byte
	my_mem_free(memx,offset);
}

void *mymalloc(u8 memx, u32 size)
{
	u32 offset;
	offset=my_mem_malloc(memx, size);//return the offset number
	if(offset==0xffffffff)
		return NULL;
	else return (void*)((u32)malloc_dev.membase[memx]+offset);
}

void *myrealloc(u8 memx, void *ptr, u32 size)
{
	u32 offset;
	offset=my_mem_malloc(memx, size);
	if(offset==0xffffffff)
		return NULL;
	else
	{
		mymemcpy((void*)((u32)malloc_dev.membase[memx]+offset), ptr, size);
		myfree(memx,ptr);
		return (void*)((u32)malloc_dev.membase[memx]+offset);
	}
}
	

