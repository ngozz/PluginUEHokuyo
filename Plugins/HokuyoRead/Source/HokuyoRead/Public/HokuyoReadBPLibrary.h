// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "HokuyoReadBPLibrary.generated.h"


UCLASS()
class UHokuyoReadBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintPure, meta = (DevelopmentOnly = false, KeyWords = "Swampy", DisplayName = "Test Name", CompactNodeTitle = "Test", ToolTip = "Pew Pew Pew @param value float pew @param ReturnValue2 Hello pew @return return pew"), Category = "HokuyoRead | Sub")
	static bool testFunction(float value, int value2, FString& ReturnValue2);

	UFUNCTION(BlueprintCallable)
	static bool hokuyoTest(int value, int& ReturnPew);

	UFUNCTION(BlueprintCallable)
	static bool hokuyoRead();
};
