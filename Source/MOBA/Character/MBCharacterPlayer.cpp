// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MBCharacterPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

AMBCharacterPlayer::AMBCharacterPlayer()
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	//input
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> IMCRef(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/MOBA/Input/IMC_Default.IMC_Default'"));
	if (IMCRef.Object != nullptr)
	{
		DefaultMappingContext = IMCRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> IAMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/MOBA/Input/Actions/IA_Move.IA_Move'"));
	if (IAMoveRef.Object != nullptr)
	{
		MoveAction = IAMoveRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> IAJumpRef(TEXT("/Script/EnhancedInput.InputAction'/Game/MOBA/Input/Actions/IA_Jump.IA_Jump'"));
	if (IAJumpRef.Object != nullptr)
	{
		JumpAction = IAJumpRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> IALookRef(TEXT("/Script/EnhancedInput.InputAction'/Game/MOBA/Input/Actions/IA_Look.IA_Look'"));
	if (IALookRef.Object != nullptr)
	{
		LookAction = IALookRef.Object;
	}
}

void AMBCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();
	APlayerController* PC = CastChecked<APlayerController>(GetController());
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
}

void AMBCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UEnhancedInputComponent* EIComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	EIComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
	EIComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	EIComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMBCharacterPlayer::Move);
	EIComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMBCharacterPlayer::Look);
}

void AMBCharacterPlayer::Move(const FInputActionValue& Value)
{
	FVector2D MoveVector = Value.Get<FVector2D>();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDir = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDir = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDir, MoveVector.X);
	AddMovementInput(RightDir, MoveVector.Y);
}

void AMBCharacterPlayer::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

