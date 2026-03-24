
#include "LSWebpBPLibrary.h"
#include "Subsystem/LSWebpMangeSubSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Core/LSWebpCore.h"
#include "Subsystem/LSWebpDisplaySubSystem.h"

void ULSWebpBPLibrary::BeginRecord(
	UObject* WorldContextObject,
	FString InGeneratedWebpPicturesPath,
	FLSWebpPictureInformation InWebpPictureInformation,
	bool& bBegin)
{
	bBegin =false;

	if (!WorldContextObject)
	{
		return ;
	}
	if (!WorldContextObject->GetWorld())
	{
		return ;
	}

	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject);
	ULSWebpMangeSubSystem* LSWebpMangeSubSystem = GameInstance->GetSubsystem<ULSWebpMangeSubSystem>();

	TSharedPtr<FLSWebpPictureInformation> LSWebpPictureInformationPtr = MakeShareable(new FLSWebpPictureInformation(InWebpPictureInformation));


	bBegin=	LSWebpMangeSubSystem->BeginRecord(InGeneratedWebpPicturesPath, LSWebpPictureInformationPtr);

}

void ULSWebpBPLibrary::BeginRecordFullViewport(
	UObject* WorldContextObject,
	FString InGeneratedWebpPicturesPath,
	bool& bBegin)
{
	bBegin =false;

	if (!WorldContextObject)
	{
		return ;
	}
	if (!WorldContextObject->GetWorld())
	{
		return ;
	}

	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject);
	ULSWebpMangeSubSystem* LSWebpMangeSubSystem = GameInstance->GetSubsystem<ULSWebpMangeSubSystem>();

	FVector2D ViewportSize;

	if (FLSWebpCore::GetViewportSize(WorldContextObject, ViewportSize))
	{
		TSharedPtr<FLSWebpPictureInformation> WebpPictureInformation = MakeShareable(new FLSWebpPictureInformation(0, 0, ViewportSize.X - 1, ViewportSize.Y - 1));
		bBegin=LSWebpMangeSubSystem->BeginRecord(InGeneratedWebpPicturesPath, WebpPictureInformation);
	}

	


}

void ULSWebpBPLibrary::EndRecord(
	UObject* WorldContextObject,
	FLSWebpFinishGenerateWebp InFinishWebpBPDegelete)
{
	if (!WorldContextObject)
	{
		return;
	}
	if (!WorldContextObject->GetWorld())
	{
		return;
	}

	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject);
	ULSWebpMangeSubSystem* LSWebpMangeSubSystem = GameInstance->GetSubsystem<ULSWebpMangeSubSystem>();

	LSWebpMangeSubSystem->EndRecord(InFinishWebpBPDegelete);


}

void ULSWebpBPLibrary::LoadWebp
(UObject* WorldContextObject, 
FLSWebpLoadAndShowWebp InLoadAndShowEbpDegelete, 
FString InWebpFilePath)
{
	if (!WorldContextObject)
	{
		return;
	}
	if (!WorldContextObject->GetWorld())
	{
		return;
	}

	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject);
	ULSWebpDisplaySubSystem* LSWebpDisplaySubSystem = GameInstance->GetSubsystem<ULSWebpDisplaySubSystem>();

	LSWebpDisplaySubSystem->LoadWebp(InLoadAndShowEbpDegelete,InWebpFilePath);
}

void ULSWebpBPLibrary::ReleaseLoadedWebp(UObject* WorldContextObject)
{

	if (!WorldContextObject)
	{
		return;
	}
	if (!WorldContextObject->GetWorld())
	{
		return;
	}
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject);
	ULSWebpDisplaySubSystem* LSWebpDisplaySubSystem = GameInstance->GetSubsystem<ULSWebpDisplaySubSystem>();

	LSWebpDisplaySubSystem->ReleaseLoadedWebp();

}

