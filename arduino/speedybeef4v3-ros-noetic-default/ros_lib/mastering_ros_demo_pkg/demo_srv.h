#ifndef _ROS_SERVICE_demo_srv_h
#define _ROS_SERVICE_demo_srv_h
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace mastering_ros_demo_pkg
{

static const char DEMO_SRV[] = "mastering_ros_demo_pkg/demo_srv";

  class demo_srvRequest : public ros::Msg
  {
    public:
      typedef const char* _in_type;
      _in_type in;

    demo_srvRequest():
      in("")
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      uint32_t length_in = strlen(this->in);
      varToArr(outbuffer + offset, length_in);
      offset += 4;
      memcpy(outbuffer + offset, this->in, length_in);
      offset += length_in;
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      uint32_t length_in;
      arrToVar(length_in, (inbuffer + offset));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_in; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_in-1]=0;
      this->in = (char *)(inbuffer + offset-1);
      offset += length_in;
     return offset;
    }

    virtual const char * getType() override { return DEMO_SRV; };
    virtual const char * getMD5() override { return "2718218ecd3037e7050a0e8416c50c33"; };

  };

  class demo_srvResponse : public ros::Msg
  {
    public:
      typedef const char* _out_type;
      _out_type out;

    demo_srvResponse():
      out("")
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      uint32_t length_out = strlen(this->out);
      varToArr(outbuffer + offset, length_out);
      offset += 4;
      memcpy(outbuffer + offset, this->out, length_out);
      offset += length_out;
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      uint32_t length_out;
      arrToVar(length_out, (inbuffer + offset));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_out; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_out-1]=0;
      this->out = (char *)(inbuffer + offset-1);
      offset += length_out;
     return offset;
    }

    virtual const char * getType() override { return DEMO_SRV; };
    virtual const char * getMD5() override { return "da0977a72deba269ef1a5e91941513be"; };

  };

  class demo_srv {
    public:
    typedef demo_srvRequest Request;
    typedef demo_srvResponse Response;
  };

}
#endif
