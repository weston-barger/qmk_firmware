#pragma once
#include QMK_KEYBOARD_H

#define MAX_STATES 8

typedef struct KeyPressEvent {
    uint16_t     keycode;
    keyrecord_t *record;
} KeyPressEvent;

typedef struct KeyPressResult {
    uint16_t next_state_index;
    bool     keypress;
} KeyPressResult;

typedef void (*StateInitFn)(void);
typedef void (*StateKeyPressFn)(void);
typedef void (*StateLeaveFn)(void);

typedef struct StateMetadata {
    bool is_shifted;
} StateMetadata;

typedef struct StateData {
    uint16_t        index;
    StateInitFn     p_on_enter;
    StateKeyPressFn p_on_press;
    StateLeaveFn    p_on_leave;
    StateMetadata   metadata;
} StateData;

typedef struct EventData {
    KeyPressEvent  *event;
    KeyPressResult *result;
} EventData;

typedef struct StateMachine {
    StateData  states[MAX_STATES];
    StateData *state;
    EventData *event_data;
	int move_buff;
} StateMachine;

StateMachine *STATE_MACHINE;

// STATE DEFINITION
#define INDEX(state) S_##state

#define ENTER(state) state##_on_enter
#define PRESS(state) state##_on_press
#define LEAVE(state) state##_on_leave

#define DECLARE_STATE(state) \
    void ENTER(state)(void); \
    void PRESS(state)(void); \
    void LEAVE(state)(void);

#define DEFINE_ENTER(state) void ENTER(state)(void)
#define DEFINE_PRESS(state) void PRESS(state)(void)
#define DEFINE_LEAVE(state) void LEAVE(state)(void)

#define INIT_STATE(state)                                                            \
    STATE_MACHINE->states[INDEX(state)] = (StateData) {                              \
        INDEX(state), &ENTER(state), &PRESS(state), &LEAVE(state), (StateMetadata) { \
            false                                              \
        }                                                                            \
    }
#define MAKE_SHIFTED(state) STATE_MACHINE->states[INDEX(state)].metadata.is_shifted = true

// STATE HELPERS
#define IS_SHIFTED STATE_MACHINE->state->metadata.is_shifted
#define CURRENT_STATE STATE_MACHINE->state
#define MOVE_BUFF STATE_MACHINE->move_buff

// FUNCTIONS
// init_sm_states => needs to be implemented by user for each keyboard
void init_sm(void);
void init_sm_states(void);
bool handle_sm_event(uint16_t keycode, keyrecord_t *record);
