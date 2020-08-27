#ifndef __KEY_EVENT_DEAL_H__
#define __KEY_EVENT_DEAL_H__

#include "typedef.h"
#include "event.h"

enum {
    KEY_POWER_ON,
    KEY_POWEROFF,
    KEY_POWEROFF_HOLD,
    KEY_MUSIC_PP,
    KEY_MUSIC_PREV,
    KEY_MUSIC_NEXT,
    KEY_MUSIC_FF,
    KEY_MUSIC_FR,
    KEY_MUSIC_CHANGE_REPEAT,
    KEY_MUSIC_CHANGE_DEV,
    KEY_MUSIC_CHANGE_DEV_REPEAT,
    KEY_MUSIC_SET_PITCH,
    KEY_MUSIC_SET_SPEED,
    KEY_MUSIC_PLAYE_BY_DEV_FILENUM,
    KEY_MUSIC_PLAYE_BY_DEV_SCLUST,
    KEY_MUSIC_PLAYE_NEXT_FOLDER,
    KEY_MUSIC_PLAYE_PREV_FOLDER,
    KEY_MUSIC_PLAYE_REC_FOLDER_SWITCH,
    KEY_VOL_UP,
    KEY_VOL_DOWN,
    KEY_CALL_LAST_NO,
    KEY_CALL_HANG_UP,
    KEY_CALL_ANSWER,
    KEY_OPEN_SIRI,
    KEY_HID_CONTROL,
    KEY_LOW_LANTECY,
    KEY_CHANGE_MODE,

    KEY_EQ_MODE,
    KEY_THIRD_CLICK,

    KEY_FM_SCAN_ALL,
    KEY_FM_SCAN_ALL_UP,
    KEY_FM_SCAN_ALL_DOWN,
    KEY_FM_PREV_STATION,
    KEY_FM_NEXT_STATION,
    KEY_FM_PREV_FREQ,
    KEY_FM_NEXT_FREQ,

    KEY_FM_EMITTER_MENU,
    KEY_FM_EMITTER_NEXT_FREQ,
    KEY_FM_EMITTER_PERV_FREQ,

    KEY_RTC_UP,
    KEY_RTC_DOWN,
    KEY_RTC_SW,
    KEY_RTC_SW_POS,

    KEY_SPDIF_SW_SOURCE,

    KEY_BT_EMITTER_SW,

    KEY_ENC_START,
    KEY_REVERB_OPEN,
    KEY_REVERB_DEEPVAL_UP,
    KEY_REVERB_DEEPVAL_DOWN,
#if (BT_FOR_APP_EN)
    KEY_TM_GMA_SEND,
    KEY_SEND_SPEECH_START,
    KEY_DUEROS_VER,
    KEY_DUEROS_SEND,
    KEY_TWS_DUEROS_RAND_SET,
    KEY_TWS_BLE_SLAVE_SPEECH_START,
    KEY_SPEECH_START_FROM_TWS,
    KEY_SPEECH_STOP_FROM_TWS,
    KEY_TWS_BLE_DUEROS_CONNECT,
    KEY_TWS_BLE_DUEROS_DISCONNECT,
#endif

    KEY_IR_NUM_0 = 0xF0,//中间不允许插入
    KEY_IR_NUM_1,
    KEY_IR_NUM_2,
    KEY_IR_NUM_3,
    KEY_IR_NUM_4,
    KEY_IR_NUM_5,
    KEY_IR_NUM_6,
    KEY_IR_NUM_7,
    KEY_IR_NUM_8,
    KEY_IR_NUM_9 = 0xF9,

    KEY_NULL = 0xFF,
};


enum {
    ONE_KEY_CTL_NEXT_PREV = 1,
    ONE_KEY_CTL_VOL_UP_DOWN,
};


#endif
