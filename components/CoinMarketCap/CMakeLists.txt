idf_component_register(SRCS "CoinMarketCap.cpp"
                    INCLUDE_DIRS "include"
                    PRIV_REQUIRES esp-tls json esp_http_client SettingsConfig 
                    # Embed OWM server root certificate into the final binary
                    # Need the entire certificate chain
                    # EMBED_TXTFILES ${project_dir}/server_certs/owm_cert.pem
                    )

project (CoinMarketCap)                    