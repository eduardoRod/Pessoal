// Encoder

#include "xc.h"
#include <stdlib.h>

typedef struct Encoder Encoder;

struct Encoder{ 
    int32_t Theta;    // Encoder position
    int16_t dTheta;   // Displacement since last update
    uint16_t cnt;     // Counter last value
    
    //uint16_t *Counter;
}; 
typedef Encoder *Enc_handle;
