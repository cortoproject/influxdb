#ifndef __DRIVER_MNT_INFLUXDB_QUERY_RESPONSE_PARSER_H__
#define __DRIVER_MNT_INFLUXDB_QUERY_RESPONSE_PARSER_H__

#include <driver/mnt/influxdb/influxdb.h>

#define JSON_PTR_VERIFY(ptr, msg) if (!ptr) { corto_throw(msg); goto error; }
#define JSON_SAFE_FREE(v) if (v != NULL) { json_value_free(v); v = NULL; }

/* Struct wrapping the current series result to be processed by
 * the influxdb_ResultCallback callback. */
typedef struct _influxdb_Query_SeriesResult {
    corto_string    name;
    JSON_Array      *values;
    JSON_Array      *columns;
    size_t          valueCount;
    bool            convertTime;
    corto_string    type;
} influxdb_Query_SeriesResult;

/* Callback used to process response series arrays. */
typedef int16_t (*influxdb_ResultCallback)(
    influxdb_mount ctx,
    influxdb_Query_SeriesResult *series,
    void *data);

typedef struct _influxdb_Query_Result {
    influxdb_ResultCallback callback;
    influxdb_mount          ctx;
    void                    *data;
} influxdb_Query_Result;

typedef struct _influxdb_mount_ResonseFilter {
    bool historical;
    uint64_t limit;
    uint64_t offset;
} influxdb_mount_ResonseFilter;

/* Function allocates memory, dest should be NULL (uninitialized) pointer */
DRIVER_MNT_INFLUXDB_EXPORT
int16_t influxdb_mount_series_deepCopy(
    influxdb_Query_SeriesResult *src,
    influxdb_Query_SeriesResult *dest);

DRIVER_MNT_INFLUXDB_EXPORT
void influxdb_mount_series_free(
    influxdb_Query_SeriesResult *series);

DRIVER_MNT_INFLUXDB_EXPORT
corto_string influxdb_mount_response_column_name(
    JSON_Array *columns,
    int pos);

DRIVER_MNT_INFLUXDB_EXPORT
int influxdb_mount_response_column_index(
    JSON_Array *cols,
    corto_string name);

DRIVER_MNT_INFLUXDB_EXPORT
int16_t influxdb_mount_response_parse(
    JSON_Value *responseValue,
    influxdb_Query_Result *result);

#endif //__DRIVER_MNT_INFLUXDB_QUERY_RESPONSE_PARSER_H__
