# LED_API_project
##API
###int LED_init();
初期化
戻り値 成功=1 , 失敗=0
###int LED_start();
点滅開始
戻り値 成功=1 , 失敗=0
###int LED_stop();
点滅停止
戻り値 成功=1 , 失敗=0
###int LED_set_pattern_num(unsigned int pattern);

##点滅パターン設定
登録してある点滅パターン番号を入れる
1:常時点灯
2:等間隔点灯
3:長点灯(2拍子)、消灯、短点灯(1拍子)消灯
4:3-3-7拍子
これ以外の点滅パターンを各自で追加してOK
戻り値 成功=1 , 失敗=0
###int LED_set_pattern_bit(char* pattern, int size);

##スピード(点滅間隔)変更
ポインタ先から指定サイズビットがパターンとなる(1=on,0=off)
ポインタの先が 0xa5,0x35 の2バイトでサイズが16なら
1010010100110101 という点滅パターン。MSBで解釈
戻り値 成功=1 , 失敗=0
###int LED_set_interval(unsigned int msec);

##点滅間隔を設定(1以上,1000以下))
戻り値 成功=1 , 失敗=0
###int LED_get_interval(int msec);
##点滅間隔の設定値取得
戻り値 msec単位のinterval値 , 失敗=-1
###int LED_get_status();

##状態取得(呼んだときに光っているかどうか)
戻り値 点灯=1 , 消灯=0
###void LED_main();

定期的に呼ぶことでLEDドライバを駆動するAPI
sample

int button_state = 0;
int led_ctrl = 0;


LED_init();

while(1){


    if(HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin)==0){  // ボタンが押された
        if(button_state == 0){
            button_state = 1;
            led_ctrl++;

            switch(led_ctrl){
            case 1:
                break;
            case 2:
                break;
            case 3:
                break;
            case 4:
                break;
            case 5:
                led_ctrl = 0;
                break;
            }
        }
    }
    else{
        if(button_state == 1){
            button_state = 0;
        }
    }

    LED_main();
    HAL_Delay(10);
}
