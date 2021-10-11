# vehicle state reported from wired-chassis 

uint8   GEAR_DRIVE=11
uint8   GEAR_NEUTRAL=12
uint8   GEAR_REVERSE=13
uint8   GEAR_PAKING = 14
uint8   GEAR_INVALID = 200

bool    ready       # weather the vehicle is ready
bool    base_ready  # weather base system is ready for driverless
                    # false: driverless key is off, or emergency brake key is on, or other error in system
bool    emergency_brake
bool    driverless
uint8   gear
bool    hand_brake
float32 speed            #km/h
bool    speed_validity
float32 roadwheel_angle  #deg ccw(-) cw(+)
bool    roadwheel_angle_validity
bool    manualctrl_detected

uint8  accel_pedel_aperture  # 0-100  invalid(255)
uint8  brake_pedel_aperture  # 0-100  invalid(255)

## light
bool turnlight_l
bool turnlight_r
bool brake_light
bool low_beam
bool high_beam

bool horn


