#include "signalinstance.h"

SignalInstance::SignalInstance()
{

}

SignalInstance& SignalInstance::GetInstance(){

    static SignalInstance RobotControl;

    return RobotControl;
}
