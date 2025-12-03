#include QMK_KEYBOARD_H
#include "keymap_introspection.h"

// TAP DANCES
enum {
  TD_LALT_RALT,
};

// https://docs.qmk.fm/features/tap_dance#example-3
typedef struct {
    uint16_t tap;
    uint16_t hold;
    uint16_t held;
} tap_dance_tap_hold_t;

void tap_dance_tap_hold_finished(tap_dance_state_t *state, void *user_data) {
    tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)user_data;

    if (state->pressed) {
        if (state->count == 1
#ifndef PERMISSIVE_HOLD
            && !state->interrupted
#endif
        ) {
            register_code16(tap_hold->hold);
            tap_hold->held = tap_hold->hold;
        } else {
            register_code16(tap_hold->tap);
            tap_hold->held = tap_hold->tap;
        }
    }
}

void tap_dance_tap_hold_reset(tap_dance_state_t *state, void *user_data) {
    tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)user_data;

    if (tap_hold->held) {
        unregister_code16(tap_hold->held);
        tap_hold->held = 0;
    }
}

#define ACTION_TAP_DANCE_TAP_HOLD(tap, hold)                                        \
    {                                                                               \
        .fn        = {NULL, tap_dance_tap_hold_finished, tap_dance_tap_hold_reset}, \
        .user_data = (void *)&((tap_dance_tap_hold_t){tap, hold, 0}),               \
    }

tap_dance_action_t tap_dance_actions[] = {
    [TD_LALT_RALT] = ACTION_TAP_DANCE_TAP_HOLD(KC_LALT, KC_RALT),
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    tap_dance_action_t *action;
    tap_dance_state_t* state;

    switch (keycode) {
        case TD(TD_LALT_RALT):
            action = tap_dance_get(QK_TAP_DANCE_GET_INDEX(keycode));
            state = tap_dance_get_state(QK_TAP_DANCE_GET_INDEX(keycode));
            if (!record->event.pressed && state != NULL && state->count && !state->finished) {
                tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)action->user_data;
                tap_code16(tap_hold->tap);
            }
    }
    return true;
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                               ,-----------------------------------------------------------.
       KC_TAB,    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                                  KC_Y,   KC_U,    KC_I,    KC_O,   KC_P,            KC_ESC,
  //|--------+--------+--------+--------+--------+--------|                               |------+-------+--------+--------+--------+-----------------|
      KC_LSFT,    KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                                  KC_H,   KC_J,    KC_K,    KC_L, KC_SCLN,          KC_BSPC,
  //|--------+--------+--------+--------+--------+--------|                               |------+-------+--------+--------+--------+-----------------|
      KC_LCTL,    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                                  KC_N,   KC_M, KC_COMM,  KC_DOT, KC_SLSH, TD(TD_LALT_RALT),
  //|--------+--------+--------+--------+--------+--------+--------|             |--------+------+-------+--------+--------+--------+-----------------|
                                          KC_LGUI,   MO(1),  KC_SPC,                KC_ENT, MO(2), 0x5028
                                      //`--------------------------'             `-----------------------'

  ),

  [1] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------------------.                    ,-------------------------------------------------------------------------.
       KC_GRV,    KC_1,          KC_2,    KC_3,       KC_4,       KC_5,                               KC_6,         KC_7,    KC_8,    KC_9,    KC_0,          KC_PMNS,
  //|--------+--------+--------------+--------+-----------+-----------|                    |--------------+-------------+--------+--------+--------+-----------------|
      KC_LSFT, KC_CAPS,    RALT(KC_6),    KC_5, RALT(KC_4), RALT(KC_5),                      RALT(KC_MINS), RALT(KC_EQL), KC_MINS, KC_LBRC, KC_RBRC,           KC_EQL,
  //|--------+--------+--------------+--------+-----------+-----------|                    |--------------+-------------+--------+--------+--------+-----------------|
      KC_LCTL, KC_NUBS, LSFT(KC_NUBS), MS_BTN1,      MS_UP,    MS_LEFT,                            MS_RGHT,      MS_DOWN, MS_BTN2, KC_QUOT, KC_NUHS, TD(TD_LALT_RALT),
  //|--------+--------+--------------+--------+-----------+-----------+-------|   |--------+--------------+-------------+--------+--------+--------+-----------------|
                                                   KC_LGUI,    _______, KC_SPC,      KC_ENT,         MO(3),       0x5028
                                               //`----------------------------'   `-------------------------------------'
  ),

  [2] = LAYOUT_split_3x6_3(
  //,--------------------------------------------------------------------.                   ,--------------------------------------------------------------------.
       KC_TAB, LSFT(KC_1), LSFT(KC_2), LSFT(KC_3), LSFT(KC_4), LSFT(KC_5),                     LSFT(KC_6), LSFT(KC_7), LSFT(KC_8), LSFT(KC_9), LSFT(KC_0), KC_ESC,
  //|--------+-----------+-----------+-----------+-----------+-----------|                   |-----------+-----------+-----------+-----------+-----------+--------|
      KC_LSFT, RALT(KC_1), RALT(KC_2), RALT(KC_3), RALT(KC_4), RALT(KC_5),                     RALT(KC_6), RALT(KC_7), RALT(KC_8), RALT(KC_9), RALT(KC_0), KC_F12,
  //|--------+-----------+-----------+-----------+-----------+-----------|                   |-----------+-----------+-----------+-----------+-----------+--------|
      KC_LCTL,      KC_F1,      KC_F1,      KC_F1,      KC_F1,      KC_F1,                          KC_F6,      KC_F7,      KC_F8,      KC_F9,     KC_F10, KC_F11,
  //|--------+-----------+-----------+-----------+-----------+-----------+-------|   |-------+-----------+-----------+-----------+-----------+-----------+--------|
                                                      KC_LGUI,      MO(3), KC_SPC,     KC_ENT,    _______,     0x5028
                                                  //`----------------------------'   `-------------------------------'
  ),

  [3] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,--------+--------+--------+--------+--------+--------------.
      QK_BOOT, KC_MUTE, KC_VOLD, KC_VOLU, KC_PSCR, MS_WHLU,                      KC_PGUP, KC_HOME,   KC_UP,  KC_END, KC_BSPC,  LCTL(KC_EQL),
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------------|
      KC_LSFT, RM_TOGG, RM_HUEU, RM_SATU, RM_VALU, MS_WHLD,                      KC_PGDN, KC_LEFT, KC_DOWN, KC_RGHT,  KC_DEL, LCTL(KC_PMNS),
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------------|
      KC_LCTL, KC_MPLY, RM_HUED, RM_SATD, RM_VALD, RM_NEXT,                      RM_SPDU, RM_SPDD,   KC_NO,   KC_NO,   KC_NO,       KC_LALT,
  //|--------+--------+--------+--------+--------+--------+-------|    |-------+--------+--------+--------+--------+--------+--------------|
                                          KC_LGUI, _______, KC_SPC,      KC_ENT, _______,  0x5028
                                      //`-------------------------'    `-------------------------'
  )
};
