/**
 * @file ArbitrageCalculator.cpp
 * @brief Class to compute and log arbitrage opportunities.
 * @version 0.1
 * @date 2024-12-05
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "Arbitrage.h"
#include <fstream>
#include <format>

// Detects arbitrage opportunities for given match based on provided odds.
// If profit percentage exceeds threshold, opportunity is logged.
void ArbitrageCalculator::detectOpportunities(
    const std::string& match,
    const std::unordered_map<std::string, OddsData>& odds,
    double threshold,
    const std::string& sport)
{
    // Calculate implied probability by summing reciprocals of odds.
    double impliedProbability = 0;
    for (const auto& [outcomeName, oddsData] : odds)
        impliedProbability += 1.0 / oddsData.odds;

    double profitPercentage = (1 - impliedProbability) * 100;
    if (profitPercentage > threshold)
        logOpportunity(match, odds, impliedProbability, sport);
}

// Logs details of an arbitrage opportunity to a file for given sport.
// Includes match information, profit percentage, and optimal bank allocations.
void ArbitrageCalculator::logOpportunity(
    const std::string& match,
    const std::unordered_map<std::string, OddsData>& odds,
    double impliedProbability,
    const std::string& sport)
{

    // Lambda function to round a double to n decimal places
    auto roundToNDecimals = [](double value, int n) {
        double factor = std::pow(10.0, n);
        return std::round(value * factor) / factor;
    };

	// Define the directory and JSON file path
    std::string directory = "arbitrage-opportunities";
    std::string filePath = std::format("{}/{}.json", directory, sport);

    // Initialize JSON data structure
    nlohmann::json jsonData;

    // Calculate profit percentage
    double firstOutcomeOdds = odds.begin()->second.odds;
    double bet = 1.0 / firstOutcomeOdds / impliedProbability * 100;
    double revenue = bet * firstOutcomeOdds;
    double profitPercentage =
        roundToNDecimals(revenue - 100, 2);

    // Prepare JSON object for this opportunity
    nlohmann::json opportunity = {
        {"match", match},
        {"profit_percentage", profitPercentage},
        {"odds", nlohmann::json::array()}
    };

    // Add odds details
    for (const auto& [outcomeName, oddsData] : odds) {
		double optimalBetSize =
            roundToNDecimals((1.0 / oddsData.odds) / impliedProbability, 6);
        opportunity["odds"].push_back({
            {"bookmaker", oddsData.bookmaker},
            {"outcome", oddsData.outcome},
            {"odds", oddsData.odds},
			{"optimal_bank_allocation", optimalBetSize}
        });
		opportunity["commence_time"] = oddsData.commenceTime;
    }

    // Append the new opportunity to the JSON data
    jsonData["arbitrage_opportunities"].push_back(opportunity);

    // Write complete JSON data to the file
    std::ofstream outputFile(filePath);
    outputFile << jsonData.dump(4);
    outputFile.close();
}