
#include "Packet.h"
#include "crc16.h"
#include "define.h"
#include <string.h>

#define PACKET_HEAD_1   0xAA
#define PACKET_HEAD_2   0x55
#define PACKET_TAIL_1   0xC3
#define PACKET_TAIL_2   0x99

static BYTE packet_send_buff[PACKET_MAX_LEN];

static BYTE packet_recv_buff[PACKET_MAX_LEN];
static int packet_recv_len = 0;
static int packet_recv_pos = 0;
static int packet_timeout = 0;

BYTE* const Packet_pSendData = packet_send_buff + 4;
const BYTE* const Packet_pRecvData = packet_recv_buff + 4;
INT16U Packet_nRecvLen = 0;

void Packet_Send(const void *pdata, int len)
{
    ASSERT((len + 8) <= PACKET_MAX_LEN);
    
    packet_send_buff[0] = PACKET_HEAD_1;
    packet_send_buff[1] = PACKET_HEAD_2;
    
    
}
