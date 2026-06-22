/*
 * led_driver.c
 *
 *  Created on: 2026/06/19
 *      Author: ryonehara
 */
# include "led_driver.h"

int interval;
int pattern;
int size;
char* str;
int msec;

int LED_init()
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);

	return 1;
}

//イニシャライズされていない状態でstartは準備不足でエラー
int LED_start()
{
	return 1;
}

int LED_stop()
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
	return 1;
}

int LED_set_pattern_num(unsigned int pattern)
{
	static int cnt3 = 0;
	static int cnt4 = 0;
	static int flag = 1;

	if(pattern < 1 || 4 < pattern){
		return 0;
	}

	switch(pattern){
		case 1:
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
			break;
		case 2:
			HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);					//点灯・消灯反転
			HAL_Delay(500);
			break;
		case 3:

			cnt3++;

			if(cnt3 <= 100){
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
			}else if(cnt3 <= 150){
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
			}else if(cnt3 <= 200){
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
			}else if(cnt3 <= 250){
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
				cnt3 = 0;
			}
			break;
		case 4:
			//一拍終わりごとに時間（100ms）カウント初期化
			//3-3-7終わりごとにフラグ初期化

			cnt4++;

			if(flag <= 16){
				//4,8,16回目以外　ON(250ms), OFF(250ms)をくりかえす
				if(flag != 4 && flag != 8 && flag != 16){
					if(cnt4 <= 25){
						HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
					}else if(cnt4 <= 50){
						HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
					}else{
						cnt4 = 0;
						flag++;
					}
				//4,8,16回目のみ　off(500ms)
				}else if(flag == 4 || flag == 8 || flag == 16){
					if(cnt4 <= 500){
						HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
					}else{
						cnt4 = 0;
						flag++;
					}
				}
			}else{
				flag = 0;
			}
	}


		return 1;
}

//32ビットが最大
int LED_set_pattern_bit(char* pattern, int size)
{
	if(size < 1 || 32 < size){
		return 0;
	}
}

/*int LED_set_interval(unsigned int msec) //10の倍数のみ
{

}

int LED_get_interval()
{

}

int LED_get_status()
{

}
*/
void LED_main()
{
	int button_state = 0;
	int led_ctrl = 0;

	LED_init();

	while(1){

		if(HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin)==0){  // ボタンが押された
			if(button_state == 0){
				button_state = 1;;
				led_ctrl++;
				LED_start();

				switch(led_ctrl){
				case 1:
					LED_set_pattern_num(pattern);
					break;
				case 2:
					LED_set_pattern_bit(str, size);
					break;
				case 3:
					LED_set_interval(msec);
					break;
				case 4:
					LED_get_status();
					break;
				case 5:
					led_ctrl = 0;
					break;
				}
				LED_stop();
			}
		}
		else{
			if(button_state == 1){
				button_state = 0;
			}
		}
	}
}

