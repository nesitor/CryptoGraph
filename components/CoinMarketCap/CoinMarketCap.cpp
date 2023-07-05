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

#include "CoinMarketCap.hpp"
#include "esp_tls.h"
#if CONFIG_MBEDTLS_CERTIFICATE_BUNDLE
#include "esp_crt_bundle.h"
#endif
#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"
#include "lwip/dns.h"

static const char* TAG = "CoinMarketCap";

// Can Test HTTPS using Local Python server - same SSL cert used for OTA
// extern const uint8_t local_server_cert_pem_start[] asm("_binary_ca_cert_pem_start");
// extern const uint8_t local_server_cert_pem_end[] asm("_binary_ca_cert_pem_end");

// Since HTTP call works for weather, we skip HTTPS for now
// extern const uint8_t owm_server_cert_pem_start[] asm("_binary_owm_cert_pem_start");
// extern const uint8_t owm_server_cert_pem_end[] asm("_binary_owm_cert_pem_end");

/*
    Free OpenWeatherAPI is available one request per min. We request once every 10mins

    We pull realtime weather from the API and dump in a json local cache file (SPIFF/SDCARD/FAT)
    Then read that file to show in the UI every 5mins.
    If device is disconnected from internet or fails, it will show the last weather update
*/

#define MAX_HTTP_RECV_BUFFER 2048
#define MAX_HTTP_OUTPUT_BUFFER 2048

// Move all these to config.json later
#define WEB_API_URL     CONFIG_COINMARKET_OWM_URL //"pro-api.coinmarketcap.com"
#define WEB_API_PORT    "80"    // not used unless we need custom port number
#define WEB_API_PATH    "/v0/aleph" //?q=" CONFIG_WEATHER_LOCATION "&units=metric&APPID="
#define WEB_API_KEY     CONFIG_COINMARKET_API_KEY //

CoinMarketCap::CoinMarketCap()
{
    // Coin cache filename
    cmc_file_name = "/spiffs/crypto/cmc_crypto.json";
    cg_file_name = "/spiffs/crypto/cg_crypto.json";
    btc_file_name = "/spiffs/crypto/btc_crypto.json";

    // Settings filename / add these after UI has these config options
    // cfg_filename = "/spiffs/settings.json";
    // cfg = new SettingsConfig(cfg_filename);
    // cfg->load_config();
}

/*
 * Get Coins from CoinMarketCap API
 * API call can fail => no wifi / connectivity issues / request limits
 * If fails, data from cache file is used.
*/
void CoinMarketCap::request_coin_update()
{
    jsonString = "{}";

    // Get coin track from CoinMarketCap and update the cache file
    if (request_json_cmc() == ESP_OK) {
        ESP_LOGI(TAG,"Updating and writing CMC info into cache - cmc_crypto.json");
        write_json(cmc_file_name);    // Save content of jsonString to file if success
    }
    ESP_LOGI(TAG,"Reading - cmc_crypto.json");
    read_json(cmc_file_name);
    ESP_LOGI(TAG,"Loading - cmc_crypto.json");
    load_cmc_json();
}

void CoinMarketCap::load_btc_json()
{
    ESP_LOGW(TAG,"load_btc_json() \n%s",jsonString.c_str());

    root = cJSON_Parse(jsonString.c_str());

    balanceinfo = cJSON_GetObjectItem (root, "balance");
    int amount = cJSON_GetObjectItem(balanceinfo,"confirmed")->valueint;
    Amount = (double) amount / (double) 100000000;

    ESP_LOGW(TAG,"BTC Amount: %f", Amount);
    // Cleanup
    cJSON_Delete(root);
}

void CoinMarketCap::load_cmc_json()
{
    ESP_LOGW(TAG,"load_cmc_json() \n%s",jsonString.c_str());
    cJSON *pair_element = NULL;
    cJSON *price_element = NULL;

    root = cJSON_Parse(jsonString.c_str());

    datainfo = cJSON_GetObjectItem(root,"data");
    
    coininfo = cJSON_GetObjectItem(datainfo,"coinPrice");

    Name = cJSON_GetObjectItem(coininfo,"name")->valuestring;
    Symbol = cJSON_GetObjectItem(coininfo,"symbol")->valuestring;
    Quote = "USD";

    Price = cJSON_GetObjectItem(coininfo,"currentPrice")->valuedouble;
    Change24h = cJSON_GetObjectItem(coininfo,"change24H")->valuedouble;
    Change7d = cJSON_GetObjectItem(coininfo,"change7D")->valuedouble;
    Change1m = cJSON_GetObjectItem(coininfo,"change1M")->valuedouble;

    AmountValue = 1 * Price;

    ESP_LOGW(TAG,"data: %3.1f$ price / %3.1f changed 1m / %3.1f changed 24h / %3.1f changed 7d",
                                            Price, Change1m,
                                            Change24h, Change7d);

    
    quoteinfo = cJSON_GetObjectItem(datainfo, "marketData");
    pricesinfo = cJSON_GetObjectItem(quoteinfo, "prices");

    /* iterate over indexs */
	int num = 0;
	cJSON_ArrayForEach(pair_element, pricesinfo) {
        price_element = cJSON_GetArrayItem(pair_element, 1);
		ChartValues[num] = price_element->valuedouble;
        num++;
	}

    // Cleanup
    cJSON_Delete(root);
}

void CoinMarketCap::load_cg_json()
{
    ESP_LOGW(TAG,"load_cg_json() \n%s",jsonString.c_str());
    cJSON *pair_element = NULL;
    cJSON *price_element = NULL;

    root = cJSON_Parse(jsonString.c_str());

    pricesinfo = cJSON_GetObjectItem(root, "prices");

    /* iterate over indexs */
	int num = 0;
	cJSON_ArrayForEach(pair_element, pricesinfo) {
        price_element = cJSON_GetArrayItem(pair_element, 1);
		ChartValues[num] = price_element->valuedouble;
        num++;
	}

    // Cleanup
    cJSON_Delete(root);
}

void CoinMarketCap::read_json(string crypto_name)
{
    // read json file to string
    ifstream jsonfile(crypto_name);
    if (!jsonfile.is_open())
    {
        ESP_LOGE(TAG,"File open for read failed %s",crypto_name.c_str());
        //save_config();  // create file with default values
    }

    jsonString.assign((std::istreambuf_iterator<char>(jsonfile)),
                (std::istreambuf_iterator<char>()));

    jsonfile.close();
}

void CoinMarketCap::write_json(string crypto_name)
{
    // cache json in flash to show if not online?
    ofstream jsonfile(crypto_name);
    if (!jsonfile.is_open())
    {
        ESP_LOGE(TAG,"File open for write failed %s",crypto_name.c_str());
        return ;//ESP_FAIL;
    }
    jsonfile << jsonString;
    jsonfile.flush();
    jsonfile.close();
}

/*
    Get Haskoin json using http - api.haskoin.com
*/
esp_err_t CoinMarketCap::request_json_btc()
{
    string apiURL = "https://api.haskoin.com/btc/xpub/";
    string apiPath = "?derive=segwit";
    string coinUrl = apiURL + CONFIG_COINMARKET_XPUB_ADDRESS + apiPath;

    return request_https_json(coinUrl);
}

/*
    Get Coingecko json using http - api.coingecko.com
*/
esp_err_t CoinMarketCap::request_json_cg()
{
    string coinUrl = "https://api.coingecko.com/api/v3/coins/aleph/market_chart?vs_currency=usd&days=15&interval=daily";

    return request_https_json(coinUrl);
}

/*
    Get CoinMarketCap json using http - sandbox-api.coinmarketcap.com
*/
esp_err_t CoinMarketCap::request_json_cmc()
{
    string convertString = "";
    string queryString = WEB_API_PATH "?" + convertString;
    string coinUrl = WEB_API_URL "" + queryString;

    return request_https_json(coinUrl);
}

/*
    Get CoinMarketCap json using http - sandbox-api.coinmarketcap.com
*/
esp_err_t CoinMarketCap::request_https_json(string coinUrl)
{
    char local_response_buffer[MAX_HTTP_OUTPUT_BUFFER] = {0};
    int content_length = 0;

    jsonString = "";

    ESP_LOGE(TAG,"HTTPS request to URL: %s", coinUrl.c_str());

#if CONFIG_MBEDTLS_CERTIFICATE_BUNDLE
    esp_http_client_config_t config = {
        .url = coinUrl.c_str(),
        .crt_bundle_attach = esp_crt_bundle_attach,
    };
#else
    esp_http_client_config_t config = {
        .url = coinUrl.c_str(),
    };
#endif

    esp_http_client_handle_t client = esp_http_client_init(&config);
    esp_err_t err = esp_http_client_set_header(client, "X-CMC_PRO_API_KEY", WEB_API_KEY);
    esp_http_client_set_method(client, HTTP_METHOD_GET);
    err = esp_http_client_open(client, 0);

    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to open HTTP connection: %s", esp_err_to_name(err));
    } else {
        content_length = esp_http_client_fetch_headers(client);
        if (content_length < 0) {
            ESP_LOGE(TAG, "HTTP client fetch headers failed");
        } else {
            int data_read = esp_http_client_read_response(client, local_response_buffer, MAX_HTTP_OUTPUT_BUFFER);
            if (data_read >= 0) {
                ESP_LOGI(TAG, "HTTP GET Status = %d, content_length = %lld",
                esp_http_client_get_status_code(client),
                esp_http_client_get_content_length(client));
                ESP_LOG_BUFFER_HEX(TAG, local_response_buffer, data_read);
            } else {
                ESP_LOGE(TAG, "Failed to read response");
            }
        }
    }

    ESP_LOGE(TAG,"JSON:\n%s",local_response_buffer);
    esp_http_client_cleanup(client);
    jsonString = local_response_buffer;
    return ESP_OK;

}
