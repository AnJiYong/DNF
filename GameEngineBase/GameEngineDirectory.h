#pragma once
#include "GameEnginePath.h"

class GameEngineFile;
class GameEngineDirectory : public GameEnginePath
{
public:
	GameEngineDirectory();
	GameEngineDirectory(const std::string& Path);
	~GameEngineDirectory();

public:
	GameEngineDirectory(const GameEngineDirectory& _other);
	GameEngineDirectory(GameEngineDirectory&& _other) noexcept;

public:
	GameEngineDirectory& operator=(const GameEngineDirectory& _other) = delete;
	GameEngineDirectory& operator=(const GameEngineDirectory&& _other) = delete;

public:
	std::string DirectoryName();
	
	bool IsRoot();
	void MoveParent();
	bool MoveParent(const std::string& _DirName);
	bool MoveChild(const std::string& _DirName);

	GameEngineDirectory& operator/(const std::string& _DirName)
	{
		MoveChild(_DirName);
		return *this;
	}

	std::string PathToPlusFileName(const std::string& _FileName);
	std::vector<GameEngineFile> GetAllFile(const std::string& _filter = "*");
	std::vector<GameEngineDirectory> GetAllDirectory(const std::string& _filter = "");
	std::vector<GameEngineDirectory> GetAllDirectoryRecursive(const std::string& _filter = "");
};

