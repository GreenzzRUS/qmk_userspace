/**
 * Copyright 2022 Charly Delay <charly@codesink.dev> (@0xcharly)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H

#include "timer.h"
#include "oneshot.h"
#include "swapper.h"

#define RESET QK_BOOT

#define ___ KC_NO

#define LA_SYM MO(_SYM)
#define LA_ALT MO(_ALT)
#define LA_NAV MO(_NAV)
#define LA_CMD MO(_CMD)
#define LA_GFN MO(_GFN)

#define QUOT  S(KC_GRV)
#define PIPE  S(KC_BSLS)
#define DPIPE S(RALT(KC_BSLS))
#define SCLN  S(KC_LBRC)
#define CLN   S(KC_RBRC)
#define GRV   RALT(KC_GRV)
#define BSL   RALT(KC_BSLS)
#define NBSP  RALT(KC_SPC)

#define SPACE_L C(G(KC_LEFT))
#define SPACE_R C(G(KC_RGHT))

#define TAB_L C(S(KC_TAB))
#define TAB_R C(KC_TAB)

enum layers {
    _DEF,
    _GAM,
    _GFN,
    _SYM,
    _NAV,
    _NUM,
    _ALT,
    _CMD,
    LAYER_POINTER,
};
enum keycodes {
    // Custom oneshot mod implementation with no timers.
    OS_SHFT = SAFE_RANGE,
    OS_CTRL,
    OS_ALT,
    OS_CMD,

    SW_WIN,  // Switch to next window         (alt-tab)
    SW_TAB,  // Switch to next browser tab    (ctrl-tab)
};

/** \brief Automatically enable sniping-mode on the pointer layer. */
#define CHARYBDIS_AUTO_SNIPING_ON_LAYER LAYER_POINTER

#ifdef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE
static uint16_t auto_pointer_layer_timer = 0;

#ifndef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS
#define CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS 1000
#endif // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS

#ifndef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD
#define CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD 8
#endif // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD
#endif     // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE

// #define LOWER MO(LAYER_LOWER)
// #define RAISE MO(LAYER_RAISE)
// #define PT_Z LT(LAYER_POINTER, KC_Z)
// #define PT_SLSH LT(LAYER_POINTER, KC_SLSH)

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_DEF] = LAYOUT(
KC_NO,      KC_Q,       KC_W,       KC_E,       KC_R,       KC_T,             KC_Y,       KC_U,       KC_I,       KC_O,       KC_P,        KC_NO,      
KC_NO,      KC_A,       KC_S,       KC_D,       KC_F,       KC_G,             KC_H,       KC_J,       KC_K,       KC_L,       KC_SCLN,     KC_NO,      
KC_NO,      KC_Z,       KC_X,       KC_C,       KC_V,       KC_B,             KC_N,       KC_M,       KC_COMM,    KC_DOT,     KC_SLSH,     KC_NO,      
                                    KC_CAPS,    KC_SPC,     LA_NAV,           LA_SYM,     KC_LSFT
    ),

    [_GAM] = LAYOUT(
KC_NO,     KC_TAB,     KC_Q,       KC_W,       KC_E,       KC_R,             ___,        ___,        KC_UP,      ___,        KC_TAB,    KC_NO,     
KC_NO,     KC_LSFT,    KC_A,       KC_S,       KC_D,       KC_F,             ___,        KC_LEFT,    KC_DOWN,    KC_RGHT,    KC_RSFT,   KC_NO,     
KC_NO,     KC_LCTL,    KC_Z,       KC_X,       KC_C,       KC_V,             ___,        ___,        ___,        ___,        KC_RCTL,   KC_NO,     
                                   KC_LALT,    KC_SPC,     LA_GFN,           DF(_DEF),   KC_SPC
    ),

    [_GFN] = LAYOUT(
KC_NO,     KC_5,       KC_1,       KC_2,       KC_3,       KC_4,             KC_NUM,     ___,        ___,        ___,        ___,        KC_NO,     
KC_NO,     KC_0,       KC_6,       KC_7,       KC_8,       KC_9,             ___,        ___,        ___,        ___,        ___,        KC_NO,     
KC_NO,     KC_G,       KC_J,       KC_I,       KC_M,       KC_T,             ___,        ___,        ___,        QK_RBT,     QK_BOOT,    KC_NO,     
                                   KC_ESC,     KC_ENT,     _______,          _______,    _______
    ),

    [_SYM] = LAYOUT(
KC_NO,     KC_1,       KC_2,       KC_3,       KC_4,       KC_5,             KC_6,       KC_7,       KC_8,       KC_9,       KC_0,       KC_NO,     
KC_NO,     KC_SCLN,    GRV,        KC_GRV,     QUOT,       KC_MINS,          KC_PLUS,    OS_SHFT,    OS_CTRL,    OS_ALT,     OS_CMD,     KC_NO,     
KC_NO,     BSL,        DPIPE,      SCLN,       KC_LBRC,    KC_UNDS,          KC_EQL,     KC_RBRC,    CLN,        PIPE,       KC_BSLS,    KC_NO,     
                                   _______,    _______,    _______,          _______,    _______
    ),


    [_NAV] = LAYOUT(
KC_NO,     SW_TAB,     SW_WIN,     TAB_L,      TAB_R,      KC_ESC,           KC_ESC,     KC_HOME,    KC_END,     KC_BSPC,    KC_DEL,    KC_NO,     
KC_NO,     OS_CMD,     OS_ALT,     OS_CTRL,    OS_SHFT,    KC_ENT,           KC_ENT,     KC_LEFT,    KC_DOWN,    KC_UP,      KC_RGHT,   KC_NO,     
KC_NO,     SPACE_L,    SPACE_R,    DF(_GAM),   KC_PSCR,    KC_TAB,           KC_TAB,     KC_PGUP,    KC_PGDN,    KC_QUOT,    KC_NUM,    KC_NO,     
                                   LA_CMD,     _______,    _______,          _______,    _______
    ),

    [_NUM] = LAYOUT(
KC_NO,     KC_P1,      KC_P2,      KC_P3,      KC_P4,      KC_P5,            KC_P6,      KC_P7,      KC_P8,      KC_P9,      KC_P0,     KC_NO,     
KC_NO,     OS_CMD,     OS_ALT,     OS_CTRL,    OS_SHFT,    KC_F11,           KC_F12,     OS_SHFT,    OS_CTRL,    OS_ALT,     OS_CMD,    KC_NO,     
KC_NO,     KC_F1,      KC_F2,      KC_F3,      KC_F4,      KC_F5,            KC_F6,      KC_F7,      KC_F8,      KC_F9,      KC_F10,    KC_NO,     
                                   _______,    _______,    _______,          _______,    _______
    ),

    [_ALT] = LAYOUT(
KC_NO,     RALT(KC_Q), RALT(KC_W), RALT(KC_E), RALT(KC_R), RALT(KC_T),       RALT(KC_Y), RALT(KC_U), RALT(KC_I),    RALT(KC_O),   RALT(KC_P),       KC_NO,     
KC_NO,     RALT(KC_A), RALT(KC_S), RALT(KC_D), RALT(KC_F), RALT(KC_G),       RALT(KC_H), RALT(KC_J), RALT(KC_K),    RALT(KC_L),   RALT(KC_SCLN),    KC_NO,     
KC_NO,     RALT(KC_Z), RALT(KC_X), RALT(KC_C), RALT(KC_V), RALT(KC_B),       RALT(KC_N), RALT(KC_M), RALT(KC_COMM), RALT(KC_DOT), RALT(KC_SLSH),    KC_NO,     
                                   _______,    NBSP,       KC_LSFT,          _______,    _______
    ),

    [_CMD] = LAYOUT(
KC_NO,     MEH(KC_Q),  MEH(KC_W),  MEH(KC_E),  MEH(KC_R),  MEH(KC_T),        MEH(KC_Y),  KC_VOLD,    KC_MUTE,       KC_VOLU,     MEH(KC_P),       KC_NO,     
KC_NO,     MEH(KC_A),  MEH(KC_S),  MEH(KC_D),  MEH(KC_F),  MEH(KC_G),        MEH(KC_H),  KC_MPRV,    KC_MPLY,       KC_MNXT,     MEH(KC_SCLN),    KC_NO,     
KC_NO,     MEH(KC_Z),  MEH(KC_X),  MEH(KC_C),  MEH(KC_V),  MEH(KC_B),        MEH(KC_N),  MEH(KC_M),  MEH(KC_COMM),  MEH(KC_DOT), MEH(KC_SLSH),    KC_NO,     
                                   _______,    _______,    _______,          KC_LGUI,    _______
    )
};
bool is_oneshot_cancel_key(uint16_t keycode) {
    switch (keycode) {
    case LA_SYM:
    case LA_NAV:
        return true;
    default:
        return false;
    }
}

bool is_oneshot_ignored_key(uint16_t keycode) {
    switch (keycode) {
    case LA_SYM:
    case LA_NAV:
    case KC_LSFT:
    case OS_SHFT:
    case OS_CTRL:
    case OS_ALT:
    case OS_CMD:
        return true;
    default:
        return false;
    }
}

bool sw_win_active = false;
bool sw_tab_active = false;

oneshot_state os_shft_state = os_up_unqueued;
oneshot_state os_ctrl_state = os_up_unqueued;
oneshot_state os_alt_state = os_up_unqueued;
oneshot_state os_cmd_state = os_up_unqueued;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    update_swapper(
        &sw_win_active, KC_LALT, KC_TAB, SW_WIN, OS_SHFT,
        keycode, record
    );
    update_swapper(
        &sw_tab_active, KC_LCTL, KC_TAB, SW_TAB, OS_SHFT,
        keycode, record
    );

    update_oneshot(
        &os_shft_state, KC_LSFT, OS_SHFT,
        keycode, record
    );
    update_oneshot(
        &os_ctrl_state, KC_LCTL, OS_CTRL,
        keycode, record
    );
    update_oneshot(
        &os_alt_state, KC_LALT, OS_ALT,
        keycode, record
    );
    update_oneshot(
        &os_cmd_state, KC_LCMD, OS_CMD,
        keycode, record
    );

    return true;
}

}
// clang-format on

#ifdef POINTING_DEVICE_ENABLE
#    ifdef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE
report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
    if (abs(mouse_report.x) > CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD || abs(mouse_report.y) > CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD) {
        if (auto_pointer_layer_timer == 0) {
            layer_on(LAYER_POINTER);
#        ifdef RGB_MATRIX_ENABLE
            rgb_matrix_mode_noeeprom(RGB_MATRIX_NONE);
            rgb_matrix_sethsv_noeeprom(HSV_GREEN);
#        endif // RGB_MATRIX_ENABLE
        }
        auto_pointer_layer_timer = timer_read();
    }
    return mouse_report;
}

void matrix_scan_user(void) {
    if (auto_pointer_layer_timer != 0 && TIMER_DIFF_16(timer_read(), auto_pointer_layer_timer) >= CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS) {
        auto_pointer_layer_timer = 0;
        layer_off(LAYER_POINTER);
#        ifdef RGB_MATRIX_ENABLE
        rgb_matrix_mode_noeeprom(RGB_MATRIX_DEFAULT_MODE);
#        endif // RGB_MATRIX_ENABLE
    }
}
#    endif // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE

#    ifdef CHARYBDIS_AUTO_SNIPING_ON_LAYER
layer_state_t layer_state_set_user(layer_state_t state) {
    // Логика три-слоя
    state = update_tri_layer_state(state, _SYM, _NAV, _NUM);
    
    // Логика снипинга
    #ifdef CHARYBDIS_AUTO_SNIPING_ON_LAYER
    charybdis_set_pointer_sniping_enabled(layer_state_cmp(state, CHARYBDIS_AUTO_SNIPING_ON_LAYER));
    #endif
    
    return state;
}
#    endif // CHARYBDIS_AUTO_SNIPING_ON_LAYER
#endif     // POINTING_DEVICE_ENABLE

#ifdef RGB_MATRIX_ENABLE
// Forward-declare this helper function since it is defined in rgb_matrix.c.
void rgb_matrix_update_pwm_buffers(void);
#endif
