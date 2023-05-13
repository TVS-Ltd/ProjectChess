// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/GameplayStatics.h"
#include "OnlineSubsystemUtils.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Interfaces/OnlineSessionInterface.h"

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "EOSGameMode.generated.h"

/**
 * 
 */
UCLASS()
class CHESSDEV_API AEOSGameMode : public AGameMode
{
	GENERATED_BODY()
	
	// AEOSGameMode();

public:
	virtual void PostLogin(APlayerController* NewPlayer) override;
};
