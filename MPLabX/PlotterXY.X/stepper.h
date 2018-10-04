
#include <xc.h>
#include <libpic30.h>

typedef struct Stepper{
    volatile uint16_t *StepPort;
    uint8_t StepPin;
    volatile uint16_t *DirPort;
    uint8_t DirPin;
    uint8_t PositiveDir;

    float LastStepTime;
    
    float Speed;
    float MaxSpeed;
    float Accel;
    float StepWait;
    float StepMin;
    float FirstStep;
    int32_t StepNumber;
    
    int32_t CurrentPos; // 200*8 passos/rot. -> 65.535 passos -> 327.68 rot. -> 1.498 m
    int32_t SetPos;
}Stepper; 
typedef Stepper *Stepp_handle;

void Stepper_Calc_New_Speed(Stepper *Motor, uint8_t overload, int32_t distance){
    int32_t StepsToGo;
    uint32_t StepsToStop = ((Motor->Speed * Motor->Speed) / ( 2 * Motor->Accel));
    float AuxWait = Motor->StepWait;
    
    if(overload){
        StepsToGo = distance;
    }else{
        StepsToGo = Motor->SetPos - Motor->CurrentPos;
    }
    
//    if(StepsToGo > 0){
//        Motor->Speed = Motor->MaxSpeed;
//        Motor->StepWait = 1000000 / (float)Motor->MaxSpeed;
//        
//    }else if(StepsToGo < 0){
//        Motor->Speed = -Motor->MaxSpeed;
//        Motor->StepWait = 1000000 / (float)Motor->MaxSpeed;
//        
//    }else{
//        Motor->Speed = 0;
//        Motor->StepWait = 0;
//    }
    /* ==================================
     * ==================================
     * ==================================
     * ==================================
     */
    if((StepsToGo == 0) && (StepsToStop <= 1)){ // Estamos parados

        Motor->Speed = 0;
        Motor->StepWait = 0;
        Motor->StepNumber = 0;
        return; 
    }

    if(StepsToGo > 0){ // Temos de movimentar no sentido positivo

        if(Motor->StepNumber > 0){ // Estamos a acelerar

            if(StepsToStop >= StepsToGo){ // Temos de parar
                Motor->StepNumber = -StepsToStop;
            }
        }else if(Motor->StepNumber < 0){ // Estamos a travar

            if(StepsToStop < StepsToGo){ // Temos de acelerar de novo
                Motor->StepNumber = -Motor->StepNumber;
            }
        }

    }else if(StepsToGo < 0){ // Temos de movimentar no sentido negativo

        if(Motor->StepNumber > 0){ // Estamos a acelerar

            if(StepsToStop >= -StepsToGo){ // Temos de parar
                Motor->StepNumber = -StepsToStop;
            }
        }else if(Motor->StepNumber < 0){ // Estamos a travar

            if(StepsToStop < -StepsToGo){ // Temos de acelerar de novo
                Motor->StepNumber = -Motor->StepNumber;
            }
        }
    }

    if(Motor->StepNumber == 0){

        AuxWait = Motor->FirstStep;
    }else{

        AuxWait = AuxWait - ((2.0 * AuxWait) / (4.0 * (Motor->StepNumber + 1)));

        if(AuxWait < Motor->StepMin){
            AuxWait = Motor->StepMin;
        }
    }

    Motor->StepNumber++;
    Motor->StepWait = AuxWait;
    Motor->Speed = 100000/AuxWait;

    if(StepsToGo < 0){
        Motor->Speed = -Motor->Speed;
    }
}

uint8_t Stepper_Step(Stepper *Motor, uint8_t min, uint8_t max, uint32_t time){

    if(Motor->StepWait > 0){
        if((Motor->LastStepTime + Motor->StepWait) <= time){
            Motor->LastStepTime = time;
            
            if((Motor->Speed > 0) && (max == 0)){

                if(Motor->PositiveDir){
                    *Motor->DirPort |= 0b1 << Motor->DirPin;
                }else{
                    *Motor->DirPort &= ((0b1 << Motor->DirPin) ^ 0xffff);
                }

                Motor->CurrentPos += 1;

                *Motor->StepPort |= 0b1 << Motor->StepPin;
                __delay_us(2);
                *Motor->StepPort &= ((0b1 << Motor->StepPin) ^ 0xffff);

            }else if((Motor->Speed < 0) && (min == 0)){

                if(Motor->PositiveDir){
                    *Motor->DirPort &= ((0b1 << Motor->DirPin) ^ 0xffff);
                }else{
                    *Motor->DirPort |= 0b1 << Motor->DirPin;
                }
                Motor->CurrentPos -= 1;

                *Motor->StepPort |= 0b1 << Motor->StepPin;
                __delay_us(2);
                *Motor->StepPort &= ((0b1 << Motor->StepPin) ^ 0xffff);

            }
            return 1;
        }
    }
    return 0;
}

uint8_t Stepper_Move_To(Stepper *Motor, int32_t Position){
    
    if(Motor->SetPos != Position){
        Motor->SetPos = Position;
        return 1;
    }
    return 0;
}

uint8_t Stepper_Move(Stepper *Motor, int32_t Steps){

    if(Steps != 0){
        return Stepper_Move_To(Motor, Motor->CurrentPos + Steps);
    }
    return 0;
}

void Stepper_Move_At(Stepper *Motor, float Speed){

    if(Motor->MaxSpeed != Speed){
        Motor->MaxSpeed = Speed;
        Motor->StepMin = 1000000 / Speed;
    }
}