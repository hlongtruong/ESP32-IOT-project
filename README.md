# ESP32-IOT-project
This is the IOT project based on ESP32 microcontroller.
- project html file is just for viewing the page only.

## Project photo and Web server Screenshot
<p align="center">
<img src="https://github.com/blaticslm/ESP32-IOT-project/blob/main/screenshot/2022_08_25_09_42_IMG_6652.JPG"  width="800">
<img src="https://github.com/blaticslm/ESP32-IOT-project/blob/main/screenshot/webpage_screenshot.png"  width="500">
</p>

## Feature Demo and presentation
You should go watch ***at least one of the videos below*** before watching the presentation

- Part1: https://www.youtube.com/watch?v=ZbPQufXMKdA
- Part2: https://www.youtube.com/watch?v=1BdyQ2VX56s
- Presentation: https://www.youtube.com/watch?v=VZbbzU-rg3w
- Presentation slide: 

## Backend and Frontend structure
<p align="center">
<img src="https://github.com/blaticslm/ESP32-IOT-project/blob/main/screenshot/esp32_iot_project.png"  width="500">
</p>

## ESP32 to Arduino program

ESP32's pwm somehow was not recognized by the server motors(may be the frequency was too high). ***Arduino was used to take ESP32 pwm as analog pins input***. Smoothing the PWM signal into analog signal needed the capacitor. Capacitor should be as big as possible (this project used 100uF capacitors). Here is some advantage for using larger capacitor:
- Less voltage rippling 
- Faster charging time
- Thus, the smoothed PWM signal can be viewed by Arduino as analog signal input like constant 5V

Here is the diagram before🟦 and after🟥 smoothing the pwm
<p align="center">
<img src="https://github.com/blaticslm/ESP32-IOT-project/blob/main/screenshot/converting.png">
</p>

## Future improvement
- Android app for this application
- Modify the naming of the program to make it more readable

