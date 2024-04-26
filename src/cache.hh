#pragma once

#include <filesystem>
#include <unordered_map>
#include <vector>

class Cache {
public:
  Cache();
  Cache(const std::filesystem::path &path, std::string newVarValue);
  std::string varValue;
  std::unordered_map<std::filesystem::path, time_t> dirs;
  std::vector<std::filesystem::path> files;

  bool envVarChanged(std::string newEnvVar);
  bool dirsModified();
  void invalidate();
};