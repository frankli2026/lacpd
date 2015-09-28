/*
 * (c) Copyright 2015 Hewlett Packard Enterprise Development LP
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License. You may obtain
 * a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations
 * under the License.
 */

#ifndef __LACP_CMN_H__
#define __LACP_CMN_H__

#define FALSE         (0)
#define TRUE          (1)
#define R_SUCCESS     (0)

/* MAC Address length in bytes */
#define MAC_BYTEADDR_SIZE  (6)

/* MAC Address length in short int */
#define MAC_SHORTADDR_SIZE (3)

/* MAC address in 3 elements (ushort) */
typedef unsigned short        macaddr_3_t[MAC_SHORTADDR_SIZE];

/* MAC address in 6 elements (bytes)  */
typedef unsigned char         macaddr_6_t[MAC_BYTEADDR_SIZE];

typedef union macaddr_u {
    unsigned char  c_mac[MAC_BYTEADDR_SIZE];
    unsigned short s_mac[MAC_SHORTADDR_SIZE];
    struct {
        unsigned long  uu_mac;
        unsigned short ss_mac;
    } aa_mac;
#define u_mac aa_mac.uu_mac
} macaddr_t;

/******************************************************************************************/
/**                             MsgLib related                                           **/
/******************************************************************************************/

/* Msg Sender ID */
#define ml_timer_index   0x11
#define ml_lport_index   0x22
#define ml_rx_pdu_index  0x33
#define ml_cfgMgr_index  0x44

#define MSGLIB_INVALID_INDEX (-1)

/******************************************************************************************/
/**                             ML_event & related                                       **/
/******************************************************************************************/
#define ML_MAX_MSG_SIZE  4096

struct ML_event;         /* Forward declaration. */

typedef void (*ML_peer_callback_func_t)(struct ML_event *event, void *data);

struct ML_version {
    short major;
    short minor;
};

struct ML_protocol_version {
    struct ML_version version;
    struct ML_protocol *p;
    int num_elements; struct ML_element *elements;
    int num_types; struct ML_type *types;
    int num_messages; struct ML_message *messages;
    int num_enum_types; struct ML_enum_type *enum_types;
};

struct ML_protocol {
    char *name; char *md5;
    int num_versions; struct ML_version *versions;  // pointer to version table
    struct ML_protocol_version *protocol_versions;  // versions supported for this protocol.
};

struct ML_event_info {
    int dummy;    // zero-length structs make me nervous
};

/* For msglib internals only. Pay no attention to the code inside that curtain. */
struct ML_event_internal {
    int do_free;
    struct ML_protocol *protocol;
    struct ML_protocol_version *protocol_version;
    struct ML_peer *peer;
    ML_peer_callback_func_t callback; // per-message callback, not per-peer
    void *callback_data;
};

struct ML_peer_instance {
    int peer;
    int instance;
    int lifetime;
    struct ML_version version; // Version of message by the sender
};

enum {
    ML_event_flags_donot_free = (1 << 0),
};

/* The event structure handed to the application in ml_get_next_event. */
typedef struct ML_event {
    int flags;
    struct ML_event_info info;
    struct ML_event_internal internal;
    struct ML_peer_instance sender;
    int serial;
    int replyto;
    int msgnum;         // enum MLm_$protocol
    void *msg;          // struct MLt_$protocol__$type
} ML_event;


/******************************************************************************************/
/**                             Timer stuff...                                           **/
/******************************************************************************************/
struct MLt_msglib__timer {
    int timer_index;
    int data;
};

/******************************************************************************************/
/**                             Misc Utilities...                                        **/
/******************************************************************************************/
extern int speed_str_to_speed(char *cfg_speed);
extern enum PM_lport_type speed_to_lport_type(int speed);
extern int lport_type_to_speed(enum PM_lport_type ptype);

#endif  // __LACP_CMN_H__