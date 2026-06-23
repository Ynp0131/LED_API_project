/*
 * led_driver.c
 *
 *  Created on: 2026/06/19
 *      Author: ryonehara
 */
#include "led_driver.h"
#include <stdio.h>
#include <string.h>

variable info;

int LED_init()
{
	//構造体初期化
	info.pattern_num = 0;
	info.pattern[33] = 0x0000;	//ビットパターン
	info.pattern_bit = 0;	//読み取ったビットの中身（０or１）
	info.interval = 100;		//何秒間隔で状態を読み取るか（カウントの目標値）
	info.status = 0;		//1：ON状態、2:OFF状態
	info.F_running = 0;		//1 : 動作状態  0 : 停止状態

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);	//消灯

	info.F_initialized = 1; 	//初期化完了フラグ

	return 1;
}

//イニシャライズされていない状態でstartは準備不足でエラー
int LED_start()
{
	if(info.F_initialized != 1){
		return 0;
	}

	info.F_running = 1;
	return 1;
}

int LED_stop()
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
	info.F_running = 0;
	return 1;
}

int LED_set_pattern_num(unsigned int pattern)
{
	static int cnt = 0;		//今経った時間
	static GPIO_PinState state; 	//0:消灯　or　1:点灯

	//**********エラー処理****************
	if(info.F_running != 1){
		return 0;
	}

	if(pattern < 1 || 4 < pattern){
		return 0;
	}
	//*********************************


	switch(pattern){
		case 1:
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
			break;
		case 2:
			HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);		//点灯・消灯反転
			break;
		case 3:
			char pattern3[1] = {0b11010};
			LED_set_pattern_bit(pattern3, 5);
			state = LED_get_status();

			state = 1 ? GPIO_PIN_SET : GPIO_PIN_RESET;

			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, state);
			break;
		case 4:
			char pattern4[4] = {0xA8,0xA8,0xAA,0xA8};
			LED_set_pattern_bit(pattern4, 32);
			state = LED_get_status();

			state = 1 ? GPIO_PIN_SET : GPIO_PIN_RESET;

			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, state);
			break;
	}

		return 1;
}

//32ビットが最大
//ポインタ変数がNULL又はsizeが範囲外でエラー)
int LED_set_pattern_bit(char* pattern, int size)
{
	int i;

//**********エラー処理***************
	if(size < 1 || 32 < size){
		return 0;
	}

	if(pattern == NULL){
		return 0;
	}
//********************************

	info.pattern_bit = pattern[0];

	for(i = 0; i < size; i++){
		pattern[i] <<= 1;
	}

	return 1;

}

int LED_set_interval(unsigned int msec) //10の倍数のみ
{
//**********エラー処理***************
	if(msec < 10 || 1000 < msec){
		return 0;
	}
//********************************

	info.interval = msec;
	return 1;
}

int LED_get_interval()
{
	return info.interval;
}

int LED_get_status()
{
	return info.pattern_bit;
}

void LED_main()
{
	int cnt = 0;	//10ms毎の秒数カウント
	cnt++;

	if(info.F_running == 1){				//開始状態
		if(cnt*10 == info.interval){		//インターバル間隔で以下の関数を呼び出す
			switch (info.num)
			 {
				 case 1:
					 LED_set_pattern_num(1);
					 LED_set_interval(100);
					 LED_start();
					 break;

				 case 2:
					 LED_set_pattern_num(2);
					 LED_set_interval(500);
					 LED_start();
					 break;

				 case 3:
					 LED_set_pattern_num(3);
					 LED_set_interval(500);
					 LED_start();
					 break;

				 case 4:
					 LED_set_pattern_num(4);
					 LED_set_interval(250);
					 LED_start();
					 break;

				 case 5:
					 LED_stop();
					 led_ctrl = 0;
					 break;
			 }
		}cnt = 0;
	}
}

