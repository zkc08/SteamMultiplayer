// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameMenu.h"

#include "Components/Button.h"

#include "PuzzlePlatformGameInstance.h"

bool UInGameMenu::Initialize() 
{
    bool Success = Super::Initialize();
    if (!Success) return false;

    BackButton->OnClicked.AddDynamic(this, &UInGameMenu::BackButtonPressed);
    if (!ensure(BackButton != nullptr)) return false;

    QuitButton->OnClicked.AddDynamic(this, &UInGameMenu::QuitButtonPressed);
    if (!ensure(QuitButton != nullptr)) return false;
    
    return  true;
}

void UInGameMenu::Setup() 
{
    this->AddToViewport();

    UWorld* World = GetWorld();
    if (!ensure(World != nullptr)) return;

    APlayerController* PlayerController = World->GetFirstPlayerController();
    if (!ensure(PlayerController != nullptr)) return;


    FInputModeUIOnly InputModeData;

    InputModeData.SetWidgetToFocus(this->TakeWidget());
    InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

    PlayerController->SetInputMode(InputModeData);

    PlayerController->bShowMouseCursor = true;
}

void UInGameMenu::TearDown() 
{
	this->RemoveFromViewport();

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	FInputModeGameOnly InputModeData;
	PlayerController->SetInputMode(InputModeData);

	PlayerController->bShowMouseCursor = false;
}

void UInGameMenu::BackButtonPressed() 
{
    TearDown();
}

void UInGameMenu::QuitButtonPressed() 
{
    if (MenuInterface != nullptr)
    {
        TearDown();
        MenuInterface->LoadMainMenu();
    }
}

void UInGameMenu::SetMenuInterface(IMenuInterface* MenuInterface) 
{
    this->MenuInterface = MenuInterface;
}
