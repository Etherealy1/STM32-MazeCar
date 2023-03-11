#include "stm32f10x.h"
#define uchar unsigned char
#define uint unsigned int
void delay_ms(unsigned long u)	
{
	unsigned long i,j;
	for(i=0;i<u;i++) for(j=0;j<12000;j++);
}

#define BASIC_TIM6 // ??????? TIM6  
#define BASIC_TIM TIM6 
#define BASIC_TIM_APBxClock_FUN RCC_APB1PeriphClockCmd 
#define BASIC_TIM_CLK RCC_APB1Periph_TIM6 
#define BASIC_TIM_IRQ TIM6_IRQn 
#define BASIC_TIM_IRQHandler TIM6_IRQHandler

#define i1 GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_4)
#define i2 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7)
#define i3 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)
#define i4 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)
#define i5 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)

#define A0_Pin GPIO_Pin_0
#define A0_GPIO_Port GPIOC
#define A1_Pin GPIO_Pin_1
#define A1_GPIO_Port GPIOC
#define A2_Pin GPIO_Pin_2
#define A2_GPIO_Port GPIOC
//接收
#define IR5_Pin GPIO_Pin_4
#define IR5_GPIO_Port GPIOA
#define IR4_Pin GPIO_Pin_5
#define IR4_GPIO_Port GPIOA
#define IR3_Pin GPIO_Pin_6
#define IR3_GPIO_Port GPIOA
#define IR2_Pin GPIO_Pin_7
#define IR2_GPIO_Port GPIOA
#define IR1_Pin GPIO_Pin_4
#define IR1_GPIO_Port GPIOC
uchar irT1=0, irT2=0, irT3=0, irT4=0, irT5=0; 
uchar forward[]={0x11,0x39,0x28,0x6c,0x44,0xc6,0x82,0x93}; 
uchar right[] ={0x11,0x99,0x88,0xcc,0x44,0x66,0x22,0x33}; 
uchar left[] ={0x11,0x33,0x22,0x66,0x44,0xcc,0x88,0x99};
uchar PA;
uchar i,j; 

uchar maze[8][8];					//前4回溯信息，后四挡板
int yiDongX[4]={0,1,0,-1};		
int yiDongY[4]={1,0,-1,0};
uchar huiSu[4]={0xdf,0xef,0x7f,0xbf};		//下右上左，回溯方向      
int x, y;					
uchar Direction;			
uchar StackX[64];		
uchar StackY[64];
uchar top=0;				



void gpio()
{
	  GPIO_InitTypeDef  GPIO_InitStructure;
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD, ENABLE);	    

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;			   
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 
  GPIO_Init(GPIOA, &GPIO_InitStructure);			     
 // GPIO_SetBits(GPIOA,GPIO_Pin_3);					//高电平
 //	GPIO_ResetBits(GPIOA,GPIO_Pin_3);				//低电平

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;			   
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;			   
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;			   
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;			   
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;			   
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;			   
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;			   
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//红外

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC| RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);

	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	

}

void ir_on(unsigned char num)
{
 num -= 1;
 if(num&0x01)
{
 GPIO_WriteBit(A0_GPIO_Port, A0_Pin, Bit_SET);
 }else
{
 GPIO_WriteBit(A0_GPIO_Port, A0_Pin, Bit_RESET);
 }
 if(num&0x02)
{
 GPIO_WriteBit(A1_GPIO_Port, A1_Pin, Bit_SET);
 }else
{
 GPIO_WriteBit(A1_GPIO_Port, A1_Pin, Bit_RESET);
 }
 if(num&0x04)
{
 GPIO_WriteBit(A2_GPIO_Port, A2_Pin, Bit_SET);
 }else
{
 GPIO_WriteBit(A2_GPIO_Port, A2_Pin, Bit_RESET);
 }
}
unsigned char get_ir(unsigned char num) //???????
{
 if(num==1)
 return irT1;
 else if(num==2)
 return irT2;
 else if(num==3)
 return irT3;
 else if(num==4)
 return irT4;
 else if(num==5)
 return irT5;
 else 
return 0;
}
void TIM6_Confing(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
	TIM_TimeBaseStructure.TIM_Period=1000;
	TIM_TimeBaseStructure.TIM_Prescaler= 71;
	TIM_TimeBaseStructure.TIM_ClockDivision=0;
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE);
	TIM_ClearFlag(TIM6, TIM_FLAG_Update);
	TIM_Cmd(TIM6, ENABLE);
}
void NVIC_Confing()
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);
}

void TIM6_IRQHandler()
{
	if(TIM_GetITStatus(BASIC_TIM,TIM_IT_Update)!=RESET)
	{
		static uint8_t Flag = 0;
		static uint8_t n = 1;
		if(!Flag)
		{
			ir_on(n);
		}
		else
		{
		if(n == 5)
				{
					if(GPIO_ReadInputDataBit(IR1_GPIO_Port, IR1_Pin))
						irT1 = 0;
					else
						irT1 = 1;
				}
			else if(n == 4)
				{	
					if(GPIO_ReadInputDataBit(IR2_GPIO_Port, IR2_Pin))
						irT2 = 0;
					else
						irT2 = 1;
				}
			else if(n == 3)
				{
					if(GPIO_ReadInputDataBit(IR3_GPIO_Port, IR3_Pin))
						irT3 = 0;
					else
						irT3 = 1;
				}
			else if(n == 2)
				{
					if(GPIO_ReadInputDataBit(IR4_GPIO_Port, IR4_Pin))
						irT4 = 0;
					else
						irT4 = 1;
				}
			else if(n == 1)
				{
					if(GPIO_ReadInputDataBit(IR5_GPIO_Port, IR5_Pin))
						irT5 = 0;
					else
						irT5 = 1;
				}
			}
			if(Flag)
				n++;
			if(n>5)
				n=0;
			Flag = !Flag;
			TIM_ClearITPendingBit(BASIC_TIM,TIM_FLAG_Update);
	}
}
	
	
	
void change(int PA)
{
	int a, b, c, d, e, f, g, h;
	h = PA % 2;
	PA = PA / 2;
	g = PA % 2;
	PA = PA / 2;
	f = PA % 2;
	PA = PA / 2;
	e = PA % 2;
	PA = PA / 2;
	d= PA % 2;
	PA = PA / 2;
	c = PA % 2;
	PA = PA / 2;
	b = PA % 2;
	PA = PA / 2;
	a = PA % 2;
	PA = PA / 2;

	
	if(a==1)
		GPIO_SetBits(GPIOA,GPIO_Pin_8);
	else
		GPIO_ResetBits(GPIOA,GPIO_Pin_8);	
	if(b==1)
		GPIO_SetBits(GPIOA,GPIO_Pin_9);
	else
		GPIO_ResetBits(GPIOA,GPIO_Pin_9);	
	if(c==1)
		GPIO_SetBits(GPIOA,GPIO_Pin_10);
	else
		GPIO_ResetBits(GPIOA,GPIO_Pin_10);	
	if(d==1)
		GPIO_SetBits(GPIOA,GPIO_Pin_11);
	else
		GPIO_ResetBits(GPIOA,GPIO_Pin_11);	
	if(e==1)
		GPIO_SetBits(GPIOA,GPIO_Pin_0);
	else
		GPIO_ResetBits(GPIOA,GPIO_Pin_0);	
		if(f==1)
		GPIO_SetBits(GPIOA,GPIO_Pin_1);
	else
		GPIO_ResetBits(GPIOA,GPIO_Pin_1);	
		if(g==1)
		GPIO_SetBits(GPIOA,GPIO_Pin_2);
	else
		GPIO_ResetBits(GPIOA,GPIO_Pin_2);	
		if(h==1)
		GPIO_SetBits(GPIOA,GPIO_Pin_3);
	else
		GPIO_ResetBits(GPIOA,GPIO_Pin_3);	
	
}

void runLU()			//?????
{
	for(j = 0; j < 4; j++)
	{
		for(i = 0; i < 8; i++) 
		{
			PA=right[i]; 			
				change(PA);
			delay_ms(2);
		}
	}
}

void runRU()			//?????
{
	for(j = 0; j < 4; j++)
	{
		for(i = 0; i < 8; i++) 
		{
			PA=left[i]; 		
				change(PA);
			delay_ms(2);
		}
	}
}

void xiuL()			
{
	if(irT2)
	{
		runLU();
	}
}

void xiuR()			//????
{
	if(irT4)	
	{
		runRU();
	}
}	

void stra()
{
	xiuL();
	xiuR();
		for(j=0;j<13;j++)
		{
			for(i=0;i<8;i++)
			{
			PA=forward[i];
			change(PA);
			delay_ms(1);
			}
		}
}

void go_stra()
{
		stra();
//		stra();
//		stra();
//		stra();
//		stra();
//		stra();
//		stra();
//		stra();
}
void go_right()
{
 
		for(j=0;j<42;j++)
		{
			for(i=0;i<8;i++)
			{
			PA=right[i];
			change(PA);
			delay_ms(2);
			}
		}
			xiuL();
	xiuR();
		Direction = (Direction+1)%4;
}

void go_left()
	
{
 
		for(j=0;j<42;j++)
		{
			for(i=0;i<8;i++)
			{
			PA=left[i];
			change(PA);
			delay_ms(2);
			}
		}
			xiuL();
	xiuR();
		Direction = (Direction+3)%4;
}

void go_back()
{
 
		for(j=0;j<84;j++)
		{
			for(i=0;i<8;i++)
			{
			PA=right[i];
			change(PA);
			delay_ms(2);
			}
		}
			xiuL();
	xiuR();
		Direction = (Direction+2)%4;
}


void yidong()			//位置移动
{
	x += yiDongX[Direction];
	y += yiDongY[Direction];
}

int judgeLeft()
{
	if(Direction==0)
	{
		if(maze[x-1][y]==0xff)
			return 1;
	}
	if(Direction==1)
	{
		if(maze[x][y+1]==0xff)
			return 1;
	}
	if(Direction==2)
	{
		if(maze[x+1][y]==0xff)
			return 1;
	}
	if(Direction==3)
	{
		if(maze[x][y-1]==0xff)
			return 1;
	}
	return 0;
}

int judgeFront()
{
	if(Direction==0)
	{
		if(maze[x][y+1]==0xff)
			return 1;
	}
	if(Direction==1)
	{
		if(maze[x+1][y]==0xff)
			return 1;
	}
	if(Direction==2)
	{
		if(maze[x][y-1]==0xff)
			return 1;
	}
	if(Direction==3)
	{
		if(maze[x-1][y]==0xff)
			return 1;
	}
	return 0;
}

int judgeRight()
{
	if(Direction==0)
	{
		if(maze[x+1][y]==0xff)
			return 1;
	}
	if(Direction==1)
	{
		if(maze[x][y-1]==0xff)
			return 1;
	}
	if(Direction==2)
	{
		if(maze[x-1][y]==0xff)
			return 1;
	}
	if(Direction==3)
	{
		if(maze[x][y+1]==0xff)
			return 1;
	}
	return 0;
}

void tuizhan()
{

	while(1)
	{
		if(StackX[top-1] == x && StackY[top-1] == y)
		{
			top--;
			break;
		}
		if((maze[x][y] & 0xf0) == 0x70)
		{
		
			if(Direction == 1)
			{
				go_left();
				go_stra();
			}
			else if(Direction == 2)
			{
				go_back();
				go_stra();
			}
			else if(Direction == 3)
			{
				go_right();
				go_stra();
			}
			else
			{
				go_stra();
			}
			//返回的Direction = 0;
			yidong();
		
		}
		else if((maze[x][y] & 0xf0) == 0xb0)
		{
			
			if(Direction == 0)
			{
				go_right();
				go_stra();
			}
			else if(Direction == 2)
			{
				go_left();
				go_stra();
			}
			else if(Direction == 3)
			{
				go_back();
				go_stra();
			}
			else
			{
				go_stra();
			}
			//Direction = 1;
			yidong();
		
		}
		else if((maze[x][y] & 0xf0) == 0xd0)
		{
			if(Direction == 0)
			{
				go_back();
				go_stra();
			}
			else if(Direction == 1)
			{
				go_right();
				go_stra();
			}
			else if(Direction == 3)
			{
				go_left();
				go_stra();
			}	
			else
			{
				go_stra();
			}
			//Direction = 2;
			yidong();
			
		}
		else
		{
			
			if(Direction == 0)
			{
				go_left();
				go_stra();
			}
			else if(Direction == 1)
			{
				go_back();
				go_stra();
			}
			else if(Direction == 2)
			{
				go_right();
				go_stra();
			}
			else
			{
				go_stra();
			}
			//Direction = 3;
			yidong();
			
		}
	}

	
}



void xunLu()
{if(!irT5 && judgeRight())
	{
		go_right();
		go_stra();
		yidong();

	}
	 
else		if(!irT3 && judgeFront())
	{
		go_stra();
		yidong();
	
	}

	
	 
	
	else if(!irT1 && judgeLeft())
	{
		go_left();
		go_stra();
		yidong();
	
	}
	else
	{
		tuizhan();
	}
}
void bianli()
{ int Flag=0;
	while(1)
	{
		uint chaLu=0;
		xunLu();
		if(maze[x][y] == 0xff)		
		{
			maze[x][y] &= huiSu[Direction];
				
		}
		switch(Direction)
		{
			case 0:
				if(!irT3 && maze[x][y+1] == 0xff)		
				{
					maze[x][y] &= 0xf5;
					chaLu++;
				}
				if(!irT1 && maze[x-1][y] == 0xff)
				{
					maze[x][y] &= 0xfc;
					chaLu++;
				}
				if(!irT5 && maze[x+1][y] == 0xff)
				{
					maze[x][y] &= 0xf9;
					chaLu++;
				}
			
			break;
			case 1:
				if(!irT3 && maze[x+1][y] == 0xff)		
				{
					maze[x][y] &=0xfa;
					chaLu++;
				}
				if(!irT1 && maze[x][y+1] == 0xff)
				{
					maze[x][y] &=0xf6;
					chaLu++;
				}
				if(!irT5 && maze[x][y-1] == 0xff)
				{
					maze[x][y] &=0xfe;
					chaLu++;
				}
				break;
			case 2:
				if(!irT3 && maze[x][y-1] == 0xff)
				{
					maze[x][y] &=0xf5;
					chaLu++;
				}
				if(!irT1 && maze[x+1][y] == 0xff)
				{
					maze[x][y] &=0xf3;
					chaLu++;
				}
				if(irT5 && maze[x-1][y] ==0xff)
				{
					maze[x][y] &=0xf6;
					chaLu++;
				}
			break;
			case 3:
				if(!irT3 && maze[x-1][y]==0xff)
				{
					maze[x][y] &=0xfa;
					chaLu++;
				}
				if(!irT1 && maze[x][y-1]==0xff)
				{
					maze[x][y] &=0xf9;
					chaLu++;
				}
				if(!irT5 && maze[x][y+1]==0xff)
				{
					maze[x][y] &=0xf3;
					chaLu++;
				}
			}
			if(chaLu>=2)
			{
				StackX[top] = x;
				StackY[top] = y;
				top++;
			}
				if(x==7 && y==7)
		{
			Flag=1;
			delay_ms(500);
		}
		if(x==0 && y==0&&Flag==1  )
		{
			go_back();

			break;
		}
		
					
		}
	}

//等高表
uchar dui[8][8];
uchar tail = 0;	
int juLi = 1;
int x, y;
typedef struct Stu
{
	uchar x;
	uchar y;
}STU;
STU stu[64];
/*uchar maze[8][8] = {  {0x07, 0x05,0x09,0x03,0x05,0x05,0x09,0x0b},
											{0x07,0x01,0x08,0x02,0x05,0x0d,0x0a,0x0a},
											{0x0b,0x0a,0x06,0x04,0x01,0x01,0x04,0x0c},
											{0x0a,0x0a,0x07,0x05,0x08,0x06,0x05,0x09},
											{0x06,0x04,0x01,0x05,0x00,0x05,0x0d,0x0a},
											{0x03,0x01,0x04,0x05,0x04,0x01,0x0d,0x0a},
											{0x0a,0x0a,0x03,0x05,0x05,0x00,0x05,0x0c},
											{0x0e,0x06,0x04,0x05,0x0d,0x06,0x05,0x0d}};*/
void rudui(uchar a, uchar b)
{
	if (dui[a][b] == 99)
	{
		tail++;
		stu[tail - 1].x = a;
		stu[tail - 1].y = b;
	}
}
void chudui()
{
//	printf("%d  %d      %d\n", stu[0].x, stu[0].y,juLi);
	int i;
	for (i = 1; i < tail; i++)
	{
		stu[i - 1].x = stu[i].x;
		stu[i - 1].y = stu[i].y;
	}
	tail--;
}

void zhibiao()
{	uchar t;
	/* uchar map[8][8] = { {0x07, 0x05,0x09,0x03,0x05,0x05,0x09,0x0b},
											{0x07,0x01,0x08,0x02,0x05,0x0d,0x0a,0x0a},
											{0x0b,0x0a,0x06,0x04,0x01,0x01,0x04,0x0c},
											{0x0a,0x0a,0x07,0x05,0x08,0x06,0x05,0x09},
											{0x06,0x04,0x01,0x05,0x00,0x05,0x0d,0x0a},
											{0x03,0x01,0x04,0x05,0x04,0x01,0x0d,0x0a},
											{0x0a,0x0a,0x03,0x05,0x05,0x00,0x05,0x0c},
											{0x0e,0x06,0x04,0x05,0x0d,0x06,0x05,0x0d} };*/

	int i, j;
	int a, b, c, d;
	for (i = 0; i < 8; i++)
	{
		for (j = 0;j< 8; j++)
		{
			dui[i][j] = 99;
	//		maze[i][j] = map[i][j];
		}
	}
	rudui(0, 0);
	dui[0][0] = juLi;
	while (1)
	{
		x = stu[0].x;
		y = stu[0].y;
		juLi =( dui[stu[0].x][stu[0].y]+1);
		if (x==7&y==7)
		{
			break;
		}

		t = (maze[stu[0].x][stu[0].y] & 0x0f);
		
		d = t % 2;
		t = t / 2;
		c = t % 2;
		t = t / 2;
		b = t % 2;
		t = t / 2;
		a = t % 2;
		t = t / 2;
	//	printf("%d %d %d %d\n", a, b, c, d);
		if (a == 0 && dui[x][y + 1] == 99)
		{
			rudui(x, y + 1);
			dui[x][y + 1] = juLi;
	}
		if (b == 0 && dui[x + 1][y] == 99)
		{
			rudui(x + 1, y);
			dui[x + 1][y] = juLi;
		}
		if (c == 0 && dui[x][y - 1] == 99)
		{
			rudui(x, y - 1);
			dui[x][y - 1] = juLi;
		}
		if (d == 0 && dui[x - 1][y] == 99)
		{
			rudui(x - 1, y);
			dui[x - 1][y] = juLi;
		}
		chudui();








	}
}
typedef struct sho
{
	uchar x;
	uchar y;
}SHO;
SHO shor[64];
int s = 0;
void find()
{	int t, a, b, c, d;

	juLi = dui[7][7];
	shor[s].x = 7;
	shor[s].y = 7;


	while (1)
	{

	//	printf("(%d , %d)    %d    %d\n ", shor[s].x, shor[s].y,juli,s);
		t = (maze[shor[s].x][shor[s].y] & 0x0f);
		d = t % 2;
		t = t / 2;
		c = t % 2;
		t = t / 2;
		b = t % 2;
		t = t / 2;
		a = t % 2;
		t = t / 2;
		if (shor[s].x == 0 && shor[s].y == 0)
		{
			break;
		}
		if (dui[shor[s].x + 1][shor[s].y] == (juLi - 1)&&b==0)
		{
			s++;
			shor[s].x = (shor[s - 1].x + 1);
			shor[s].y = shor[s - 1].y;
			juLi--;
		}
		else if (dui[shor[s].x][shor[s].y + 1] == (juLi - 1)&&a==0)
		{
			s++;
			shor[s].y = (shor[s - 1].y + 1);
			shor[s].x = shor[s - 1].x;
			juLi--;
		}
		else if (dui[shor[s].x - 1][shor[s].y] == (juLi - 1)&&d==0)
		{
			s++;
			shor[s].x = (shor[s - 1].x - 1);
			shor[s].y = shor[s - 1].y;
			juLi--;
		}
		else if (dui[shor[s].x][shor[s].y - 1] == (juLi - 1)&&c==0)
		{
			s++;
			shor[s].y = (shor[s - 1].y - 1);
			shor[s].x = shor[s - 1].x;
			juLi--;
		
		}
		
	}
}

void chongchi()
{
	while(1)
	{
		if(shor[s-1].y==shor[s].y+1)
		{
			switch(Direction)
			{
				case 0:go_stra();
				break;
				case 1:go_left();
							go_stra();
				break;
				case 2:go_back();
				go_stra();
				break;
				case 3:go_right();
				go_stra();
			}
			s--;
		}
		else if(shor[s-1].x==shor[s].x+1)
		{
			switch(Direction)
			{
				case 0:go_right();
					go_stra();
				break;
				case 1:go_stra();
				break;
				case 2:go_left();
						go_stra();
						break;
				case 3:
					go_back();
					go_stra();
			}
			s--;
		}
		else if(shor[s-1].y==shor[s].y-1)
		{
			switch(Direction)
			{
				case 0:go_back();
					go_stra();
				break;
				case 1:go_right();
					go_stra();
				break;
				case 2:
						go_stra();
						break;
				case 3:
					go_left();
					go_stra();
			}
			s--;
		}
		else if(shor[s-1].x==shor[s].x-1)
		{
			switch(Direction)
			{
				case 0:go_left();
					go_stra();
				break;
				case 1:go_back();
					go_stra();
				break;
				case 2:go_right();
						go_stra();
						break;
				case 3:
					go_stra();
			}
			s--;
		}
		if(s==0)
		{
			break;
		}
	}
}
	

void bizhang()
{if(!irT3 )
	{
		go_stra();
		
	
	}
else 
{
	if(!irT5)
	{
			go_right();
	}
	else if(!irT1)
	{
		go_left();
	}
	else
	{
		go_back();
	}
}
}
int main()
{
	
	gpio();
	NVIC_Confing();
	TIM6_Confing();
//	while(1)
//	{
//		bizhang(); 
//	}
	
	for(x = 0; x < 8; x++)		//初始化
	{
		for(y = 0; y < 8; y++)
		{
			maze[x][y] = 0xff;
		}
	}
	x = y = 0;			
	StackX[top] = 0;	//栈
	StackY[top] = 0;
	top++;				
	maze[x][y] = 0x07;	//起点设置

	Direction=0 ;		
	delay_ms(1000);
	bianli();
	
		zhibiao();
	go_back();
	go_back();
			find();
	go_back();
	go_back();
			chongchi();
}
