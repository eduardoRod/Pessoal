%% Example for servo, dc and stepper motors

% This is a very simple example that shows how to use the functions for the
% servo, dc, and stepper motors. Remember that the functions for both dc and 
% stepper motors require: 1) the adafruit motor shield, 2) the adafruit motor 
% library, and 3) the file srv.pde (or motorsrv.pde) to be pre-loaded on the 
% arduino board. The servo functions can be used with any of the 3 pde
% source files shipped with the ArduinoIO package, however, if the adafruit
% motor shield is mounted on top or the arduino Uno board, then only the
% servo attached on pin #9 and #10 (respectively the inner and outer
% connectors on the upper left corner of the motor shield) are easily accessible.
% See the Readme.txt file for more details.

%% create arduino object and connect to board
if exist('a','var') && isa(a,'arduino') && isvalid(a),
    % nothing to do    
else
    a=arduino('DEMO');
end

%% Servos (this can be done with just a servo connected to the Arduino)

% prints the status of servo on pin #10
a.servoStatus(1); 
  
% attach servo on pin #10
a.servoAttach(01);

% prints the status of all (70) servos
a.servoStatus;

% move servo on pin #10 to 70 degrees position
a.servoWrite(10,70);

% move servo on pin #10 to 110 degrees position
a.servoWrite(10,110);

% move servo on pin #10 to 90 degrees position
a.servoWrite(10,90);

% reads angle from servo on pin #10
val=a.servoRead(10)

% attach servo on pin #10
a.servoDetach(10);

%% DC motors (this requires both adafruit motor shield and AFMotor library installed)

% gets speed of motor 4 (upper right port on the motor shield)
a.motorSpeed(4)      

% sets speed of motor 4 as 200/255
a.motorSpeed(4,200)      

% prints the speed of all motors
a.motorSpeed;    
        
% runs motor 1 forward
a.motorRun(4,'forward');    
 
% runs motor 3 backward
a.motorRun(4,'backward');     

% release motor 1
a.motorRun(4,'release');      

%% Stepper motors (this requires both adafruit motor shield and AFMotor library installed)

% sets speed of stepper 2 as 50 rpm
a.stepperSpeed(1,50)      

% prints the speed of stepper 2
a.stepperSpeed(1);         

% rotates stepper 1 forward of 100 steps in interleave mode 
a.stepperStep(1,'forward','double',100);

% rotates stepper 2 backward of 50 steps in single mode 
a.stepperStep(1,'backward','single',50); 

% releases stepper 2
a.stepperStep(1,'release'); 

% close session
delete(a)

%   Copyright 2011 The MathWorks, Inc.
