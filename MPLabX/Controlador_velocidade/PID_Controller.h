// Controlador PID

#include "xc.h"
#include <stdlib.h>

typedef struct PIDController PIDController;

PIDController *initController (void);
void PID_setParams(PIDController*, int16_t, int16_t, int16_t, int16_t);
void PID_setRestrictions(PIDController*, int32_t, int32_t, int32_t, int32_t);
void PID_newSetPoint(PIDController*, int32_t);
void PID_newInput(PIDController*, int32_t);
void PID_updateController(PIDController*);

struct PIDController{
    // Control variables
    int16_t Kp;
    int16_t Ki;
    int16_t Kd;
    int16_t time_ms;
    int32_t MAX_INPUT;
    int32_t MIN_INPUT;
    int32_t MAX_OUTPUT;
    int32_t MIN_OUTPUT;
    
    // Process variables
    int32_t set_point;
    int32_t input;
    int32_t last_input;
    int32_t error;
    int32_t last_error;
    int32_t total_error;
    int32_t output;
    
    // Process functions
    void (*setParams)(PIDController*,int16_t,int16_t,int16_t,int16_t);
    void (*setRestrictions)(PIDController*,int32_t,int32_t,int32_t,int32_t);
    void (*newSetPoint)(PIDController*,int32_t);
    void (*newInput)(PIDController*,int32_t);
    void (*updateController)(PIDController*);
};
typedef PIDController *PIDCont_handle;

PIDController *initController (void){
    PIDController *Cnt = (PIDController *)malloc(sizeof(PIDController));
    
    Cnt->Kp = 1;
    Cnt->Ki = 0;
    Cnt->Kd = 0;
    Cnt->time_ms = 0;
    Cnt->MIN_INPUT = 0;
    Cnt->MAX_INPUT = 0;
    Cnt->MIN_OUTPUT = 0;
    Cnt->MAX_OUTPUT = 0;
    Cnt->last_error = 0;
    Cnt->total_error = 0;
    Cnt->last_input = 0;
    
    Cnt->setParams = PID_setParams;
    Cnt->setRestrictions = PID_setRestrictions;
    Cnt->newSetPoint = PID_newSetPoint;
    Cnt->newInput = PID_newInput;
    Cnt->updateController = PID_updateController;
    
    return Cnt;
}

void PID_setParams(PIDController *self, int16_t gainP, int16_t gainI, int16_t gainD, int16_t time){
    
    self->Kp = gainP;
    self->Ki = gainI;
    self->Kd = gainD;
    self->time_ms = time;
}

void PID_setRestrictions(PIDController *self, int32_t MinI, int32_t MaxI, int32_t MinO, int32_t MaxO){
    self->MIN_INPUT = MinI;
    self->MAX_INPUT = MaxI;
    self->MIN_OUTPUT = MinO;
    self->MAX_OUTPUT = MaxO;
}

void PID_newSetPoint( PIDController *self, int32_t val){
    if(self->set_point != val){
        if(val > self->MAX_INPUT)
            self->set_point = self->MAX_INPUT;
        else if(val < self->MIN_INPUT)
            self->set_point = self->MIN_INPUT;
        else
            self->set_point = val;
    }
}

void PID_newInput( PIDController *self, int32_t val){
    self->last_input = self->input;
    self->input = val;
    
    
    self->last_error = self->error;
    self->error = self->set_point - self->input;
    if(self->Ki > 0)
        self->total_error += self->error * (self->time_ms/1000);
    
}

void PID_updateController( PIDController *self){
    if(self->time_ms > 0){
        uint32_t P, I, D, PID;
        
        if(self->Kp > 0){
            P = self->Kp * self->error;
        }else{
            P = 0;
        }
        
        if(self->Ki > 0){
            I = self->Ki * self->total_error;
        }else{
            I = 0;
        }
        
        if(self->Kd > 0){
            D = self->Kd * ((self->last_error - self->error) / self->time_ms/1000);
        }else{
            D = 0;
        }
        
        // atribuição e restrição de output
        PID = P + I + D;
        if (PID > self->MAX_OUTPUT)
            self->output = self->MAX_OUTPUT;
        else if (PID < self->MIN_OUTPUT)
            self->output = self->MIN_OUTPUT;
        else
            self->output = PID;
    }
}
