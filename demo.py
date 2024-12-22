import json
import os

# This is a demo Python script that tries to make a profit from the detected
# arbitrages!

# Assume bank size of $1000
bank_size = 1000
arbitrages = []
directory = "arbitrage-opportunities/"

# 1. Extract arbitrages from "arbitrage-opportunities/" directory
try:
	for filename in os.listdir(directory):
		if filename.endswith(".json"):  # Process only JSON files
			file_path = os.path.join(directory, filename)
			with open(file_path, 'r') as file:
				cur_arbitrages = json.load(file)
				if "arbitrage_opportunities" in cur_arbitrages:
					arbitrages.extend(
						cur_arbitrages["arbitrage_opportunities"])
except Exception as e:
	print(f"Unexpected exception: ", e)

print(f"Found {len(arbitrages)} arbitrages in total \n")

# sort arbitrages by profit percentage
arbitrages.sort(key = lambda x: -x['profit_percentage'])

for arbitrage in arbitrages:
	print(f"Found arbitrage: {arbitrage['match']} - "
	      f"{arbitrage['commence_time']}")
	print(f"Profit percentage: {arbitrage['profit_percentage']}%")

	# print optimal bet size
	print(f"Optimal bet sizes (assume bank size of ${bank_size}):")
	for odds in arbitrage["odds"]:
		print(f"   {odds['outcome']}: "
		      f"${round(odds['optimal_bank_allocation'] * bank_size, 2)} "
			  f"({odds['bookmaker']}, {odds['odds']})")

	# print cases for each
	print("Possible outcomes")
	for i, odds in enumerate(arbitrage["odds"]):
		bet_size = round(odds['optimal_bank_allocation'] * bank_size, 2)
		_odds = round(odds['odds'], 2)
		win_amount = round(bet_size * _odds, 2)
		net = round(win_amount - bank_size, 2)
		print(f"   Case {i+1} ({odds['outcome']}): win = {bet_size} * "
		      f"{_odds} = ${win_amount}, lose = ${bank_size}, net = ${net}")
	print("\n\n")