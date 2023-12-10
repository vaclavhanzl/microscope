// Microscope RGB lamp driver
// Copyright (C) 2023 Vaclav Hanzl
// See https://github.com/vaclavhanzl/microscope

#include <avr/io.h>

void delay_us(int us){ /* delay us microseconds, may be quite approximate */
  volatile int loo;
  for(loo = (us-(us>>4)); loo; loo--);
}

void delay_ms(int ms){ /* delay ms miliseconds */
  volatile int loo;
  for(loo = ms; loo; loo--)
      delay_us(1000);
}

/*
0: -------________________          typ.   350+800
   ----======_____________======           +-150
   ----_____________                fastest
   ----------___________________    slowest

1: --------------____________       typ.  700+600
   -----------======_________======       +-150
   -----------_________             fastest
   -----------------_______________ slowest
             ^ sampling moment 
*/

inline void send_zero(void){
  PORTB = 1; PORTB = 1; PORTB = 1; PORTB = 1; PORTB = 1; // 5 times

  PORTB = 0; PORTB = 0; PORTB = 0; PORTB = 0; PORTB = 0;
  PORTB = 0; PORTB = 0; PORTB = 0; PORTB = 0; PORTB = 0;
  PORTB = 0; // 11 times
}

inline void send_one(void){
  PORTB = 1; PORTB = 1; PORTB = 1;  PORTB = 1; PORTB = 1; // 10 times
  PORTB = 1; PORTB = 1; PORTB = 1;  PORTB = 1; PORTB = 1;

  PORTB = 0; PORTB = 0; PORTB = 0;  PORTB = 0; PORTB = 0; // 9 times
  PORTB = 0; PORTB = 0; PORTB = 0;  PORTB = 0;
}

void send_reset(void){
  PORTB = 0;
  delay_us(600);
}


void send_num(void){ // send mean number
  send_zero(); send_zero(); send_zero();
  send_one();
  send_zero(); send_zero(); send_zero(); send_zero();
}

void send_bignum(void){ // send big number
  send_one(); send_one(); send_one();
  send_zero(); send_zero(); send_zero(); send_zero(); send_zero();
}
void send_bnum(void){ // send not so big number
  send_zero(); send_zero();
  send_one(); send_one(); send_one();
  send_zero(); send_zero(); send_zero();
}

void send_smallnum(void){ // send small number
  send_zero(); send_zero(); send_zero(); send_zero(); send_zero();
  send_one();
  send_zero(); send_zero();
}

void send_dark(){ // send zero
  send_zero(); send_zero(); send_zero(); send_zero();
  send_zero(); send_zero(); send_zero(); send_zero();
}

void send_red(){
  send_dark();
  send_num();
  send_dark();
}
void send_brightred(){
  send_dark();
  send_bignum();
  send_dark();
}
void send_green(){
  send_num();
  send_dark();
  send_dark();
}
void send_blue(){
  send_dark();
  send_dark();
  send_num();
}
void send_bblue(){
  send_dark();
  send_dark();
  send_bnum();
}
void send_brightblue(){
  send_dark();
  send_dark();
  send_bignum();
}

void send_black(){
  send_dark();
  send_dark();
  send_dark();
}


int main(void){
  int shift = 0;
  int i;

  DDRB=0xFF; // port B are all outputs

  PORTB = 2;    // blink HW-260 onboard LED once
  delay_ms(100);
  PORTB = 0;

  while(1){
    for(i = 0; i < 12; i++)  // outer ring
      send_red();

    for(i = 0; i < 8; i++)  // mid ring
      send_green();

    send_bblue(); // center LED

    send_reset();
    delay_ms(50);
  }
}



