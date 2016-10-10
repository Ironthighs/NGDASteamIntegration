// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Online.h"
#include "GameFramework/GameSession.h"
#include "NGDAGameSession.generated.h"

/**
 * 
 */
UCLASS()
class NGDA_API ANGDAGameSession : public AGameSession
{
	GENERATED_BODY()
	

protected:

	DECLARE_EVENT_OneParam(AGravballGameSession, FOnFindSessionsComplete, bool /* wasSuccessful */)
	FOnFindSessionsComplete FindSessionsCompleteEvent;

	FDelegateHandle OnCreateSessionCompleteDelegateHandle;
	FDelegateHandle OnStartSessionCompleteDelegateHandle;
	FDelegateHandle OnFindSessionsCompleteDelegateHandle;
	FDelegateHandle OnJoinSessionCompleteDelegateHandle;

	FString TravelURL;
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;

	bool bCreatingSession = false;

public:

	FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;
	FOnStartSessionCompleteDelegate OnStartSessionCompleteDelegate;
	FOnFindSessionsCompleteDelegate OnFindSessionsCompleteDelegate;
	FOnJoinSessionCompleteDelegate OnJoinSessionCompleteDelegate;

	bool CreateSession(TSharedPtr<const FUniqueNetId> playerId, const FString& MapName, bool bIsLAN, bool bIsPresence, int32 MaxNumPlayers);
	void OnCreateSessionComplete(FName sessionName, bool wasSuccessful);

	bool StartSession(FName sessionName);
	void OnStartSessionComplete(FName InSessionName, bool bWasSuccessful);

	bool FindSession(TSharedPtr<const FUniqueNetId> playerId);
	void OnFindSessionComplete(bool wasSuccessful);

	bool JoinSession(TSharedPtr<const FUniqueNetId> playerId, FName sessionName, const FOnlineSessionSearchResult& searchResult);
	void OnJoinSessionComplete(FName sessionName, EOnJoinSessionCompleteResult::Type joinResult);

	FOnFindSessionsComplete& OnFindSessionsComplete() { return FindSessionsCompleteEvent; }

	TArray<FOnlineSessionSearchResult> GetSearchResults() { return SessionSearch->SearchResults; }

};