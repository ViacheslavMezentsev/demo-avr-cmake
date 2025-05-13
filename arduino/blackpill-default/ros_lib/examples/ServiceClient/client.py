<<<<<<< HEAD
#!/usr/bin/env python

"""
Sample code to use with ServiceClient.pde
"""

import roslib; roslib.load_manifest("rosserial_arduino")
import rospy

from rosserial_arduino.srv import *

def callback(req):
    print("The arduino is calling! Please send it a message:")
    t = TestResponse()
    t.output = raw_input()
    return t

rospy.init_node("service_client_test")
rospy.Service("test_srv", Test, callback)
rospy.spin()
=======
#!/usr/bin/env python

"""
Sample code to use with ServiceClient.pde
"""

import roslib; roslib.load_manifest("rosserial_arduino")
import rospy

from rosserial_arduino.srv import *

def callback(req):
    print("The arduino is calling! Please send it a message:")
    t = TestResponse()
    t.output = raw_input()
    return t

rospy.init_node("service_client_test")
rospy.Service("test_srv", Test, callback)
rospy.spin()
>>>>>>> 5881ee9d9a49cdc272890e0007b9baca97e186f3
