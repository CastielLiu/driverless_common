#ifndef DRIVERLESS_COMMON_STRUCTS_H_
#define DRIVERLESS_COMMON_STRUCTS_H_

#include <atomic>
#include <boost/thread/locks.hpp>    
#include <boost/thread/shared_mutex.hpp>
#include <boost/thread/recursive_mutex.hpp>    
#include <mutex>
#include <driverless_common/VehicleState.h>

typedef boost::shared_mutex SharedMutex;
typedef boost::unique_lock<SharedMutex> WriteLock;
typedef boost::shared_lock<SharedMutex> ReadLock;
//读写锁使用方法
//SharedMutex wr_mutex
//WriteLock wlck(wr_mutex)
//ReadLock  rlck(wr_mutex)

typedef boost::recursive_mutex RecursiveMutex;
typedef boost::recursive_mutex::scoped_lock RecursiveLock;
//递归锁,同一线程可多次获得锁, 不同线程互斥
//RecursiveMutex re_mutex
//RecursiveLock rlck(re_mutex)


/*@brief 车辆控制信息*/
typedef struct ControlCmd
{
	ControlCmd()
	{
		validity = false;
		speed_validity = false;
		speed = 0.0;
		roadWheelAngle = 0.0;
		hand_brake = false;
		turnLight = 0;
		brake = 0;
	}

	//当validity有效时,无需在对speed_validity进行判断
	//当validity无效时，需判断speed_validity是否有效!
	bool  validity;       //指令的全局有效性
	bool  speed_validity; //仅速度指令的有效性
	float speed;
	bool steer_validity;
	float roadWheelAngle;

	bool hand_brake;
	uint8_t gear;
	uint8_t brake;
	uint8_t turnLight; // 0 关灯,1左转,2右转
	uint8_t stopLight; // 0 关灯

	void display(const std::string& prefix)
	{
		std::cout << prefix << "\t["
			<< "valid:" << validity << "\t" 
			<< "speed:" << speed << "\t"
			<< "brake:" << int(brake) << "\t"
			<< "angle:" << roadWheelAngle 
			<< "]\r\n"; 
	}
} controlCmd_t;

/*@brief 停车点信息*/
class ParkingPoint
{
public:
	ParkingPoint()
	{
		index = 0;
		parkingDuration = 0;
		isParking = false;
	}
	ParkingPoint(size_t _index, float _duration)
	{
		index = _index;
		parkingDuration = _duration;
		isParking = false;
	}
	ParkingPoint(size_t _index, float _duration, double _time, bool _parking)
	{
	    index = _index;
	    parkingDuration = _duration;
	    parkingTime = _time;
	    isParking = _parking;
	}
	
	size_t index; //停车点在全局路径中的索引
	float  parkingDuration; //停车时长，单位s，0表示不停车,-1永久停车  //此定义不得轻易改动
	double parkingTime;     //停车时刻
	bool   isParking;       //正在停车
};

class ParkingPoints
{
public:
	std::vector<ParkingPoint> points;
	size_t next_index = 0;
	bool sorted = false;

	size_t size() const {return points.size();}
	void push_back(const ParkingPoint& point)
	{
		points.push_back(point);
	} 
	const ParkingPoint& operator[](size_t i)const  {return points[i];}
	ParkingPoint& operator[](size_t i)             {return points[i];}

	bool available() const { return next_index  < points.size();}

	void sort() //停车点由小到大排序
	{
		std::sort(points.begin(),points.end(),
			[](const ParkingPoint& point1,const ParkingPoint& point2)
			{return point1.index < point2.index;});
		sorted = true;
	}

	bool isSorted() const {return sorted;}

	void print(const std::string& prefix) const 
	{
		for(auto &point:points)
			printf("[%s] parking point index: %lu  duration: %.1f",prefix.c_str(), point.index,point.parkingDuration);
	}
	
	ParkingPoint& next()
	{
		if(!available())
			next_index = 0;

		return points[next_index];
	}

	void clear()
	{
		points.clear();
		next_index = 0;
		sorted = false;
	}

};

/*@brief 交通灯点信息*/
class TrafficLightPoint
{
public:
	TrafficLightPoint()
	{
		index = 0;
		parkingDuration = 0;
		isParking = false;
	}
	TrafficLightPoint(size_t _index, float _duration)
	{
		index = _index;
		parkingDuration = _duration;
		isParking = false;
	}
	TrafficLightPoint(size_t _index, float _duration, double _time, bool _parking)
	{
	    index = _index;
	    parkingDuration = _duration;
	    parkingTime = _time;
	    isParking = _parking;
	}
	
	size_t index; //交通灯点在全局路径中的索引
	float  parkingDuration; //停车时长，单位s，0表示不停车,-1永久停车  //此定义不得轻易改动
	double parkingTime;     //停车时刻
	bool   isParking;       //正在停车
};

class TrafficLightPoints
{
public:
	std::vector<TrafficLightPoint> points;
	size_t next_index = 0;
	bool sorted = false;

	size_t size() const {return points.size();}
	void push_back(const TrafficLightPoint& point)
	{
		points.push_back(point);
	} 
	const TrafficLightPoint& operator[](size_t i)const  {return points[i];}
	TrafficLightPoint& operator[](size_t i)             {return points[i];}

	bool available() const { return next_index  < points.size();}

	void sort() //按索引由小到大排序
	{
		std::sort(points.begin(),points.end(),
			[](const TrafficLightPoint& point1,const TrafficLightPoint& point2)
			{return point1.index < point2.index;});
		sorted = true;
	}

	bool isSorted() const {return sorted;}

	void print(const std::string& prefix) const 
	{
		for(auto &point:points)
			printf("[%s] traffic light point index: %lu  duration: %.1f",prefix.c_str(), point.index,point.parkingDuration);
	}
	
	TrafficLightPoint& next()
	{
		if(!available())
			next_index = 0;

		return points[next_index];
	}
public:

	void clear()
	{
		points.clear();
		next_index = 0;
		sorted = false;
	}

};

/*@brief 路径转向区间信息 */
class TurnRange
{
public:
	enum TurnType
	{
		TurnType_Left = -1,
		TurnType_None = 0,
		TurnType_Right = 1,
	};

	int type;
	size_t start_index;
	size_t end_index;

	TurnRange(int _type, size_t _start_index, size_t _end_index)
	{
		type = _type;
		start_index = _start_index;
		end_index = _end_index;
	}
	uint8_t getCurrentLight() const
	{
		if(type == TurnType_Left) // 0 关灯,1左转,2右转
			return 1;
		else if(type == TurnType_Right)
			return 2;
		else
			return 0;
	}
};

class TurnRanges
{
public:
	std::vector<TurnRange> ranges;

	size_t size() const {return ranges.size();}
	void clear()
	{
		ranges.clear();
	}
	
};

/*@brief 路径限速区间信息*/
class SpeedRange
{
public:
    float speed;
    size_t start_index;
    size_t end_index;
    
    SpeedRange(float _speed, size_t _start_index, size_t _end_index)
    {
        speed = _speed;
        start_index = _start_index;
        end_index = _end_index;
    }
};

class SpeedRanges
{
public:
    std::vector<SpeedRange> ranges;
    
    size_t size() const {return ranges.size();}
    void clear()
    {
        ranges.clear();
    }
};

/*@brief 位置信息*/
class Point
{
public:
	double x,y,z;
	Point(){}
	Point(double _x, double _y, double _z=0.0):
		x(_x), y(_y), z(_z){}
	
	float disTo(const Point& point) const 
	{
		float dx = point.x - x;
		float dy = point.y - y;
		return sqrt(dx*dx + dy*dy);
	}
};

/*@brief 位姿信息*/
class Pose : public Point
{
public:
	double yaw;
};

/*@brief 路径点信息*/
class GpsPoint : public Pose
{
public:
	double longitude;
	double latitude;
    float curvature;
    float left_width;
	float right_width;
};

class Path
{
public:
	std::vector<GpsPoint> points;
	float resolution;
	bool  has_curvature;               //是否包含路径曲率

	//std::atomic<size_t> pose_index;    //距离车辆最近路径点的索引
	size_t pose_index;                 //考虑到该信息不属于路径信息，后期将其删除
	
	size_t final_index;                //终点索引，路径被载入时的最后一个点的索引
									   //对于被延伸的路径路径最后一个点可能与路径终点不一致

	ParkingPoints park_points;         //停车点信息
	TurnRanges    turn_ranges;		   //转向区间信息
	SpeedRanges   speed_ranges;        //限速区间信息

public:
	size_t size() const {return points.size();}
	const GpsPoint& operator[](size_t i) const {return points[i];}
	GpsPoint& operator[](size_t i)             {return points[i];}
	
	Path(){} //当定义了拷贝构造函数时，编译器将不提供默认构造函数，需显式定义

	//路径是否可循环驾驶
	//通过路径起点和尾点的距离是否在一定范围内，判断路径是否可循环驾驶
	bool recyclable(float max_dis = 0.5) const
	{
		//终点索引大于点的个数，程序异常！
		//终点索引过小则路径较短，无法‘循环’驾驶
		if(final_index >= points.size() || final_index < 2)
			return false;

		//起点与终点距离过远，无法循环驾驶
		if(points[0].disTo(points[final_index]) > max_dis)
			return false;
		return true;
	}

	//获取路径上距离参考点(ref_p)最近的点
	GpsPoint nearest(const Point& ref_p) const
	{
		float min_dis = 99999;
		size_t nearest_index = 0;
		for(size_t i=0; i<points.size(); ++i)
		{
			float dis = ref_p.disTo(points[i]);
			//std::cout << i << "\t" << dis << std::endl;
			if(dis < min_dis)
			{
				min_dis = dis;
				nearest_index = i;
			}
		}
		//std::cout << "nearest_index:" << nearest_index <<"\tmin_dis:" << min_dis << std::endl;
		return points[nearest_index];
	}
	
	void clear()                       //清空路径信息
	{
		points.clear();
		resolution = 0.0;
		has_curvature = false;
		pose_index = 0;
		final_index = 0;
		park_points.clear();
		turn_ranges.clear();
		speed_ranges.clear();
	}

	bool finish() const {return pose_index>=final_index;}
	float remaindDis()
	{
		float dis = (final_index - pose_index) * resolution;
		if(dis < 0) dis = 0;
		return dis;
	}

	//延伸路径, 在路径的末尾增加一段距离的点
	bool extend(float extendDis)
	{
		//if(extended) return false;

		//取最后一个点与倒数第n个点的连线向后插值
		//总路径点不足n个,退出
		int n = 5;
		//std::cout << "extendPath: " << points.size() << "\t" << points.size()-1 << std::endl;
		if(points.size()-1 < n)
		{
			printf("path points is too few (%lu), extend path failed",points.size()-1);
			return false;
		}
		int endIndex = points.size()-1;
		
		float dx = (points[endIndex].x - points[endIndex-n].x)/n;
		float dy = (points[endIndex].y - points[endIndex-n].y)/n;
		float ds = sqrt(dx*dx+dy*dy);

		GpsPoint point;
		float remaind_dis = 0.0;
		for(size_t i=1;;++i)
		{
			point.x = points[endIndex].x + dx*i;
			point.y = points[endIndex].y + dy*i;
			point.curvature = 0.0;
			points.push_back(point);
			remaind_dis += ds;
			if(remaind_dis > extendDis)
				break;
		}
		return true;
	}
};

/*@brief 车辆参数 */
class VehicleParams
{
public:
	float max_roadwheel_angle;
	float min_roadwheel_angle;
	float min_radius;
	float max_speed;
	float wheel_base;
	float wheel_track;
	float width;
	float length;
	float steer_clearance; //转向间隙

	bool validity;
	VehicleParams()
	{
		validity = false;
	}
};

/*@brief 车辆状态 内部状态+外部状态
 * 更新车辆状态的线程利用类方法进行更新
 * 读取车辆状态的线程先创建副本，然后直接访问副本成员
*/
#define LOCK true
#define UNLOCK  false
//此类中的读写锁是否具有意义，如果在外部全局加锁，是否会更合理
class VehicleState 
{
public:
	bool base_ready = true; //线控系统就绪(硬件急停释放, 自动驾驶开关闭合等要素)
	bool driverless_mode;   //是否为自动驾驶模式
	uint8_t gear;           //档位
	float   speed = 0.0;        //车速 km/h
	float   steer_angle = 0.0;  //前轮转角
	Pose    pose;         //车辆位置

	bool speed_validity = false;
	bool steer_validity = false;
	bool pose_validity  = false;

	SharedMutex wr_mutex;//读写锁
	
public:
	void setSpeed(const float& val)
	{
		WriteLock writeLock(wr_mutex);
		speed = val;
	}

	void setSteerAngle(const float& val)
	{
		WriteLock writeLock(wr_mutex);
		steer_angle = val;
	}

	void setPose(const Pose& val)
	{
		WriteLock writeLock(wr_mutex);
		pose = val;
	}

	void setGear(uint8_t g)
	{
		WriteLock lc(wr_mutex);
		gear = g;
	}
	
	void setPoseValid(bool flag)
	{
		WriteLock writeLock(wr_mutex);
		pose_validity = flag;
	}

	uint8_t getGear()
	{
		ReadLock readLock(wr_mutex);
		return gear;
	}

	float getSpeed(bool lock = UNLOCK)
	{
		if(lock)
		{
			ReadLock readLock(wr_mutex);
			return speed;
		}
		return speed;
	}
	float getSteerAngle(bool lock = UNLOCK)
	{
		if(lock)
		{
			ReadLock readLock(wr_mutex);
			return steer_angle;
		}
		return steer_angle;
	}

	Pose getPose(bool lock = UNLOCK)
	{
		if(lock)
		{
			ReadLock readLock(wr_mutex);
			return pose;
		}
		return pose;
	}
	
	bool getPoseValid() const
	{
		return pose_validity;
	}

	// 车速是否足够小, 用于停车判断
	bool speedLowEnough()
	{
		ReadLock readLock(wr_mutex);
		return fabs(speed) < 0.1;
	}

	bool isDriveGear()
	{
		ReadLock readLock(wr_mutex);
		return gear == driverless_common::VehicleState::GEAR_DRIVE;
	}

	bool isReverseGear()
	{
		ReadLock readLock(wr_mutex);
		return gear == driverless_common::VehicleState::GEAR_REVERSE;
	}

	bool isNeutralGear()
	{
		ReadLock readLock(wr_mutex);
		return gear == driverless_common::VehicleState::GEAR_NEUTRAL;
	}

	VehicleState(){} //当定义了拷贝构造函数时，编译器将不提供默认构造函数，需显式定义

	// 重载拷贝构造和赋值构造函数, 以避免读写锁拷贝报错
	VehicleState(const VehicleState& obj)
	{
		WriteLock lck(wr_mutex);
		this->driverless_mode = obj.driverless_mode;
		this->base_ready  = obj.base_ready;
		this->gear        = obj.gear;
		this->speed       = obj.speed;
		this->steer_angle = obj.steer_angle;
		this->pose        = obj.pose;
		this->speed_validity    = obj.speed_validity;
		this->steer_validity    = obj.steer_validity;
		this->pose_validity     = obj.pose_validity;
	}
	const VehicleState& operator=(const VehicleState& obj)
	{
		WriteLock lck(wr_mutex);
		this->driverless_mode = obj.driverless_mode;
		this->base_ready  = obj.base_ready;
		this->gear        = obj.gear;
		this->speed       = obj.speed;
		this->steer_angle = obj.steer_angle;
		this->pose        = obj.pose;
		this->speed_validity    = obj.speed_validity;
		this->steer_validity    = obj.steer_validity;
		this->pose_validity     = obj.pose_validity;
		return *this;
	}

	bool validity(std::string& info)
	{
		bool ok = true;
		if(!speed_validity)
		{
            info += "[speed] ";
			ok = false;
		}
		if(!steer_validity)
		{
            info += "[steer] ";
			ok = false;
		}
		if(pose.x <100 || pose.y <100) //the pose from gps is invailed!
		{
            info += "[pose] ";
			ok = false;
			pose_validity = false;
		}
		else
			pose_validity = true;
			
		if(!base_ready)
		{
			info += "[base] ";
			ok = false;
		}

		if(!ok)
			info = "Invalid " + info;

		return ok;
	}


};


#endif
