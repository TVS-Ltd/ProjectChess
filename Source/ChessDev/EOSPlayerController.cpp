// Fill out your copyright notice in the Description page of Project Settings.


#include "EOSPlayerController.h"


void AEOSPlayerController::OnNetCleanup(UNetConnection* Connection)
{
    UEOSGameInstance* GameInstanceRef = Cast<UEOSGameInstance>(GetWorld()->GetGameInstance());

    if (GameInstanceRef)
    {
        GameInstanceRef->DestroyEOSSession();
    }

    Super::OnNetCleanup(Connection);
}

