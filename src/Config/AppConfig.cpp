/**
 * @file AppConfig.cpp
 * @brief Class to fetch environment variables defined by user
 * @version 0.1
 * @date 2024-12-05
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "AppConfig.h"

// Retrieves the API key from the "API_KEY" environment variable.
std::string AppConfig::getApiKey() {
	const char* apiKey = getenv("API_KEY");
	if (!apiKey) throw std::runtime_error("API_KEY not set");
	return std::string(apiKey);
}

// Retrieves the thread count from the "THREAD_COUNT" environment variable.
std::optional<int> AppConfig::getThreadCount() {
	const char* threadCount = getenv("THREAD_COUNT");
	if (!threadCount) return std::nullopt;
	return std::stoi(threadCount);
}