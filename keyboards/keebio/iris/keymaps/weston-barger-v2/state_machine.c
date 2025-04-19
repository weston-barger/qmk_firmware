// impl
#include "state_machine.h"
#include "vim.h"

void init_sm(void) {
    STATE_MACHINE                                       = (StateMachine *)malloc(sizeof(StateMachine));
    STATE_MACHINE->move_buff                            = 0;
    STATE_MACHINE->event_data                           = (EventData *)malloc(sizeof(EventData));
    STATE_MACHINE->event_data->event                    = (KeyPressEvent *)malloc(sizeof(KeyPressEvent));
    STATE_MACHINE->event_data->event->keycode           = 0;
    STATE_MACHINE->event_data->event->record            = NULL;
    STATE_MACHINE->event_data->result                   = (KeyPressResult *)malloc(sizeof(KeyPressResult));
    STATE_MACHINE->event_data->result->next_state_index = INDEX(INITIAL_STATE);
    STATE_MACHINE->event_data->result->keypress         = false;

    init_sm_states();

    STATE_MACHINE->state = &STATE_MACHINE->states[INDEX(INITIAL_STATE)];
    STATE_MACHINE->state->p_on_enter();
    return;
}

bool handle_sm_event(uint16_t keycode, keyrecord_t *record) {
    if (!record->event.pressed) {
        return true;
    }
    StateData      *state  = STATE_MACHINE->state;
    KeyPressResult *result = STATE_MACHINE->event_data->result;

    STATE_MACHINE->event_data->event->keycode = keycode;
    STATE_MACHINE->event_data->event->record  = record;

    result->next_state_index = STATE_MACHINE->state->index;
    result->keypress         = false;

    state->p_on_press();

    if (result->next_state_index != state->index) {
        if (state->p_on_leave != NULL) {
            state->p_on_leave();
        }
        state = &STATE_MACHINE->states[result->next_state_index];
        state->p_on_enter();
        STATE_MACHINE->state = state;
    }
    return !(result->keypress);
}
