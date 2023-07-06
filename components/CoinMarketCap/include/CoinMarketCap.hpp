/*
MIT License

Copyright (c) 2022 Sukesh Ashok Kumar

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

/*
{
  "status": {
    "timestamp": "2023-03-08T12:30:10.540Z",
    "error_code": 0,
    "error_message": null,
    "elapsed": 396,
    "credit_count": 1,
    "notice": null
  },
  "data": {
    "BTC": {
      "id": 1,
      "name": "Bitcoin",
      "symbol": "BTC",
      "slug": "bitcoin",
      "num_market_pairs": 10082,
      "date_added": "2013-04-28T00:00:00.000Z",
      "tags": [
        "mineable",
        "pow",
        "sha-256",
        "store-of-value",
        "state-channel",
        "coinbase-ventures-portfolio",
        "three-arrows-capital-portfolio",
        "polychain-capital-portfolio",
        "binance-labs-portfolio",
        "blockchain-capital-portfolio",
        "boostvc-portfolio",
        "cms-holdings-portfolio",
        "dcg-portfolio",
        "dragonfly-capital-portfolio",
        "electric-capital-portfolio",
        "fabric-ventures-portfolio",
        "framework-ventures-portfolio",
        "galaxy-digital-portfolio",
        "huobi-capital-portfolio",
        "alameda-research-portfolio",
        "a16z-portfolio",
        "1confirmation-portfolio",
        "winklevoss-capital-portfolio",
        "usv-portfolio",
        "placeholder-ventures-portfolio",
        "pantera-capital-portfolio",
        "multicoin-capital-portfolio",
        "paradigm-portfolio",
        "btc-ecosystem"
      ],
      "max_supply": 21000000,
      "circulating_supply": 19311768,
      "total_supply": 19311768,
      "is_active": 1,
      "platform": null,
      "cmc_rank": 1,
      "is_fiat": 0,
      "self_reported_circulating_supply": null,
      "self_reported_market_cap": null,
      "tvl_ratio": null,
      "last_updated": "2023-03-08T12:28:00.000Z",
      "quote": {
        "USD": {
          "price": 22073.65691595661,
          "volume_24h": 24728043114.32274,
          "volume_change_24h": 52.4852,
          "percent_change_1h": -0.12181079,
          "percent_change_24h": -1.39865092,
          "percent_change_7d": -7.10944942,
          "percent_change_30d": -3.42964715,
          "percent_change_60d": 30.46260299,
          "percent_change_90d": 31.02144248,
          "market_cap": 426281341272.54956,
          "market_cap_dominance": 42.1574,
          "fully_diluted_market_cap": 463546795235.09,
          "tvl": null,
          "last_updated": "2023-03-08T12:28:00.000Z"
        }
      }
    }
  }
}
*/

#ifndef TUX_CMC_H_
#define TUX_CMC_H_

#include <iostream>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <inttypes.h>
#include <sys/param.h>

#include "esp_log.h"
#include <fstream>

#include "esp_http_client.h"
#include <cJSON.h>
using namespace std;
#include "SettingsConfig.hpp"

class CoinMarketCap
{
    public:
        string Name;       // Bitcoin
        string Symbol;     // BTC
        string Quote;      // USD
        float Price;
        float Change1m;
        float Change24h;
        float Change7d;
        double Amount;
        double AmountValue;
        double ChartValues[16];

        /* Constructor */
        CoinMarketCap();

        /* HTTPS request to the Coin API */
        void request_coin_update();

        /* Handle json response */

    private:
        SettingsConfig *cfg;
        string cfg_filename;  /* Settings config filename*/

        string cmc_file_name; /* Coin cache filename */
        string cg_file_name; /* Coin cache filename */
        string btc_file_name; /* Coin cache filename */
        string jsonString;

        cJSON *root;
        cJSON *errorinfo;
        cJSON *datainfo;
        cJSON *balanceinfo;
        cJSON *pricesinfo;
        cJSON *coininfo;
        cJSON *quoteinfo;
        cJSON *quotecoininfo;

        /* Load data from cache file to the instance */
        void load_cmc_json();
        void load_cg_json();
        void load_btc_json();

        /* Read cache json from flash/sdcard */
        void read_json(string crypto_name);

        /* Write cache json on flash/sdcard */
        void write_json(string crypto_name);

        esp_err_t request_json_cg();
        esp_err_t request_json_cmc();
        esp_err_t request_json_btc();

        esp_err_t request_https_json(string coinUrl);

    protected:

};

#endif
