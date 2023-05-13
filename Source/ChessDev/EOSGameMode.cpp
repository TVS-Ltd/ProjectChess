// Fill out your copyright notice in the Description page of Project Settings.


#include "EOSGameMode.h"

void AEOSGameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);

    if (NewPlayer)
    {
        FUniqueNetIdRepl UniqueNetIDRepl;

        if (NewPlayer->IsLocalController())
        {
            ULocalPlayer* LocalPlayerRef = NewPlayer->GetLocalPlayer();

            if (LocalPlayerRef)
            {
                UniqueNetIDRepl = LocalPlayerRef->GetPreferredUniqueNetId();
            } else
            {
                UNetConnection* RemoteNetConnectionRef = Cast<UNetConnection>(NewPlayer->Player);

                check(IsValid(RemoteNetConnectionRef));

                UniqueNetIDRepl = RemoteNetConnectionRef->PlayerId;
            }
        } else
        {
            UNetConnection* RemoteNetConnectionRef = Cast<UNetConnection>(NewPlayer->Player);

            check(IsValid(RemoteNetConnectionRef));

            UniqueNetIDRepl = RemoteNetConnectionRef->PlayerId;
        }
        
        TSharedPtr<const FUniqueNetId> UniqueNetId = UniqueNetIDRepl.GetUniqueNetId();

        check(UniqueNetId != nullptr)

        IOnlineSubsystem* SubsystemRef = Online::GetSubsystem(NewPlayer->GetWorld());
        IOnlineSessionPtr SessionRef = SubsystemRef->GetSessionInterface();

        bool wasRegistrationSuccessfull = SessionRef->RegisterPlayer(FName("TestSession"), *UniqueNetId, false);

        if (wasRegistrationSuccessfull)
        {
            UE_LOG(LogTemp, Warning, TEXT("Registration Successful!"));

            if(GEngine)
		    {
		    	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Registration Successful!"));
		    }
        }
    }
}


