#ifndef __MODEL_API_H__
#define __MODEL_API_H__

//< Detail in "MshMDLv1.0.1"
#define SIG_MESH_VENDOR_CONTROL             1 // examples/vendor_control.c
#define SIG_MESH_VENDOR_CONTROL2            2 // examples/vendor_control2.c
// more...

//< Config whick example will use in <examples>
#define CONFIG_MESH_MODEL                   SIG_MESH_VENDOR_CONTROL

#define BYTE_LEN(x...)                      sizeof((u8 []) {x})

#define MAC_TO_LITTLE_ENDIAN(x) \
    (x & 0xff), \
    ((x >> 8) & 0xff), \
    ((x >> 16) & 0xff), \
    ((x >> 24) & 0xff), \
    ((x >> 32) & 0xff), \
    ((x >> 40) & 0xff)

#define MAC_TO_BIG_ENDIAN(x) \
    ((x >> 40) & 0xff), \
    ((x >> 32) & 0xff), \
    ((x >> 24) & 0xff), \
    ((x >> 16) & 0xff), \
    ((x >> 8) & 0xff), \
    (x & 0xff)


#endif /* __MODEL_API_H__ */
