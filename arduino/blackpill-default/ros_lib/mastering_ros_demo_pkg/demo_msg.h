#ifndef _ROS_mastering_ros_demo_pkg_demo_msg_h
#define _ROS_mastering_ros_demo_pkg_demo_msg_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace mastering_ros_demo_pkg
{

  class demo_msg : public ros::Msg
  {
    public:
      typedef const char* _greeting_type;
      _greeting_type greeting;
      typedef int32_t _number_type;
      _number_type number;

    demo_msg():
      greeting(""),
      number(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      uint32_t length_greeting = strlen(this->greeting);
      varToArr(outbuffer + offset, length_greeting);
      offset += 4;
      memcpy(outbuffer + offset, this->greeting, length_greeting);
      offset += length_greeting;
      union {
        int32_t real;
        uint32_t base;
      } u_number;
      u_number.real = this->number;
      *(outbuffer + offset + 0) = (u_number.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_number.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_number.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_number.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->number);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      uint32_t length_greeting;
      arrToVar(length_greeting, (inbuffer + offset));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_greeting; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_greeting-1]=0;
      this->greeting = (char *)(inbuffer + offset-1);
      offset += length_greeting;
      union {
        int32_t real;
        uint32_t base;
      } u_number;
      u_number.base = 0;
      u_number.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_number.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_number.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_number.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->number = u_number.real;
      offset += sizeof(this->number);
     return offset;
    }

    virtual const char * getType() override { return "mastering_ros_demo_pkg/demo_msg"; };
    virtual const char * getMD5() override { return "4d44a7bb82ba135859a5038705fd5c1d"; };

  };

}
#endif
