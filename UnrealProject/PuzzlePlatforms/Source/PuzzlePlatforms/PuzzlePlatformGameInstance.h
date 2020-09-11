// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MenuSystem/MenuInterface.h"
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
	void Host() override;

	UFUNCTION(Exec)
	void Join(const FString& Address) override;

	UFUNCTION(BlueprintCallable)
	void LoadMenu();

	UFUNCTION(BlueprintCallable)
	void LoadInGameMenu();

	void CancelInGameMenu();

	void QuitToMainMenu();

	virtual void LoadMainMenu() override;

	virtual void QuitGame() override;

private:
	TSubclassOf<class UUserWidget> MenuClass;

	TSubclassOf<class UUserWidget> InGameMenuClass;

	class UMainMenu* Menu;

	class UInGameMenu* InGameMenu;		
};