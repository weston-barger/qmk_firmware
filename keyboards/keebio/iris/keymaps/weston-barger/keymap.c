// Copyright 2023 Danny Nguyen (@nooges)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

#define _DEFAULT 0
#define _VIM 1
#define _NORMAL 2
#define _VISUAL 3
#define _SPECIAL 4
#define _COMMAND 5

enum custom_keycodes { QWERTY = SAFE_RANGE, EOW, BOW, EOL, BOL, TAB_LEFT, TAB_RIGHT, TERM };

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  [_DEFAULT] = LAYOUT(
     TO(_NORMAL),  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                               KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_BSPC,
     KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                               KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSLS,
     KC_LCTL, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                               KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,
     KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    TO(_VIM),         KC_ESC,  KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,
                              KC_LGUI, MO(_COMMAND), KC_ENT,                    KC_SPC,  MO(_SPECIAL), KC_RALT
  ),

  [_VIM] = LAYOUT(
     KC_ESC, _______, _______, _______, _______, _______,                            _______, _______, _______, _______, _______, _______,
     _______, _______, _______, _______, _______, _______,                            _______, _______, _______, _______, _______, _______,
     _______, _______, _______, _______, _______, _______,                            _______, _______, _______, _______, _______, _______,
     _______, _______, _______, _______, _______, _______, TO(_DEFAULT),          _______, _______, _______, _______, _______, _______, _______,
                                    _______, _______, _______,                   _______, _______, _______
  ),

  [_NORMAL] = LAYOUT(
     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, EOL, XXXXXXX,                            XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, BOL, XXXXXXX,
     XXXXXXX, XXXXXXX, XXXXXXX, EOW, XXXXXXX, XXXXXXX,                            XXXXXXX, XXXXXXX, TO(_DEFAULT), XXXXXXX, XXXXXXX, XXXXXXX,
     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                            KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, XXXXXXX, XXXXXXX,
     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, TG(_VISUAL), BOW,          XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                                    XXXXXXX, XXXXXXX, XXXXXXX,                   XXXXXXX, XXXXXXX, XXXXXXX
  ),

  [_VISUAL] = LAYOUT(
     _______, _______, _______, _______, _______, _______,                            _______, _______, _______, _______, _______, _______,
     _______, _______, _______, _______, _______, _______,                            _______, _______, _______, _______, _______, _______,
     _______, _______, _______, _______, _______, _______,                            _______, _______, _______, _______, _______, _______,
     _______, _______, _______, _______, _______, _______, _______,          _______, _______, _______, _______, _______, _______, _______,
                                    _______, _______, _______,                   _______, _______, _______
  ),

  [_SPECIAL] = LAYOUT(
     S(KC_GRV), XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                            XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
     XXXXXXX, XXXXXXX, XXXXXXX, S(KC_LBRC), S(KC_LBRC), TERM,                            XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
     XXXXXXX, XXXXXXX, XXXXXXX, S(KC_9), S(KC_0), XXXXXXX,                            XXXXXXX, KC_MINS, S(KC_MINS), KC_EQL, S(KC_EQL), KC_GRV,
     KC_LSFT, XXXXXXX, XXXXXXX, KC_LBRC, KC_RBRC, XXXXXXX, XXXXXXX,          XXXXXXX, XXXXXXX, XXXXXXX, S(KC_COMM), S(KC_DOT), XXXXXXX, KC_RSFT,
                                    KC_LGUI, XXXXXXX, XXXXXXX,                   XXXXXXX, XXXXXXX, XXXXXXX
  ),

  [_COMMAND] = LAYOUT(
     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                            TAB_LEFT, TAB_RIGHT, XXXXXXX, XXXXXXX, XXXXXXX, KC_DEL,
     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                            XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                            KC_LEFT, KC_DOWN, KC_UP, KC_RIGHT, XXXXXXX, XXXXXXX,
     KC_LSFT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,          QK_BOOT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_RSFT,
                                    KC_LGUI, XXXXXXX, XXXXXXX,                   XXXXXXX, XXXXXXX, KC_LALT
  ),

/*
  [_BLANK] = LAYOUT(
     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                            XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                            XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                            XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,          XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                                    XXXXXXX, XXXXXXX, XXXXXXX,                   XXXXXXX, XXXXXXX, XXXXXXX
  ),
*/
};
// clang-format on

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        switch (keycode) {
            case TAB_RIGHT:
                SEND_STRING(SS_DOWN(X_LGUI) SS_DOWN(X_LALT) SS_DOWN(X_RIGHT) SS_UP(X_RIGHT) SS_UP(X_LALT) SS_UP(X_LGUI));
                return false;
            case TAB_LEFT:
                SEND_STRING(SS_DOWN(X_LGUI) SS_DOWN(X_LALT) SS_DOWN(X_LEFT) SS_UP(X_LEFT) SS_UP(X_LALT) SS_UP(X_LGUI));
                return false;
            case EOW:
                SEND_STRING(SS_DOWN(X_LALT) SS_DOWN(X_RIGHT) SS_UP(X_RIGHT) SS_UP(X_LALT));
                return false;
            case BOW:
                SEND_STRING(SS_DOWN(X_LALT) SS_DOWN(X_LEFT) SS_UP(X_LEFT) SS_UP(X_LALT));
                return false;
            case EOL:
                if (get_mods() & MOD_MASK_SHIFT) {
                    SEND_STRING(SS_DOWN(X_LGUI) SS_DOWN(X_RIGHT) SS_UP(X_RIGHT) SS_UP(X_LGUI));
                    return false;
                }
                break;
            case BOL:
                SEND_STRING(SS_DOWN(X_LGUI) SS_DOWN(X_LEFT) SS_UP(X_LEFT) SS_UP(X_LGUI));
                return false;
            case TERM:
                SEND_STRING(SS_DOWN(X_LGUI) SS_DOWN(X_LSFT) SS_DOWN(X_T) SS_UP(X_T) SS_UP(X_LSFT) SS_UP(X_LGUI));
                return false;
        }
    }

    return true;
};

void keyboard_post_init_user(void) {
    set_single_persistent_default_layer(_DEFAULT);
    rgb_matrix_disable();
}

layer_state_t layer_state_set_user(layer_state_t state) {
    if (layer_state_cmp(state, _DEFAULT)) {
        rgb_matrix_disable();
        return state;
    }
    rgb_matrix_enable();
    if (layer_state_cmp(state, _VIM)) {
        rgb_matrix_set_color_all(255, 0, 0);
    } else if (layer_state_cmp(state, _NORMAL)) {
        rgb_matrix_set_color_all(0, 255, 0);
    }
    return state;
}
