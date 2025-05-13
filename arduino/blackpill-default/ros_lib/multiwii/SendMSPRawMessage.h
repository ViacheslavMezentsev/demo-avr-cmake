#ifndef _ROS_SERVICE_SendMSPRawMessage_h
#define _ROS_SERVICE_SendMSPRawMessage_h
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "multiwii/MSPRawMessage.h"

namespace multiwii
{

static const char SENDMSPRAWMESSAGE[] = "multiwii/SendMSPRawMessage";

  class SendMSPRawMessageRequest : public ros::Msg
  {
    public:
      typedef multiwii::MSPRawMessage _msg_type;
      _msg_type msg;

    SendMSPRawMessageRequest():
      msg()
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      offset += this->msg.serialize(outbuffer + offset);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      offset += this->msg.deserialize(inbuffer + offset);
     return offset;
    }

    virtual const char * getType() override { return SENDMSPRAWMESSAGE; };
    virtual const char * getMD5() override { return "0e01cb4af5047b763cb98e1ac7628835"; };

  };

  class SendMSPRawMessageResponse : public ros::Msg
  {
    public:

    SendMSPRawMessageResponse()
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
     return offset;
    }

    virtual const char * getType() override { return SENDMSPRAWMESSAGE; };
    virtual const char * getMD5() override { return "d41d8cd98f00b204e9800998ecf8427e"; };

  };

  class SendMSPRawMessage {
    public:
    typedef SendMSPRawMessageRequest Request;
    typedef SendMSPRawMessageResponse Response;
  };

}
#endif
