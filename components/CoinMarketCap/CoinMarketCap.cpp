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

#define MAX_HTTP_RECV_BUFFER 512
#define MAX_HTTP_OUTPUT_BUFFER 1024

// Move all these to config.json later
#define WEB_API_URL     CONFIG_COINMARKET_OWM_URL //"pro-api.coinmarketcap.com"
#define WEB_API_PORT    "80"    // not used unless we need custom port number
#define WEB_API_PATH    "/v1/cryptocurrency/quotes/latest" //?q=" CONFIG_WEATHER_LOCATION "&units=metric&APPID="
#define WEB_API_KEY     CONFIG_COINMARKET_API_KEY //

CoinMarketCap::CoinMarketCap()
{
    // Coin cache filename
    file_name = "/spiffs/crypto/crypto.json";

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
    if (request_json_over_http() == ESP_OK) {
        ESP_LOGI(TAG,"Updating and writing into cache - crypto.json");
        write_json();    // Save content of jsonString to file if success
    }
    ESP_LOGI(TAG,"Reading - crypto.json");
    read_json();

    ESP_LOGI(TAG,"Loading - crypto.json");
    load_json();
}

void CoinMarketCap::load_json()
{
    ESP_LOGW(TAG,"load_json() \n%s",jsonString.c_str());

    // try
	// {

    // 19.8°С temperature from 18.9°С to 19.8 °С, wind 1.54 m/s. clouds 20 %, 1017 hpa
    root = cJSON_Parse(jsonString.c_str());

    datainfo = cJSON_GetObjectItem(root,"data");
    coininfo = cJSON_GetObjectItem(datainfo,"BTC");

    Name = cJSON_GetObjectItem(coininfo,"name")->valuestring;
    Symbol = cJSON_GetObjectItem(coininfo,"symbol")->valuestring;
    Quote = "USD";

    quoteinfo = cJSON_GetObjectItem(coininfo,"quote");
    quotecoininfo = cJSON_GetObjectItem(quoteinfo,"USD");

    Price = cJSON_GetObjectItem(quotecoininfo,"price")->valuedouble;
    Change1h = cJSON_GetObjectItem(quotecoininfo,"percent_change_1h")->valuedouble;
    Change24h = cJSON_GetObjectItem(quotecoininfo,"percent_change_24h")->valuedouble;
    Change7d = cJSON_GetObjectItem(quotecoininfo,"percent_change_7d")->valuedouble;

    Amount = 0.112345;
    AmountValue = 2205.03;
    
    ESP_LOGW(TAG,"data: %3.1f$ price / %3.1f changed 1h / %3.1f changed 24h / %3.1f changed 7d",
                                            Price, Change1h,
                                            Change24h, Change7d);

    // Cleanup
    cJSON_Delete(root);
}

void CoinMarketCap::read_json()
{
    // read json file to string    
    ifstream jsonfile(file_name);
    if (!jsonfile.is_open())
    {
        ESP_LOGE(TAG,"File open for read failed %s",file_name.c_str());
        //save_config();  // create file with default values
    }

    jsonString.assign((std::istreambuf_iterator<char>(jsonfile)),
                (std::istreambuf_iterator<char>()));

    jsonfile.close();    
}

void CoinMarketCap::write_json()
{
    // cache json in flash to show if not online?
    ofstream jsonfile(file_name);
    if (!jsonfile.is_open())
    {
        ESP_LOGE(TAG,"File open for write failed %s",file_name.c_str());
        return ;//ESP_FAIL;
    }
    jsonfile << jsonString;
    jsonfile.flush();
    jsonfile.close();    
}

esp_err_t coin_http_event_handle(esp_http_client_event_t *evt)
{
    static char *output_buffer;  // Buffer to store response of http request from event handler
    static int output_len;       // Stores number of bytes read
    switch(evt->event_id) {
        case HTTP_EVENT_ERROR:
            ESP_LOGI(TAG, "HTTP_EVENT_ERROR");
            break;
        case HTTP_EVENT_ON_CONNECTED:
            ESP_LOGI(TAG, "HTTP_EVENT_ON_CONNECTED");
            break;
        case HTTP_EVENT_HEADER_SENT:
            ESP_LOGI(TAG, "HTTP_EVENT_HEADER_SENT");
            break;
        case HTTP_EVENT_ON_HEADER:
            ESP_LOGI(TAG, "HTTP_EVENT_ON_HEADER");
            printf("%.*s", evt->data_len, (char*)evt->data);
            break;
        case HTTP_EVENT_ON_DATA:
            ESP_LOGI(TAG, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
            ESP_LOGI(TAG, "HTTP_EVENT_ON_DATA, %s", (const char *) evt->data);
            if (!esp_http_client_is_chunked_response(evt->client)) {
                // If user_data buffer is configured, copy the response into the buffer
                int copy_len = 0;
                if (evt->user_data) {
                    copy_len = MIN(evt->data_len, (MAX_HTTP_OUTPUT_BUFFER - output_len));
                    if (copy_len) {
                        memcpy(evt->user_data + output_len, evt->data, copy_len);
                    }
                } else {
                    const int buffer_len = esp_http_client_get_content_length(evt->client);
                    if (output_buffer == NULL) {
                        output_buffer = (char *) malloc(buffer_len);
                        output_len = 0;
                        if (output_buffer == NULL) {
                            ESP_LOGE(TAG, "Failed to allocate memory for output buffer");
                            return ESP_FAIL;
                        }
                    }
                    copy_len = MIN(evt->data_len, (buffer_len - output_len));
                    if (copy_len) {
                        memcpy(output_buffer + output_len, evt->data, copy_len);
                    }
                }
                output_len += copy_len;
            }            
            break;
        case HTTP_EVENT_ON_FINISH:
            ESP_LOGI(TAG, "HTTP_EVENT_ON_FINISH");
            if (output_buffer != NULL) {
                // Response is accumulated in output_buffer. Uncomment the below line to print the accumulated response
                // ESP_LOG_BUFFER_HEX(TAG, output_buffer, output_len);
                free(output_buffer);
                output_buffer = NULL;
            }
            output_len = 0;
            break;
        case HTTP_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "HTTP_EVENT_DISCONNECTED");
            if (output_buffer != NULL) {
                free(output_buffer);
                output_buffer = NULL;
            }
            output_len = 0;
            break;
        case HTTP_EVENT_REDIRECT:
            break;
    }
    return ESP_OK;
}

/*
    Get CoinMarketCap certificate - sandbox-api.coinmarketcap.com
    openssl s_client -showcerts -connect = sandbox-api.coinmarketcap.com:443 </dev/null
*/
esp_err_t CoinMarketCap::request_json_over_https()
{
    ESP_LOGI(TAG, "HTTPS request to get cryptos");
    return ESP_OK;
}

/*
    Get CoinMarketCap json using http - sandbox-api.coinmarketcap.com
*/
esp_err_t CoinMarketCap::request_json_over_http()
{
    char local_response_buffer[MAX_HTTP_OUTPUT_BUFFER] = {0};

    jsonString = "";
    string convertString = "convert=USD&symbol=BTC";
    string queryString = WEB_API_PATH "?" + convertString;
    string coinUrl = WEB_API_URL "" + queryString;

    ESP_LOGI(TAG, "HTTP request to get cryptos");
    ESP_LOGE(TAG,"URL: %s", coinUrl.c_str());

    esp_http_client_config_t config = {
        .url = coinUrl.c_str(),
        .event_handler = coin_http_event_handle,
        .user_data = local_response_buffer,
    };

    esp_http_client_handle_t client = esp_http_client_init(&config);
    esp_err_t err = esp_http_client_set_header(client, "X-CMC_PRO_API_KEY", WEB_API_KEY);
    err = esp_http_client_perform(client);

    if (err == ESP_OK) {
    ESP_LOGI(TAG, "Status = %d, content_length = %" PRId64 , // PRIu64  / PRIx64 to print in hexadecimal.
            esp_http_client_get_status_code(client),
            esp_http_client_get_content_length(client));
    } else {
        return ESP_FAIL;
    }

    esp_http_client_cleanup(client);    
    ESP_LOGE("JSON", "%s", (const char *) local_response_buffer);
    jsonString = local_response_buffer;
    return ESP_OK;

}