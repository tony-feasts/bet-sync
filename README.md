# Low-Latency Arbitrage Detection Engine for Sports Betting

## Table of Contents

-   [File Documentation](#file-documentation)
-   [Project Overview](#project-overview)
-   [Design and Implementation](#design-and-implementation)
    -   [Architecture](#architecture)
    -   [Data Flow](#data-flow)
    -   [Multithreading](#multithreading)
    -   [Modern C++ Features](#modern-c-features)
-   [Data Description](#data-description)
-   [Tutorial](#tutorial)
    -   [Prerequisites](#prerequisites)
    -   [Installation](#installation)
    -   [Configuration](#configuration)
    -   [Running the Application](#running-the-application)
-   [Testing and Measurement](#testing-and-measurement)
    -   [Performance Metrics](#performance-metrics)
    -   [Results](#results)
-   [Conclusion](#conclusion)
-   [References](#references)

---

## File Documentation

This project is set up to automatically create HTML and PDF documentation using Doxygen, a tool for generating documentation directly from the source code.

You can find the generated HTML file in `html/index.html`.

Alternatively, you can generate the documentation directly via Doxygen via `doxygen Doxyfile`. See the official Doxygen website for installation.

---

## Project Overview

This project is a high-performance, low-latency arbitrage detection engine tailored for sports betting. Arbitrage opportunities arise when discrepancies between odds offered by different bookmakers allow for guaranteed profit by placing bets on all possible outcomes. The engine ingests live data from multiple betting platforms, standardizes the data format, and uses multithreaded processing to detect and notify users of potential arbitrage opportunities in real-time.

### Example of an Arbitrage Opportunity

Consider a hypothetical soccer match between Team A and Team B:

-   **Platform X** offers odds of **2.10** for Team A to win.
-   **Platform Y** offers odds of **2.20** for Team B to win.

By placing bets of \$100 on each outcome:

-   If **Team A wins**, the payout from Platform X is \$210.
-   If **Team B wins**, the payout from Platform Y is \$220.

Regardless of the outcome, the bettor spends \$200 and receives at least \$210, ensuring a **risk-free profit**.

---

## Design and Implementation

### Architecture

The application follows a modular design, separating concerns into distinct components:

-   **Data Service** (`OddsAPI`): Handles fetching and parsing of odds data from external APIs.
-   **Configuration Manager** (`AppConfig`): Manages application configuration, including environment variables.
-   **Arbitrage Engine** (`ArbitrageManager` and `ArbitrageCalculator`): Processes odds data to detect arbitrage opportunities.
-   **Main Application** (`main.cpp`): Orchestrates the workflow, including multithreading and performance measurement.

### Data Flow

1. **Configuration Loading**: The application reads the API key and optional thread count (for multithreading) from environment variables using `AppConfig`.
2. **Data Fetching**: `OddsAPI` fetches the list of available sports and odds data for each sport.
3. **Data Processing**: `ArbitrageManager` parses the odds data and stores it in a structured format.
4. **Arbitrage Detection**: `ArbitrageCalculator` computes potential arbitrage opportunities based on the odds.
5. **Result Logging**: Detected arbitrage opportunities are logged into separate files for each sport in the `arbitrage-opportunities` directory.
6. **Performance Measurement**: Execution times for single-threaded and multithreaded runs are measured and displayed.

### Multithreading

The application leverages multithreading to improve performance:

-   **Thread Pool**: Uses `boost::asio::thread_pool` to manage a pool of threads based on the thread count.
-   **Task Distribution**: Fetching and processing of odds data for each sport are distributed across threads.

### Modern C++ Features

The application utilizes several modern C++ (C++17 and above) features to enhance performance and code readability:

-   **Structured Bindings**: Simplify iteration over maps and unpacking of tuples.
    -   _Usage_: In `ArbitrageManager.cpp` and `ArbitrageCalculator.cpp` when iterating over containers.
-   **`std::optional`**: Handle optional values without using sentinel values.
    -   _Usage_: In `AppConfig.cpp` for optional thread count configuration.
-   **`std::filesystem`**: Perform file system operations in a portable way.
    -   _Usage_: In `main.cpp` for clearing old arbitrage opportunity files.
-   **`std::format`**: Provide safe and convenient string formatting.
    -   _Usage_: In `ArbitrageManager.cpp` for constructing match names and file paths.

---

## Data Description

The application interacts with The Odds API to fetch real-time sports betting odds. The data includes:

-   **Sports List**: A list of available sports with unique identifiers.
-   **Odds Data**: For each sport, the odds data includes:
    -   **Match Information**: Home and away teams.
    -   **Bookmakers**: A list of bookmakers offering odds for the match.
    -   **Markets**: Betting markets (e.g., head-to-head).
    -   **Outcomes**: Possible outcomes with associated odds.

The odds data is parsed and structured into:

-   **Odds Map**: A nested `std::unordered_map` storing the best odds for each outcome of each match.

---

## Tutorial

### Prerequisites

-   **Operating System**: macOS (adjust paths and package manager for other OS)
-   **C++ Compiler**: Supports C++23
-   **Package Manager**: Homebrew (for dependency installation)
-   **Environment Variables**:
    -   `API_KEY`: Your API key from The Odds API.
    -   `THREAD_COUNT` (optional): Number of threads for multithreading (defaults to number of cpu cores).

### Installation

1. **Clone the Repository**:

    ```bash
    git clone https://github.com/tony-feasts/bet-sync
    cd VantEdge
    ```

2. **Install Dependencies**:

    ```bash
    brew install boost
    brew install openssl
    ```

### Configuration

1. **Set Environment Variables**:

    ```bash
    export API_KEY="YOUR_API_KEY"
    export THREAD_COUNT="YOUR_THREAD_COUNT"  # Optional
    ```

    Replace `YOUR_API_KEY` with your actual API key from [The Odds API](https://the-odds-api.com/).

2. **Modify Makefile Paths (if necessary)**:

    Ensure that the `CXXFLAGS` and `LDFLAGS` in the `Makefile` point to the correct include and library paths for Boost and OpenSSL.

### Running the Application

1. **Compile the Project**:

    ```bash
    make
    ```

    This will generate the executable in the `bin` directory.

2. **Run the Application**:

    ```bash
    ./bin/arbitrage_engine
    ```

    Sample Output:

    ```
    Multi-threaded execution time: 0.341887s
    ```

3. **View Arbitrage Opportunities**:

    Check the `arbitrage-opportunities` directory for JSON files containing detailed information about detected arbitrage opportunities for each sport.

4. **Run Demo Python Script**

    Run the python script `demo.py` to show how the arbitrage opportunities can be used to generate money. This script will pull arbitrage data from the `arbitrage-opportunities/` directory and compute bet sizes using a bank of $1000. You can run the script using `python3 demo.py`.

---

## Testing and Measurement

### Performance Metrics

The application's performance is measured by comparing the execution times of single-threaded and multithreaded runs:

-   **Single-Threaded Execution**: Processes each sport sequentially.
-   **Multithreaded Execution**: Utilizes multiple threads to process sports concurrently.

### Results

Sample measurements with 100 threads:

```
Single-threaded execution time: 34.9127s
Multi-threaded execution time: 0.341887s
```

The multithreaded execution significantly reduces the processing time, demonstrating the effectiveness of parallel processing in improving performance.

### Example Arbitrage Opportunity

Below shows an example of an arbitrage opportunity pulled using our engine.

```
Found arbitrage: Niagara Purple Eagles vs LIU Sharks - 2024-12-01T17:00:00Z
Profit percentage: 6.36%
Optimal bet sizes:
   Niagara Purple Eagles: $556.85 (betmgm, 1.91)
   LIU Sharks: $443.16 (lowvig, 2.4)
Possible outcomes
   Case 1 (Niagara Purple Eagles): win = 556.85 * 1.91 = $1063.58, lose = $1000, net = $63.58
   Case 2 (LIU Sharks): win = 443.16 * 2.4 = $1063.58, lose = $1000, net = $63.58
```

In this example, we can see that using a bank size of $1,000, we can make a guaranteed profit of $63.58. We note:

-   We have a match between Niagara Purple Eagles vs LIU Sharks
-   The odds for Niagara Purple Eagles is at 1.91 (by bookmaker `betmgm`).
-   The odds for LIU Sharks is at 2.4 (by bookmaker `lowvig`).
-   We can place $556.85 on Niagara Purple Eagles and $443.16 on LIU Sharks.
-   We have two possible outcomes for the match:
    -   **Niagara Purple Eagles win:** we win $556.85 \* 1.91 - $1000 = $63.58
    -   **LIU Sharks win:** we win $443.16 \* 2.4 - $1000 = $63.58
-   As can be seen, regardless of the game outcome, we are guaranteed a profit of $63.58.

---

## Conclusion

This arbitrage detection engine efficiently identifies risk-free betting opportunities by leveraging modern C++ features and multithreading. The modular design allows for easy maintenance and scalability. By utilizing real-time data and optimizing performance, the application serves as a valuable tool for bettors seeking to capitalize on arbitrage opportunities.

---

## References

-   **The Odds API**: [https://the-odds-api.com/](https://the-odds-api.com/)
-   **Boost Libraries**: [https://www.boost.org/](https://www.boost.org/)
-   **Modern C++ Features**:
    -   C++17 Features: [https://en.cppreference.com/w/cpp/17](https://en.cppreference.com/w/cpp/17)
    -   C++20 Features: [https://en.cppreference.com/w/cpp/20](https://en.cppreference.com/w/cpp/20)
-   **OpenAI ChatGPT**: Documentation assistance provided by OpenAI's ChatGPT for refining design documentation and implementation descriptions.

---

**Note**: Ensure you comply with The Odds API's terms of service regarding request limits and data usage.
