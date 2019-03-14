#include <ZWayLib.h>

using namespace std;

int main()
{
    ZWay zway       = NULL;
    ZWLog logger    = zlog_create(stdout, Debug);

    ZWError r = zway_init(&zway, ZSTR("/dev/ttyACM0"), NULL, NULL, NULL, NULL, logger);

    printf("Hello World!\n");

    return 0;
}
