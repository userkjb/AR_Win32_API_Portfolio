#include "ImageRenderer.h"
#include "EngineResourcesManager.h"
#include <EngineBase/EngineDebug.h>
#include "Actor.h"
#include "EngineCore.h"

UImageRenderer::UImageRenderer()
{
}

UImageRenderer::~UImageRenderer()
{
}

/// <summary>
/// 인자로 받은 이름의 이미지를 찾아서 설정한다.
/// UEngineResourcesManager에서 이미지를 찾아 가져온다.
/// </summary>
/// <param name="_Name">이미지 이름</param>
/// <param name="_InfoIndex">렌더 순서</param>
void UImageRenderer::SetImage(std::string_view _Name, int _InfoIndex)
{
	Image = UEngineResourcesManager::GetInst().FindImg(_Name);
}

void UImageRenderer::Render(float _DeltaTime)
{
	if (nullptr == Image)
	{
		MsgBoxAssert("이미지가 존재하지 않는 Renderer 입니다.");
	}

	// 순서 중요!
	FTransform RendererTrans = GetTransform();

	// Actor
	FTransform ActorTrans = GetOwner()->GetTransform();

	// Component의 위치는 부모인 Actor에 상대적이다.
	// 부모 Actor의 위치를 더해줘야 한다.
	RendererTrans.AddPosition(ActorTrans.GetPosition());

	// TransColor 원리
	// 특정 색상과 1비트도 차이가 나지 않는 색상은 출력하지 않는다.
	// TransCopy에서만.
	// PNG일 경우에만.
	EWindowImageType ImageType = Image->GetImageType();

	switch (ImageType)
	{
	case EWindowImageType::IMG_NONE:
	{
		MsgBoxAssert("이미지 타입이 NONE 입니다.");
		break;
	}
	case EWindowImageType::IMG_BMP: // BMP 인 경우 일반적으로 TransCopy로 투명 처리를 한다.
		GEngine->MainWindow.GetWindowImage()->TransCopy(Image, RendererTrans, InfoIndex, TransColor);
		break;
	case EWindowImageType::IMG_PNG:
		GEngine->MainWindow.GetWindowImage()->AlphaCopy(Image, RendererTrans, InfoIndex, TransColor);
		break;
	default:
		MsgBoxAssert("특정 색 및 투명 처리가 불가능한 이미지 입니다.");
		break;
	}
}

void UImageRenderer::BeginPlay()
{
	USceneComponent::BeginPlay();
}
