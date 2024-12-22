/**
 * @file OddsAPI.cpp
 * @brief Class to fetch data from the Odds API (using HTTP requests)
 * @version 0.1
 * @date 2024-12-05
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "OddsAPI.h"
#include <boost/beast.hpp>
#include <iostream>
#include <fstream>
#include <format>

namespace beast = boost::beast;
namespace asio = boost::asio;
namespace http = beast::http;
namespace ssl = asio::ssl;
using tcp = asio::ip::tcp;
using json = nlohmann::json;

constexpr char API_HOST[] = "api.the-odds-api.com";
constexpr char SPORT_API_ENDPOINT_TEMPLATE[] =
    "/v4/sports/{}/odds/?apiKey={}&regions=uk,us&markets=h2h";
constexpr char SPORTS_LIST_ENDPOINT[] = "/v4/sports/?apiKey={}";
constexpr int PORT = 443;  // https port

// Fetch json data from the API endpoint
// Returns an empty json object if the request fails
json OddsAPI::fetch(const std::string& endpoint) {
    try {
        // Resolve the host
        auto const results = resolver_.resolve(API_HOST, std::to_string(PORT));

        // Connect to the host
        ssl::stream<tcp::socket> stream(resolver_.get_executor(), sslContext_);
        asio::connect(stream.lowest_layer(), results);

        // Perform the SSL handshake
        stream.handshake(ssl::stream_base::client);

        // Set up the HTTP request
        http::request<http::string_body> req{http::verb::get, endpoint, 11};
        req.set(http::field::host, API_HOST);

        // Send the HTTP request
        http::write(stream, req);

        // Receive the HTTP response
        beast::flat_buffer buffer;
        http::response<http::string_body> res;
        http::read(stream, buffer, res);

        // Return the parsed JSON response
        if (res.result() != http::status::ok) return json();
        return json::parse(res.body());
    } catch (const std::exception& e) {
        return json();
    }
}

// Fetch odds for a specific sport
json OddsAPI::fetchOdds(const std::string& apiKey, const std::string& sport) {
    std::string endpoint =
        std::format(SPORT_API_ENDPOINT_TEMPLATE, sport, apiKey);
    return fetch(endpoint);
}

// Fetch the list of available sports
std::vector<std::string> OddsAPI::fetchSports(const std::string& apiKey) {
    std::string endpoint = std::format(SPORTS_LIST_ENDPOINT, apiKey);
    json jsonData = fetch(endpoint);
    std::vector<std::string> sports;
    for (const auto& sport : jsonData) sports.push_back(sport["key"]);
    return sports;
}