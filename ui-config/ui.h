// SquareLine LVGL GENERATED FILE
// EDITOR VERSION: SquareLine Studio 1.2.1
// LVGL VERSION: 8.3.4
// PROJECT: CryptoGraph

#ifndef _CRYPTOGRAPH_UI_H
#define _CRYPTOGRAPH_UI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

extern lv_obj_t * ui_mainScreen;
extern lv_obj_t * ui_cryptoPanel;
extern lv_obj_t * ui_titleCryptoPanel;
extern lv_obj_t * ui_cryptoImage;
extern lv_obj_t * ui_cryptoLabel;
extern lv_obj_t * ui_priceLabel;
extern lv_obj_t * ui_assetsCryptoPanel;
extern lv_obj_t * ui_amountLabel;
extern lv_obj_t * ui_changesCryptoPanel1;
extern lv_obj_t * ui_changesLabel1;
extern lv_obj_t * ui_divisorLabel1;
extern lv_obj_t * ui_changesLabel2;
extern lv_obj_t * ui_divisorLabel2;
extern lv_obj_t * ui_changesLabel3;
extern lv_obj_t * ui_cryptoChart;
extern lv_obj_t * ui_toolPanel;
void ui_event_configButton(lv_event_t * e);
extern lv_obj_t * ui_configButton;
extern lv_obj_t * ui_configScreen;
extern lv_obj_t * ui_configPanel;
void ui_event_closeButton(lv_event_t * e);
extern lv_obj_t * ui_closeButton;
extern lv_obj_t * ui_ipLabel;
extern lv_obj_t * ui_qrImage;
void ui_event_wifiResetButton(lv_event_t * e);
extern lv_obj_t * ui_wifiResetButton;


LV_IMG_DECLARE(ui_img_630538576);    // assets/Bitcoin-Logo-mini-2.png




void ui_init(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
