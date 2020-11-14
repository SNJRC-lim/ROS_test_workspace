import rospy
import rosserial
from std_msgs.msg import ByteMultiArray
from std_msgs.msg import Int16MultiArray


def RobotGoCallback(go):
    #serial.write(go.data[0])
    #serial.write(go.data[1])


def communication():
    rospy.Subscriber('robot_go_array', Int16MultiArray, RobotGoCallback)
    pub = rospy.Publisher('arduino_info_array', ByteMultiArray, queue_size=1)
    rospy.init_node('serial_communication', anonymous=True)

    rospy.spin()
    
if __name__ == '__main__':
    communication()    