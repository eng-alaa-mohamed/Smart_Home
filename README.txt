Smart Home Project 

A smart home allows homeowners to control appliances, thermostats, lights, and other devices remotely using 
a Smartphone or tablet through Bluetooth.

First phase: control the appliances through terminal emulator program (uCon) by running commands.
The appliances include: control light, fan, door control and smart light based on ambient lumens and smart fan based on ambient temperature 

System Design:

The system is controlled over serial port (UART) with 9600 baud rate, 8-bit data frame, no parity and one stop bit. It is password protected allows up to 3 login trials. Once logged in, a list of commands are displayed. Basically each function has 2 commands, one to turn-on and the other to turn off. Functions are listed below:

- Smart light: The output light brightness is controlled by ambient light intensity 
Implementation: Pin PA2 gets analogue input from LDR (Light Dependent Resistor); 10bit ADC resolution is used. Timer_1 PWM (B) is utilized which gets the converted digital value as input and output to pin PD3. The output “effective power” is inversely proportional to light intensity

- Smart fan: The fan speed is controlled by the ambient temperature 
Implementation: Pin PA3 gets analogue input from thermal sensor; 10bit ADC resolution is used 
Timer_0 PWM is utilized which gets the converted digital value as input and output to pin PB3 
The output “effective power” is direct proportional to ambient temperature

- Open/close door: uses servo motor (open door: 90 degree) (close door: zero degree) utilizing Timer_1 PWM (A)

- Light on/off: uses pin PA0 as an output and apply High/Low according to received UART command 

- Fan on/off: uses pin PA1 as an output and apply High/Low according to received UART command 

As an initial phase, commands are strings sent over UART then later this will be controlled over a Bluetooth module and commands will be numeric values 

