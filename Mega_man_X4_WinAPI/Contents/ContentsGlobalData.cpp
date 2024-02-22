#include "ContentsGlobalData.h"

UWindowImage* UContentsGlobalData::ColMapImage = nullptr;

UContentsGlobalData::UContentsGlobalData()
{

}

UContentsGlobalData::~UContentsGlobalData()
{
}

FVector UContentsGlobalData::GetMousePos()
{
	return GEngine->MainWindow.GetMousePosition();
}
