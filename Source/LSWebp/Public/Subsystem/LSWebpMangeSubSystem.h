
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "LSWebpType.h"
#include "Tickable.h"
#include "LSWebpMangeSubSystem.generated.h"

DECLARE_DELEGATE_OneParam(FGenerateWebpCallBack,bool)

/**
 * Internal use,maybe you can use FLSwebpCore API or LSWebpLibrary.
 */
UCLASS()
class  ULSWebpMangeSubSystem : public UGameInstanceSubsystem, public FTickableGameObject
{
	friend class ULSWebpBPLibrary;
	
	GENERATED_BODY()
public:


	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

public:


	virtual void    Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;
	virtual bool    IsTickable() const override;

protected:
	bool BeginRecord(FString& InGeneratedWebpPicturesPath, TSharedPtr< FLSWebpPictureInformation> InWebpPictureInformation);
	void RecordOneFrame(float DeltaTime);
	void EndRecord( FLSWebpFinishGenerateWebp& InFinshWebpBPDelegate);

	UFUNCTION()
	void GenerateWebpCallBack(bool bGenerateWebp);

	void ResetRecord();



protected:
	UFUNCTION()
		void ScreenShotCallback(int32 InWidth, int32 InHeight, const TArray<FColor>& InColors);

private:

	ELSWebpProcessType ProcessType;
	FString GeneratedWebpPicturesPath;
	TSharedPtr< FLSWebpPictureInformation> 	 WebpPictureInformation;

	FDelegateHandle ScreenHandle;
	TArray<TArray<FColor>> WebPColor;
	TArray<int32> WebpTimestepMillisecond;

	FGenerateWebpCallBack GenerateWebpCallBackDelegate;
	FLSWebpFinishGenerateWebp FinshWebpBPDelegate;

	FCriticalSection LSWebpMutex;
};