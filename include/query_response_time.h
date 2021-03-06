#ifndef __DRIVER_MNT_INFLUXDB_QUERY_RESPONSE_TIME_H__
#define __DRIVER_MNT_INFLUXDB_QUERY_RESPONSE_TIME_H__

#include <driver/mnt/influxdb/influxdb.h>

DRIVER_MNT_INFLUXDB_EXPORT
int16_t influxdb_mount_response_time(JSON_Object *output, JSON_Value *value);

#endif //__DRIVER_MNT_INFLUXDB_QUERY_RESPONSE_TIME_H__
