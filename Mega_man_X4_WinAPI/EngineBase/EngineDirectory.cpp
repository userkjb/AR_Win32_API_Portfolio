#include "EngineDirectory.h"

UEngineDirectory::UEngineDirectory()
{
}

UEngineDirectory::~UEngineDirectory()
{
}

std::list<UEngineFile> UEngineDirectory::AllFile(std::vector<std::string> _Ext, bool _Recursive)
{
	return std::list<UEngineFile>();
}


void UEngineDirectory::AllFileRecursive(const std::string_view _Path, std::list<UEngineFile>& _Result, std::vector<std::string> _Ext, bool _Recursive)
{

}
