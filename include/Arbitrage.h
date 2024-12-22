/**
 * @file Arbitrage.h
 * @brief Encapsulates the logic for handling odds data, calculating potential
 * arbitrage opportunities, and managing the detection and logging of those
 * opportunities based on specific thresholds.
 * @version 0.1
 * @date 2024-12-05
 *
 * @copyright Copyright (c) 2024
 *
 */
#ifndef ARBITRAGE_H
#define ARBITRAGE_H

#include <string>
#include <unordered_map>
#include <nlohmann/json.hpp>

/**
 * @brief Holds the decimal odds for a specific outcome with a particular
 * bookmaker.
 *
 */
struct OddsData {
	std::string bookmaker;
    std::string outcome;
	std::string commenceTime;
    double odds;
};

/**
 * @brief ArbitrageManager handles the processing of odds data for a specific
 * sport identifying potential arbitrage opportunities based on a threshold.
 */

class ArbitrageManager {
public:
    /**
     * Constructor for ArbitrageManager.
     *
     * @param data JSON data containing betting odds.
     * @param sport The sport category (e.g., "soccer").
     * @param percentThreshold The percent increase of stake required for an
	 * arbitrage opportunity to be reported.
     */
    ArbitrageManager(
		const nlohmann::json& data,
		const std::string& sport,
		double percentThreshold = 0.0);

	/**
	 * @brief Computes arbitrage opportunities.
	 *
	 * Iterates through the odds map and uses the ArbitrageCalculator to
	 * detect arbitrage opportunities based on the
	 * percentage threshold.
	 */
    void findArbitrageOpportunities();

private:
	std::unordered_map<std::string,
					   std::unordered_map<std::string, OddsData>> oddsMap_;
	std::string sport_;
	double percentThreshold_;

	/**
	 * @brief Parses odds data into the odds map.
	 *
	 * The odds map is structured as follows:
	 * - Outer key: Match identifier (e.g., "Team A vs Team B").
	 * - Inner key: Outcome name (e.g., "Team A wins").
	 * - Value: OddsData structure containing bookmaker, outcome name,
	 *   commencement time, and odds value.
	 *
	 * @param data The JSON data containing match and odds information.
	 */
    void parseOddsData(const nlohmann::json& data);
};

/**
 * @brief ArbitrageCalculator provides methods to detect and log arbitrage
 * opportunities for specific matches and betting odds
 *
 */
class ArbitrageCalculator {
public:
    /**
     * Detects arbitrage opportunities for a given match and odds.
     *
     * @param match The name of the match (e.g., "Team A vs Team B").
     * @param odds The betting odds for the match from various bookmakers.
     * @param threshold The stake percentage threshold for detecting arbitrage.
     * @param sport The sport category (e.g., "soccer").
     */
	void detectOpportunities(
		const std::string& match,
    	const std::unordered_map<std::string, OddsData>& odds,
    	double threshold,
    	const std::string& sport);

private:
	/**
     * Logs arbitrage opportunities in a JSON file
     *
     * @param match The name of the match (e.g., "Team A vs Team B").
     * @param odds The betting odds for the match from various bookmakers.
     * @param impliedProbability The implied probability of the odds for the
	 * match.
     * @param sport The sport category (e.g., "soccer").
     */
	void logOpportunity(
		const std::string& match,
		const std::unordered_map<std::string, OddsData>& odds,
		double impliedProbability,
		const std::string& sport);
};

#endif