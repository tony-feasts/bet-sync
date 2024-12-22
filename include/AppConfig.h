/**
 * @file AppConfig.h
 * @brief Provides a simple interface to retrieve important configuration
 * values (e.g. API key, thread count)
 * @version 0.1
 * @date 2024-12-05
 *
 * @copyright Copyright (c) 2024
 *
 */
#ifndef APPCONFIG_H
#define APPCONFIG_H

#include <string>
#include <optional>

/**
 * @brief retrieve configuration values (e.g. API key, thread count) from the
 * environment variables.
 */

class AppConfig {
public:
    /**
     * Retrieves the API key from the "API_KEY" environment variable.
     *
     * @throws std::runtime_error If "API_KEY" environment variable is not set.
     * @return A string containing the API key.
     */
	static std::string getApiKey();

	/**
     * Retrieves the thread count from the "THREAD_COUNT" environment variable.
     *
     * @return An optional integer containing the thread count. If the variable
     * is not set, returns std::nullopt.
     */
	static std::optional<int> getThreadCount();
};

#endif