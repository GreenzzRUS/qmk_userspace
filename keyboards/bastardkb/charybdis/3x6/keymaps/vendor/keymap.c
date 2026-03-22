/**
Copyright 2022 Charly Delay [charly@codesink.dev](mailto:charly@codesink.dev) (@0xcharly)
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program.  If not, see [http://www.gnu.org/licenses/](http://www.gnu.org/licenses/).
*/
/**
Charybdis 3x6 Keymap - Merged with keymap-36.c functionality
Based on original Charybdis keymap by Charly Delay
Enhanced with layers, one-shot mods, and swapper from keymap-36.c
*/
#include QMK_KEYBOARD_H
#include "timer.h"
#include "oneshot.h"
#include "swapper.h"

// ============================================================================
// LAYERS
// ============================================================================
enum charybdis_keymap_layers {
    LAYER_BASE = 0,
    LAYER_LOWER,
    LAYER_RAISE,
    LAYER_POINTER,
    LAYER_SYM,
    LAYER_NAV,
    LAYER_NUM,      // Tri-layer: SYM + NAV
    LAYER_ALT,
    LAYER_CMD,
    LAYER_GAM,      // Gaming layer (was _GAM)
};

// ============================================================================
// CHARYBDIS-SPECIFIC SETTINGS
// ============================================================================
#define CHARYBDIS_AUTO_SNIPING_ON_LAYER LAYER_POINTER

#ifdef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE
static uint16_t auto_pointer_layer_timer = 0;
#    ifndef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS
#        define CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS 1000
#    endif
#    ifndef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD
#        define CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD 8
#    endif
#endif

// ============================================================================
// KEYCODE DEFINITIONS
// ============================================================================
#define LOWER MO(LAYER_LOWER)
#define RAISE MO(LAYER_RAISE)
#define PT_Z LT(LAYER_POINTER, KC_Z)
#define PT_SLSH LT(LAYER_POINTER, KC_SLSH)

// Layers from keymap-36.c
#define LA_SYM MO(LAYER_SYM)
#define LA_NAV MO(LAYER_NAV)
#define LA_ALT MO(LAYER_ALT)
#define LA_CMD MO(LAYER_CMD)
#define LA_GFN MO(LAYER_GAM)

// Special characters
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

// Custom keycodes for one-shot mods and swapper
enum keycodes {
    OS_SHFT = SAFE_RANGE,
    OS_CTRL,
    OS_ALT,
    OS_CMD,
    SW_WIN,
    SW_TAB,
};

// ============================================================================
// KEYMAPS
// ============================================================================
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* BASE LAYER
 * ┌─────────┬─────────┬─────────┬─────────┬─────────┬─────────┐       ┌─────────┬─────────┬─────────┬─────────┬─────────┬─────────┐
 * │   Tab   │    Q    │    W    │    E    │    R    │    T    │       │    Y    │    U    │    I    │    O    │    P    │   GUI   │
 * ├─────────┼─────────┼─────────┼─────────┼─────────┼─────────┤       ├─────────┼─────────┼─────────┼─────────┼─────────┼─────────┤
 * │   Shift │    A    │    S    │    D    │    F    │    G    │       │    H    │    J    │    K    │    L    │   ;     │   Ctrl  │
 * ├─────────┼─────────┼─────────┼─────────┼─────────┼─────────┤       ├─────────┼─────────┼─────────┼─────────┼─────────┼─────────┤
 * │   Ctrl  │    Z    │    X    │    C    │    V    │    B    │       │    N    │    M    │    ,    │    .    │    /    │   Shift │
 * └─────────┴─────────┴─────────┴─────────┴─────────┴─────────┘       └─────────┴─────────┴─────────┴─────────┴─────────┴─────────┘
 *                         ┌─────────┬─────────┬─────────┐           ┌─────────┬─────────┬─────────┐
 *                         │  Bksp   │   Nav   │   Space │           │   Ent   │   Sym   │  Shift  │
 *                         └─────────┴─────────┴─────────┘           └─────────┴─────────┴─────────┘
 */
[LAYER_BASE] = LAYOUT(
    KC_TAB,    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,       KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_RGUI,
    KC_LSFT,   KC_A,    KC_S,    KC_D,    KC_F,    KC_G,       KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_RCTL,
    KC_LCTL,   PT_Z,    KC_X,    KC_C,    KC_V,    KC_B,       KC_N,    KC_M,    KC_COMM, KC_DOT,  PT_SLSH, KC_RSFT,
    KC_BSPC,   LA_NAV,  KC_SPC,          KC_ENT,  LA_SYM,     KC_RSFT
),

/* LOWER LAYER (Media & Numbers)
 */
[LAYER_LOWER] = LAYOUT(
    XXXXXXX,   RGB_TOG, KC_MNXT, KC_MPLY, KC_MPRV, XXXXXXX,    KC_LBRC, KC_7,    KC_8,    KC_9,    KC_RBRC, XXXXXXX,
    XXXXXXX,   KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, XXXXXXX,    KC_PPLS, KC_4,    KC_5,    KC_6,    KC_PMNS, XXXXXXX,
    XXXXXXX,   XXXXXXX, XXXXXXX, XXXXXXX, EE_CLR,  QK_BOOT,    KC_PAST, KC_1,    KC_2,    KC_3,    KC_PSLS, XXXXXXX,
    XXXXXXX,   XXXXXXX, _______,          XXXXXXX, _______,    XXXXXXX
),

/* RAISE LAYER (Navigation & Volume)
 */
[LAYER_RAISE] = LAYOUT(
    XXXXXXX,   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,    XXXXXXX, KC_VOLU, KC_MUTE, KC_VOLD, XXXXXXX, XXXXXXX,
    XXXXXXX,   KC_LEFT, KC_UP,   KC_DOWN, KC_RGHT, XXXXXXX,    XXXXXXX, KC_RSFT, KC_RCTL, KC_RALT, KC_RGUI, XXXXXXX,
    XXXXXXX,   KC_HOME, KC_PGUP, KC_PGDN, KC_END,  XXXXXXX,    QK_BOOT, EE_CLR,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    _______,   _______, XXXXXXX,          _______, XXXXXXX,    XXXXXXX
),

/* POINTER LAYER (Trackball controls)
 */
[LAYER_POINTER] = LAYOUT(
    QK_BOOT,   EE_CLR,  XXXXXXX, XXXXXXX, DPI_MOD, S_D_MOD,    S_D_MOD, DPI_MOD, XXXXXXX, XXXXXXX, EE_CLR,  QK_BOOT,
    XXXXXXX,   KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, XXXXXXX,    XXXXXXX, KC_RSFT, KC_RCTL, KC_RALT, KC_RGUI, XXXXXXX,
    XXXXXXX,   _______, DRGSCRL, SNIPING, XXXXXXX, XXXXXXX,    XXXXXXX, XXXXXXX, SNIPING, DRGSCRL, _______, XXXXXXX,
    KC_BTN2,   KC_BTN1, KC_BTN3,          KC_BTN3, KC_BTN1,    KC_BTN2
),

/* SYM LAYER (Symbols & One-shot mods)
 */
[LAYER_SYM] = LAYOUT(
    KC_1,      KC_2,    KC_3,    KC_4,    KC_5,    KC_6,       KC_7,    KC_8,    KC_9,    KC_0,    KC_EQL,  KC_MINS,
    SCLN,      GRV,     KC_GRV,  QUOT,    KC_MINS, KC_PLUS,    OS_SHFT, OS_CTRL, OS_ALT,  OS_CMD,  KC_LBRC, KC_RBRC,
    BSL,       DPIPE,   SCLN,    KC_LBRC, KC_UNDS, KC_EQL,     KC_RBRC, CLN,     PIPE,    KC_BSLS, KC_COMM, KC_DOT,
    _______,   _______, _______,          _______, _______,    _______
),

/* NAV LAYER (Navigation & Window switching)
 */
[LAYER_NAV] = LAYOUT(
    SW_TAB,    SW_WIN,  TAB_L,   TAB_R,   KC_ESC,  KC_ESC,     KC_HOME, KC_END,  KC_BSPC, KC_DEL,  KC_INS,  KC_PGUP,
    OS_CMD,    OS_ALT,  OS_CTRL, OS_SHFT, KC_ENT,  KC_ENT,     KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, KC_PGDN, KC_PGUP,
    SPACE_L,   SPACE_R, DF(LAYER_GAM), KC_PSCR, KC_TAB, KC_TAB, KC_VOLD, KC_VOLU, KC_MUTE, KC_MPLY, KC_MSTP, KC_PGDN,
    LA_CMD,    _______, _______,          _______, _______,    _______
),

/* NUM LAYER (Tri-layer: SYM + NAV = NUM)
 */
[LAYER_NUM] = LAYOUT(
    KC_F1,     KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,      KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,
    OS_CMD,    OS_ALT,  OS_CTRL, OS_SHFT, S(KC_1), S(KC_2),    S(KC_3), S(KC_4), S(KC_5), S(KC_6), S(KC_7), S(KC_8),
    S(KC_9),   S(KC_0), S(KC_LBRC), S(KC_RBRC), S(KC_COMM), S(KC_DOT), KC_LBRC, KC_RBRC, PIPE, KC_BSLS, KC_PSLS, KC_QUES,
    _______,   _______, _______,          _______, _______,    _______
),

/* ALT LAYER (Alt-gr symbols for international chars)
 */
[LAYER_ALT] = LAYOUT(
    RALT(KC_Q), RALT(KC_W), RALT(KC_E), RALT(KC_R), RALT(KC_T), RALT(KC_Y), RALT(KC_U), RALT(KC_I), RALT(KC_O), RALT(KC_P), XXXXXXX, XXXXXXX,
    RALT(KC_A), RALT(KC_S), RALT(KC_D), RALT(KC_F), RALT(KC_G), RALT(KC_H), RALT(KC_J), RALT(KC_K), RALT(KC_L), RALT(KC_SCLN), XXXXXXX, XXXXXXX,
    RALT(KC_Z), RALT(KC_X), RALT(KC_C), RALT(KC_V), RALT(KC_B), RALT(KC_N), RALT(KC_M), RALT(KC_COMM), RALT(KC_DOT), RALT(KC_SLSH), XXXXXXX, XXXXXXX,
    _______,    NBSP,       KC_LSFT,          _______, _______,    _______
),

/* CMD LAYER (Meh keys for shortcuts + Media)
 */
[LAYER_CMD] = LAYOUT(
    MEH(KC_Q), MEH(KC_W), MEH(KC_E), MEH(KC_R), MEH(KC_T), MEH(KC_Y), MEH(KC_U), MEH(KC_I), MEH(KC_O), MEH(KC_P), KC_VOLD, KC_VOLU,
    MEH(KC_A), MEH(KC_S), MEH(KC_D), MEH(KC_F), MEH(KC_G), MEH(KC_H), MEH(KC_J), MEH(KC_K), MEH(KC_L), MEH(KC_SCLN), KC_MPRV, KC_MNXT,
    MEH(KC_Z), MEH(KC_X), MEH(KC_C), MEH(KC_V), MEH(KC_B), MEH(KC_N), MEH(KC_M), MEH(KC_COMM), MEH(KC_DOT), MEH(KC_SLSH), KC_MUTE, KC_MPLY,
    _______,   _______,   _______,          KC_LGUI, _______,    _______
),

/* GAMING LAYER
 */
[LAYER_GAM] = LAYOUT(
    KC_TAB,    KC_Q,    KC_W,    KC_E,    KC_R,    XXXXXXX,    XXXXXXX, XXXXXXX, KC_UP,   XXXXXXX, KC_TAB,  XXXXXXX,
    KC_LSFT,   KC_A,    KC_S,    KC_D,    KC_F,    XXXXXXX,    XXXXXXX, KC_LEFT, KC_DOWN, KC_RGHT, XXXXXXX, KC_RSFT,
    KC_LCTL,   KC_Z,    KC_X,    KC_C,    KC_V,    XXXXXXX,    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_RCTL,
    KC_LALT,   KC_SPC,   LA_GFN,          DF(LAYER_BASE), KC_SPC, KC_PSCR
),

};

// ============================================================================
// ONE-SHOT MODIFIER FUNCTIONS (from keymap-36.c)
// ============================================================================
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

// ============================================================================
// SWAPPER & ONE-SHOT STATE
// ============================================================================
bool sw_win_active = false;
bool sw_tab_active = false;
oneshot_state os_shft_state = os_up_unqueued;
oneshot_state os_ctrl_state = os_up_unqueued;
oneshot_state os_alt_state = os_up_unqueued;
oneshot_state os_cmd_state = os_up_unqueued;

// ============================================================================
// KEY PROCESSING
// ============================================================================
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // Handle swapper (alt-tab, ctrl-tab)
    update_swapper(
        &sw_win_active, KC_LALT, KC_TAB, SW_WIN, OS_SHFT,
        keycode, record
    );
    update_swapper(
        &sw_tab_active, KC_LCTL, KC_TAB, SW_TAB, OS_SHFT,
        keycode, record
    );
    
    // Handle one-shot modifiers
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

// ============================================================================
// LAYER STATE (Tri-layer + Charybdis sniping)
// ============================================================================
layer_state_t layer_state_set_user(layer_state_t state) {
    // Charybdis auto-sniping on pointer layer
    #ifdef CHARYBDIS_AUTO_SNIPING_ON_LAYER
    charybdis_set_pointer_sniping_enabled(layer_state_cmp(state, CHARYBDIS_AUTO_SNIPING_ON_LAYER));
    #endif
    
    // Tri-layer: SYM + NAV = NUM
    return update_tri_layer_state(state, LAYER_SYM, LAYER_NAV, LAYER_NUM);
}

// ============================================================================
// POINTING DEVICE (Auto pointer layer trigger)
// ============================================================================
#ifdef POINTING_DEVICE_ENABLE
#    ifdef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE
report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
    if (abs(mouse_report.x) > CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD || 
        abs(mouse_report.y) > CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD) {
        if (auto_pointer_layer_timer == 0) {
            layer_on(LAYER_POINTER);
#        ifdef RGB_MATRIX_ENABLE
            rgb_matrix_mode_noeeprom(RGB_MATRIX_NONE);
            rgb_matrix_sethsv_noeeprom(HSV_GREEN);
#        endif
        }
        auto_pointer_layer_timer = timer_read();
    }
    return mouse_report;
}

void matrix_scan_user(void) {
    if (auto_pointer_layer_timer != 0 && 
        TIMER_DIFF_16(timer_read(), auto_pointer_layer_timer) >= CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS) {
        auto_pointer_layer_timer = 0;
        layer_off(LAYER_POINTER);
#        ifdef RGB_MATRIX_ENABLE
        rgb_matrix_mode_noeeprom(RGB_MATRIX_DEFAULT_MODE);
#        endif
    }
}
#    endif
#endif

// ============================================================================
// RGB MATRIX (if enabled)
// ============================================================================
#ifdef RGB_MATRIX_ENABLE
void rgb_matrix_update_pwm_buffers(void);
#endif
