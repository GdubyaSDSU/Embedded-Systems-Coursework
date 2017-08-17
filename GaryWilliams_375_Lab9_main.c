/*	Gary Williams
 *	COMPE 375-01
 *	Lab 9 Due: 4/28/17
 *
 *      Referenced: USARTsample.c by Dr. Baris Aksanli
 *
 *      Used USART2 to transmit RedID continuously. Output read
 *        using RS232 board and micro USB cable connected to terminal.
 */

#include "stm32f0xx.h"

static __IO uint32_t TimingDelay;
void delay (int a);
static int Baud = 9600;
static long F_CLK = 48000000L;


void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x0000)
  { 
    TimingDelay--;
  }
}


void USART_init()
{
  // Please Go though the Reference Manual- Page 103- Figure 11 to understand the clock setting 
  // Enable GPIOA and USART2 Clocks- See RCC_AHBENR and RCC_AP1B1ENR Registers
  RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
  RCC->APB1ENR |= RCC_APB1ENR_USART2EN; 
  
  // Enable HSE and HSI Clock- See RCC_CR register
  //RCC->CR |= (uint32_t)0x00010001;
  RCC->CR |= RCC_CR_HSEON | RCC_CR_HSION;
  
  // Select HSE as System Clock - See RCC_CFGR Register
  //RCC->CFGR &= ~((uint32_t)0x00000002);
  //RCC->CFGR |= (uint32_t)0x00000001;
  RCC->CFGR |= RCC_CFGR_SWS_0;
  
  // Select SYSCLK or HSI or PCLK as clock source for USART - See RCC_CFGR3 Register
  //RCC->CFGR3 |= (uint32_t)0x00010000; //Syscllk USART2 clock source
  RCC->CFGR3 |= RCC_CFGR3_USART2SW_0;   //SysCLK USART2 clock source
  
  //Set PA2 and PA3 MODE ad Alternate Function
  GPIOA->MODER |= (uint32_t)0x000000A0;
  
  //Set the Alternate function as USART Tx- Refer Table 14 in the Data Sheet(Not Reference Manual)
  GPIOA->AFR[0] |= (uint32_t)0x00001100;        //I think this is the AFRL
  
  //Set the Alternate function as USART Rx 
  
  // Temperarily Disable USART2 for setting the Baud rates - See USARTx_CR1 Register
  USART2->CR1 &= ~((uint32_t)0x00000001);
  
  // Set USART2_BRR register - Baud Rate Setting = F_CLK/Baud_Rate
  USART2->BRR = F_CLK/Baud;                //0x1388;
  
  // Set the Number of Stop Bits as 1 - See USARTx_CR1 Register
  //default 8 bit & 1 stop
  
  // Enable Recieve and Transmit using TE and RE bits in USARTx_CR1 Register
  USART2->CR1 |= (uint32_t)0x0000000D;  //TE, RE, and UE bits
  //USART2->CR1 |= USART2_CR1_TE | USART_CR1_RE;
  
  // Enable USART using UE bit in USARTx_CR1 Register  
}


int main(void)
{
  char redid[]= {'8','0','3','1','6','2','4','9','7'};
  /* Declare you Variables*/
  USART_init();

  while(1)
  {
    /* Transmit and Receive codes */
    for(int i=0; i<sizeof(redid); i++){
      while(!(USART2->ISR & (uint32_t)0x00000080));     //wait for TXE flag
      //TXE - transmit data register empty flag
      USART2->TDR = redid[i];
    }
    while(!(USART2->ISR & (uint32_t)0x00000080));
    USART2->TDR = 0x0D;         //Send carriage return
    while(!(USART2->ISR & (uint32_t)0x00000080));
    USART2->TDR = 0x0A;         //Send line feed
    while(!(USART2->ISR & (uint32_t)0x00000080));
    TimingDelay = (uint32_t) F_CLK/2;
    TimingDelay_Decrement();
  }
  
  return 0;
}
