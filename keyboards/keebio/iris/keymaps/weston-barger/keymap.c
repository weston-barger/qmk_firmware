#include QMK_KEYBOARD_H

#define _QWERTY 0
#define _INSERT 1
#define _NORMAL 2
#define _VISUAL 3
#define _SPECIAL 4
#define _COMMAND 5

enum custom_keycodes { VIM_E = SAFE_RANGE, VIM_B, VIM_4, VIM_A, VIM_0, VIM_H, VIM_J, VIM_K, VIM_L, VIM_D, VIM_X, VIM_BSPC, VIM_Y, VIM_P, VIM_U, VIM_R, VIM_V, TAB_LEFT, TAB_RIGHT, TERM };

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  [_QWERTY] = LAYOUT(
     KC_ESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                               KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_BSPC,
     KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                               KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSLS,
     KC_LCTL, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                               KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,
     KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    TO(_INSERT),         KC_ESC,  KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,
                              KC_LGUI, MO(_COMMAND), KC_ENT,                    KC_SPC,  MO(_SPECIAL), KC_RALT
  ),

  [_INSERT] = LAYOUT(
     TO(_NORMAL), _______, _______, _______, _______, _______,                            _______, _______, _______, _______, _______, _______,
     _______, _______, _______, _______, _______, _______,                            _______, _______, _______, _______, _______, _______,
     _______, _______, _______, _______, _______, _______,                            _______, _______, _______, _______, _______, _______,
     _______, _______, _______, _______, _______, _______, TO(_QWERTY),          _______, _______, _______, _______, _______, _______, _______,
                                    _______, _______, _______,                   _______, _______, _______
  ),

  [_NORMAL] = LAYOUT(
     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, VIM_4, XXXXXXX,                            XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, VIM_0, XXXXXXX,
     XXXXXXX, XXXXXXX, XXXXXXX, VIM_E, VIM_R, XXXXXXX,                            VIM_Y, VIM_U, TO(_INSERT), XXXXXXX, VIM_P, XXXXXXX,
     _______, VIM_A, XXXXXXX, VIM_D, XXXXXXX, XXXXXXX,                            VIM_H, VIM_J, VIM_K, VIM_L, XXXXXXX, XXXXXXX,
     _______, XXXXXXX, VIM_X, XXXXXXX, VIM_V, VIM_B, TO(_QWERTY),          XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______,
                                    XXXXXXX, XXXXXXX, XXXXXXX,                   XXXXXXX, XXXXXXX, XXXXXXX
  ),

  [_VISUAL] = LAYOUT(
     _______, _______, _______, _______, _______, _______,                            _______, _______, _______, _______, _______, VIM_BSPC,
     _______, _______, _______, _______, _______, _______,                            _______, _______, XXXXXXX, _______, _______, _______,
     _______, _______, _______, _______, _______, _______,                            _______, _______, _______, _______, _______, _______,
     _______, _______, _______, _______, VIM_V, _______, _______,          _______, _______, _______, _______, _______, _______, _______,
                                    _______, _______, _______,                   _______, _______, _______
  ),

  [_SPECIAL] = LAYOUT(
     S(KC_GRV), XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                            XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
     XXXXXXX, XXXXXXX, XXXXXXX, S(KC_LBRC), S(KC_RBRC), TERM,                            XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
     XXXXXXX, XXXXXXX, XXXXXXX, S(KC_9), S(KC_0), XXXXXXX,                            XXXXXXX, KC_MINS, S(KC_MINS), KC_EQL, S(KC_EQL), KC_GRV,
     KC_LSFT, XXXXXXX, XXXXXXX, KC_LBRC, KC_RBRC, XXXXXXX, XXXXXXX,          QK_BOOT, XXXXXXX, XXXXXXX, S(KC_COMM), S(KC_DOT), XXXXXXX, KC_RSFT,
                                    KC_LGUI, XXXXXXX, XXXXXXX,                   XXXXXXX, XXXXXXX, XXXXXXX
  ),

  [_COMMAND] = LAYOUT(
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

enum keeb_modes { M_INSERT, M_QWERTY, M_NORMAL, M_VISUAL };
enum keeb_modes keeb_mode = M_INSERT;

bool delete_mode = false;

#define TAB_RIGHT_SEQ SS_DOWN(X_LGUI) SS_DOWN(X_LALT) SS_DOWN(X_RIGHT) SS_UP(X_RIGHT) SS_UP(X_LALT) SS_UP(X_LGUI)
#define TAB_LEFT_SEQ SS_DOWN(X_LGUI) SS_DOWN(X_LALT) SS_DOWN(X_LEFT) SS_UP(X_LEFT) SS_UP(X_LALT) SS_UP(X_LGUI)
#define VIM_E_SEQ SS_DOWN(X_LALT) SS_DOWN(X_RIGHT) SS_UP(X_RIGHT) SS_UP(X_LALT)
#define VIM_B_SEQ SS_DOWN(X_LALT) SS_DOWN(X_LEFT) SS_UP(X_LEFT) SS_UP(X_LALT)
#define VIM_H_SEQ SS_DOWN(X_LEFT) SS_UP(X_LEFT)
#define VIM_J_SEQ SS_DOWN(X_DOWN) SS_UP(X_DOWN)
#define VIM_K_SEQ SS_DOWN(X_UP) SS_UP(X_UP)
#define VIM_L_SEQ SS_DOWN(X_RIGHT) SS_UP(X_RIGHT)
#define VIM_4_SEQ SS_DOWN(X_LGUI) SS_DOWN(X_RIGHT) SS_UP(X_RIGHT) SS_UP(X_LGUI)
#define VIM_0_SEQ SS_DOWN(X_LGUI) SS_DOWN(X_LEFT) SS_UP(X_LEFT) SS_UP(X_LGUI)
#define TERM_SEQ SS_DOWN(X_LGUI) SS_DOWN(X_LSFT) SS_DOWN(X_T) SS_UP(X_T) SS_UP(X_LSFT) SS_UP(X_LGUI)

#define COPY_SEQ SS_DOWN(X_LGUI) SS_DOWN(X_C) SS_UP(X_C) SS_UP(X_LGUI)
#define CUT_SEQ SS_DOWN(X_LGUI) SS_DOWN(X_X) SS_UP(X_X) SS_UP(X_LGUI)
#define PASTE_SEQ SS_DOWN(X_LGUI) SS_DOWN(X_V) SS_UP(X_V) SS_UP(X_LGUI)
#define UNDO_SEQ SS_DOWN(X_LGUI) SS_DOWN(X_Z) SS_UP(X_Z) SS_UP(X_LGUI)
#define REDO_SEQ SS_DOWN(X_LSFT) SS_DOWN(X_LGUI) SS_DOWN(X_Z) SS_UP(X_Z) SS_UP(X_LGUI) SS_UP(X_LSFT)

#define WIGGLE_SEQ SS_DOWN(X_LEFT) SS_UP(X_LEFT) SS_DOWN(X_RIGHT) SS_UP(X_RIGHT)

#define ADD_SHIFT(expr) SS_DOWN(X_LSFT) expr SS_UP(X_LSFT)
#define VISUAL_SS(seq) (keeb_mode == M_VISUAL ? SEND_STRING(ADD_SHIFT(seq)) : SEND_STRING(seq))

#define SEND_KEY(key) (SEND_STRING(SS_DOWN(key) SS_UP(key)))

/*
 * IMPLMENTATIONS
 */

bool is_vim_nav(uint16_t keycode, keyrecord_t *record) {
    if (!record->event.pressed) {
        return false;
    }
    if (keycode == VIM_4 && get_mods() & MOD_MASK_SHIFT) {
        return true;
    }
    // Bug: vim_4 is in this range
    return VIM_E <= keycode && keycode <= VIM_L;
}

bool is_nav_to_insert(uint16_t keycode, keyrecord_t *record) {
    if (!record->event.pressed) {
        return false;
    }
    return keycode == VIM_A;
}

bool process_vim_nav(uint16_t keycode, keyrecord_t *record) {
    const uint8_t mod_state = get_mods();
    if (record->event.pressed) {
        switch (keycode) {
            case VIM_E:
                SEND_STRING(VIM_E_SEQ);
                return false;
            case VIM_B:
                SEND_STRING(VIM_B_SEQ);
                return false;
            case VIM_H:
                SEND_STRING(VIM_H_SEQ);
                return false;
            case VIM_J:
                SEND_STRING(VIM_J_SEQ);
                return false;
            case VIM_K:
                SEND_STRING(VIM_K_SEQ);
                return false;
            case VIM_L:
                SEND_STRING(VIM_L_SEQ);
                return false;
            case VIM_A:
                if (delete_mode) {
                    return false;
                }
            case VIM_4:
                if (mod_state & MOD_MASK_SHIFT) {
                    del_mods(mod_state);
                    VISUAL_SS(VIM_4_SEQ);
                    set_mods(mod_state);
                    return false;
                }
                break;
            case VIM_0:
                SEND_STRING(VIM_0_SEQ);
                return false;
        }
    }
    return true;
}

bool is_vim_edit(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        return VIM_X <= keycode && keycode <= VIM_P;
    }
    return false;
}

bool process_vim_edit(uint16_t keycode, keyrecord_t *record) {
    const uint8_t mod_state = get_mods();

    if (record->event.pressed) {
        switch (keycode) {
            case VIM_X:
                if (mod_state & MOD_MASK_SHIFT) {
                    del_mods(mod_state);
                    SEND_KEY(X_BSPC);
                    set_mods(mod_state);
                } else {
                    SEND_KEY(X_DEL);
                }
                return false;
            case VIM_Y:
                SEND_STRING(COPY_SEQ);
                return false;
            case VIM_P:
                SEND_STRING(PASTE_SEQ);
                return false;
        }
    }
    return true;
}

bool is_vim_oops(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        return VIM_U <= keycode && keycode <= VIM_R;
    }
    return false;
}

bool process_vim_oops(uint16_t keycode, keyrecord_t *record) {
    const uint8_t mod_state = get_mods();
    if (record->event.pressed) {
        switch (keycode) {
            case VIM_U:
                SEND_STRING(UNDO_SEQ);
                return false;
            case VIM_R:
                if (mod_state & MOD_MASK_CTRL) {
                    del_mods(mod_state);
                    SEND_STRING(REDO_SEQ);
                    set_mods(mod_state);
                    return false;
                }
        }
    }
    return true;
}

bool is_vim_delete(uint16_t keycode, keyrecord_t *record) {
    return record->event.pressed && keycode == VIM_D;
}

bool is_visual_toggle(uint16_t keycode, keyrecord_t *record) {
    return record->event.pressed && keycode == VIM_V;
}

bool process_vim_del(uint16_t keycode, keyrecord_t *record) {
    const uint8_t mod_state = get_mods();
    if (record->event.pressed) {
        switch (keycode) {
            case VIM_D:
                if (!delete_mode && mod_state & MOD_MASK_SHIFT) {
                    del_mods(mod_state);
                    SEND_STRING(ADD_SHIFT(VIM_4_SEQ));
                    SEND_KEY(X_DEL);
                    set_mods(mod_state);
                } else if (delete_mode) {
                    del_mods(mod_state);
                    SEND_STRING(VIM_0_SEQ);
                    add_oneshot_mods(MOD_MASK_SHIFT);
                    SEND_STRING(VIM_4_SEQ);
                    del_oneshot_mods(MOD_MASK_SHIFT);
                    SEND_KEY(X_DEL);
                    SEND_KEY(X_DEL);
                    set_mods(mod_state);

                } else {
                    delete_mode = true;
                }
                return false;
        }
    }
    return true;
}

bool process_delete_mode_user(uint16_t keycode, keyrecord_t *record) {
    if (!record->event.pressed) {
        return true;
    }
    if (keycode == KC_RSFT || keycode == KC_LSFT) {
        return true;
    }
    if (is_vim_nav(keycode, record)) {
        add_oneshot_mods(MOD_MASK_SHIFT);
        process_vim_nav(keycode, record);
        del_oneshot_mods(MOD_MASK_SHIFT);
        SEND_KEY(X_DEL);
    } else if (is_vim_delete(keycode, record)) {
        process_vim_del(keycode, record);
    }
    delete_mode = false;
    return false;
}

bool process_normal_mode_user(uint16_t keycode, keyrecord_t *record) {
   bool ret;
    if (delete_mode) {
        return process_delete_mode_user(keycode, record);
    } else if (is_vim_delete(keycode, record)) {
        return process_vim_del(keycode, record);
    } else if (is_vim_nav(keycode, record)) {
         ret= process_vim_nav(keycode, record);
         if (!ret && is_nav_to_insert(keycode, record)){
            layer_move(_INSERT);
         }
         return ret;
    } else if (is_vim_edit(keycode, record)) {
        return process_vim_edit(keycode, record);
    } else if (is_vim_oops(keycode, record)) {
        return process_vim_oops(keycode, record);
    } else if (is_visual_toggle(keycode, record)) {
        layer_on(_VISUAL);
        return false;
    }
    return true;
}

bool process_visual_mode_user(uint16_t keycode, keyrecord_t *record) {
    bool ret = true;
    if (is_vim_nav(keycode, record)) {
        add_oneshot_mods(MOD_MASK_SHIFT);
        ret = process_vim_nav(keycode, record);
        del_oneshot_mods(MOD_MASK_SHIFT);
        return ret;
    }
    if (is_vim_delete(keycode, record)) {
        SEND_KEY(X_DEL);
        ret = false;
        SEND_STRING(WIGGLE_SEQ);
    } else if (is_vim_edit(keycode, record)) {
        SEND_STRING(COPY_SEQ);
        ret = process_vim_edit(keycode, record);
        SEND_STRING(WIGGLE_SEQ);
    } else if (is_visual_toggle(keycode, record)) {
        ret = false;
    }
    if (!ret) {
        layer_move(_NORMAL);
    }
    return ret;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (keeb_mode == M_VISUAL) {
        return process_visual_mode_user(keycode, record);
    }
    if (keeb_mode == M_NORMAL) {
        return process_normal_mode_user(keycode, record);
    }

    if (record->event.pressed) {
        switch (keycode) {
            case TAB_RIGHT:
                SEND_STRING(TAB_RIGHT_SEQ);
                return false;
            case TAB_LEFT:
                SEND_STRING(TAB_LEFT_SEQ);
                return false;
            case TERM:
                SEND_STRING(TERM_SEQ);
                return false;
        }
    }
    return true;
};

void keyboard_post_init_user(void) {
    // debug_enable = true;
    // debug_matrix = true;
    set_single_persistent_default_layer(_QWERTY);
    layer_move(_INSERT);
    rgb_matrix_disable();
}

void update_keeb_mode(layer_state_t state) {
    if (layer_state_cmp(state, _QWERTY)) {
        keeb_mode = M_QWERTY;
    } else if (layer_state_cmp(state, _INSERT)) {
        keeb_mode = M_INSERT;
    } else if (layer_state_cmp(state, _VISUAL)) {
        keeb_mode = M_VISUAL;
    } else if (layer_state_cmp(state, _NORMAL)) {
        keeb_mode = M_NORMAL;
    }
    delete_mode = false;
    return;
}

layer_state_t layer_state_set_user(layer_state_t state) {
    // STATE UPDATE
    update_keeb_mode(state);

    // COLORS
    if (keeb_mode == M_INSERT) {
        rgb_matrix_disable();
        return state;
    }
    rgb_matrix_enable();
    rgb_matrix_set_flags(LED_FLAG_ALL); // do we need this?
    rgb_matrix_mode(RGB_MATRIX_SOLID_COLOR);
    if (keeb_mode == M_QWERTY) {
        rgb_matrix_sethsv(HSV_RED);
    } else if (keeb_mode == M_NORMAL) {
        rgb_matrix_sethsv(HSV_GREEN);
    } else if (keeb_mode == M_VISUAL) {
        rgb_matrix_sethsv(HSV_YELLOW);
    }
    return state;
}
