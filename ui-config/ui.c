// SquareLine LVGL GENERATED FILE
// EDITOR VERSION: SquareLine Studio 1.2.1
// LVGL VERSION: 8.3.4
// PROJECT: CryptoGraph

#include "ui.h"
#include "ui_helpers.h"

///////////////////// VARIABLES ////////////////////
lv_obj_t * ui_mainScreen;
lv_obj_t * ui_cryptoPanel;
lv_obj_t * ui_titleCryptoPanel;
lv_obj_t * ui_cryptoImage;
lv_obj_t * ui_cryptoLabel;
lv_obj_t * ui_priceLabel;
lv_obj_t * ui_assetsCryptoPanel;
lv_obj_t * ui_amountLabel;
lv_obj_t * ui_changesCryptoPanel1;
lv_obj_t * ui_changesLabel1;
lv_obj_t * ui_divisorLabel1;
lv_obj_t * ui_changesLabel2;
lv_obj_t * ui_divisorLabel2;
lv_obj_t * ui_changesLabel3;
lv_obj_t * ui_cryptoChart;
lv_obj_t * ui_toolPanel;
void ui_event_configButton(lv_event_t * e);
lv_obj_t * ui_configButton;
lv_obj_t * ui_configScreen;
lv_obj_t * ui_configPanel;
void ui_event_closeButton(lv_event_t * e);
lv_obj_t * ui_closeButton;
lv_obj_t * ui_ipLabel;
lv_obj_t * ui_qrImage;
void ui_event_wifiResetButton(lv_event_t * e);
lv_obj_t * ui_wifiResetButton;

///////////////////// TEST LVGL SETTINGS ////////////////////
#if LV_COLOR_DEPTH != 16
    #error "LV_COLOR_DEPTH should be 16bit to match SquareLine Studio's settings"
#endif
#if LV_COLOR_16_SWAP !=1
    #error "LV_COLOR_16_SWAP should be 1 to match SquareLine Studio's settings"
#endif

///////////////////// ANIMATIONS ////////////////////

///////////////////// FUNCTIONS ////////////////////
void ui_event_configButton(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        _ui_screen_change(ui_configScreen, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0);
    }
}
void ui_event_closeButton(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        _ui_screen_change(ui_mainScreen, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0);
    }
}
void ui_event_wifiResetButton(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        _ui_screen_change(ui_mainScreen, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0);
    }
}

///////////////////// SCREENS ////////////////////
void ui_mainScreen_screen_init(void)
{
    ui_mainScreen = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_mainScreen, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_cryptoPanel = lv_obj_create(ui_mainScreen);
    lv_obj_set_width(ui_cryptoPanel, 440);
    lv_obj_set_height(ui_cryptoPanel, 280);
    lv_obj_set_x(ui_cryptoPanel, 0);
    lv_obj_set_y(ui_cryptoPanel, 15);
    lv_obj_set_align(ui_cryptoPanel, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_cryptoPanel, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(ui_cryptoPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_cryptoPanel, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_cryptoPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_cryptoPanel, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_cryptoPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_titleCryptoPanel = lv_obj_create(ui_cryptoPanel);
    lv_obj_set_width(ui_titleCryptoPanel, 440);
    lv_obj_set_height(ui_titleCryptoPanel, 25);
    lv_obj_set_x(ui_titleCryptoPanel, 0);
    lv_obj_set_y(ui_titleCryptoPanel, -129);
    lv_obj_set_align(ui_titleCryptoPanel, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_titleCryptoPanel, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(ui_titleCryptoPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_titleCryptoPanel, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_titleCryptoPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_titleCryptoPanel, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_titleCryptoPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_cryptoImage = lv_img_create(ui_titleCryptoPanel);
    lv_img_set_src(ui_cryptoImage, &ui_img_630538576);
    lv_obj_set_width(ui_cryptoImage, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_cryptoImage, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_cryptoImage, -32);
    lv_obj_set_y(ui_cryptoImage, 0);
    lv_obj_set_align(ui_cryptoImage, LV_ALIGN_LEFT_MID);
    lv_obj_add_flag(ui_cryptoImage, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_cryptoImage, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_img_set_zoom(ui_cryptoImage, 100);

    ui_cryptoLabel = lv_label_create(ui_titleCryptoPanel);
    lv_obj_set_width(ui_cryptoLabel, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_cryptoLabel, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_cryptoLabel, 20);
    lv_obj_set_y(ui_cryptoLabel, 0);
    lv_obj_set_align(ui_cryptoLabel, LV_ALIGN_LEFT_MID);
    lv_label_set_text(ui_cryptoLabel, "Bitcoin / USD");
    lv_obj_set_style_text_color(ui_cryptoLabel, lv_color_hex(0xFF8C00), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_cryptoLabel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_priceLabel = lv_label_create(ui_titleCryptoPanel);
    lv_obj_set_width(ui_priceLabel, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_priceLabel, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_priceLabel, LV_ALIGN_RIGHT_MID);
    lv_label_set_text(ui_priceLabel, "$ 25.253");
    lv_obj_set_style_text_color(ui_priceLabel, lv_color_hex(0x910000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_priceLabel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_assetsCryptoPanel = lv_obj_create(ui_cryptoPanel);
    lv_obj_set_width(ui_assetsCryptoPanel, 440);
    lv_obj_set_height(ui_assetsCryptoPanel, 40);
    lv_obj_set_x(ui_assetsCryptoPanel, 0);
    lv_obj_set_y(ui_assetsCryptoPanel, -91);
    lv_obj_set_align(ui_assetsCryptoPanel, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_assetsCryptoPanel, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(ui_assetsCryptoPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_assetsCryptoPanel, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_assetsCryptoPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_assetsCryptoPanel, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_assetsCryptoPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_amountLabel = lv_label_create(ui_assetsCryptoPanel);
    lv_obj_set_width(ui_amountLabel, 220);
    lv_obj_set_height(ui_amountLabel, 15);
    lv_obj_set_x(ui_amountLabel, -10);
    lv_obj_set_y(ui_amountLabel, 0);
    lv_obj_set_align(ui_amountLabel, LV_ALIGN_LEFT_MID);
    lv_label_set_text(ui_amountLabel, "0.03536482 BTC | 2.205 US$");
    lv_obj_set_style_text_color(ui_amountLabel, lv_color_hex(0x910000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_amountLabel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_amountLabel, LV_TEXT_ALIGN_AUTO, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui_amountLabel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_amountLabel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_amountLabel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_amountLabel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_changesCryptoPanel1 = lv_obj_create(ui_assetsCryptoPanel);
    lv_obj_set_width(ui_changesCryptoPanel1, 190);
    lv_obj_set_height(ui_changesCryptoPanel1, 15);
    lv_obj_set_align(ui_changesCryptoPanel1, LV_ALIGN_RIGHT_MID);
    lv_obj_set_flex_flow(ui_changesCryptoPanel1, LV_FLEX_FLOW_ROW_WRAP);
    lv_obj_set_flex_align(ui_changesCryptoPanel1, LV_FLEX_ALIGN_END, LV_FLEX_ALIGN_END, LV_FLEX_ALIGN_END);
    lv_obj_clear_flag(ui_changesCryptoPanel1, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(ui_changesCryptoPanel1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_changesCryptoPanel1, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_changesCryptoPanel1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_changesCryptoPanel1, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_changesCryptoPanel1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui_changesCryptoPanel1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_changesCryptoPanel1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_changesCryptoPanel1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_changesCryptoPanel1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_row(ui_changesCryptoPanel1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_column(ui_changesCryptoPanel1, 2, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_changesLabel1 = lv_label_create(ui_changesCryptoPanel1);
    lv_obj_set_width(ui_changesLabel1, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_changesLabel1, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_changesLabel1, LV_ALIGN_RIGHT_MID);
    lv_label_set_text(ui_changesLabel1, "1H:  - 23%");
    lv_obj_set_style_text_color(ui_changesLabel1, lv_color_hex(0x007A06), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_changesLabel1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_changesLabel1, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_changesLabel1, &lv_font_montserrat_10, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_divisorLabel1 = lv_label_create(ui_changesCryptoPanel1);
    lv_obj_set_width(ui_divisorLabel1, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_divisorLabel1, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_divisorLabel1, LV_ALIGN_RIGHT_MID);
    lv_label_set_text(ui_divisorLabel1, " | ");
    lv_obj_set_style_text_color(ui_divisorLabel1, lv_color_hex(0x5D5D5D), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_divisorLabel1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_divisorLabel1, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_divisorLabel1, &lv_font_montserrat_10, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_changesLabel2 = lv_label_create(ui_changesCryptoPanel1);
    lv_obj_set_width(ui_changesLabel2, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_changesLabel2, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_changesLabel2, LV_ALIGN_RIGHT_MID);
    lv_label_set_text(ui_changesLabel2, " 1D: + 1.05%");
    lv_obj_set_style_text_color(ui_changesLabel2, lv_color_hex(0x910006), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_changesLabel2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_changesLabel2, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_changesLabel2, &lv_font_montserrat_10, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_divisorLabel2 = lv_label_create(ui_changesCryptoPanel1);
    lv_obj_set_width(ui_divisorLabel2, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_divisorLabel2, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_divisorLabel2, LV_ALIGN_RIGHT_MID);
    lv_label_set_text(ui_divisorLabel2, " | ");
    lv_obj_set_style_text_color(ui_divisorLabel2, lv_color_hex(0x5D5D5D), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_divisorLabel2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_divisorLabel2, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_divisorLabel2, &lv_font_montserrat_10, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_changesLabel3 = lv_label_create(ui_changesCryptoPanel1);
    lv_obj_set_width(ui_changesLabel3, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_changesLabel3, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_changesLabel3, LV_ALIGN_RIGHT_MID);
    lv_label_set_text(ui_changesLabel3, "7D: - 50%");
    lv_obj_set_style_text_color(ui_changesLabel3, lv_color_hex(0x007A06), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_changesLabel3, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_changesLabel3, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_changesLabel3, &lv_font_montserrat_10, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_cryptoChart = lv_chart_create(ui_cryptoPanel);
    lv_obj_set_width(ui_cryptoChart, 420);
    lv_obj_set_height(ui_cryptoChart, 190);
    lv_obj_set_x(ui_cryptoChart, 0);
    lv_obj_set_y(ui_cryptoChart, 35);
    lv_obj_set_align(ui_cryptoChart, LV_ALIGN_CENTER);
    lv_chart_set_type(ui_cryptoChart, LV_CHART_TYPE_LINE);
    lv_chart_set_axis_tick(ui_cryptoChart, LV_CHART_AXIS_PRIMARY_X, 0, 0, 0, 0, true, 50);
    lv_chart_set_axis_tick(ui_cryptoChart, LV_CHART_AXIS_PRIMARY_Y, 0, 0, 5, 2, false, 50);
    lv_chart_series_t * ui_cryptoChart_series_1 = lv_chart_add_series(ui_cryptoChart, lv_color_hex(0xFF8C00),
                                                                      LV_CHART_AXIS_PRIMARY_Y);
    static lv_coord_t ui_cryptoChart_series_1_array[] = { 0, 10, 20, 40, 80, 80, 40, 20, 10, 0 };
    lv_chart_set_ext_y_array(ui_cryptoChart, ui_cryptoChart_series_1, ui_cryptoChart_series_1_array);
    lv_obj_set_style_radius(ui_cryptoChart, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_cryptoChart, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_cryptoChart, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_cryptoChart, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_cryptoChart, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_width(ui_cryptoChart, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_pad(ui_cryptoChart, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_bg_color(ui_cryptoChart, lv_color_hex(0x000000), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_cryptoChart, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_size(ui_cryptoChart, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);

    ui_toolPanel = lv_obj_create(ui_mainScreen);
    lv_obj_set_width(ui_toolPanel, 480);
    lv_obj_set_height(ui_toolPanel, 30);
    lv_obj_set_x(ui_toolPanel, 0);
    lv_obj_set_y(ui_toolPanel, -145);
    lv_obj_set_align(ui_toolPanel, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_toolPanel, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(ui_toolPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_toolPanel, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_toolPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_toolPanel, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_toolPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_configButton = lv_btn_create(ui_toolPanel);
    lv_obj_set_width(ui_configButton, 32);
    lv_obj_set_height(ui_configButton, 32);
    lv_obj_set_x(ui_configButton, -224);
    lv_obj_set_y(ui_configButton, 0);
    lv_obj_set_align(ui_configButton, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_configButton, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_configButton, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(ui_configButton, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_configButton, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_configButton, 50, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_add_event_cb(ui_configButton, ui_event_configButton, LV_EVENT_ALL, NULL);

}
void ui_configScreen_screen_init(void)
{
    ui_configScreen = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_configScreen, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_configPanel = lv_obj_create(ui_configScreen);
    lv_obj_set_width(ui_configPanel, 450);
    lv_obj_set_height(ui_configPanel, 290);
    lv_obj_set_align(ui_configPanel, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_configPanel, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_closeButton = lv_btn_create(ui_configPanel);
    lv_obj_set_width(ui_closeButton, 64);
    lv_obj_set_height(ui_closeButton, 64);
    lv_obj_set_x(ui_closeButton, -208);
    lv_obj_set_y(ui_closeButton, -128);
    lv_obj_set_align(ui_closeButton, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_closeButton, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_closeButton, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(ui_closeButton, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_closeButton, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_closeButton, 20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_ipLabel = lv_label_create(ui_configPanel);
    lv_obj_set_width(ui_ipLabel, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_ipLabel, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_ipLabel, 0);
    lv_obj_set_y(ui_ipLabel, -110);
    lv_obj_set_align(ui_ipLabel, LV_ALIGN_CENTER);
    lv_label_set_text(ui_ipLabel, "Waiting for IP...");

    ui_qrImage = lv_img_create(ui_configPanel);
    lv_obj_set_width(ui_qrImage, 100);
    lv_obj_set_height(ui_qrImage, 100);
    lv_obj_set_x(ui_qrImage, 0);
    lv_obj_set_y(ui_qrImage, -30);
    lv_obj_set_align(ui_qrImage, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_qrImage, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_qrImage, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_wifiResetButton = lv_btn_create(ui_configPanel);
    lv_obj_set_width(ui_wifiResetButton, 200);
    lv_obj_set_height(ui_wifiResetButton, 50);
    lv_obj_set_x(ui_wifiResetButton, 0);
    lv_obj_set_y(ui_wifiResetButton, 75);
    lv_obj_set_align(ui_wifiResetButton, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_wifiResetButton, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_wifiResetButton, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    lv_obj_add_event_cb(ui_closeButton, ui_event_closeButton, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_wifiResetButton, ui_event_wifiResetButton, LV_EVENT_ALL, NULL);

}

void ui_init(void)
{
    lv_disp_t * dispp = lv_disp_get_default();
    lv_theme_t * theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED),
                                               true, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    ui_mainScreen_screen_init();
    ui_configScreen_screen_init();
    lv_disp_load_scr(ui_mainScreen);
}
