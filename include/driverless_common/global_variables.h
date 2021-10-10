#ifndef DRIVERLESS_COMMON_GLOBAL_VARIABLES_H_
#define DRIVERLESS_COMMON_GLOBAL_VARIABLES_H_

// 此enum务必按照数字顺序填写
// 否则访问StateName时将出错
enum SystemState
{
    State_Idle    = 0,  //空闲, 停止控制指令发送，退出自动驾驶模式
    State_Drive   = 1,  //前进,前进档
    State_Reverse = 2,  //后退,后退档
    State_Stop    = 3,  //停车, 速度为零时切换为空闲

    State_SwitchToDrive  = 4,  //任务切换为前进，
                                //①若当前为R挡，速度置零->切N挡->切D档
                                //②若当前为D档，不进行其他操作
                                //跳转到前进模式
    State_SwitchToReverse= 5,  //任务切换为倒车
                                //①若当前为R档，不进行其他操作
                                //②若当前为D档，速度置零->切N档->切R档
                                //跳转到后退模式
    State_ForceExternControl=6, //强制使用外部控制器状态


    State_OfflineDebug = 7,   //离线调试

    State_TaskComplete  = 8,  //任务完成
    State_TaskPreempt   = 9,  //任务被中断
};

static const std::vector<std::string> StateName = {"State_Idle", "State_Drive", "State_Reverse",
                    "State_Stop", "State_SwitchToDrive", "State_SwitchToReverse",
                    "State_ForceExternControl", "State_OfflineDebug", "Task Complete", "Task Preempt"};

#endif