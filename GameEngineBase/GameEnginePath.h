#pragma once

class GameEnginePath
{
public:		
	GameEnginePath();
	GameEnginePath(std::filesystem::path _path);
	~GameEnginePath();

public:
	GameEnginePath(const GameEnginePath& _other);
	GameEnginePath(GameEnginePath&& _other) noexcept;

public:
	GameEnginePath& operator=(const GameEnginePath& _other) = delete;
	GameEnginePath& operator=(const GameEnginePath&& _other) = delete;

public:
	bool IsExist();
	static std::string GetFileName(std::string _Path);
	std::string GetFileName();
	std::string GetFullPath();
	std::string GetFileNameWithOutExtension();
	std::string GetExtension();

protected:
	std::filesystem::path path_;
};

