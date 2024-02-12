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
/// ���ڷ� ���� �̸��� �̹����� ã�Ƽ� �����Ѵ�.
/// UEngineResourcesManager���� �̹����� ã�� �����´�.
/// </summary>
/// <param name="_Name">�̹��� �̸�</param>
/// <param name="_InfoIndex">���� ����</param>
void UImageRenderer::SetImage(std::string_view _Name, int _InfoIndex)
{
	Image = UEngineResourcesManager::GetInst().FindImg(_Name);
}

void UImageRenderer::Render(float _DeltaTime)
{
	if (nullptr == Image)
	{
		MsgBoxAssert("�̹����� �������� �ʴ� Renderer �Դϴ�.");
	}

	// ���� �߿�!
	FTransform RendererTrans = GetTransform();

	// Actor
	FTransform ActorTrans = GetOwner()->GetTransform();

	// Component�� ��ġ�� �θ��� Actor�� ������̴�.
	// �θ� Actor�� ��ġ�� ������� �Ѵ�.
	RendererTrans.AddPosition(ActorTrans.GetPosition());

	// TransColor ����
	// Ư�� ����� 1��Ʈ�� ���̰� ���� �ʴ� ������ ������� �ʴ´�.
	// TransCopy������.
	// PNG�� ��쿡��.
	EWindowImageType ImageType = Image->GetImageType();

	switch (ImageType)
	{
	case EWindowImageType::IMG_NONE:
	{
		MsgBoxAssert("�̹��� Ÿ���� NONE �Դϴ�.");
		break;
	}
	case EWindowImageType::IMG_BMP: // BMP �� ��� �Ϲ������� TransCopy�� ���� ó���� �Ѵ�.
		GEngine->MainWindow.GetWindowImage()->TransCopy(Image, RendererTrans, InfoIndex, TransColor);
		break;
	case EWindowImageType::IMG_PNG:
		GEngine->MainWindow.GetWindowImage()->AlphaCopy(Image, RendererTrans, InfoIndex, TransColor);
		break;
	default:
		MsgBoxAssert("Ư�� �� �� ���� ó���� �Ұ����� �̹��� �Դϴ�.");
		break;
	}
}

void UImageRenderer::BeginPlay()
{
	USceneComponent::BeginPlay();
}
