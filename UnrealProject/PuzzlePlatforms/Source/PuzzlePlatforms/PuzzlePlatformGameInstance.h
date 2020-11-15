// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MenuSystem/MenuInterface.h"
#include "OnlineSessionInterface.h"
#include "OnlineSubsystem.h"
#include "PuzzlePlatformGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UPuzzlePlatformGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()

public:
	UPuzzlePlatformGameInstance(const FObjectInitializer & ObjectInitializer);

	virtual void Init();

	UFUNCTION(Exec)
	void Host(FString ServerName) override;

	UFUNCTION(Exec)
	void Join(uint32 Index) override;

	void StartSession();

	UFUNCTION(BlueprintCallable)
	void LoadMenu();

	UFUNCTION(BlueprintCallable)
	void LoadInGameMenu();

	void CancelInGameMenu();

	void QuitToMainMenu();

	virtual void LoadMainMenu() override;

	virtual void QuitGame() override;

	void RefreshServerList() override;

private:
	TSubclassOf<class UUserWidget> MenuClass;

	TSubclassOf<class UUserWidget> InGameMenuClass;

	TSharedPtr<class FOnlineSessionSearch> SessionSearch;

	class UMainMenu* Menu;

	class UInGameMenu* InGameMenu;	

	IOnlineSessionPtr SessionInterface;

	void OnCreateSessionComplete(FName SessionName, bool Success);
	void OnDestroySessionComplete(FName SessionName, bool Success);
	void OnFindSessionsComplete(bool Success);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	FString DesiredServerName;

	void CreateSession();	
};