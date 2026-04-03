#include "encoder.h"
#include "i2c.h"
#include "tim.h" //分配tim6 1000Hz

int32_t raw_angle[NUM_ENCODERS] = {0, 0, 0, 0, 0, 0, 0};
float Joint_Angle[NUM_ENCODERS] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
HAL_StatusTypeDef status[NUM_ENCODERS] = {HAL_OK, HAL_OK, HAL_OK, HAL_OK, HAL_OK, HAL_OK, HAL_OK};

static const uint16_t Device_Adress[NUM_ENCODERS] = {
    0x10 << 1,
    0x12 << 1,

    0x20 << 1,
    0x21 << 1,
    0x22 << 1,

    0x36 << 1, //4.7kΩ
    0x40 << 1, //10kΩ
};

static I2C_HandleTypeDef* const Handler[NUM_ENCODERS] = {
    &hi2c3,
    &hi2c3,

    &hi2c1,
    &hi2c1,
    &hi2c1,

    &hi2c3,
    &hi2c1,
};

static float Regulate_Radians(float angle){
    while(angle > PI) {angle -= 2.0f * PI;}
    while(angle < -PI) {angle += 2.0f * PI;}
    return angle;
}

void Encoder_Init(void){
    HAL_TIM_Base_Start_IT(&htim6);
}

void Encoder_Task(void) //1000Hz频率执行
{
    static const uint16_t RAW_ANGLE = 0x0C;
    static uint8_t rx_buffer[NUM_ENCODERS][2];
    static uint16_t cnt = 0;

    cnt = (cnt + 1) % 20; //每个任务50Hz(20ms) 最多20个任务
    
    for(int i=0; i<NUM_ENCODERS; i++){
        if(i == cnt){
            status[i] = HAL_I2C_Mem_Read_IT(Handler[i], Device_Adress[i], RAW_ANGLE, I2C_MEMADD_SIZE_8BIT, rx_buffer[i], 2);
        }
        if(10+i == cnt){
            raw_angle[i] = (int32_t)(((rx_buffer[i][0] << 8) | rx_buffer[i][1]) & 0x0FFF);
        }
    }

    if(cnt == 19){
        static const int32_t Joint_Offset[NUM_ENCODERS] = {
            -2652,
            -3744,
            -3368,
            -2164,
            -3689,
            -3386,
            -1094
        };
        #define PI_OVER_2048 (PI / 2048.0f) //12位分辨率
        //编码器安装顺序是乱的
        Joint_Angle[0] = + Regulate_Radians((Joint_Offset[0] + raw_angle[0]) * PI_OVER_2048 );

        Joint_Angle[1] = + Regulate_Radians((Joint_Offset[1] + raw_angle[2]) * PI_OVER_2048);
        
        Joint_Angle[2] = - Regulate_Radians((Joint_Offset[2] + raw_angle[1]) * PI_OVER_2048);

        Joint_Angle[3] = - Regulate_Radians((Joint_Offset[3] + raw_angle[3]) * PI_OVER_2048);

        Joint_Angle[4] = + Regulate_Radians((Joint_Offset[4] + raw_angle[4]) * PI_OVER_2048);

        Joint_Angle[5] = - Regulate_Radians((Joint_Offset[5] + raw_angle[6]) * PI_OVER_2048);

        Joint_Angle[6] = - Regulate_Radians((Joint_Offset[6] + raw_angle[5]) * PI_OVER_2048);
    }
}
