// Fill out your copyright notice in the Description page of Project Settings.

#include "NGDA.h"
#include "UnrealMath.h"
#include "NGDAGameSession.h"

ANGDAGameSession::ANGDAGameSession(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	//OnCreateSessionCompleteDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(this, &ANGDAGameSession::OnCreateSessionComplete);
	//OnStartSessionCompleteDelegate = FOnStartSessionCompleteDelegate::CreateUObject(this, &ANGDAGameSession::OnStartSessionComplete);
}


bool ANGDAGameSession::CreateSession(TSharedPtr<const FUniqueNetId> playerId, const FString& MapName, bool bIsLAN, bool bIsPresence, int32 MaxNumPlayers)
{
	if (!OnCreateSessionCompleteDelegate.IsBound())
	{
		OnCreateSessionCompleteDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(this, &ANGDAGameSession::OnCreateSessionComplete);
	}

	// Get Unreal's OnlineSubsystem. This is an interface that allows different OnlineSubsystems to be used.
	// Currently we're using the Steam OnlineSubsystem.
	IOnlineSubsystem* onlineSubsystem = IOnlineSubsystem::Get();

	// Get the SessionInterface from the OnlineSubsystem which will be used to create a session.
	IOnlineSessionPtr session = onlineSubsystem->GetSessionInterface();
	IOnlineIdentityPtr identitySubsystem = onlineSubsystem->GetIdentityInterface();


	//onlineSubsystem->SetUsingMultiplayerFeatures(*(identitySubsystem->GetUniquePlayerId(0)), true);


	// The StartURL will be used after creating the game session as the place to ServerTravel to.
	// This will be received by the clients connecting to the host which they will use to load the 
	// proper map and whatever else information provided in the URL.
	// We store the StartURL for later.
	TravelURL = FString::Printf(TEXT("/Game/StarterContent/Maps/%s?game=Regular%s"), *MapName, TEXT("?listen"));

	if (session.IsValid() && playerId.IsValid())
	{
		TSharedPtr<class FOnlineSessionSettings> sessionSettings = MakeShareable(new FOnlineSessionSettings());

		// Just setting some session settings. I just feel like I want these to be defined, but
		// in order to have others see your session in a list you MUST have bShouldAdvertise = true
		sessionSettings->bIsLANMatch = bIsLAN;
		sessionSettings->bUsesPresence = bIsPresence;
		sessionSettings->NumPublicConnections = MaxNumPlayers;
		sessionSettings->Set(SETTING_MAPNAME, MapName, EOnlineDataAdvertisementType::ViaOnlineService);

		// Again, you MUST have bShouldAdvertise = true if you want others to see your game session.
		sessionSettings->bShouldAdvertise = true;
		sessionSettings->bAllowJoinViaPresence = true;
		sessionSettings->bAllowJoinInProgress = true;
		// Add the delegate to the event.
		OnCreateSessionCompleteDelegateHandle = session->AddOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegate);

		// Create the session. Remember, we're "dereferencing" the values of our shared pointers.
		session->CreateSession(*playerId, FName(*FString::Printf(TEXT("Game %d"), FMath::Rand())), *sessionSettings);
	}

	return false;
}

void ANGDAGameSession::OnCreateSessionComplete(FName sessionName, bool wasSuccessful)
{
	IOnlineSubsystem* onlineSubsystem = IOnlineSubsystem::Get();
	IOnlineSessionPtr session = onlineSubsystem->GetSessionInterface();
	session->ClearOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegateHandle);
	if (wasSuccessful)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, TEXT("Session Created Successfully!"));
//		GetWorld()->ServerTravel(TravelURL);

//		UGameplayStatics::OpenLevel(GetWorld(), "MyLevel", true, "listen");
		bCreatingSession = true;
		StartSession(sessionName);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, TEXT("Session Creation Failed!"));
	}
}

bool ANGDAGameSession::StartSession(FName sessionName)
{
	if (!OnStartSessionCompleteDelegate.IsBound())
	{
		OnStartSessionCompleteDelegate = FOnStartSessionCompleteDelegate::CreateUObject(this, &ANGDAGameSession::OnStartSessionComplete);
	}
	IOnlineSubsystem* onlineSubsystem = IOnlineSubsystem::Get();
	IOnlineSessionPtr session = onlineSubsystem->GetSessionInterface();
	OnStartSessionCompleteDelegateHandle = session->AddOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegate);
	return session->StartSession(sessionName);
}

void ANGDAGameSession::OnStartSessionComplete(FName InSessionName, bool bWasSuccessful)
{
	IOnlineSubsystem* onlineSubsystem = IOnlineSubsystem::Get();
	IOnlineSessionPtr session = onlineSubsystem->GetSessionInterface();
	session->ClearOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegateHandle);
	if (bWasSuccessful)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, FString("Session Started Successfully! ").Append(TravelURL));
		FString URL;
		if (bCreatingSession)
		{
			GetWorld()->ServerTravel(TravelURL);
		}
		else if(session->GetResolvedConnectString(InSessionName, URL))
		{
			APlayerController* playerController = GetWorld()->GetFirstPlayerController();
			if (playerController)
			{
				playerController->ClientTravel(URL, ETravelType::TRAVEL_Absolute);
			}
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, TEXT("Session Failed to Start!"));
	}
}

bool ANGDAGameSession::FindSession(TSharedPtr<const FUniqueNetId> playerId)
{
	if (!OnFindSessionsCompleteDelegate.IsBound())
	{
		OnFindSessionsCompleteDelegate = FOnFindSessionsCompleteDelegate::CreateUObject(this, &ANGDAGameSession::OnFindSessionComplete);
	}

	IOnlineSubsystem* onlineSubsystem = IOnlineSubsystem::Get();
	IOnlineSessionPtr session = onlineSubsystem->GetSessionInterface();
	IOnlineIdentityPtr identitySubsystem = onlineSubsystem->GetIdentityInterface();
	onlineSubsystem->SetUsingMultiplayerFeatures(*(identitySubsystem->GetUniquePlayerId(0)), true);
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	SessionSearch->bIsLanQuery = false;
	SessionSearch->MaxSearchResults = 20;
	SessionSearch->PingBucketSize = 50;
	SessionSearch->SearchState = EOnlineAsyncTaskState::NotStarted;
	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
	OnFindSessionsCompleteDelegateHandle = session->AddOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegate);
	bool calledFindSession = session->FindSessions(*playerId, SessionSearch.ToSharedRef());

	if (calledFindSession)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, "Find Session called successfully");
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, "Find Session call failed!");
	}
	return calledFindSession;
}

void ANGDAGameSession::OnFindSessionComplete(bool wasSuccessful)
{
	IOnlineSubsystem* onlineSubsystem = IOnlineSubsystem::Get();
	IOnlineSessionPtr session = onlineSubsystem->GetSessionInterface();
	session->ClearOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegateHandle);
	if (wasSuccessful)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, FString::Printf(TEXT("Successfully Called Find Sessions! Found %d sessions"), SessionSearch->SearchResults.Num()));

		for (int i = 0; i < SessionSearch->SearchResults.Num(); i++)
		{
			FOnlineSessionSearchResult result = SessionSearch->SearchResults[i];
			GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, result.Session.OwningUserName);
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, TEXT("Find Sessions Call Failed!"));
	}
}

bool ANGDAGameSession::JoinSession(TSharedPtr<const FUniqueNetId> playerId, FName sessionName, const FOnlineSessionSearchResult& searchResult)
{
	if (!OnJoinSessionCompleteDelegate.IsBound())
	{
		OnJoinSessionCompleteDelegate = FOnJoinSessionCompleteDelegate::CreateUObject(this, &ANGDAGameSession::OnJoinSessionComplete);
	}

	IOnlineSubsystem* onlineSubsystem = IOnlineSubsystem::Get();
	
	if (onlineSubsystem)
	{
		IOnlineIdentityPtr identityPtr = onlineSubsystem->GetIdentityInterface();
		IOnlineSessionPtr sessionPtr = onlineSubsystem->GetSessionInterface();

		OnJoinSessionCompleteDelegateHandle = sessionPtr->AddOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegate);
		bool joinCalled = sessionPtr->JoinSession(*playerId, sessionName /*FName(*(searchResult.Session.OwningUserName))*/, searchResult);

		if (joinCalled)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, TEXT("Join Called"));
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, TEXT("Join Failed"));
		}
	}
	return false;
}

void ANGDAGameSession::OnJoinSessionComplete(FName sessionName, EOnJoinSessionCompleteResult::Type joinResult)
{
	if (joinResult == EOnJoinSessionCompleteResult::Success)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, FString("Successfully Joined %s").Append(sessionName.ToString()));
		bCreatingSession = false;
		StartSession(sessionName);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, TEXT("Failed to join"));

	}
}