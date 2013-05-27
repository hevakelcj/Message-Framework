
#ifndef __PACKET_H__
#define __PACKET_H__

#ifdef   __cplusplus
extern "C" {
#endif

#define PACKET_MAX_LEN  512

extern void Packet_Send(const void *pdata, int len);
extern bool Packet_Recv(const void *pdata, int len);
extern void Packet_TimerHandle(void); 

#ifdef   __cplusplus
}
#endif

#endif  //__PACKET_H__
