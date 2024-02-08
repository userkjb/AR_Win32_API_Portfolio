#include "ImageRenderer.h"
#include "EngineResourcesManager.h"

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

}
