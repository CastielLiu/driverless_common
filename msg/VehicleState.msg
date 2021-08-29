
uint8   GEAR_DRIVE=11
uint8   GEAR_NEUTRAL=12
uint8   GEAR_REVERSE=13
uint8   GEAR_PAKING = 14

bool    ready
bool    driverless
uint8   gear
bool    hand_brake
bool    emergency_brake
float32 speed            #km/h
float32 roadwheel_angle  #deg ccw(-) cw(+)
bool    manualCtrlDetected

