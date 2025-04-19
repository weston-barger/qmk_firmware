#include QMK_KEYBOARD_H
#include "state_machine.h"
#include "vim.h"

// clang-format off

#define L_SPECIAL 4
#define L_COMMAND 5

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  [L_QWERTY] = LAYOUT(
     KC_ESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                               KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_BSPC,
     KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                               KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSLS,
     KC_LCTL, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                               KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,
     KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    VIM_TOGGLE,         KC_ESC,  KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,
                              KC_LGUI, MO(L_COMMAND), KC_ENT,                    KC_SPC,  MO(L_SPECIAL), KC_RALT
  ),

  [L_INSERT] = LAYOUT(
     VIM_ESC, _______, _______, _______, _______, _______,                            _______, _______, _______, _______, _______, _______,
     _______, _______, _______, _______, _______, _______,                            _______, _______, _______, _______, _______, _______,
     _______, _______, _______, _______, _______, _______,                            _______, _______, _______, _______, _______, _______,
     _______, _______, _______, _______, _______, _______, VIM_TOGGLE,          _______, _______, _______, _______, _______, _______, _______,
                                    _______, _______, _______,                   _______, _______, _______
  ),

  [L_NORMAL] = LAYOUT(
     XXXXXXX, VIM_1, VIM_2, VIM_3, VIM_4, VIM_5,                            VIM_6, VIM_7, VIM_8, VIM_9, VIM_0, _______,
     XXXXXXX, XXXXXXX, XXXXXXX, VIM_E, VIM_R, XXXXXXX,                            VIM_Y, VIM_U, VIM_I, VIM_O, VIM_P, XXXXXXX,
     _______, VIM_A, XXXXXXX, VIM_D, XXXXXXX, XXXXXXX,                            VIM_H, VIM_J, VIM_K, VIM_L, XXXXXXX, XXXXXXX,
     _______, XXXXXXX, VIM_X, XXXXXXX, VIM_V, VIM_B, VIM_TOGGLE ,          XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______,
                                    XXXXXXX, XXXXXXX, XXXXXXX,                   XXXXXXX, XXXXXXX, XXXXXXX
  ),

  [L_VISUAL] = LAYOUT(
     VIM_ESC, _______, _______, _______, _______, _______,                            _______, _______, _______, _______, _______, VIM_BSPC,
     _______, _______, _______, _______, _______, _______,                            _______, _______, XXXXXXX, _______, _______, _______,
     _______, _______, _______, _______, _______, _______,                            _______, _______, _______, _______, _______, _______,
     _______, _______, _______, _______, VIM_V, _______, _______,          _______, _______, _______, _______, _______, _______, _______,
                                    _______, _______, _______,                   _______, _______, _______
  ),

  [L_SPECIAL] = LAYOUT(
     S(KC_GRV), XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                            XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
     XXXXXXX, XXXXXXX, XXXXXXX, S(KC_LBRC), S(KC_RBRC), TERM,                            XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
     XXXXXXX, XXXXXXX, XXXXXXX, S(KC_9), S(KC_0), XXXXXXX,                            XXXXXXX, KC_MINS, S(KC_MINS), KC_EQL, S(KC_EQL), KC_GRV,
     KC_LSFT, XXXXXXX, XXXXXXX, KC_LBRC, KC_RBRC, XXXXXXX, XXXXXXX,          QK_BOOT, XXXXXXX, XXXXXXX, S(KC_COMM), S(KC_DOT), XXXXXXX, KC_RSFT,
                                    KC_LGUI, XXXXXXX, XXXXXXX,                   XXXXXXX, XXXXXXX, XXXXXXX
  ),

  [L_COMMAND] = LAYOUT(
     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                            TAB_LEFT, TAB_RIGHT, XXXXXXX, XXXXXXX, XXXXXXX, KC_DEL,
     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                            XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
     KC_LCTL, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                            KC_LEFT, KC_DOWN, KC_UP, KC_RIGHT, XXXXXXX, XXXXXXX,
     KC_LSFT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, QK_BOOT,          XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_RSFT,
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
    return handle_sm_event(keycode, record);
}

void keyboard_post_init_user(void) {
    debug_enable   = true;
    debug_matrix   = true;
    debug_keyboard = true;
    set_single_persistent_default_layer(L_QWERTY);
    init_sm();
}

// TODO: do we need to implement this hook?
layer_state_t layer_state_set_user(layer_state_t state) {
    return state;
}
