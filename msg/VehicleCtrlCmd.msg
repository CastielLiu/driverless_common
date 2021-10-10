
uint8 GEAR_INITIAL=0
uint8 GEAR_DRIVE=1
uint8 GEAR_REVERSE=9
uint8 GEAR_NEUTRAL=10
uint8 GEAR_PARKING=11


bool    driverless
uint8   gear
float32 speed       #km/h
float32 brake       #0-100
float32 accelerate  #m/s2
float32 roadwheel_angle  #deg ccw(-) cw(+)

bool    emergency_brake
bool    hand_brake
bool    turnlight_r
bool    turnlight_l
bool    low_beam
bool    high_beam
bool    brake_light
bool    horn
