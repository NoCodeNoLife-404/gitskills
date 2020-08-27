
## Start
> :book: 目录文件结构 :book:
```java
▾ mesh/
    ▾ api/
        feature_correct.h
        mesh_config_common.c
        model_api.c
        model_api.h
        unix_timestamp.c
        unix_timestamp.h
    ▾ board/
      ▾ bd29/
          board_ac630x_demo.c
          board_ac630x_demo_cfg.h
          board_ac6311_demo.c
          board_ac6311_demo_cfg.h
          board_ac6313_demo.c
          board_ac6313_demo_cfg.h
          board_ac6318_demo.c
          board_ac6318_demo_cfg.h
          board_ac6319_demo.c
          board_ac6319_demo_cfg.h
          board_config.h
    ▾ examples/
        vendor_control.c
        vendor_control2.c
```

>### 应用实例选择
---
- 在 [**api/model_api.h**](./api/model_api.h) 下，通过配置`CONFIG_MESH_MODEL`选择相应例子，SDK提供了2个应用实例。
```C
#define SIG_MESH_VENDOR_CONTROL              1 // examples/vendor_client.c
#define SIG_MESH_VENDOR_CONTROL2             2 // examples/vendor_server.c

#define CONFIG_MESH_MODEL                   SIG_MESH_VENDOR_CONTROL 
```
>### Board 配置
---
在 [**board/xxxx/board_config.h**](./board/bd29/board_config.h) 下，可以根据不同封装选择不同 **board**，以 **AC63X** 为例，默认选择`CONFIG_BOARD_AC630X_DEMO`作为目标板。
```C
/*
*  板级配置选择
*/
#define CONFIG_BOARD_AC630X_DEMO
// #define CONFIG_BOARD_AC6311_DEMO
// #define CONFIG_BOARD_AC6313_DEMO
// #define CONFIG_BOARD_AC6318_DEMO
// #define CONFIG_BOARD_AC6319_DEMO

#include "board_ac630x_demo_cfg.h"
#include "board_ac6311_demo_cfg.h"
#include "board_ac6313_demo_cfg.h"
#include "board_ac6318_demo_cfg.h"
#include "board_ac6319_demo_cfg.h"
```

## 应用实例
  >### SIG_MESH_VENDOR_CONTROL
  ---
  - #### 简介
    该实例能自行配网
    ```C
    -> 设备名称：OnOff_ctr
    -> Node Features：NO
    -> Authentication方式：NO OOB
    -> Elements个数：3
    -> Model：Configuration Server + Generic On Off Client
    ```
  - #### 实际操作
    - 基本配置

      使用 **GPIO_PORTA_01** 作为串口 **debug** 脚，波特率为 **1000000**

      使用 **PA3** 作为 **AD** 按键

      设备名称为`OnOff_ctr`，**MAC** 地址为`11:22:33:44:55:66`

      设置 [**api/model_api.h**](./api/model_api.h)
      ```C
      #define CONFIG_MESH_MODEL                   SIG_MESH_VENDOR_CONTROL
      ```
      设置 [**board/xxxx/board_xxxx_demo_cfg.h**](./board/bd29/board_ac630x_demo_cfg.h)
      ```C
      #define TCFG_UART0_TX_PORT                  IO_PORTA_01
      #define TCFG_UART0_BAUDRATE                 1000000
      #define TCFG_ADKEY_ENABLE                   ENABLE_THIS_MOUDLE //是否使能AD按键
      #define TCFG_ADKEY_PORT                     IO_PORTA_03 //注意选择的IO口是否支持AD功能
      #define TCFG_ADKEY_AD_CHANNEL               AD_CH_PA3
      ```
      配置节点的地址
      ```
      #define NODE_ADDR 0x0001
      ```
    - 编译工程并下载到目标板，接好串口，接好 **AD** 按键，上电或者复位设备

    - 如果结合下一小节 **SIG_MESH_VENDOR_CONTROL2**，就能互相控制的LED的亮灭了

  - #### 代码解读
    - 设置小机的 `LED`灯端口

      使用 `PA1`，`PA2`，`PA4`分别作为3个led灯的控制引脚
      ```c
      #define LED0_GPIO_PIN       0
      #define LED1_GPIO_PIN       1
      #define LED2_GPIO_PIN       2

      const u8 led_use_port[3] = {
          IO_PORTA_01,
          IO_PORTA_02,
          IO_PORTA_04,
      };

      static struct onoff_state onoff_state[3] = {
      { .led_gpio_pin = LED0_GPIO_PIN },
      { .led_gpio_pin = LED1_GPIO_PIN },
      { .led_gpio_pin = LED2_GPIO_PIN },
      };
      ```
    - 为节点添加 **publish** 行为
      ```c
      bt_mesh_cfg_app_key_add(net_idx, node_addr, net_idx, app_idx, app_key, NULL);
      ```
    - 把 **APPKEY** 绑定到3个元素上
      ```c
      bt_mesh_cfg_mod_app_bind_vnd(net_idx, node_addr, elem_addr, app_idx,
                                  BT_MESH_VENDOR_MODEL_ID_CLI,
                                  BT_COMP_ID_LF,
                                  NULL);
      bt_mesh_cfg_mod_app_bind_vnd(net_idx, node_addr, elem_addr + 1, app_idx,
                                  BT_MESH_VENDOR_MODEL_ID_CLI,
                                  BT_COMP_ID_LF,
                                  NULL);

      bt_mesh_cfg_mod_app_bind_vnd(net_idx, node_addr, elem_addr + 2, app_idx,
                                  BT_MESH_VENDOR_MODEL_ID_CLI,
                                  BT_COMP_ID_LF,
                                  NULL);                             
      ```
    - 设置各个 **model** 的 **publish** 属性
      ```c
      struct bt_mesh_cfg_mod_pub pub;
      pub.addr = 0x0004;              //对面小机第一个元素地址
      pub.app_idx = app_idx;
      pub.cred_flag = 0;
      pub.ttl = 7;
      pub.period = 0;
      pub.transmit = 0;
      log_info("bt_mesh_cfg_mod_pub_set_vnd client");
      bt_mesh_cfg_mod_pub_set_vnd(net_idx, node_addr, elem_addr,
                                  BT_MESH_VENDOR_MODEL_ID_CLI,
                                  BT_COMP_ID_LF,
                                  &pub, NULL);

      pub.addr = 0x0005;             //对面小机第二个元素地址
      pub.app_idx = app_idx;
      pub.cred_flag = 0;
      pub.ttl = 7;
      pub.period = 0;
      pub.transmit = 0;
      log_info("bt_mesh_cfg_mod_pub_set_vnd client");
      bt_mesh_cfg_mod_pub_set_vnd(net_idx, node_addr, elem_addr + 1,
                                  BT_MESH_VENDOR_MODEL_ID_CLI,
                                  BT_COMP_ID_LF,
                                  &pub, NULL);

      pub.addr = 0x0006;            //对面小机第三个元素地址
      pub.app_idx = app_idx;
      pub.cred_flag = 0;
      pub.ttl = 7;
      pub.period = 0;
      pub.transmit = 0;
      log_info("bt_mesh_cfg_mod_pub_set_vnd client");
      bt_mesh_cfg_mod_pub_set_vnd(net_idx, node_addr, elem_addr + 2,
                                  BT_MESH_VENDOR_MODEL_ID_CLI,
                                  BT_COMP_ID_LF,
                                  &pub, NULL);                            
      ```
    - 配置各个元素的 **model id**
      ```c
      static struct bt_mesh_elem elements[3] = {
        BT_MESH_ELEM(0, root_models,        vendor_control_models),
        BT_MESH_ELEM(1, BT_MESH_MODEL_NONE, vendor_control_models_1),
        BT_MESH_ELEM(2, BT_MESH_MODEL_NONE, vendor_control_models_2),
      };
      ```
>### SIG_MESH_VENDOR_CONTROL2
---
- #### 简介
  该实例能自行配网
  ```C
  -> 设备名称：OnOff_ctr2
  -> Node Features：NO
  -> Authentication方式：NO OOB
  -> Elements个数：3
  -> Model：Configuration Server + Generic On Off Client
  ```
- #### 实际操作
  - 基本配置

    使用 **GPIO_PORTA_01** 作为串口 **debug** 脚，波特率为 **1000000**

    使用 **PA3** 作为 **AD** 按键

    设备名称为`OnOff_ctr2`，**MAC** 地址为`11:22:33:44:55:77`

    设置 [**api/model_api.h**](./api/model_api.h)
    ```C
    #define CONFIG_MESH_MODEL                   SIG_MESH_VENDOR_CONTROL2
    ```
    设置 [**board/xxxx/board_xxxx_demo_cfg.h**](./board/bd29/board_ac630x_demo_cfg.h)
    ```C
    #define TCFG_UART0_TX_PORT                  IO_PORTA_01
    #define TCFG_UART0_BAUDRATE                 1000000
    #define TCFG_ADKEY_ENABLE                   ENABLE_THIS_MOUDLE //是否使能AD按键
    #define TCFG_ADKEY_PORT                     IO_PORTA_03 //注意选择的IO口是否支持AD功能
    #define TCFG_ADKEY_AD_CHANNEL               AD_CH_PA3
    ```
    配置节点的地址
    ```c
    #define NODE_ADDR 0x0004
    ```
  - 编译工程并下载到目标板，接好串口，接好 **AD** 按键，上电或者复位设备

  - 结合上一节的共两台小机，此时就可以相互控制对方的led灯了

- #### 代码解读
  - 设置小机的 `LED`灯端口

    使用 `PA1`，`PA2`，`PA4`分别作为3个led灯的控制引脚
    ```c
    #define LED0_GPIO_PIN       0
    #define LED1_GPIO_PIN       1
    #define LED2_GPIO_PIN       2

    const u8 led_use_port[3] = {
        IO_PORTA_01,
        IO_PORTA_02,
        IO_PORTA_04,
    };

    static struct onoff_state onoff_state[3] = {
    { .led_gpio_pin = LED0_GPIO_PIN },
    { .led_gpio_pin = LED1_GPIO_PIN },
    { .led_gpio_pin = LED2_GPIO_PIN },
    };
    ```
  - 为节点添加 **publish** 行为
    ```c
    bt_mesh_cfg_app_key_add(net_idx, node_addr, net_idx, app_idx, app_key, NULL);
    ```
  - 把 **APPKEY** 绑定到3个元素上
    ```c
    bt_mesh_cfg_mod_app_bind_vnd(net_idx, node_addr, elem_addr, app_idx,
                                BT_MESH_VENDOR_MODEL_ID_CLI,
                                BT_COMP_ID_LF,
                                NULL);
    bt_mesh_cfg_mod_app_bind_vnd(net_idx, node_addr, elem_addr + 1, app_idx,
                                BT_MESH_VENDOR_MODEL_ID_CLI,
                                BT_COMP_ID_LF,
                                NULL);

    bt_mesh_cfg_mod_app_bind_vnd(net_idx, node_addr, elem_addr + 2, app_idx,
                                BT_MESH_VENDOR_MODEL_ID_CLI,
                                BT_COMP_ID_LF,
                                NULL);                             
    ```
  - 设置各个 **model** 的 **publish** 属性
    ```c
    struct bt_mesh_cfg_mod_pub pub;
    pub.addr = 0x0001;              //对面小机第一个元素地址
    pub.app_idx = app_idx;
    pub.cred_flag = 0;
    pub.ttl = 7;
    pub.period = 0;
    pub.transmit = 0;
    log_info("bt_mesh_cfg_mod_pub_set_vnd client");
    bt_mesh_cfg_mod_pub_set_vnd(net_idx, node_addr, elem_addr,
                                BT_MESH_VENDOR_MODEL_ID_CLI,
                                BT_COMP_ID_LF,
                                &pub, NULL);

    pub.addr = 0x0002;             //对面小机第二个元素地址
    pub.app_idx = app_idx;
    pub.cred_flag = 0;
    pub.ttl = 7;
    pub.period = 0;
    pub.transmit = 0;
    log_info("bt_mesh_cfg_mod_pub_set_vnd client");
    bt_mesh_cfg_mod_pub_set_vnd(net_idx, node_addr, elem_addr + 1,
                                BT_MESH_VENDOR_MODEL_ID_CLI,
                                BT_COMP_ID_LF,
                                &pub, NULL);

    pub.addr = 0x0003;            //对面小机第三个元素地址
    pub.app_idx = app_idx;
    pub.cred_flag = 0;
    pub.ttl = 7;
    pub.period = 0;
    pub.transmit = 0;
    log_info("bt_mesh_cfg_mod_pub_set_vnd client");
    bt_mesh_cfg_mod_pub_set_vnd(net_idx, node_addr, elem_addr + 2,
                                BT_MESH_VENDOR_MODEL_ID_CLI,
                                BT_COMP_ID_LF,
                                &pub, NULL);                            
    ```
  - 配置各个元素的 **model id**
    ```c
    static struct bt_mesh_elem elements[3] = {
      BT_MESH_ELEM(0, root_models,        vendor_control_models),
      BT_MESH_ELEM(1, BT_MESH_MODEL_NONE, vendor_control_models_1),
      BT_MESH_ELEM(2, BT_MESH_MODEL_NONE, vendor_control_models_2),
    };
    ```
