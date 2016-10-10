// Fill out your copyright notice in the Description page of Project Settings.

#include "NGDA.h"
#include "NGDAGameSession.h"
#include "MainMenuWidget.h"




bool UMainMenuWidget::OnClickCreateGame()
{
	ANGDAGameSession* gameSession = GetGameSession();
	IOnlineSubsystem* onlineSubsystem = IOnlineSubsystem::Get();
	IOnlineIdentityPtr onlineIdentitySystem = onlineSubsystem->GetIdentityInterface();
	bool createSessionCalled = gameSession->CreateSession(onlineIdentitySystem->GetUniquePlayerId(0), FString("MyLevel"), false, true, 4);
	if (createSessionCalled)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, TEXT("Create Session Called Successfully!"));
	}

	return true;
}

bool UMainMenuWidget::OnClickFindSession()
{

	ANGDAGameSession* gameSession = GetGameSession();
	IOnlineSubsystem* onlineSubsystem = IOnlineSubsystem::Get();
	IOnlineIdentityPtr onlineIdentitySystem = onlineSubsystem->GetIdentityInterface();

	if (gameSession)
	{
		gameSession->OnFindSessionsComplete().AddUObject(this, &UMainMenuWidget::FindSessionComplete);
		return gameSession->FindSession(onlineIdentitySystem->GetUniquePlayerId(0));
	}
	return false;
}

void UMainMenuWidget::FindSessionComplete(bool wasSuccessful)
{
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, TEXT("FindSessionComplete callback!"));
}

bool UMainMenuWidget::OnClickJoinGame()
{
	ANGDAGameSession* gameSession = GetGameSession();
	IOnlineSubsystem* onlineSubsystem = IOnlineSubsystem::Get();
	IOnlineIdentityPtr onlineIdentitySystem = onlineSubsystem->GetIdentityInterface();
	FOnlineSessionSearchResult searchResult = gameSession->GetSearchResults()[0];
	return gameSession->JoinSession(onlineIdentitySystem->GetUniquePlayerId(0), FName(*(searchResult.Session.OwningUserName)), searchResult);
}

ANGDAGameSession* UMainMenuWidget::GetGameSession()
{
	UWorld* world = GetWorld();
	ANGDAGameSession* gameSession = nullptr;
	if (world)
	{
		gameSession = Cast<ANGDAGameSession>(world->GetAuthGameMode()->GameSession);
	}
	return gameSession;
}