#include <include/common.h>

corto_id INFLUX_MOUNT_ID = "influx_weather";
corto_string INFLUX_DB_HOST = "http://localhost:8086";
corto_string INFLUX_DB_NAME = "unit_test";

influxdb_Mount influxdbMount = NULL;

int CreateManualMount(corto_object mountPoint)
{
    influxdbMount = influxdb_MountDeclareChild(root_o, INFLUX_MOUNT_ID);
    corto_string fromPath = corto_fullpath(NULL, mountPoint);
    corto_query* query = corto_queryCreate("//", fromPath, NULL, NULL, NULL, 0, 0, NULL, NULL);
    corto_queuePolicy *queue = corto_queuePolicyCreate(25);

    int policyMask = 0;
    policyMask |= CORTO_MOUNT_NOTIFY | CORTO_MOUNT_QUERY;
    corto_mountPolicy *policy = corto_mountPolicyCreate(CORTO_LOCAL_OWNER,
         policyMask,
         0,
         queue,
         0,
         false);

     influxdb_RetentionPolicy rp = influxdb_RetentionPolicyCreate(
         "unit_test",
         INFLUX_DB_HOST,
         INFLUX_DB_NAME,
         "1h0m0s",
         1,
         NULL);

    test_assert(rp != NULL);

    if (influxdb_MountDefine(influxdbMount,
        query,
        "text/json",
        policy,
        INFLUX_DB_HOST,     /* hostname */
        INFLUX_DB_NAME,    /* database name */
        rp,                /* retention policy */
        NULL,              /* username */
        NULL))             /* password */
    {
        corto_error("Failed to define weather mount");
        goto error;
    }

    corto_release(query);
    corto_release(queue);
    corto_release(policy);

    return 0;
error:
    return -1;
}

int CreateHistoricalManualMount(corto_object mountPoint)
{
    influxdbMount = influxdb_MountDeclareChild(root_o, INFLUX_MOUNT_ID);
    corto_string fromPath = corto_fullpath(NULL, mountPoint);
    corto_query* query = corto_queryCreate("//", fromPath, NULL, NULL, NULL, 0, 0, NULL, NULL);
    corto_queuePolicy *queue = corto_queuePolicyCreate(25);
    int policyMask = 0;
    policyMask |= CORTO_MOUNT_HISTORY_QUERY;
    corto_mountPolicy *policy = corto_mountPolicyCreate(CORTO_LOCAL_OWNER,
         policyMask,
         1,
         queue,
         0,
         false);

     influxdb_RetentionPolicy rp = influxdb_RetentionPolicyCreate(
         "unit_test",
         INFLUX_DB_HOST,
         INFLUX_DB_NAME,
         "1h0m0s",
         1,
         NULL);

    test_assert(rp != NULL);

    if (influxdb_MountDefine(influxdbMount,
        query,
        "text/json",
        policy,
        INFLUX_DB_HOST,     /* hostname */
        INFLUX_DB_NAME,    /* database name */
        rp,                /* retention policy */
        NULL,              /* username */
        NULL))             /* password */
    {
        corto_error("Failed to define weather mount");
        goto error;
    }

    corto_release(query);
    corto_release(queue);
    corto_release(policy);

    return 0;
error:
    return -1;
}

int DumbRandom(int min, int max){
   return min + rand() / (RAND_MAX / (max - min + 1) + 1);
}

void QuickWeatherUpdate(test_Weather weather, int temp, int humidity, int uv) {
    corto_time now;
    corto_timeGet(&now);
    if (test_WeatherUpdate(
        weather,
        DumbRandom(temp-20, temp+20),
        DumbRandom(humidity-10, humidity+10),
        DumbRandom(uv-3, 12),
        &now)) {
        corto_error("Weather update failed. Error: %s", corto_lasterr());
    }

}

int16_t CreateWeatherObjects(corto_object weather)
{
    corto_time now;
    corto_timeGet(&now);

    test_State kentucky = test_StateCreateChild(weather, "kentucky", "south", true);
    test_State texas =  test_StateCreateChild(weather, "texas", "southwest", false);
    test_State california =  test_StateCreateChild(weather, "california", "west", false);
    test_City houston = test_CityCreateChild(texas, "houston", 1837, 8538000);
    test_City lexington = test_CityCreateChild(kentucky, "lexington", 1782, 318449);
    test_City nicholasville = test_CityCreateChild(kentucky, "nicholasville", 1798, 30006);
    test_City sanDiego = test_CityCreateChild(california, "San Diego", 1769, 1407000);

    test_Weather weatherH = test_WeatherCreateChild(houston, "weather", 82, 45.5, 8, &now);
    test_Weather weatherL = test_WeatherCreateChild(lexington, "weather", 95, 78.8, 6, &now);
    test_Weather weatherN = test_WeatherCreateChild(nicholasville, "weather", 45, 47, 4, &now);
    test_Weather weatherS = test_WeatherCreateChild(sanDiego, "weather", 50, 48, 6, &now);

    sleep(2);

    int cnt = 0;
    while (cnt < 2) {
        cnt++;
        QuickWeatherUpdate(weatherL, 82, 45, 8);
        QuickWeatherUpdate(weatherH, 95, 78, 6);
        QuickWeatherUpdate(weatherN, 45, 47, 4);
        QuickWeatherUpdate(weatherS, 50, 48, 6);
        usleep(1000);
    }

    corto_release(kentucky);
    corto_release(texas);
    corto_release(california);
    corto_release(houston);
    corto_release(lexington);
    corto_release(nicholasville);
    corto_release(sanDiego);

    return 0;
}
