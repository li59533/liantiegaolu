using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

/*
 * 
typedef struct
{
    uint8_t head;
    uint8_t cmd;
    uint16_t len;
    uint8_t * payload;
    uint8_t sum;
    uint8_t foot;
}LN_Protocol_t;
 * 
 * 
 */

namespace Serialtool
{
    class protocol
    {
        byte[] protocol_buf = new byte[100];
        UInt32 protocol_buflen = 0;


        //public byte[] protocol_buf = new byte[100];


        public const byte Protocol_head = 0x7e;
        public const byte Protocol_foot = 0x7e;

        public enum LN_protocol_cmd_e
        {
            NORMAL_MODE = 0X00,

        };

        public void Protocol_Write_Cmd(byte cmd)
        {
            protocol_buf[0] = Protocol_head;
            protocol_buf[1] = cmd;
        }

        public void Protocol_Add_TLV(byte tag, byte len , byte[] value)
        {

        }

    }
}


