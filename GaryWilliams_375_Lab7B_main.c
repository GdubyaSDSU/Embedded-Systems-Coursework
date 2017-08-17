/*	Gary Williams
 *	COMPE 375-01
 *	Lab 7B Due: 3/24/17
 *
 *      Referenced: Discover-Flash sample by Dr. Baris Aksanli
 *      Really, I only added the Morse Code functionality
 */

#include "main.h"
#include "stm32f0xx_conf.h"
#include <stdio.h>
#include <ctype.h>

#define MS_DELAY_HSI (1000)
#define SECOND_DELAY (1000*MS_DELAY_HSI)

uint32_t TickValue=0;

void Init_GPIO(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOC | RCC_AHBPeriph_GPIOB, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 ;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
}
void TimingDelay_Decrement(void)
{
  TickValue--;
}
void delay(uint32_t counts)
{
  while(counts-- != 0) ;
}
void delay_ms(uint32_t n_ms)
{
  SysTick_Config(8000*PLL_MUL_X - 30);
  TickValue = n_ms;
  while(TickValue == n_ms) ;
  SysTick_Config(8000*PLL_MUL_X);
  while(TickValue != 0) ;
}
void dot(void){
   GPIOC->ODR = 0x0300;
   delay_ms(200);
   GPIOC->ODR = 0x0000;
   delay_ms(200);
}
void dash(void){
  GPIOC->ODR = 0x0300;
  delay_ms(600);
  GPIOC->ODR = 0x0000;
  delay_ms(200);
}
void spaceL(void){
  delay_ms(400);
}
void spaceW(void){
  delay_ms(800);
}
void blinkMorse(char *n);

int main(void)
{
  Init_GPIO();
  char *whoami = "Gary Williams 803162497";
  blinkMorse(whoami);
}

void blinkMorse(char *n){
   int i;
   //For this assignment, endless loop of received string
   while(1){
    i=0;
    while(n[i]){
      switch(toupper(n[i])){
      case 'A':
        dot(); dash(); spaceL(); break;
      case 'B':
        dash(); dot(); dot(); dot(); spaceL(); break;
      case 'C':
        dash(); dot(); dash(); dot(); spaceL(); break;
      case 'D':
        dash(); dot(); dot(); spaceL(); break;
      case 'E':
        dot(); spaceL(); break;
      case 'F':
        dot(); dot(); dash(); dot(); spaceL(); break;
      case 'G':
        dash(); dash(); dot(); spaceL(); break;
      case 'H':
        dot(); dot(); dot(); dot(); spaceL(); break;
      case 'I':
        dot(); dot(); spaceL(); break;
      case 'J':
        dot(); dash(); dash(); dash(); spaceL(); break;
      case 'K':
        dash(); dot(); dash(); spaceL(); break;
      case 'L':
        dot(); dash(); dot(); dot(); spaceL(); break;
      case 'M':
        dash(); dash(); spaceL(); break;
      case 'N':
        dash(); dot(); spaceL(); break;
      case 'O':
        dash(); dash(); dash(); spaceL(); break;
      case 'P':
        dot(); dash(); dash(); dot(); spaceL(); break;
      case 'Q':
        dash(); dash(); dot(); dash(); spaceL(); break;
      case 'R':
        dot(); dash(); dot(); spaceL(); break;
      case 'S':
        dot(); dot(); dot(); spaceL(); break;
      case 'T':
        dash(); spaceL(); break;
      case 'U':
        dot(); dot(); dash(); spaceL(); break;
      case 'V':
        dot(); dot(); dot(); dash(); spaceL(); break;
      case 'W':
        dot(); dash(); dash(); spaceL(); break;
      case 'X':
        dash(); dot(); dot(); dash(); spaceL(); break;
      case 'Y':
        dash(); dot(); dash(); dash(); spaceL(); break;
      case 'Z':
        dash(); dash(); dot(); dot(); spaceL(); break;
      case '0':
        dash(); dash(); dash(); dash(); dash(); spaceL(); break;
      case '1':
        dot(); dash(); dash(); dash(); dash(); spaceL(); break;
      case '2':
        dot(); dot(); dash(); dash(); dash(); spaceL(); break;
      case '3':
        dot(); dot(); dot(); dash(); dash(); spaceL(); break;
      case '4':
        dot(); dot(); dot(); dot(); dash(); spaceL(); break;
      case '5':
        dot(); dot(); dot(); dot(); dot(); spaceL(); break;
      case '6':
        dash(); dot(); dot(); dot(); dot(); spaceL(); break;
      case '7':
        dash(); dash(); dot(); dot(); dot(); spaceL(); break;
      case '8':
        dash(); dash(); dash(); dot(); dot(); spaceL(); break;
      case '9':
        dash(); dash(); dash(); dash(); dot(); spaceL(); break;
      case ' ':
        spaceW(); break;
      }
      i++;
    }
    delay_ms(5000);     //To delay between output streams to show seperate
   }
}