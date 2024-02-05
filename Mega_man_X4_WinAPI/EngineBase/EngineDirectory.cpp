#include "EngineDirectory.h"
#include "EngineString.h"
#include "EngineFile.h"

UEngineDirectory::UEngineDirectory()
{
}

UEngineDirectory::UEngineDirectory(const UEnginePath& _Path)
	: UEnginePath(_Path.GetFullPath())
{
}

UEngineDirectory::~UEngineDirectory()
{
}

std::list<UEngineFile> UEngineDirectory::AllFile(std::vector<std::string> _Ext, bool _Recursive)
{
	std::list<UEngineFile> Result;
	
	for (size_t i = 0; i < _Ext.size(); i++)
	{
		_Ext[i] = UEngineString::ToUpper(_Ext[i]);
	}

	// Path : EnginePath -> std::filesystem::path Path.
	AllFileRecursive(Path.string(), Result, _Ext, _Recursive);

	return Result;
}


void UEngineDirectory::AllFileRecursive(const std::string_view _Path, std::list<UEngineFile>& _Result, std::vector<std::string> _Ext, bool _Recursive)
{

}
