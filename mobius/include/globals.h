#pragma once

#include <string>
#include <atomic>

extern std::atomic<bool> decrypt_unlocked;
extern std::atomic<bool> door_open;
const std::string door_id = "1457802";
const std::string password = "30049274";
