from flask import Flask
from datetime import datetime
app = Flask(__name__)
import requests
import json


def percentage_change(market_element):
    try:
        open_price = float(market_element['open'])
        current_buy_price = float(market_element['buy'])
        current_sell_price = float(market_element['sell'])
        percentage_change_buy = ((current_buy_price - open_price) / open_price) * 100
        percentage_change_sell = ((current_sell_price - open_price) / open_price) * 100
        return {
            "coin": market_element['baseMarket'],
            "marketType": market_element['quoteMarket'],
            "percentage_change_buy": percentage_change_buy,
            "percentage_change_sell": percentage_change_sell
        }
    except:
        print(market_element)


def analyse_market(markets):
    flag = False
    for market in markets:
        sorted_market = sorted([percentage_change(element) for element in market],key=lambda x: x['percentage_change_buy'],reverse=True)
        # total_change = 0
        for ele in sorted_market:
            if(ele['percentage_change_buy'] >= 50):
                return "1"
            # total_change += ele['percentage_change_buy'] 
    return "0"
def parse_market_data(data):
    inr_market = [x for x in data if x['quoteMarket'] == 'inr' and x['status'] == 'active' and x['baseMarket'] != 'usdt']
    usdt_market = [x for x in data if x['quoteMarket'] == 'usdt' and x['status'] == 'active' and x['type'] != 'STF']
    return analyse_market([inr_market,usdt_market])

@app.route('/')
def homepage():
    try:
        response = requests.get('https://api.wazirx.com/api/v2/market-status')
        if response.status_code == 200:
            data = response.json()
            return(parse_market_data(data['markets']))
        return "0"
    except:
        return "0"

if __name__ == '__main__':
    app.run(debug=True, use_reloader=True)