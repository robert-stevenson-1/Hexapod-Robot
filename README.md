# 3rd Year University Project - Hexapod Robot
 This was my 3rd Year university project that was done across the year.

It involved building and programming a Hexapod Robot which ended up including for hand pose/gesture recognition and face detection.

It can be manually controlled through a Server-Client setup over LAN, with the robots POV being streamed to the client program.
The Client program process the the video frames through the ML models which was recieved from the Robot's communication server on the Raspberry Pi.

The communication server was deployed directly on the robot, and the main connection hub all the LAN connections for streaming IO data. It also handle commuinicating with the Arduino Mega.

The Arduino Mega was in charge of controlling all the hexapods servos, and was where the Inverse Kinematic calulations where performed for translating/moving the robots limbs.

