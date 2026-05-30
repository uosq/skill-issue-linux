#include "../../../thirdparty/sol3/sol.hpp"

#include "../../../sdk/interfaces/interfaces.h"

#include <filesystem>
#include <iostream>
#include <fstream>

std::optional<std::filesystem::path> resolve_safe_path(const std::filesystem::path& root, const std::string& user_path)
{
	try
	{
		std::filesystem::path absolute_target = std::filesystem::weakly_canonical(root / user_path);
		std::filesystem::path relative = std::filesystem::relative(absolute_target, root);

		if (relative.empty() || (relative.string().find("..") == std::string::npos && *relative.begin() != ".."))
			return absolute_target;
	}
	// i fucking hate exceptions
	catch (...)
	{}

	return std::nullopt; // shit gone wild
}

void BindFS(sol::state& lua)
{
	const char* game_root = interfaces::Engine->GetGameDirectory();
	std::filesystem::path absolute_root = std::filesystem::canonical(game_root);

	auto fs_table = lua.create_table("fs");

	fs_table["mkdir"] = [absolute_root](std::string user_path) -> std::tuple<bool, std::string>
	{
		auto safe_path = resolve_safe_path(absolute_root, user_path);
		if (!safe_path) return std::make_tuple(false, "Not a safe path. Stay inside TF2's folder!");

		try
		{
			bool created = std::filesystem::create_directories(*safe_path);

			if (created)
				return std::make_tuple(true, "Directory created");
			else
				return std::make_tuple(false, "Directory not created");
		}
		catch (const std::exception& e)
		{
			return std::make_tuple(false, e.what());
		}

		return std::make_tuple(false, "Oh shit");
	};

	fs_table["write"] = [absolute_root](std::string user_path, std::string content) -> std::tuple<bool, std::string>
	{
		auto safe_path = resolve_safe_path(absolute_root, user_path);
		if (!safe_path) return std::make_tuple(false, "Not a safe path. Stay inside TF2's folder!");

		std::ofstream file(*safe_path, std::ios::out | std::ios::trunc);
		if (!file.is_open()) return std::make_tuple(false, "Failed to open file for writing");

		file << content;
		return std::make_tuple(true, "Success writing");
	};

	fs_table["append"] = [absolute_root](std::string user_path, std::string content) -> std::tuple<bool, std::string>
	{
		auto safe_path = resolve_safe_path(absolute_root, user_path);
		if (!safe_path) return std::make_tuple(false, "Not a safe path. Stay inside TF2's folder!");

		std::ofstream file(*safe_path, std::ios::out | std::ios::app);
		if (!file.is_open()) return std::make_tuple(false, "Failed to open file for appending.");

		file << content;
		return std::make_tuple(true, "Success appending");
	};

	fs_table["read"] = [absolute_root](std::string user_path) -> std::tuple<bool, std::string>
	{
		auto safe_path = resolve_safe_path(absolute_root, user_path);
		if (!safe_path) return std::make_tuple(false, "Not a safe path. Stay inside TF2's folder!");

		if (!std::filesystem::exists(*safe_path) || !std::filesystem::is_regular_file(*safe_path))
			return std::make_tuple(false, "File does not exist");

		std::ifstream file(*safe_path);

		// 50 * 1024 * 1024 bytes = 50 MB
		const uintmax_t MAX_BYTES = 50 * 1024 * 1024;

		if (std::filesystem::file_size(*safe_path) > MAX_BYTES)
			return std::make_tuple(false, "File exceeds 50 MB");

		std::stringstream buffer;
		buffer << file.rdbuf();
		return std::make_tuple(true, buffer.str());
	};
}