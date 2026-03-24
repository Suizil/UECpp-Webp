#pragma once
#include "CoreMinimal.h"
#include "LSWebpType.generated.h"




DECLARE_DYNAMIC_DELEGATE_OneParam(FLSWebpFinishGenerateWebp,bool,bFinishGenerate);
DECLARE_DYNAMIC_DELEGATE_FourParams(FLSWebpLoadAndShowWebp,bool,bLoad,UTexture2D*,OutWebpPicture,int32,WebpWidth,int32,WebpHeight);


USTRUCT(BlueprintType)
struct LSWEBP_API FLSWebpPictureInformation 
{
	GENERATED_USTRUCT_BODY()
public:
	FLSWebpPictureInformation();
	FLSWebpPictureInformation(int32 InX0,int32 InY0,int32 InX1,int32 InY1);
	FLSWebpPictureInformation(FLSWebpPictureInformation& InWebpPictureInformation);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LSWebp")
		int32 X0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LSWebp")
		int32 Y0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LSWebp")
		int32 X1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LSWebp")
		int32 Y1;

	int32 GetPictureWidth();
	int32 GetPictureHeight();

	void ResetPictureInformation();
};


UENUM()
enum class ELSWebpProcessType :uint8
{
	None,
	Recording,
	Generating,
	Max
};

UENUM()
enum class ELSWebpLoadAndShowType :uint8
{
	None,
	Loading,
	Showing,
	Max
};