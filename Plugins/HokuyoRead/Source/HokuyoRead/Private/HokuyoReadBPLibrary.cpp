// Copyright Epic Games, Inc. All Rights Reserved.

#include "HokuyoReadBPLibrary.h"
#include "HokuyoRead.h"
#include "urg_sensor.h"
#include "urg_utils.h"

bool UHokuyoReadBPLibrary::testFunction(float value, int value2, FString& ReturnValue2)
{
	ReturnValue2 = "Hello World!";
	return true;
}

bool UHokuyoReadBPLibrary::hokuyoTest(int value, int& ReturnPew)
{
	return true;
}

bool UHokuyoReadBPLibrary::hokuyoRead()
{
	enum {
		CAPTURE_TIMES = 99,
	};
	urg_t urg;
	int ret;
	long* data;
	int data_max;
	long time_stamp;
	int i;

	// Open the connection with the device
	ret = urg_open(&urg, URG_ETHERNET, "192.168.3.202", 10940);
	if (ret < 0) {
		return false;
	}

	// Get length datas
	data_max = urg_max_data_size(&urg);
	data = (long*)malloc(sizeof(long) * data_max);
	if (data == nullptr) {
		return false;
	}

	// \~english Case where the measurement range (start/end steps) is defined
	urg_set_scanning_parameter(&urg,
		urg_deg2step(&urg, -1),
		urg_deg2step(&urg, +1), 0);

	urg_start_measurement(&urg, URG_DISTANCE, URG_SCAN_INFINITY, 0, 1);
	// Get datas
	for (i = 0; i < CAPTURE_TIMES; ++i) {
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
			int32 x = (int32)(l * cos(radian));
			int32 y = (int32)(l * sin(radian));
			// Print x and y values in real-time
			FString xString = FString::FromInt(x);
			FString yString = FString::FromInt(y);
			FString xyString = "x: " + xString + ", y: " + yString;
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, xyString);
		}
	}

	// Close the connection
	free(data);
	urg_close(&urg);

	return true;
}
