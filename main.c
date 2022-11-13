#include <stdio.h>
#include <limits.h>
#include "TM4C123GXL.h"

void delay(){
  for(int volatile i=0;i<1;i++){
    for(int volatile j=0;j<20000;j++){}
  }
}
char getInput(){
  int volatile PORTE=GPIO_PORTE_DATA_R^0xFFU,PORTB=GPIO_PORTB_DATA_R^0xFFU,Priority=0x80,activePin;
  while(PORTE==0x00U && PORTB==0x00U){PORTE=GPIO_PORTE_DATA_R^0xFFU;PORTB=GPIO_PORTB_DATA_R^0xFFU;}
  while(Priority!=0x00){
    activePin=PORTB&Priority;
    switch(activePin){
      case 0x80: return '7';
                 break;
      case 0x40: return '6';
                 break;
      case 0x20: return '5';
                 break;
      case 0x10: return '4';
                 break;
      case 0x08: return '3';
                 break;
      case 0x04: return '2';
                 break;
      case 0x02: return '1';
                 break;
      case 0x01: return '0';
                 break;
    }
    Priority=Priority>>1;
  }
  Priority=0x10;
  while(Priority!=0x00){
    activePin=PORTE&Priority;
    switch(activePin){
      case 0x01: return '8';
                 break;
      case 0x02: return '9';
                 break;
      case 0x04: return '+';
                 break;
      case 0x08: return '-';
                 break;
      case 0x10: return '=';
                 break;           
    }
    Priority=Priority>>1;
  }
  return NULL;
}
int result(int operand1,int operand2,char operation){
  if(operation=='+')
    return operand1+operand2;
  else
    return operand1-operand2;
}
int main()
{ 
  SYSCTL_RCGCGPIO_R=0x12U;
  GPIO_PORTE_DIR_R=GPIO_PORTB_DIR_R=0x00U;
  GPIO_PORTE_PUR_R=GPIO_PORTB_PUR_R=0xFFU;
  GPIO_PORTE_DEN_R=GPIO_PORTB_DEN_R=0xFFU;
  while(1){
  
    int volatile operand1,operand2;
    char volatile operation,input;
    long long int check;
start:
      operand1=operand2=0;
      check=0;
      operation='\0';
operand1:
    delay();
    input=getInput();
    if(input=='+' || input=='-' || input=='='){
      printf("Syntax error:re-enter operand1\n");
      goto start;
    }
    while (input!='+'&&input!='-' && input!= '=' && check<INT_MAX){
      int volatile buffer=input-'0';
      operand1=operand1*10+buffer;
      check=check*10+buffer;
      delay();
      printf("%c",input);
      input=getInput();
    }
    if(check>INT_MAX){
      printf("Overflow:First operand get beyond its max limit of %d.Please re-enter operand1 \n",INT_MAX);
      goto operand1;
    }
    while(input=='+' || input=='-'){
    operation=input;
    delay();
    input=getInput();
    } 
    if(input == '='){
      printf("%c%d \n",input,operand1);
      goto start;
    }
    printf("%c",operation);
operand2:
  check=0;
    while (input!='=' && check<INT_MAX){
      int buffer=input-'0';
      operand2=operand2*10+buffer;
      check=check*10+buffer;
      delay();
      printf("%c",input);
      input=getInput();
    }
    if(check>INT_MAX){
      printf("Overflow:Second operand get beyond its max limit of %d.Please re-enter operand2\n",INT_MAX);
      goto operand2;
    }
    if(operation=='+'?operand2<INT_MAX-operand1:1)
      printf("%c%d\n",input,result(operand1,operand2,operation));
    else
      printf("Overflow:Sum get beyond its max limit of %d.Please re-enter the experision correctly\n",INT_MAX);
    goto start;
  }
  return 0;
}