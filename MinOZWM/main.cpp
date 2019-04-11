//#include <ZWayLib.h>

#include "../src/Manager.h"

using namespace OpenZWaveMe;

int main()
{
    /*ZWay zway       = NULL;
    ZWLog logger    = zlog_create(stdout, Debug);

    ZWError r = zway_init(&zway, ZSTR("/dev/ttyACM0"), NULL, NULL, NULL, NULL, logger);

    printf("Hello World!\n");*/

    Manager::Create();

    return 0;
}
