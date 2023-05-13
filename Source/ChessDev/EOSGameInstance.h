// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "OnlineSubsystemUtils.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Interfaces/OnlineSessionInterface.h"


#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "EOSGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class CHESSDEV_API UEOSGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "EOS")
	void LoginWithEOS(FString ID, FString Token, FString LoginType);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EOS")
	FString GetPlayerUsername();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EOS")
	bool IsPlayerLoggedIn();

	UFUNCTION(BlueprintCallable, Category = "EOS")
	void CreateEOSSession(bool isDedicatedServer, bool isLanServer, int32 NumOfPublicConnections);

	UFUNCTION(BlueprintCallable, Category = "EOS")
	void FindSessionAndJoin();

	UFUNCTION(BlueprintCallable, Category = "EOS")
	void JoinEOSSession();

	UFUNCTION(BlueprintCallable, Category = "EOS")
	void DestroyEOSSession();
	
	void LoginWithEOS_Return(int32 LocalUserNum, bool wasSuccessfull, const FUniqueNetId& UserID, const FString& Error);
	void OnCreateSessionCompleted(FName SessionName, bool wasSuccessfull);
	void OnDestroySessionCompleted(FName SessionName, bool wasSuccessfull);
	void OnFindSessionCompleted(bool wasSuccessfull);
	void OnJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

protected:
	TSharedPtr<FOnlineSessionSearch> SessionSearch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS")
	FString OpenLevelURL;
};