# OutSide Home Automation

Since most of home automation ideas are with respect to appliances used inside the house, we decided to automate processes outside home environment that usually require physical actions and are mundane.

Link to for the dashboard: https://outsidehomeautomation.herokuapp.com/

<img src="demo/dashboard.jpeg" width="380">

# 1. Smart Car Parking System

This is a simple code for getting the data from Ultrasonic Sensor using ESP32 and Sending the data directly to server via HTTPS request.
Here we are using ESP32 to communicate to the Heroku Server for sending the distance from the Ultrasonic sensor and show the update on the live dashboard using live website. Once car comes to the defined proper place in the parking garage the door of the garage will close automatically.

## Sensors and Gadgets Used:

1. 2 Ultrasonic sensor
2. ESP32
3. Servo Motor

<img src="demo/car.jpeg" width="480">

# 2. Smart Solar Panel

We are using a ESP 32 to control to the control the angle of the panel. The server checks for the time and day and gets the amount of voltage that solar panel is gathering, then it sends the optimal position to face towards.

## Sensors and Gadgets Used:

1. ESP 32
2. Servo Motor
3. Solar Panel

<img src="demo/solar_frame.jpeg" width="480"> 
<br>
Architecture 
<img src="demo/solar_arch.jpeg" width="480">

# 3. Model

Description

## Sensors and Gadgets Used:

1. ABC

Image\

# 4. Model

Description

## Sensors and Gadgets Used:

1. ABC

Image\

# 5. Model

Description

## Sensors and Gadgets Used:

1. ABC

Image\
