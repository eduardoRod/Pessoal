%% Basic example for analog and digital IO, servos and encoders

% This is a very simple example that shows how to use the basic functions 
% for analog and digital IO, Servo motors and Encoders.

%% create arduino object and connect to board
a=arduino('COM9');

%% basic analog and digital IO

% specify pin mode for pins 4, 13 and 5
a.pinMode(4,'input');
a.pinMode(13,'output');
a.pinMode(5,'output');

% read digital input from pin 4
dv=a.digitalRead(4);

% output the digital value (0 or 1) to pin 13
a.digitalWrite(13,dv);

% read analog input from analog pin 5 (physically != from digital pin 5)
av=a.analogRead(5);

% normalize av from 0:1023 to 0:254
av=(av/1023)*254;

% ouptput value on digital (pwm) pin 5 (again, different from digital pin 5)
a.analogWrite(5,round(av))

% change reference voltage for analog pins to external
a.analogReference('external');

% change it back to default
a.analogReference('default');

%% some serial port -related commands

% gets the name of the serial port to which the arduino is connected to
a.serial

% flushes the PC's serial input buffer (just in case)
a.flush;

% sends number 42 to the arduino and back (to see if it's still there)
a.roundTrip(42)

%% servo motors

% attach servo on pin #9
a.servoAttach(9); 

% return the status of all servos
a.servoStatus; 

% rotates servo on pin #9 to 100 degrees
a.servoWrite(9,100); 

% reads angle from servo on pin #9
val=a.servoRead(9);

% detach servo from pin #9
a.servoDetach(9); 

%% encoders

% attach encoder #0 on pins 3 (pin A) and 2 (pin B)
a.encoderAttach(0,3,2)

% read the encoder position
a.encoderRead(0)

% attach encoder #2 on pins 18 (pin A) and 21 (pin B)
a.encoderAttach(2,18,21)

% sets debouncing delay to 17 (~1.7ms) for encoder #2
a.encoderDebounce(2,17)

% read position or encoder #2
a.encoderRead(2)

% reset position of encoder #2
a.encoderReset(2)

% get status of all three encoders
a.encoderStatus;

% detach encoder #0
a.encoderDetach(0);

%% close session
delete(a)

% Copyright 2011 The MathWorks, Inc.