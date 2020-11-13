import rospy
import serial
from std_msgs.msg import Bool
from std_msgs.msg import Int16MultiArray

serial = serial.Serial('/dev/ttyACM0', 115200, timeout=0.5)

def RobotGoCallback(go):
    serial.write(go.data[0])
    serial.write(go.data[1])


def communication():
    rospy.Subscriber('robot_go_array', Int16MultiArray, RobotGoCallback)
    rospy.init_node('serial_communication', anonymous=True)

    rospy.spin()
    
if __name__ == '__main__':
    communication()    