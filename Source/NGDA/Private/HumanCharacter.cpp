// Fill out your copyright notice in the Description page of Project Settings.

#include "NGDA.h"
#include "HumanCharacter.h"


// Sets default values
AHumanCharacter::AHumanCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AHumanCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHumanCharacter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

// Called to bind functionality to input
void AHumanCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);


	//InputComponent->BindAction(ACTION_JUMP, IE_Pressed, this, &ACharacter::Jump);
	//InputComponent->BindAction(ACTION_JUMP, IE_Released, this, &ACharacter::StopJumping);
	//InputComponent->BindAction(ACTION_PRIMARY_FIRE, IE_Pressed, this, &AHumanCharacter::PrimaryFirePressed);
	//InputComponent->BindAction(ACTION_PRIMARY_FIRE, IE_Released, this, &AHumanCharacter::PrimaryFireReleased);
	//InputComponent->BindAction(ACTION_SECONDARY_FIRE, IE_Pressed, this, &AHumanCharacter::SecondaryFirePressed);
	//InputComponent->BindAction(ACTION_SECONDARY_FIRE, IE_Released, this, &AHumanCharacter::SecondaryFireReleased);
	//InputComponent->BindAction(ACTION_USE, IE_Pressed, this, &AHumanCharacter::UsePressed);

	InputComponent->BindAxis(AXIS_MOVE_FORWARD, this, &AHumanCharacter::MoveForward);
	InputComponent->BindAxis(AXIS_MOVE_RIGHT, this, &AHumanCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	InputComponent->BindAxis(AXIS_TURN, this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis(AXIS_TURN_RATE, this, &AHumanCharacter::TurnAtRate);
	InputComponent->BindAxis(AXIS_LOOK_UP, this, &APawn::AddControllerPitchInput);
	InputComponent->BindAxis(AXIS_LOOK_UP_RATE, this, &AHumanCharacter::LookUpAtRate);



}

void AHumanCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AHumanCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AHumanCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		FVector CamLoc;
		FRotator CamRot;

		GetController()->GetPlayerViewPoint(CamLoc, CamRot);

		CamLoc = this->GetActorLocation();

		const FVector Directionn = CamRot.Vector();
		const FVector StartTrace = CamLoc + Directionn * 100;
		const FVector EndTrace = StartTrace + Directionn * 200;
		DrawDebugLine(GetWorld(), StartTrace, EndTrace, FColor::Blue, false, -1, 0, 12.333);

		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AHumanCharacter::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}
