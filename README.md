# Sorting robot & RC car


### Project description
  Our project includes a sorting robot for the 'luggage' and an RC car with automated movement that takes the disks from the conveyor belt to the assigned gates.
  The robot uses Raspberry Pi and the RC car uses an Arduino Uno and we have chosen C++ as the programming language.
  The disks first get sorted by height and then by width via a slope. In case a bigger object gets stuck in the width filter, a laser sensor will identify this and an error will be displayed, leading to the manual removal of the disk. The disks that pass the width filter will fall into a tube, under which there is a light sensor. A piston will push the disks out of the tube at timed intervals onto a second conveyor belt. A display will show the color of the disk and the gate it is supposed to get to.
  The RC car will be waiting at the end of the second conveyor belt to pick up the disks and get them to the gates. The car will have automated movement. In order to identify if the car is next to the conveyor belt and ready to take the next disk to the gate, we will use laser sensors.

### Files description




