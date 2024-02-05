#pragma once
#include "EnginePath.h"

class UEngineFile : public UEnginePath
{
public :
	UEngineFile();
	UEngineFile(std::filesystem::path _Path);
	~UEngineFile();
};