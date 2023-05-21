// Fill out your copyright notice in the Description page of Project Settings.


#include "EOSGameInstance.h"

void UEOSGameInstance::LoginWithEOS(FString ID, FString Token, FString LoginType)
{
    IOnlineSubsystem* SubsystemRef = Online::GetSubsystem(this->GetWorld());

    if (SubsystemRef)
    {
        IOnlineIdentityPtr IdentityPtrRef = SubsystemRef->GetIdentityInterface();

        if (IdentityPtrRef)
        {
            FOnlineAccountCredentials AccountDetails;

            AccountDetails.Id = ID;
            AccountDetails.Token = Token;
            AccountDetails.Type = LoginType;

            IdentityPtrRef->OnLoginCompleteDelegates->AddUObject(this, &UEOSGameInstance::LoginWithEOS_Return);

            IdentityPtrRef->Login(0, AccountDetails);
        }
    }


}

void UEOSGameInstance::LoginWithEOS_Return(int32 LocalUserNum, bool wasSuccessfull, const FUniqueNetId& UserID, const FString& Error)
{
    if (wasSuccessfull)
    {
        UE_LOG(LogTemp, Warning, TEXT("Successful Login!"));

        if(GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Successful Login!"));
		}
    } else
    {
        UE_LOG(LogTemp, Error, TEXT("Login failed, Reason: %S"), *Error);

        // if(GEngine)
		// {
		// 	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Login failed, Reason: %S"), *Error);
		// }
    }
}

FString UEOSGameInstance::GetPlayerUsername()
{
    IOnlineSubsystem* SubsystemRef = Online::GetSubsystem(this->GetWorld());

    if (SubsystemRef)
    {
        IOnlineIdentityPtr IdentityPtrRef = SubsystemRef->GetIdentityInterface();

        if (IdentityPtrRef)
        {
            if (IdentityPtrRef->GetLoginStatus(0) == ELoginStatus::LoggedIn)
            {
                return IdentityPtrRef->GetPlayerNickname(0);
            }
        }
    }

    return FString("ERROR");
}

bool UEOSGameInstance::IsPlayerLoggedIn()
{
    IOnlineSubsystem* SubsystemRef = Online::GetSubsystem(this->GetWorld());

    if (SubsystemRef)
    {
        IOnlineIdentityPtr IdentityPtrRef = SubsystemRef->GetIdentityInterface();

        if (IdentityPtrRef)
        {
            if (IdentityPtrRef->GetLoginStatus(0) == ELoginStatus::LoggedIn)
            {
                return true;
            }
        }
    }

    return false;
}

void UEOSGameInstance::CreateEOSSession(bool isDedicatedServer, bool isLanServer, int32 NumOfPublicConnections)
{
    IOnlineSubsystem* SubsystemRef = Online::GetSubsystem(this->GetWorld());

    if (SubsystemRef)
    {
        IOnlineSessionPtr SessionPtrRef = SubsystemRef->GetSessionInterface();

        if (SessionPtrRef)
        {
            FOnlineSessionSettings SessionCreationInfo;

            SessionCreationInfo.bIsDedicated = isDedicatedServer;
            SessionCreationInfo.bAllowInvites = true;
            SessionCreationInfo.bIsLANMatch = isLanServer;
            SessionCreationInfo.NumPublicConnections = NumOfPublicConnections;
            SessionCreationInfo.bUseLobbiesIfAvailable = false;
            SessionCreationInfo.bUsesPresence = false;
            SessionCreationInfo.bShouldAdvertise = true;
            SessionCreationInfo.Set(SEARCH_KEYWORDS, FString("Random"), EOnlineDataAdvertisementType::ViaOnlineService);

            SessionPtrRef->OnCreateSessionCompleteDelegates.AddUObject(this, &UEOSGameInstance::OnCreateSessionCompleted);
            SessionPtrRef->CreateSession(0, FName("TestSession"), SessionCreationInfo);
        }
    }
}

void UEOSGameInstance::OnCreateSessionCompleted(FName SessionName, bool wasSuccessfull)
{
    if (wasSuccessfull)
    {
        GetWorld()->ServerTravel(OpenLevelURL);
    }
}

void UEOSGameInstance::FindSessionAndJoin()
{
    IOnlineSubsystem* SubsystemRef = Online::GetSubsystem(this->GetWorld());

    if (SubsystemRef)
    {
        IOnlineSessionPtr SessionPtrRef = SubsystemRef->GetSessionInterface();

        if (SessionPtrRef)
        {
            SessionSearch = MakeShareable(new FOnlineSessionSearch());

            SessionSearch->bIsLanQuery = false;
            SessionSearch->MaxSearchResults = 20;
            SessionSearch->QuerySettings.SearchParams.Empty();

            SessionPtrRef->OnFindSessionsCompleteDelegates.AddUObject(this, &UEOSGameInstance::OnFindSessionCompleted);
            SessionPtrRef->FindSessions(0, SessionSearch.ToSharedRef());
        }
    }
}

void UEOSGameInstance::JoinEOSSession()
{

}

void UEOSGameInstance::OnFindSessionCompleted(bool wasSuccessfull)
{
    if (wasSuccessfull)
    {
        IOnlineSubsystem* SubsystemRef = Online::GetSubsystem(this->GetWorld());

        if (SubsystemRef)
        {
            IOnlineSessionPtr SessionPtrRef = SubsystemRef->GetSessionInterface();

            if (SessionPtrRef && SessionSearch->SearchResults.Num() > 0)
            {
                SessionPtrRef->OnJoinSessionCompleteDelegates.AddUObject(this, &UEOSGameInstance::OnJoinSessionCompleted);
                SessionPtrRef->JoinSession(0, FName("TestSession"), SessionSearch->SearchResults[0]);
            } else
            {
                CreateEOSSession(false, false, 2);
            }
        }
    } else
    {
        CreateEOSSession(false, false, 2);
    }
}

void UEOSGameInstance::OnJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
    if (Result == EOnJoinSessionCompleteResult::Success)
    {
        APlayerController* PlayerControllerRef = GetFirstLocalPlayerController(GetWorld());
        

        if (PlayerControllerRef)
        {
            FString JoinAddress;

            IOnlineSubsystem* SubsystemRef = Online::GetSubsystem(this->GetWorld());

            if (SubsystemRef)
            {
                IOnlineSessionPtr SessionPtrRef = SubsystemRef->GetSessionInterface();

                if (SessionPtrRef)
                {
                    SessionPtrRef->GetResolvedConnectString(FName("TestSession"), JoinAddress);

                    if (!JoinAddress.IsEmpty())
                    {
                        PlayerControllerRef->ClientTravel(JoinAddress, ETravelType::TRAVEL_Absolute);
                    }
                }
            }
        }
    }
}

void UEOSGameInstance::DestroyEOSSession()
{
    IOnlineSubsystem* SubsystemRef = Online::GetSubsystem(this->GetWorld());

    if (SubsystemRef)
    {
        IOnlineSessionPtr SessionPtrRef = SubsystemRef->GetSessionInterface();

        if (SessionPtrRef)
        {
            SessionPtrRef->OnDestroySessionCompleteDelegates.AddUObject(this, &UEOSGameInstance::OnDestroySessionCompleted);
            SessionPtrRef->DestroySession(FName("TestSession"));
        }
    }
}

void UEOSGameInstance::OnDestroySessionCompleted(FName SessionName, bool wasSuccessfull)
{
    if (wasSuccessfull)
    {
        UE_LOG(LogTemp, Warning, TEXT("Destroyed session successfully!"));

        if(GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Destroyed session successfully!"));
		}
    } else
    {
        UE_LOG(LogTemp, Error, TEXT("Couldn't destroy session!"));

        if(GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Couldn't destroy session!"));
		}
    }
}