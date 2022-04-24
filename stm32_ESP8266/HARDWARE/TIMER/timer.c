#include "timer.h"
#include "gizwits_product.h"
#include "dht11.h"
#include "bsp_ili9341_lcd.h"
#include "bsp_adc.h"
//////////////////////////////////////////////////////////////////////////////////	 
//通用定时器2中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3!
void TIM2_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //时钟使能
	
	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); //使能指定的TIM2中断,允许更新中断

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器


	TIM_Cmd(TIM2, ENABLE);  //使能TIMx					 
}


float ADC_ConvertedValueLocal=0;//雨量ADC电压值
u8 temperature=0,humidity=0;//温湿度
// 全局变量，用于保存转换计算后的电压值 

void LCD_Show_Situation(void)
{
    char dispBuff1[100],dispBuff2[100];
    int Result = 0;
    LCD_SetFont(&Font8x16);
    LCD_SetTextColor(RED);
    
    Result= ADC_GetConversionValue(ADC_x);
    ADC_ConvertedValueLocal =(float) Result/4096*3.3; 
	sprintf(dispBuff1,"当前雨量AD转换电压：%.2f ",ADC_ConvertedValueLocal);
    
    if(ADC_ConvertedValueLocal >=0&&ADC_ConvertedValueLocal <0.8)//雨滴
    {
        LCD_SetTextColor(RED);
        ILI9341_DisplayStringEx(160,84, 32,32,"无雨",0);
    }
    else if(ADC_ConvertedValueLocal >=0.8&&ADC_ConvertedValueLocal <1.6)
    {
        LCD_SetTextColor(YELLOW);
        ILI9341_DisplayStringEx(160,84, 32,32,"小雨",0);
    }
    else if(ADC_ConvertedValueLocal >=1.6&&ADC_ConvertedValueLocal <2.4)
    {
        LCD_SetTextColor(BLUE);
       ILI9341_DisplayStringEx(160,84, 32,32,"中雨",0);
    }
    else
    {
        LCD_SetTextColor(WHITE);
        ILI9341_DisplayStringEx(160,84, 32,32,"大雨",0);
        
    }
    
//    LCD_ClearLine(LINE(4));
    ILI9341_DispStringLine_EN_CH(LINE(4),dispBuff1);

    LCD_SetTextColor(RED);
    ILI9341_DispStringLine_EN_CH(LINE(7),"我是一条分界线--------");
    
    DHT11_Read_Data(&temperature,&humidity); 	//读取温湿度值
    if(humidity>75||temperature>35)
        {
            LCD_SetTextColor(WHITE);
        }
        else if(humidity<50||temperature<20)
        {
            LCD_SetTextColor(BLUE);
        }
        else 
        {
            LCD_SetTextColor(RED);
        }
    sprintf(dispBuff2,"温度:%d℃  湿度:%d %%RH",temperature,humidity);
//    LCD_ClearLine(LINE(9));
    ILI9341_DispStringLine_EN_CH(LINE(9),dispBuff2);
//	printf("温度:%d.%d℃\r\n湿度:%d.%d%%RH\r\n",DHT11_Data.temp_int, DHT11_Data.temp_deci,DHT11_Data.humi_int, DHT11_Data.humi_deci);

	
}

u8 count=0;//计时
//定时器2中断服务程序
void TIM2_IRQHandler(void)   //TIM2中断
{
        static u16 cnt=0;
        if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
		{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //清除TIMx更新中断标志 
		}
		gizTimerMs();//系统毫秒定时
        cnt++;
        if(cnt>1000)//1S
        {
            count++;
            LCD_Show_Situation();//温湿度采集，无网状态也能正常工作
            cnt=0;  
        }
//        if(count==6)//6秒更新一次
//        {
//             LCD_Show_Situation();
//        }
}









