/*************************************************************************
    > File Name: main.cpp
    > Author: FanMing Luo
    > Mail: 151190065@smail.nju.edu.cn 
    > Created Time: 2017年01月12日 星期四 20时43分36秒
 ************************************************************************/
#include "infantry.h"
#include <iostream>
#include "ros/ros.h"
#include "stdlib.h"
#include "std_msgs/UInt8MultiArray.h"
#include "std_msgs/MultiArrayDimension.h"
#include <queue>
#include <assert.h>
#include "robot_protocol_msgs/RobotMotor.h"
#include "robot_protocol_msgs/RobotError.h"
#include "robot_protocol_msgs/RobotRC.h"
#include "robot_protocol_msgs/RobotOdometer.h"
#include "robot_protocol_msgs/RobotIMU.h"
#include "robot_protocol_msgs/RobotGimbal.h"
#include "robot_protocol_msgs/RobotControl.h"

#include "main.h"
ros::Publisher pub;
std_msgs::UInt8MultiArray m;
UnpackStep unpack_step_e_;
unsigned char protocol_packet_[PACK_MAX_SIZE];
int index_, byte_, data_length_;
GimbalControl gimbal_control_data_;
ChassisControl chassis_control_data_;
FrameHeader computer_frame_header_;
GameInfo game_information_;
HurtData robot_hurt_data_;
ShootData real_shoot_data_;
RfidData rfid_data_;
GameResult game_result_data_;
ChassisInfo chassis_information_;
GimbalInfo gimbal_information_;
ShootInfo shoot_task_data_;
InfantryError global_error_data_;
GameBuff get_buff_data_;
ServerToUser student_download_data_;
ConfigMessage config_response_data_;
CalibrateResponse cali_response_data_;
RcInfo rc_info_data_;
VersionInfo version_info_data_;
ShootControl shoot_task_control_;
void info (const std_msgs::UInt8MultiArray &msg){
    int read_len_ = msg.layout.dim[0].size;
    int static_len_ = msg.layout.dim[0].size;
    unsigned char * rx_buf_ = (unsigned char *)malloc(read_len_ * sizeof ( unsigned char ));
    for (int i = 0;i < read_len_; i++)
    {
        rx_buf_[i] = msg.data[i];
    }
    if (read_len_ > 0) {
        while (read_len_--) {
            byte_ = rx_buf_[static_len_ - read_len_];
            switch (unpack_step_e_) {
                case STEP_HEADER_SOF: {
                    if (byte_ == UP_REG_ID) {
                        protocol_packet_[index_++] = byte_;
                        unpack_step_e_ = STEP_LENGTH_LOW;
                    } else {
                        index_ = 0;
                    }
                }
                    break;
                case STEP_LENGTH_LOW: {
                    data_length_ = byte_;
                    protocol_packet_[index_++] = byte_;
                    unpack_step_e_ = STEP_LENGTH_HIGH;
                }
                    break;
                case STEP_LENGTH_HIGH: {
                    data_length_ |= (byte_ << 8);
                    protocol_packet_[index_++] = byte_;
                    if (data_length_ < (PROTOCAL_FRAME_MAX_SIZE - HEADER_LEN - CMD_LEN - CRC_LEN)) {
                        unpack_step_e_ = STEP_FRAME_SEQ;
                    } else {
                        LOG_WARNING << "Data length too big";
                        unpack_step_e_ = STEP_HEADER_SOF;
                        index_ = 0;
                    }
                }
                    break;
                case STEP_FRAME_SEQ: {
                    protocol_packet_[index_++] = byte_;
                    unpack_step_e_ = STEP_HEADER_CRC8;
                }
                    break;
                case STEP_HEADER_CRC8: {
                    protocol_packet_[index_++] = byte_;
                    bool crc8_result = VerifyCrcOctCheckSum(protocol_packet_, HEADER_LEN);
                    if (!crc8_result) {
                        LOG_WARNING << "CRC 8 error";
                    }
                    if ((index_ == HEADER_LEN) && crc8_result) {
                        if (index_ < HEADER_LEN) {
                            LOG_WARNING << "CRC 8 index less.";
                        }
                        unpack_step_e_ = STEP_DATA_CRC16;
                    } else {
                        unpack_step_e_ = STEP_HEADER_SOF;
                        index_ = 0;
                    }
                }
                    break;
                case STEP_DATA_CRC16: {
                    if (index_ < (HEADER_LEN + CMD_LEN + data_length_ + CRC_LEN)) {
                        protocol_packet_[index_++] = byte_;
                    } else if (index_ > (HEADER_LEN + CMD_LEN + data_length_ + CRC_LEN)) {
                        LOG_WARNING << "Index Beyond";
                    }
                    if (index_ == (HEADER_LEN + CMD_LEN + data_length_ + CRC_LEN)) {
                        unpack_step_e_ = STEP_HEADER_SOF;
                        index_ = 0;
                        if (VerifyCrcHexCheckSum(protocol_packet_, HEADER_LEN + CMD_LEN + data_length_ + CRC_LEN)) {
                            DataHandle();
                        } else {
                            LOG_WARNING << "CRC16 error";
                        }
                    }
                }
                    break;
                default: {
                    LOG_WARNING << "Unpack not well";
                    unpack_step_e_ = STEP_HEADER_SOF;
                    index_ = 0;
                }
                    break;
            }
        }
    }
    
    free(rx_buf_);
}


void DataHandle() {
    auto *p_header = (FrameHeader *) protocol_packet_;
    uint16_t data_length = p_header->data_length;
    uint16_t cmd_id = *(uint16_t *) (protocol_packet_ + HEADER_LEN);
    uint8_t *data_addr = protocol_packet_ + HEADER_LEN + CMD_LEN;
    switch (cmd_id) {
        case GAME_INFO_ID: memcpy(&game_information_, data_addr, data_length);
            
            break;
        case REAL_BLOOD_DATA_ID: memcpy(&robot_hurt_data_, data_addr, data_length);
            
            break;
        case REAL_SHOOT_DATA_ID: memcpy(&real_shoot_data_, data_addr, data_length);
            break;
        case REAL_RFID_DATA_ID: memcpy(&rfid_data_, data_addr, data_length);
            break;
        case GAME_RESULT_ID: memcpy(&game_result_data_, data_addr, data_length);
            break;
        case GAIN_BUFF_ID: memcpy(&get_buff_data_, data_addr, data_length);
            break;
        case SERVER_TO_USER_ID: memcpy(&student_download_data_, data_addr, data_length);
            break;
        case CHASSIS_DATA_ID: {
            
        }
            break;
        case GIMBAL_DATA_ID: memcpy(&gimbal_information_, data_addr, data_length);
            
            break;
        case SHOOT_TASK_DATA_ID: memcpy(&shoot_task_data_, data_addr, data_length);
            
            break;
        case INFANTRY_ERR_ID: memcpy(&global_error_data_, data_addr, data_length);
            
            break;
        case CONFIG_RESPONSE_ID: memcpy(&config_response_data_, data_addr, data_length);
            
            break;
        case CALI_RESPONSE_ID: memcpy(&cali_response_data_, data_addr, data_length);
            
            break;
        case REMOTE_CTRL_INFO_ID: memcpy(&rc_info_data_, data_addr, data_length);
            
            break;
        case BOTTOM_VERSION_ID: memcpy(&version_info_data_, data_addr, data_length);
            
            break;
        default:
            break;
    }
}
unsigned char _tmp[PACK_MAX_SIZE];
void RobotControl(const robot_protocol_msgs::RobotControl &msg){

    m.layout.dim[0].label = "control";
    m.layout.dim[0].size =10;
    m.data.resize(10);
    _tmp[0]=0xaa;_tmp[1]=0xaf;_tmp[2]=0x25;
    for (int i=0;i<10;i++)m.data[i]=_tmp[i];
    pub.publish(m);
}

int main(int argc, char **argv){
	ros::init(argc, argv, "rm_unpack_info");
    ros::NodeHandle n;
   
	ros::Subscriber sub = n.subscribe("uartRE", 1000, &info);
    ros::Subscriber sub2 = n.subscribe("RobotControl", 1, &RobotControl);
    pub = n.advertise<std_msgs::UInt8MultiArray>("array_write", 1000);
	ros::spin();

	return 0;
}




uint8_t GetCrcOctCheckSum(uint8_t *message, uint32_t length, uint8_t crc) {
    uint8_t index;
    while (length--) {
        index = crc ^ (*message++);
        crc = kCrcOctTable[index];
    }
    return (crc);
}

bool VerifyCrcOctCheckSum(uint8_t *message, uint16_t length) {
    uint8_t expected = 0;
    if ((message == 0) || (length <= 2)) {
        LOG_WARNING << "Verify CRC8 false";
        return false;
    }
    expected = GetCrcOctCheckSum(message, length - 1, kCrc8);
    return (expected == message[length - 1]);
}

void AppendCrcOctCheckSum(uint8_t *message, uint16_t length) {
    uint8_t crc = 0;
    if ((message == 0) || (length <= 2)) {
        LOG_WARNING << "Append CRC8 NULL";
        return;
    };
    crc = GetCrcOctCheckSum(message, length - 1, kCrc8);
    message[length - 1] = crc;
}

uint16_t GetCrcHexCheckSum(uint8_t *message, uint32_t length, uint16_t crc) {
    uint8_t data;
    if (message == NULL) {
        return 0xFFFF;
    }
    while (length--) {
        data = *message++;
        (crc) = ((uint16_t) (crc) >> 8) ^ kCrcTable[((uint16_t) (crc) ^ (uint16_t) (data)) & 0x00ff];
    }
    return crc;
}

bool VerifyCrcHexCheckSum(uint8_t *message, uint32_t length) {
    uint16_t expected = 0;
    if ((message == NULL) || (length <= 2)) {
        LOG_WARNING << "Verify CRC16 bad";
        return false;
    }
    expected = GetCrcHexCheckSum(message, length - 2, kCrc);
    return ((expected & 0xff) == message[length - 2] && ((expected >> 8) & 0xff) == message[length - 1]);
}

void AppendCrcHexCheckSum(uint8_t *message, uint32_t length) {
    uint16_t crc = 0;
    if ((message == NULL) || (length <= 2)) {
        LOG_WARNING << "Append CRC 16 NULL";
        return;
    }
    crc = GetCrcHexCheckSum(message, length - 2, kCrc);
    message[length - 2] = (uint8_t) (crc & 0x00ff);
    message[length - 1] = (uint8_t) ((crc >> 8) & 0x00ff);
}
