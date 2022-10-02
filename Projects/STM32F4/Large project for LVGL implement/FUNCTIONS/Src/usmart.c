#include "usmart.h"
#include "usart.h"
#include "sys.h"

u8 *sys_cmd_tab[]=
{
	"?",
	"help",
	"list",
	"id",
	"hex",
	"dec",
	"runtime",
};

u8 usmart_sys_cmd_exe(u8 *str)
{
	u8 i, sfname[MAX_FNAME_LEN];
	u8 pnum,rval;
	u32 res;
	res=usmart_get_cmdname(str,sfname,&i,MAX_FNAME_LEN);
	if(res) return USMART_FUNCERR;
	str+=i;
	for(i=0;i<sizeof(sys_cmd_tab)/4;i++)//size of return byte, /4 means the number of integer
	{
		if(usmart_strcmp(sfname,sys_cmd_tab[i])==0) break;
	}
	switch(i)
	{
		case 0:
		case 1:
			printf("\r\n");
#if USMART_USE_HELP
			printf("----------------KV USMART HELP DESK\r\n-------------");
			printf(" This allows users to modify parameters of function(supprt hex/dec number)\r\n");
			printf("USMART contains 7 commands:\r\n");
			printf("?:      Get help information\r\n");
			printf("help:   Get help information\r\n");
			printf("list:   Available function list\r\n\n");
			printf("id:     Function ID list\r\n\n");
			printf("hex:    Hex display, blanck+number to excute transformation\r\n\n");
			printf("dec:    Dec display, blanck+number to excute transformation\r\n\n");
			printf("runtime:1,Enable runtime;0,disable runtime;\r\n\n");
			printf("Enter function name and parameter and end with enter key\r\n");    
			printf("-------------------------------------------------------- \r\n");
#else
			printf("Command disabled\r\n")
#endif
			break;
		case 2:
			printf("Function List\r\n");
			for(i=0;i<usmart_dev.fnum;i++) printf("%s\r\n",usmart_dev.funs[i].name);
			break;
		case 3:
			printf("Function ID\r\n");
			for(i=0;i<usmart_dev.fnum;i++)
		{
			usmart_get_fname((u8*)usmart_dev.funs[i].name,sfname,&pnum,&rval);
			printf("%s id is:\r\n 0x%08X\r\n",sfname,usmart_dev.funs[i].func);
		}
		break;
		case 4:
			usmart_get_aparm(str,sfname,&i);
			if(i==0)
			{
				i=usmart_str2num(sfname,&res);
				if(i==0)//start transform
				{
					printf("HEX:0x%0X\r\n",res);//%0x is used fill 0 when requiring SPECIFIC WIDTH
				}else if(i!=4)return USMART_PARMERR;
				else
				{
					printf("hex display\r\n");
					usmart_dev.sptype= SP_TYPE_HEX;
				}
			}else return USMART_PARMERR;
			break;
		case 5:
			usmart_get_aparm(str,sfname,&i);
			if(i==0)
			{
				i=usmart_str2num(sfname,&res);
				if(i==0)
				{
					printf("DEC:%lu\r\n",res);
				}else if(i!=4) return USMART_PARMERR;
				else
				{
					printf("Dec display\r\n");
					usmart_dev.sptype=SP_TYPE_DEC;
				}
			}else return USMART_PARMERR;
			break;
		case 6:
			usmart_get_aparm(str,sfname,&i);
			if(i==0)
			{
				i=usmart_str2num(sfname,&res);
				if(i==0)
				{
					if(USMART_ENTIMX_SCAN==0) printf("Run time SCAN not enabled");
					else
					{
						usmart_dev.runtimeflag=res;
						if(usmart_dev.runtimeflag)printf("Run time scan on\r\n");
						else printf("Run time scan off\r\n");
					}
				}else return USMART_PARMERR;
			}else return USMART_PARMERR;
			break;
		default:
			return USMART_FUNCERR;
	}
	return 0;
}
		
void Timer4_Init(u16 arr,u16 psc)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;
	TIM_TimeBaseInitStructure.TIM_CounterMode= TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period=arr;
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);
	
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM4,ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0X03;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void TIM4_IRQHandler(void)
{ 		    		  			    
	if(TIM_GetITStatus(TIM4,TIM_IT_Update)==SET)//溢出中断
	{
		usmart_dev.scan();	//执行usmart扫描	
		TIM_SetCounter(TIM4,0);		//清空定时器的CNT
		TIM_SetAutoreload(TIM4,100);//恢复原来的设置		    				   				     	    	
	}				   
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);  //清除中断标志位    
}

void usmart_reset_runtime(void)
{
	TIM_ClearFlag(TIM4,TIM_FLAG_Update);
	TIM_SetAutoreload(TIM4,0xffff);
	TIM_SetCounter(TIM4,0);
	usmart_dev.runtime=0;
}

void usmart_init(u8 sysclk)
{
	#if USMART_ENTIMX_SCAN==1
		Timer4_Init(1000,(u32)sysclk*100-1);
	
	#endif
		usmart_dev.sptype=1;
}
	
u8 usmart_cmd_rec(u8*str)
{
	u8 sta, i , rval;
	u8 rpnum, spnum;
	u8 rfname[MAX_FNAME_LEN];
	u8 sfname[MAX_FNAME_LEN];
	sta=usmart_get_fname(str,rfname,&rpnum,&rval);
	if(sta)
		return sta;
	for(i=0;i<usmart_dev.fnum;i++)
	{
		sta=usmart_get_fname((u8*)usmart_dev.funs[i].name,sfname, &spnum,&rval);
		if(sta)
			return sta;
		if(usmart_strcmp(sfname,rfname)==0)
		{
			if(spnum>rpnum)
				return USMART_PARMERR;
			usmart_dev.id=i;
			break;
		}
	}
	if(i==usmart_dev.fnum)
		return USMART_NOFUNCFIND;
	sta=usmart_get_fparam(str,&i);
	if(sta)
		return sta;
	usmart_dev.pnum=i;
	return USMART_OK;
}

void usmart_exe(void)
{
	u8 id,i;
	u32 res;
	u32 temp[MAX_PARM];
	u8 sfname[MAX_FNAME_LEN];
	u8 pnum,rval;
	id=usmart_dev.id;
	if(id>=usmart_dev.fnum) return;
	usmart_get_fname((u8*)usmart_dev.funs[id].name,sfname,&pnum,&rval);
	printf("\r\n%s(",sfname);
	for(i=0;i<pnum;i++)
	{
		if(usmart_dev.parmtype&(1<<i))
		{
			printf("%c",'"');
			printf("%s",usmart_dev.parm+usmart_get_parmpos(i));
			printf("%c",'"');
			temp[i]=(u32)&(usmart_dev.parm[usmart_get_parmpos(i)]);
		}else
		{
			temp[i]=*(u32*)(usmart_dev.parm+usmart_get_parmpos(i));
			if(usmart_dev.sptype==SP_TYPE_DEC)
				printf("0x%X", temp[i]);
		}
		if(i!=pnum-1)
			printf(",");
	}
	printf(")");
	usmart_reset_runtime();
	switch(usmart_dev.pnum)
	{
		case 0:
			res=(*(u32(*)())usmart_dev.funs[id].func)();
			break;
		case 1:
			res=(*(u32(*)())usmart_dev.funs[id].func)(temp[0]);
			break;
		case 2:
			res=(*(u32(*)())usmart_dev.funs[id].func)(temp[0]);
		case 3:
			res=(*(u32(*)())usmart_dev.funs[id].func)(temp[0],temp[1],temp[2]);
			break;
	    case 4://有4个参数
			res=(*(u32(*)())usmart_dev.funs[id].func)(temp[0],temp[1],temp[2],temp[3]);
			break;
	    case 5://有5个参数
			res=(*(u32(*)())usmart_dev.funs[id].func)(temp[0],temp[1],temp[2],temp[3],temp[4]);
			break;
	    case 6://有6个参数
			res=(*(u32(*)())usmart_dev.funs[id].func)(temp[0],temp[1],temp[2],temp[3],temp[4],\
			temp[5]);
			break;
	    case 7://有7个参数
			res=(*(u32(*)())usmart_dev.funs[id].func)(temp[0],temp[1],temp[2],temp[3],temp[4],\
			temp[5],temp[6]);
			break;
	    case 8://有8个参数
			res=(*(u32(*)())usmart_dev.funs[id].func)(temp[0],temp[1],temp[2],temp[3],temp[4],\
			temp[5],temp[6],temp[7]);
			break;
	    case 9://有9个参数
			res=(*(u32(*)())usmart_dev.funs[id].func)(temp[0],temp[1],temp[2],temp[3],temp[4],\
			temp[5],temp[6],temp[7],temp[8]);
			break;
	    case 10://有10个参数
			res=(*(u32(*)())usmart_dev.funs[id].func)(temp[0],temp[1],temp[2],temp[3],temp[4],\
			temp[5],temp[6],temp[7],temp[8],temp[9]);
			break;
	}
	usmart_get_runtime();
	if(rval==1)
	{
		if(usmart_dev.sptype==SP_TYPE_DEC)printf("=%lu;\r\n",res);
		else printf("=0x%x;\r\n",res);
	}else printf(";\r\n");
	if(usmart_dev.runtimeflag)
	{
		printf("Function Run time:%d.%ldms\r\n",usmart_dev.runtime/10,usmart_dev.runtime%10);
	}
}	
u32 usmart_get_runtime(void)
{
	if(TIM_GetFlagStatus(TIM4,TIM_FLAG_Update)==SET)
	{
		usmart_dev.runtime+=0xffff;// the max counting value of TIM4 is 0xffff
	}
	usmart_dev.runtime+=TIM_GetCounter(TIM4);
	return usmart_dev.runtime;
}

void usmart_scan(void)
{
	u8 sta,len;
	if(USART_RX_STA&0X8000)
	{
		len=USART_RX_STA&0x3fff;
		USART_RX_BUF[len]='\0';
		sta=usmart_dev.cmd_rec(USART_RX_BUF);
		if(sta==0)
			usmart_dev.exe();
		else
		{
			len=usmart_sys_cmd_exe(USART_RX_BUF);
			if(len!=USMART_FUNCERR)
				sta=len;
			if(sta)
			{
				switch(sta)
				{
					case USMART_FUNCERR:
						printf("function error!\r\n");
						break;
					case USMART_PARMERR:
						printf("parameter error!\r\n");
						break;
					case USMART_PARMOVER:
						printf("parameter overflow!\r\n");
						break;
					case USMART_NOFUNCFIND:
						printf("unmatched function!\r\n");
						break;
				}
			}
		}
		USART_RX_STA=0;
	}
}

#if USMART_USE_WRFUNS==1 	//如果使能了读写操作
//读取指定地址的值		 
u32 read_addr(u32 addr)
{
	return *(u32*)addr;//	
}
//在指定地址写入指定的值		 
void write_addr(u32 addr,u32 val)
{
	*(u32*)addr=val; 	
}
#endif

