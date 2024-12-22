/**
 * @file OddsAPI.h
 * @brief  Encapsulate the logic for fetching data from the Odds API
 * @version 0.1
 * @date 2024-12-05
 *
 * @copyright Copyright (c) 2024
 *
 */
#ifndef ODDSAPI_H
#define ODDSAPI_H

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <nlohmann/json.hpp>

/**
 * @brief The class provides methods for interacting with The Odds API to fetch
 * sports and betting odds data using HTTPS.
 *
 */
class OddsAPI {
public:
    /**
     * Constructs an OddsAPI instance.
     *
     * @param ioc A reference to a Boost ASIO io_context instance used for
     * asynchronous operations.
     * @param ctx A reference to a Boost ASIO SSL context used for managing
     * SSL/TLS settings.
     */
    OddsAPI(boost::asio::io_context& ioc, boost::asio::ssl::context& ctx)
        : resolver_(ioc), sslContext_(ctx) {}

    /**
     * Fetches betting odds for a specific sport.
     *
     * @param apiKey The API key for authenticating with The Odds API.
     * @param sport The identifier of the sport for which odds should be
     * retrieved.
     * @return A nlohmann::json object containing the odds data.
     *         Returns an empty JSON object if the request fails.
     */
    nlohmann::json fetchOdds(
        const std::string& apiKey,
        const std::string& sport);

    /**
     * Fetches a list of all available sports from The Odds API.
     *
     * @param apiKey The API key for authenticating with The Odds API.
     * @return A vector of strings containing the keys of all available sports.
     * Returns an empty vector if the request fails or if no sports are
     * available.
     */
	std::vector<std::string> fetchSports(const std::string& apiKey);

private:
    /**
     * Fetches JSON data from a specified API endpoint.
     *
     * @param endpoint The API endpoint to send the GET request to.
     * @return A nlohmann::json object containing the response data.
     *         Returns an empty JSON object if the request fails.
     */
    nlohmann::json fetch(const std::string& endpoint);

    boost::asio::ip::tcp::resolver resolver_; // translates hostnames to ips
	boost::asio::ssl::context& sslContext_; // manages secure connections
};

#endif