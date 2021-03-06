#ifndef __DRIVER_MNT_INFLUXDB_QUERY_RESPONSE_H__
#define __DRIVER_MNT_INFLUXDB_QUERY_RESPONSE_H__

#include <driver/mnt/influxdb/influxdb.h>
#include <driver/mnt/influxdb/query_response_parser.h>

DRIVER_MNT_INFLUXDB_EXPORT
int16_t influxdb_mount_response_result_type(
    influxdb_Query_SeriesResult *series);

DRIVER_MNT_INFLUXDB_EXPORT
int16_t influxdb_mount_response_result_value(
    corto_record *result,
    JSON_Value *value,
    JSON_Object *resultJson,
    corto_string name,
    bool convertTime);

DRIVER_MNT_INFLUXDB_EXPORT
int16_t influxdb_mount_response_result_update(
    influxdb_Query_SeriesResult *series,
    JSON_Array *values,
    corto_record *r);

DRIVER_MNT_INFLUXDB_EXPORT
int16_t influxdb_mount_query_response_handler(
    influxdb_mount this,
    httpclient_Result *r,
    influxdb_mount_ResonseFilter *filter);

#endif //__DRIVER_MNT_INFLUXDB_QUERY_RESPONSE_H__
