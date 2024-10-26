import rclpy
from rclpy.node import Node
from std_msgs.msg import Float32
import serial
import time

class SonarPublisher(Node):
    def __init__(self):
        super().__init__('sonar_publisher')
        
        # ROS 2 publishers for angle and distance
        self.angle_publisher = self.create_publisher(Float32, 'sonar_angle', 10)
        self.distance_publisher = self.create_publisher(Float32, 'sonar_distance', 10)
        
        # Set up serial connection to Pico
        self.serial_port = serial.Serial('/dev/ttyACM0', 9600, timeout=1)
        
        # Timer to periodically read and publish data
        self.timer = self.create_timer(0.2, self.publish_data)

    def publish_data(self):
        if self.serial_port.in_waiting > 0:
            try:
                # Read and decode the data from serial
                line = self.serial_port.readline().decode('utf-8').strip()
                
                # Parse the angle and distance from the received line
                if line.startswith("Angle:"):
                    parts = line.split(", ")
                    angle = float(parts[0].split(": ")[1].replace("°", ""))
                    distance = float(parts[1].split(": ")[1].replace(" cm", ""))
                    
                    # Publish angle and distance
                    angle_msg = Float32()
                    distance_msg = Float32()
                    angle_msg.data = angle
                    distance_msg.data = distance
                    self.angle_publisher.publish(angle_msg)
                    self.distance_publisher.publish(distance_msg)
                    
                    # Log the published values
                    self.get_logger().info(f"Published Angle: {angle}°, Distance: {distance} cm")

            except (ValueError, IndexError):
                self.get_logger().warn("Received invalid data format")

def main(args=None):
    rclpy.init(args=args)
    sonar_publisher = SonarPublisher()
    rclpy.spin(sonar_publisher)
    sonar_publisher.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()
