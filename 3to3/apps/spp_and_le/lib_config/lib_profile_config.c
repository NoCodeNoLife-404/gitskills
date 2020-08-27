#include "system/includes.h"
#include "app_config.h"
#include "btcontroller_config.h"
#include "btstack/bt_profile_config.h"
#include "bt_common.h"
#include "btstack/avctp_user.h"

#define LOG_TAG     "[BT-CFG]"
#define LOG_ERROR_ENABLE
#define LOG_INFO_ENABLE
/* #define LOG_DUMP_ENABLE */
#include "debug.h"


typedef struct {
    // linked list - assert: first field
    void *offset_item;

    // data is contained in same memory
    u32        service_record_handle;
    u8         *service_record;
} service_record_item_t;

extern const u8 sdp_pnp_service_data[];
extern const u8 sdp_spp_service_data[];
extern service_record_item_t  sdp_record_item_begin[];
extern service_record_item_t  sdp_record_item_end[];

#define SDP_RECORD_HANDLER_REGISTER(handler) \
	const service_record_item_t  handler \
		sec(.sdp_record_item)

#if TCFG_USER_BLE_ENABLE

#if (TCFG_BLE_DEMO_SELECT == DEF_BLE_DEMO_ADV)
const int config_stack_modules = (BT_BTSTACK_LE_ADV) | BT_BTSTACK_CLASSIC;
#else

#if (TCFG_USER_EDR_ENABLE)
const int config_stack_modules = BT_BTSTACK_LE | BT_BTSTACK_CLASSIC;
#else
const int config_stack_modules = BT_BTSTACK_LE;
#endif
#endif

#else
#if (TCFG_USER_EDR_ENABLE)
const int config_stack_modules = BT_BTSTACK_CLASSIC;
#else
const int config_stack_modules = 0;
#endif

#endif


#if (USER_SUPPORT_PROFILE_SPP==1)
u8 spp_profile_support = 1;
SDP_RECORD_HANDLER_REGISTER(spp_sdp_record_item) = {
    .service_record = (u8 *)sdp_spp_service_data,
    .service_record_handle = 0x00010004,
};
#endif

const u8 hid_conn_depend_on_dev_company = 0;
const u8 sdp_get_remote_pnp_info = 0;


#if ((TCFG_USER_BLE_ENABLE) && (TCFG_BLE_DEMO_SELECT != DEF_BLE_DEMO_ADV))
u8 app_le_pool[900] sec(.btstack_pool)  ALIGNED(4);
#endif

#if(TCFG_USER_EDR_ENABLE)
#ifdef CONFIG_CPU_BD29
u8 app_bredr_pool[672] sec(.btstack_pool) ALIGNED(4);
u8 app_bredr_profile[692] sec(.btstack_pool) ALIGNED(4);
#else
u8 app_bredr_pool[1468] sec(.btstack_pool) ALIGNED(4);
u8 app_bredr_profile[1048] sec(.btstack_pool) ALIGNED(4);
#endif      //CONFIG_CPU_BD29
#else
u8 app_bredr_pool[0] sec(.btstack_pool) ALIGNED(4);
u8 app_bredr_profile[0] sec(.btstack_pool) ALIGNED(4);
#endif

#if(TCFG_USER_EDR_ENABLE ||TCFG_USER_BLE_ENABLE)
u8 app_l2cap_pool[70] sec(.btstack_pool) ALIGNED(4);
#else
u8 app_l2cap_pool[0] sec(.btstack_pool) ALIGNED(4);
#endif

u8 *get_bredr_pool_addr(void)
{
    u16 len = 0;

    if (STACK_MODULES_IS_SUPPORT(BT_BTSTACK_CLASSIC)) {
        len = get_bredr_pool_len();
        printf("bredr pool len %d\n", len);
        if (len > sizeof(app_bredr_pool)) {
            ASSERT(0, "bredr_pool is small\n");
        }
        return &app_bredr_pool;
    }
    return NULL;
}

u8 *get_le_pool_addr(void)
{
    u16 len = 0;

#if ((TCFG_USER_BLE_ENABLE) && (TCFG_BLE_DEMO_SELECT != DEF_BLE_DEMO_ADV))
    if (STACK_MODULES_IS_SUPPORT(BT_BTSTACK_LE)) {
        len = get_le_pool_len();
        printf("le pool len %d\n", len);
        if (len > sizeof(app_le_pool)) {
            ASSERT(0, "le_pool is small\n");
        }

        return &app_le_pool;
    }
#endif
    return NULL;
}

u8 *get_l2cap_stack_addr(void)
{
    u16 len = 0;

    if (STACK_MODULES_IS_SUPPORT(BT_BTSTACK_CLASSIC) || STACK_MODULES_IS_SUPPORT(BT_BTSTACK_LE)) {
        len = get_l2cap_stack_len();
        printf("l2cap stack len %d\n", len);
        if (len > sizeof(app_l2cap_pool)) {
            ASSERT(0, "l2cap pool is small\n");
        }
        return &app_l2cap_pool;
    } else {
        return NULL;
    }
}

u8 *get_profile_pool_addr(void)
{
    u16 len = 0;

    if (STACK_MODULES_IS_SUPPORT(BT_BTSTACK_CLASSIC)) {

        len = get_profile_pool_len();
        printf("bredr profile pool len %d\n", len);
        if (len > sizeof(app_bredr_profile)) {
            ASSERT(0, "bredr_profile is small\n");
        }
        return &app_bredr_profile;
    }
    return NULL;
}

const u8 a2dp_mutual_support = 0;
const u8 more_addr_reconnect_support = 0;
const u8 more_hfp_cmd_support = 0;
const u8 more_avctp_cmd_support = 0;
const u8 hci_inquiry_support = 0;
const u8 btstack_emitter_support  = 0;  /*定义用于优化代码编译*/


/*u8 l2cap_debug_enable = 0xf0;
u8 rfcomm_debug_enable = 0xf;
u8 profile_debug_enable = 0xff;
u8 ble_debug_enable    = 0xff;
u8 btstack_tws_debug_enable = 0xf;*/

