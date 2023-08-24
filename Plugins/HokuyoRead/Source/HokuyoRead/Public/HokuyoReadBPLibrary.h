// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "urg_sensor.h"
#include "urg_utils.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "HokuyoReadBPLibrary.generated.h"

USTRUCT(BlueprintType)
struct FUrgWrapper //This is a wrapper struct that will be used to pass the urg_t type to the blueprint. Can't pass the urg_t type directly to the blueprint.
{
	GENERATED_BODY()

	// Add an instance of the urg_t type
	urg_t Urg;
};

UCLASS(BlueprintType)
class UHokuyoReadBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	//open the connection
	UFUNCTION(BlueprintCallable)
	static bool openConnection(UPARAM(ref) FUrgWrapper& UrgWrapper); //UPARAM(ref) is used to pass a reference to the struct. UE doesn't support references by default using '&'.

	//get the distance
	UFUNCTION(BlueprintCallable)
	static bool getDistance(UPARAM(ref) FUrgWrapper& UrgWrapper, int CaptureTimes);

	//close the connection
	UFUNCTION(BlueprintCallable)
	static bool closeConnection(UPARAM(ref) FUrgWrapper& UrgWrapper);

	//check the boolean result, if it's false, close the connection
	UFUNCTION(BlueprintCallable)
	static void closeConnectionifFalse(bool Result, UPARAM(ref) FUrgWrapper& UrgWrapper);

	//set scanning degrees
	UFUNCTION(BlueprintCallable)
	static bool setScanningParameters(UPARAM(ref) FUrgWrapper& UrgWrapper);

	//test function
	UFUNCTION(BlueprintPure, meta = (DevelopmentOnly = false, KeyWords = "Swampy", DisplayName = "Test Name", CompactNodeTitle = "Test", ToolTip = "Pew Pew Pew @param value float pew @param ReturnValue2 Hello pew @return return pew"), Category = "HokuyoRead | Sub")
	static bool testFunction(float value, int value2, FString& ReturnValue2);

	//test function
	UFUNCTION(BlueprintCallable)
	static bool hokuyoTest(int value, int& ReturnPew);

	UFUNCTION(BlueprintCallable)
	static void getDistanceNonBlocking(UPARAM(ref) FUrgWrapper& UrgWrapper, int CaptureTimes);
};