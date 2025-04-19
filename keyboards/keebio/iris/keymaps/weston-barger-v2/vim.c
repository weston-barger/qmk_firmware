#include "vim.h"
#include "print.h"

void init_sm_states(void) {
    INIT_STATE(INSERT);
    INIT_STATE(NORMAL);
    INIT_STATE(QWERTY);
    INIT_STATE(VISUAL);
    INIT_STATE(DELETE);

    MAKE_SHIFTED(VISUAL);
}

#define SEND_KEY(key) (SEND_STRING(SS_DOWN(key) SS_UP(key)))
#define ADD_SHIFT(seq) SS_DOWN(X_LSFT) seq SS_UP(X_LSFT)
#define SEND_SEQ(seq) IS_SHIFTED ? SEND_STRING(ADD_SHIFT(seq)) : SEND_STRING(seq)
#define SEND_STRING_SHIFTED(seq) SEND_STRING(ADD_SHIFT(seq))

uint16_t get_keycode(void) {
    return STATE_MACHINE->event_data->event->keycode;
}

void key_pressed(void) {
    STATE_MACHINE->event_data->result->keypress = true;
    return;
}

bool should_return(void) {
    return STATE_MACHINE->event_data->result->keypress;
}

void next_state_is(uint16_t state) {
    STATE_MACHINE->event_data->result->next_state_index = state;
    return;
}

void change_state(uint16_t new_state) {
    key_pressed();
    next_state_is(new_state);
    return;
}

#define START_MOTION for (int ii = 0; ii < (MOVE_BUFF == 0 ? 1 : MOVE_BUFF); ii++) {
#define END_MOTION \
    }              \
    reset_move_buff();

void reset_move_buff(void) {
    STATE_MACHINE->move_buff = 0;
}
void add_buff(int amount) {
    if (1 <= amount && amount <= 9) {
        MOVE_BUFF = 10 * MOVE_BUFF + amount;
    } else if (amount == 0) {
        MOVE_BUFF = 10 * MOVE_BUFF;
    }
    key_pressed();
    return;
}
void proc_number(void) {
    const uint8_t mod_state = get_mods();
    if (mod_state & MOD_MASK_SHIFT) {
        return;
    }
    switch (get_keycode()) {
        case VIM_1:
            add_buff(1);
            break;
        case VIM_2:
            add_buff(2);
            break;
        case VIM_3:
            add_buff(3);
            break;
        case VIM_4:
            add_buff(4);
            break;
        case VIM_5:
            add_buff(5);
            break;
        case VIM_6:
            add_buff(6);
            break;
        case VIM_7:
            add_buff(7);
            break;
        case VIM_8:
            add_buff(8);
            break;
        case VIM_9:
            add_buff(9);
            break;
        case VIM_0:
            if (MOVE_BUFF > 0) {
                add_buff(0);
            }
            break;
    }
    uprintf("\nkeycode = 0x%04X \nbuff = %d\n", get_keycode(), MOVE_BUFF);
    return;
}
// IMPLEMENTAION
void proc_special_keys(void) {
    // TAB_LEFT, TAB_RIGHT, TERM
    switch (get_keycode()) {
        case TAB_RIGHT:
            SEND_STRING(TAB_RIGHT_SEQ);
            key_pressed();
            return;
        case TAB_LEFT:
            SEND_STRING(TAB_LEFT_SEQ);
            key_pressed();
            return;
        case TERM:
            SEND_STRING(TERM_SEQ);
            key_pressed();
            return;
    }
}
// TODO: split into shifted and non-shifted
void proc_nav_keys(void) {
    switch (get_keycode()) {
        case VIM_E:
            START_MOTION
            SEND_SEQ(VIM_E_SEQ);
            END_MOTION
            key_pressed();
            return;
        case VIM_B:
            START_MOTION
            SEND_SEQ(VIM_B_SEQ);
            END_MOTION
            key_pressed();
            return;
        case VIM_H:
            START_MOTION
            SEND_SEQ(VIM_H_SEQ);
            END_MOTION
            key_pressed();
            return;
        case VIM_J:
            START_MOTION
            SEND_SEQ(VIM_J_SEQ);
            END_MOTION
            key_pressed();
            return;
        case VIM_K:
            START_MOTION
            SEND_SEQ(VIM_K_SEQ);
            END_MOTION
            key_pressed();
            return;
        case VIM_L:
            START_MOTION
            SEND_SEQ(VIM_L_SEQ);
            END_MOTION
            key_pressed();
            return;
        case VIM_0:
            SEND_SEQ(VIM_0_SEQ);
            key_pressed();
            return;
    }

    const uint8_t mod_state = get_mods();
    switch (get_keycode()) {
        case VIM_4:
            if (mod_state & MOD_MASK_SHIFT) {
                del_mods(mod_state);
                SEND_SEQ(VIM_4_SEQ);
                set_mods(mod_state);
            }
            key_pressed();
            return;
    }
}

void proc_clipboard_keys(void) {
    switch (get_keycode()) {
        case VIM_Y:
            SEND_STRING(COPY_SEQ);
            key_pressed();
            reset_move_buff();
            return;
        case VIM_P:
            SEND_STRING(PASTE_SEQ);
            reset_move_buff();
            key_pressed();
            return;
        case VIM_U:
            SEND_STRING(UNDO_SEQ);
            reset_move_buff();
            key_pressed();
            return;
    }
    return;
}

DEFINE_ENTER(QWERTY) {
    layer_move(L_QWERTY);
    rgb_matrix_enable();
    rgb_matrix_set_flags(LED_FLAG_ALL); // do we need this?
    rgb_matrix_mode(RGB_MATRIX_SOLID_COLOR);
    rgb_matrix_sethsv(HSV_RED);
    return;
}
DEFINE_PRESS(QWERTY) {
    switch (get_keycode()) {
        case VIM_TOGGLE:
            change_state(INDEX(INSERT));
            return;
    }
    proc_special_keys();
    return;
}
DEFINE_LEAVE(QWERTY) {
    return;
}

DEFINE_ENTER(INSERT) {
    layer_move(L_INSERT);
    rgb_matrix_disable();
    return;
}

DEFINE_PRESS(INSERT) {
    switch (get_keycode()) {
        case VIM_TOGGLE:
            change_state(INDEX(QWERTY));
            return;
        case VIM_ESC:
            change_state(INDEX(NORMAL));
            return;
    }
    proc_special_keys();
    return;
}

DEFINE_LEAVE(INSERT) {
    return;
}

DEFINE_ENTER(NORMAL) {
    reset_move_buff();
    layer_move(L_NORMAL);
    rgb_matrix_enable();
    rgb_matrix_set_flags(LED_FLAG_ALL); // do we need this?
    rgb_matrix_mode(RGB_MATRIX_SOLID_COLOR);
    rgb_matrix_sethsv(HSV_GREEN);
    return;
}
DEFINE_PRESS(NORMAL) {
    switch (get_keycode()) {
        case VIM_TOGGLE:
            change_state(INDEX(QWERTY));
            return;
        case VIM_I:
            change_state(INDEX(INSERT));
            return;
        case VIM_V:
            change_state(INDEX(VISUAL));
            return;
    }

    proc_number();
    if (should_return()) {
        return;
    }

    proc_clipboard_keys();
    if (should_return()) {
        return;
    }

    proc_nav_keys();
    if (should_return()) {
        return;
    }

    const uint8_t mod_state = get_mods();
    switch (get_keycode()) {
        case VIM_D:
            if (mod_state & MOD_MASK_SHIFT) {
                del_mods(mod_state);
                SEND_STRING_SHIFTED(VIM_4_SEQ);
                SEND_KEY(X_DEL);
                set_mods(mod_state);
                reset_move_buff();
                key_pressed();
            } else {
                change_state(INDEX(DELETE));
            }
            return;
        case VIM_A:
            if (mod_state & MOD_MASK_SHIFT) {
                del_mods(mod_state);
                SEND_STRING(VIM_4_SEQ);
                set_mods(mod_state);
                change_state(INDEX(INSERT));
                reset_move_buff();
            }
            key_pressed();
            return;
        case VIM_O:
            if (mod_state & MOD_MASK_SHIFT) {
                del_mods(mod_state);
                SEND_STRING(VIM_0_SEQ);
                SEND_KEY(X_ENTER);
                SEND_KEY(X_UP);
                set_mods(mod_state);
            } else {
                SEND_STRING(VIM_4_SEQ);
                SEND_KEY(X_ENTER);
            }
            change_state(INDEX(INSERT));
            reset_move_buff();
            return;
        case VIM_X:
            if (mod_state & MOD_MASK_SHIFT) {
                del_mods(mod_state);
                START_MOTION
                SEND_KEY(X_BSPC);
                END_MOTION
                set_mods(mod_state);
            } else {
                START_MOTION
                SEND_KEY(X_DEL);
                END_MOTION
            }
            key_pressed();
            return;
        case VIM_R:
            if (mod_state & MOD_MASK_CTRL) {
                del_mods(mod_state);
                SEND_STRING(REDO_SEQ);
                set_mods(mod_state);
                key_pressed();
            } else {
                START_MOTION
                SEND_STRING_SHIFTED(VIM_L_SEQ);
                END_MOTION
                change_state(INDEX(INSERT));
            }
            reset_move_buff();
            return;
    }
    return;
}
DEFINE_LEAVE(NORMAL) {
    return;
}

DEFINE_ENTER(VISUAL) {
    layer_on(L_VISUAL);
    rgb_matrix_enable();
    rgb_matrix_set_flags(LED_FLAG_ALL); // do we need this?
    rgb_matrix_mode(RGB_MATRIX_SOLID_COLOR);
    rgb_matrix_sethsv(HSV_YELLOW);
    return;
}
DEFINE_PRESS(VISUAL) {
    switch (get_keycode()) {
        case VIM_V:
        case VIM_ESC:
            change_state(INDEX(NORMAL));
            return;
    }
    proc_number();
    if (should_return()) {
        return;
    }

    proc_clipboard_keys();
    if (should_return()) {
        change_state(INDEX(NORMAL));
        return;
    }

    proc_nav_keys();
    if (should_return()) {
        return;
    }

    switch (get_keycode()) {
        case VIM_D:
            SEND_STRING(CUT_SEQ);
            change_state(INDEX(NORMAL));
            return;
        case VIM_X:
            SEND_STRING(CUT_SEQ);
            change_state(INDEX(NORMAL));
            return;
    }

    return;
}
DEFINE_LEAVE(VISUAL) {
    layer_off(L_VISUAL);
    reset_move_buff();
    return;
}

DEFINE_ENTER(DELETE) {
    rgb_matrix_enable();
    rgb_matrix_set_flags(LED_FLAG_ALL); // do we need this?
    rgb_matrix_mode(RGB_MATRIX_SOLID_COLOR);
    rgb_matrix_sethsv(HSV_BLUE);
    return;
}
DEFINE_PRESS(DELETE) {
    switch (get_keycode()) {
        case KC_LSFT:
        case KC_RSFT:
            return;
    }

    proc_number();
    if (should_return()) {
        return;
    }

    const uint8_t mod_state = get_mods();
    switch (get_keycode()) {
        case VIM_D:
            del_mods(mod_state);
            SEND_STRING(VIM_0_SEQ);
            SEND_STRING_SHIFTED(VIM_4_SEQ);
            SEND_STRING(SS_DELAY(10));
            SEND_KEY(X_DEL);
            SEND_KEY(X_DEL);
            set_mods(mod_state);
            change_state(INDEX(NORMAL));
            reset_move_buff();
            return;
    }

    STATE_MACHINE->state->metadata.is_shifted = true;
    proc_nav_keys();
    STATE_MACHINE->state->metadata.is_shifted = false;
    if (STATE_MACHINE->event_data->result->keypress) {
        SEND_STRING(SS_DELAY(10));
        SEND_KEY(X_DEL);
    }
    change_state(INDEX(NORMAL));
    reset_move_buff();
    return;
}
DEFINE_LEAVE(DELETE) {
    return;
}
