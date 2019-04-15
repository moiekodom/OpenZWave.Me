//#include <ZWayLib.h>

#include "../src/Manager.h"

using namespace OpenZWaveMe;

int main()
{
    //ZWay zway       = NULL;
    //ZWLog logger    = zlog_create(stdout, Debug);

    //ZWError r = zway_init(&zway, ZSTR("/dev/ttyACM0"), ZSTR("config"), ZSTR("translations"), ZSTR("ZDDX"), NULL, logger);

    //Log::Write(OpenZWave::LogLevel_Error, "Error code: %d", 0);

    Options::Create( "/usr/local/etc/openzwave/", "", "");
	Options::Get()->AddOptionInt("SaveLogLevel", OpenZWave::LogLevel_Detail);
	Options::Get()->AddOptionInt("QueueLogLevel", OpenZWave::LogLevel_Debug);
	Options::Get()->AddOptionInt("DumpTrigger", OpenZWave::LogLevel_Error);
	Options::Get()->AddOptionInt("PollInterval", 500 );
	Options::Get()->AddOptionBool("IntervalBetweenPolls", TRUE);
	Options::Get()->AddOptionBool("ValidateValueChanges", TRUE);
	Options::Get()->Lock();

    Manager::Create();

    Manager::Get()->AddDriver("/dev/ttyACM0");

    return 0;
}
