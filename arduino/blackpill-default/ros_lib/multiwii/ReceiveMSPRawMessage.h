#ifndef _ROS_SERVICE_ReceiveMSPRawMessage_h
#define _ROS_SERVICE_ReceiveMSPRawMessage_h
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "multiwii/MSPRawMessage.h"

namespace multiwii
{

static const char RECEIVEMSPRAWMESSAGE[] = "multiwii/ReceiveMSPRawMessage";

  class ReceiveMSPRawMessageRequest : public ros::Msg
  {
    public:
      typedef uint8_t _id_type;
      _id_type id;

    ReceiveMSPRawMessageRequest():
      id(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      *(outbuffer + offset + 0) = (this->id >> (8 * 0)) & 0xFF;
      offset += sizeof(this->id);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      this->id =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->id);
     return offset;
    }

    virtual const char * getType() override { return RECEIVEMSPRAWMESSAGE; };
    virtual const char * getMD5() override { return "541b98e964705918fa8eb206b65347b3"; };

  };

  class ReceiveMSPRawMessageResponse : public ros::Msg
  {
    public:
      typedef multiwii::MSPRawMessage _msg_type;
      _msg_type msg;

    ReceiveMSPRawMessageResponse():
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

    virtual const char * getType() override { return RECEIVEMSPRAWMESSAGE; };
    virtual const char * getMD5() override { return "0e01cb4af5047b763cb98e1ac7628835"; };

  };

  class ReceiveMSPRawMessage {
    public:
    typedef ReceiveMSPRawMessageRequest Request;
    typedef ReceiveMSPRawMessageResponse Response;
  };

}
#endif
