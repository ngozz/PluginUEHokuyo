// Copyright Epic Games, Inc. All Rights Reserved.

#include "HokuyoReadBPLibrary.h"
#include "HokuyoRead.h"
#include "Async/AsyncWork.h"

bool UHokuyoReadBPLibrary::testFunction(float value, int value2, FString& ReturnValue2)
{
	ReturnValue2 = "Hello World!";
	return true;
}

bool UHokuyoReadBPLibrary::hokuyoTest(int value, int& ReturnPew)
{
	return true;
}

void UHokuyoReadBPLibrary::closeConnectionifFalse(bool Result, UPARAM(ref) FUrgWrapper& UrgWrapper)
{
    if (!Result) {
		closeConnection(UrgWrapper);
	}
}

bool UHokuyoReadBPLibrary::openConnection(UPARAM(ref) FUrgWrapper& UrgWrapper)
{
    urg_t& urg = UrgWrapper.Urg;
    int ret;
    // Open the connection with the device
    ret = urg_open(&urg, URG_ETHERNET, "192.168.3.202", 10940);
    //ret = urg_open(&urg, URG_SERIAL, "COM3", 115200); //Example of serial connection. COM3 is the port name, 115200 is the baudrate.
    if (ret < 0) {
        return false;
    }
    return true;
}

bool UHokuyoReadBPLibrary::setScanningParameters(UPARAM(ref) FUrgWrapper& UrgWrapper)
{
    urg_t& urg = UrgWrapper.Urg;

    // \~english Case where the measurement range (start/end steps) is defined
    urg_set_scanning_parameter(&urg,
        urg_deg2step(&urg, -1),
        urg_deg2step(&urg, +1), 0);

    return true;
}

bool UHokuyoReadBPLibrary::getDistance(UPARAM(ref) FUrgWrapper& UrgWrapper, int CaptureTimes)
{
    urg_t& urg = UrgWrapper.Urg;
    int ret;
    long* data;
    int data_max;
    long time_stamp;
    int i;

    // Get length datas
    data_max = urg_max_data_size(&urg);
    data = (long*)malloc(sizeof(long) * data_max);
    if (data == nullptr) {
        urg_close(&urg);
        return false;
    }

    urg_start_measurement(&urg, URG_DISTANCE, URG_SCAN_INFINITY, 0, 1);
    // Get datas
    for (i = 0; CaptureTimes == 0 || i < CaptureTimes; ++i) {
        ret = urg_get_distance(&urg, data, &time_stamp);
        if (ret <= 0) {
            free(data);
            urg_close(&urg);
            return false;
        }
        (void)time_stamp;

        int j;
        long min_distance;
        long max_distance;
        urg_distance_min_max(&urg, &min_distance, &max_distance);
        for (j = 0; j < ret; ++j) {
            long l = data[j];
            double radian;

            if ((l <= min_distance) || (l >= max_distance)) {
                continue;
            }
            radian = urg_index2rad(&urg, j);
            int32 x = (int32)(l * sin(radian));
            int32 y = (int32)(l * cos(radian));
            // Print x and y values in real-time
            FString xString = FString::FromInt(x);
            FString yString = FString::FromInt(y);
            FString xyString = "x: " + xString + ", y: " + yString;
            if (GEngine)
                GEngine->AddOnScreenDebugMessage(-1, 0.025f, FColor::Red, xyString);
        }
    }

    // Free memory
    free(data);

    return true;
}

bool UHokuyoReadBPLibrary::closeConnection(UPARAM(ref) FUrgWrapper& UrgWrapper)
{
    urg_t& urg = UrgWrapper.Urg;
    // Close the connection
    urg_close(&urg);

    return true;
}

class FGetDistanceTask : public FNonAbandonableTask
{
    friend class FAutoDeleteAsyncTask<FGetDistanceTask>;

    FUrgWrapper& UrgWrapper;
    int CaptureTimes;

public:
    FGetDistanceTask(FUrgWrapper& InUrgWrapper, int InCaptureTimes)
        : UrgWrapper(InUrgWrapper)
        , CaptureTimes(InCaptureTimes)
    {
    }

    void DoWork()
    {
        UHokuyoReadBPLibrary::getDistance(UrgWrapper, CaptureTimes);
    }

    FORCEINLINE TStatId GetStatId() const
    {
        RETURN_QUICK_DECLARE_CYCLE_STAT(FGetDistanceTask, STATGROUP_ThreadPoolAsyncTasks);
    }
};

void UHokuyoReadBPLibrary::getDistanceNonBlocking(UPARAM(ref) FUrgWrapper& UrgWrapper, int CaptureTimes)
{
    (new FAutoDeleteAsyncTask<FGetDistanceTask>(UrgWrapper, CaptureTimes))->StartBackgroundTask();
}