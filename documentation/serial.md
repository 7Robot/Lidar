# Serial communication with pic board

## install
```
sudo apt-get install ros-kinetic-serial
```

## doc officielle http://docs.ros.org/kinetic/api/serial/html/index.html

communication:
  baudrate: 57600
  port: /dev/ttyUSB0 (PC)
        /dev/AMA0 (RPI)
  ending character: '\n'

Problem with super user
  https://answers.ros.org/question/189457/rosrun-as-super-user/
  https://answers.ros.org/question/46790/failed-to-open-port-devttyusb0/
