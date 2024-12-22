/**
 * @file main.cpp
 * @brief Entry point of the program.
 * @version 0.1
 * @date 2024-12-05
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <iostream>
#include <chrono>
#include "OddsAPI.h"
#include "Arbitrage.h"
#include "AppConfig.h"
#include <boost/asio/thread_pool.hpp>
#include <boost/asio/post.hpp>
#include <filesystem>
#include <thread>

namespace fs = std::filesystem;

/**
 * @brief Entry point of the program.
 * - Reads API key and core count from environment variables.
 * - Fetches sports and odds data.
 * - Computes arbitrage opportunities both single-threaded and multi-threaded.
 * - Reports execution times for comparison.
 */
int main() {
	// apiKey is read from environment variable, exception is thrown if not set
	std::string apiKey = AppConfig::getApiKey();

	// Ready oddsAPI object to fetch sports and odds
    boost::asio::io_context ioc;
	boost::asio::ssl::context ctx(boost::asio::ssl::context::tlsv12_client);
	OddsAPI oddsAPI(ioc, ctx);

	// Fetch all sport names
	std::vector<std::string> sports = oddsAPI.fetchSports(apiKey);

	// threadCount is read from environment variable
	// defaults to number of sports.
	std::optional<int> tc = AppConfig::getThreadCount();
	int threadCount = tc.value_or(std::thread::hardware_concurrency());

	// Clear old arbitrage opportunities
	for (const auto& f : fs::directory_iterator("arbitrage-opportunities"))
		if (f.path().filename() != ".gitkeep") fs::remove(f.path());

	// For all sports, call API and compute arbitrages
	// Single-threaded execution
	// auto singleThreadedStart = std::chrono::high_resolution_clock::now();
	// for (const auto& sport : sports) {
	// 	// Fetch odds
	// 	auto oddsData = oddsAPI.fetchOdds(apiKey, sport);
	// 	if (oddsData.is_null()) continue;
	// 	// Compute Arbitrage Opportunities
	// 	ArbitrageManager ArbitrageManager(oddsData, sport);
	// 	ArbitrageManager.findArbitrageOpportunities();
	// }
	// auto singleThreadedEnd = std::chrono::high_resolution_clock::now();
	// auto elapsed =
	// 	std::chrono::duration<double>(singleThreadedEnd - singleThreadedStart);
	// std::cout << "Single-threaded execution time: "
	// 		  << elapsed.count() << "s\n";

	// Multithreaded execution
	boost::asio::thread_pool pool(threadCount);
	auto multiThreadedStart = std::chrono::high_resolution_clock::now();
	for (const auto& sport : sports) {
		// Post a task to the thread pool
		boost::asio::post(pool, [sport, apiKey, &oddsAPI]() {
			auto oddsData = oddsAPI.fetchOdds(apiKey, sport);
			if (oddsData.is_null()) return;

			ArbitrageManager ArbitrageManager(oddsData, sport);
			ArbitrageManager.findArbitrageOpportunities();
		});
	}
	pool.join();
	auto multiThreadedEnd = std::chrono::high_resolution_clock::now();
	auto multiThreadedElapsed =
		std::chrono::duration<double>(multiThreadedEnd - multiThreadedStart);
	std::cout << "Multi-threaded execution time: "
	<< multiThreadedElapsed.count() << "s\n";

}