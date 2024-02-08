#include "ImageRenderer.h"
#include "EngineResourcesManager.h"

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

}
