#include "stdafx.h"
#include "base64.h"


static unsigned __int16
ReadUint16(const unsigned char *&buf)
{
  unsigned __int16 x = ((unsigned __int16) buf[0]) | ((unsigned __int16) buf[1] << 8);
  buf += sizeof(x);
  return x;
}

static unsigned __int32
ReadUint32(const unsigned char *&buf)
{
  unsigned __int32 x = ( (unsigned __int32) buf[0])        |
               (((unsigned __int32) buf[1]) << 8)  |
               (((unsigned __int32) buf[2]) << 16) |
               (((unsigned __int32) buf[3]) << 24);
  buf += sizeof(x);
  return x;
}

bool ParseType2Msg(char *EncodedMsg, Type2Msg *msg)
{
  // make sure inBuf is long enough to contain a meaningful type2 msg.
  //
  // 0  NTLMSSP Signature
  // 8  NTLM Message Type
  // 12 Target Name
  // 20 Flags
  // 24 Challenge
  // 32 end of header, start of optional data blocks
  //

  memset(msg,0,sizeof(*msg));
  unsigned char Buf[4*1024];
  unsigned __int32 inLen = decode_base64(Buf,EncodedMsg);	
  if (inLen==0) return false;

  if (inLen < NTLM_TYPE2_HEADER_LEN)
    return false;

  const unsigned char *cursor = (const unsigned char *) &Buf;

  // verify NTLMSSP signature
  if (memcmp(cursor, NTLM_SIGNATURE, sizeof(NTLM_SIGNATURE)) != 0)
    return false;
  cursor += sizeof(NTLM_SIGNATURE);

  // verify Type-2 marker
  if (memcmp(cursor, NTLM_TYPE2_MARKER, sizeof(NTLM_TYPE2_MARKER)) != 0)
    return false;
  cursor += sizeof(NTLM_TYPE2_MARKER);

  // skip target name security buffer
  cursor +=8;

  // read flags
  msg->flags = ReadUint32(cursor);

  // read challenge
  memcpy(msg->challenge, cursor, sizeof(msg->challenge));
  cursor += sizeof(msg->challenge);


  // we currently do not implement LMv2/NTLMv2 or NTLM2 responses,
  // so we can ignore target information.  we may want to enable
  // support for these alternate mechanisms in the future.
  return true;
}


bool ParseType3Msg(char *EncodedMsg, Type3Msg *msg)
{
  //0 NTLMSSP Signature Null-terminated ASCII "NTLMSSP" (0x4e544c4d53535000) 
  //8 NTLM Message Type long (0x03000000) 
  //12 LM/LMv2 Response security buffer 
  //20 NTLM/NTLMv2 Response security buffer 
  //28 Target Name security buffer 
  //36 User Name security buffer 
  //44 Workstation Name security buffer 
  //

  memset(msg,0,sizeof(*msg));
  unsigned char Buf[4*1024];
  unsigned __int32 inLen = decode_base64(Buf,EncodedMsg);	
  if (inLen==0) return false;

  if (inLen < NTLM_TYPE3_HEADER_LEN)
    return false;

  const unsigned char *cursor = (const unsigned char*) &Buf;

  // verify NTLMSSP signature
  if (memcmp(cursor, NTLM_SIGNATURE, sizeof(NTLM_SIGNATURE)) != 0)
    return false;
  cursor += sizeof(NTLM_SIGNATURE);

  // verify Type-3 marker
  if (memcmp(cursor, NTLM_TYPE3_MARKER, sizeof(NTLM_TYPE3_MARKER)) != 0)
    return false;
  cursor += sizeof(NTLM_TYPE3_MARKER);


  unsigned __int32 offset;
  int strlen;

  // Read LM Response
  if (ReadUint16(cursor)!=sizeof(msg->lmresponse)) return false;
  ReadUint16(cursor); // discard next 16-bit value
  offset = ReadUint32(cursor); // get offset from inBuf
  memcpy(msg->lmresponse,(const char*) &Buf+offset,sizeof(msg->lmresponse));

  // Read NTLM Response
  if (ReadUint16(cursor)!=sizeof(msg->ntlmresponse)) return false;
  ReadUint16(cursor); // discard next 16-bit value
  offset = ReadUint32(cursor); // get offset from inBuf
  memcpy(msg->ntlmresponse,(const char*) &Buf+offset,sizeof(msg->ntlmresponse));

  // Skip TargetName buffer
  cursor+=8;

  // Read UserName
  strlen = ReadUint16(cursor);
  if ((strlen<=0)||(strlen>sizeof(msg->username))) return false;
  ReadUint16(cursor); // discard next 16-bit value
  offset = ReadUint32(cursor); // get offset from inBuf
  memcpy(msg->username,(const char*) &Buf+offset,strlen);
  msg->usernamelen = strlen;

  // Read WorkStation
  strlen = ReadUint16(cursor);
  if ((strlen<=0)||(strlen>sizeof(msg->workstation))) return false;
  ReadUint16(cursor); // discard next 16-bit value
  offset = ReadUint32(cursor); // get offset from inBuf
  memcpy(msg->workstation,(const char*) &Buf+offset,strlen);
  msg->workstationlen = strlen;

  return true;
}
