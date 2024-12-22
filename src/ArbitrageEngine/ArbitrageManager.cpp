/**
 * @file ArbitrageManager.cpp
 * @brief Class to manage the parsing and computation of arbitrage
 * opportunities
 * @version 0.1
 * @date 2024-12-05
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "Arbitrage.h"
#include <format>

// Initializes sport and threshold, then parses odds data to populate oddsMap_
ArbitrageManager::ArbitrageManager(
	const nlohmann::json& data,
	const std::string& sport,
	double percentThreshold)
	: sport_(sport), percentThreshold_(percentThreshold)
{
    parseOddsData(data);
}

// Computes arbitrage opportunities from oddsMap_
void ArbitrageManager::findArbitrageOpportunities()
{
	ArbitrageCalculator calculator;
	for (const auto& [match, odds] : oddsMap_) {
        if (!odds.empty())
		calculator.detectOpportunities(
            match, odds, percentThreshold_, sport_);
    }
}

// Parses Odds data into oddsMap_
// oddsMap_ is a nested map: 
// - Outer key: match identifier (std::string)
// - Inner key: outcome name (std::string)
// - Value: OddsData structure
void ArbitrageManager::parseOddsData(const nlohmann::json& data) {
    for (const auto& game : data) {
        const std::string homeTeam = game.value("home_team", "");
        const std::string awayTeam = game.value("away_team", "");
		const std::string commenceTime = game.value("commence_time", "");
        const std::string match = std::format("{} vs {}", homeTeam, awayTeam);
        auto& matchOdds = oddsMap_[match];

        for (const auto& bookmaker : game["bookmakers"]) {
            const std::string bookmakerKey = bookmaker.value("key", "");
            for (const auto& market : bookmaker["markets"]) {
                if (market.value("key", "") != "h2h") continue;
                for (const auto& outcome : market["outcomes"]) {
                    const std::string outcomeName = outcome.value("name", "");
                    const double outcomeOdds = outcome.value("price", 0.0);
                    // Maximum odds create the best arbitrage opportunities
                    auto it = matchOdds.find(outcomeName);
                    if (it == matchOdds.end() || it->second.odds < outcomeOdds)
                        matchOdds[outcomeName] = OddsData{
                            bookmakerKey,
                            outcomeName,
							commenceTime,
                            outcomeOdds
                        };
                }
            }
        }
    }
}