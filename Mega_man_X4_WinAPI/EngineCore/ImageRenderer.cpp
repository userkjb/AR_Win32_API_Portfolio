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

void UImageRenderer::SetOrder(int _Order)
{
	// ���� Actor ����
	AActor* Owner = GetOwner();
	// Actor �� �ִ� Level ����.
	ULevel* Level = Owner->GetWorld();

	std::map<int, std::list<UImageRenderer*>>& Renderers = Level->Renderers;

	// ���� ����(Actor)�� �ٲ�� ���� Ȥ�ó� ���� �ٸ� ���� ���ߴٸ�
	// �� ���� ���� ���´�.
	Renderers[GetOrder()].remove(this);

	// ���� ����(Actor)�� �ٲ��.
	UTickObject::SetOrder(_Order);

	Renderers[GetOrder()].push_back(this);

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
		GEngine->MainWindow.GetBackBufferImage()->TransCopy(Image, RendererTrans, InfoIndex, TransColor);
		break;
	case EWindowImageType::IMG_PNG:
		GEngine->MainWindow.GetBackBufferImage()->AlphaCopy(Image, RendererTrans, InfoIndex, TransColor);
		break;
	default:
		MsgBoxAssert("Ư�� �� �� ���� ó���� �Ұ����� �̹��� �Դϴ�.");
		break;
	}
}

void UImageRenderer::CreateAnimation(
	std::string_view _AnimationName, 
	std::string_view _ImageName, 
	int _Start, 
	int _End, 
	float _Inter, 
	bool _Loop)
{
}

void UImageRenderer::CreateAnimation(
	std::string_view _AnimationName,
	std::string_view _ImageName,
	std::vector<int> _Indexs, 
	float _Inter,
	bool _Loop)
{
}

void UImageRenderer::ChangeAnimation(std::string_view _AnimationName, bool _IsForce, int _StartIndex, float _Time)
{
}

void UImageRenderer::AnimationReset()
{
}

void UImageRenderer::BeginPlay()
{
	USceneComponent::BeginPlay();
}


// ===== AnimationInfo Class =====

int UAnimationInfo::Update(float _DeltaTime)
{
	IsEnd = false;
	CurTime -= _DeltaTime;

	if (0.0f >= CurTime)
	{
		CurTime = Times[CurFrame];
		++CurFrame;
	}

	//  6                 6
	if (Indexs.size() <= CurFrame)
	{
		IsEnd = true;
		if (true == Loop)
		{
			// //            0  1  2  3  4  5 
			//    Indexs => 20 21 22 23 24 25
			CurFrame = 0;
		}
		else
		{
			//                               
			//               0  1  2  3  4  5 
			//    Indexs => 20 21 22 23 24 25
			--CurFrame;
		}
	}

	int Index = Indexs[CurFrame];

	return Index;
}
