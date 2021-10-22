std_msgs/Header header


uint8   state            # system state id

string  location_source  # gps? slam? xxx?

float64 position_x
float64 position_y
float64 yaw
float64 longitude    # deg
float64 latitude     # deg

float32 lateral_error     # m left(-) right(+)
float32 yaw_error         # deg  ccw(-) cw(+)

float32 command_speed     # km/h
float32 vehicle_speed     # km/h
float32 roadwheel_angle   # deg  ccw(-) cw(+)

float32 nearest_object_distance


