#include QMK_KEYBOARD_H
#include "keymap_us_international.h"

bool is_alt_tab_active = false;
uint16_t alt_tab_timer = 0;

bool is_page_scroll_layer = false;
bool is_alt_tab_layer = false;

enum Layers {
    _QWERTY,
    _2WERTY,
    _LOWER,
    _RAISE,
    _ADJUST,
    _GAME,
};

typedef struct {
    bool is_press_action;
    uint8_t state;
} tap;

enum {
    SINGLE_TAP,
    SINGLE_HOLD,
    DOUBLE_TAP,
};

enum {
    MUTE_LAYER2, // Our custom tap dance key; add any other tap dance keys to this enum
    SPACE_RAISE,
    ENCODER_LAYER,
};

// Function to determine the current tapdance state
uint8_t cur_dance(qk_tap_dance_state_t *state);

// `finished` and `reset` functions for each tapdance keycode
void spaceraise_finished(qk_tap_dance_state_t *state, void *user_data);
void spaceraise_reset(qk_tap_dance_state_t *state, void *user_data);
void encoder_finished(qk_tap_dance_state_t *state, void *user_data);
void encoder_reset(qk_tap_dance_state_t *state, void *user_data);

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
 /*
 * Base Layer: 1. QWERTY (Windows)
 *
 * ,---------------------------------------------------.                ,-------------------------------------------------.
 * | ESC    |   1  |   2  |   3  |   4  |   5  |       |                |      |   6  |   7  |   8  |   9  |   0  |  BKSP |
 * |--------+------------+------+------+------+--------|                |------+------------+------+------+------+--------|
 * | TAB    |   Q  |   W  |   E  |   R  |   T  |       |                |      |   Y  |   U  |   I  |   O  |   P  |       |
 * |--------+------------+------+------+------+--------|                |------+------------+------+------+------+--------|
 * | LSHIFT |   A  |   S  |  D   |   F  |   G  |       |                |      |   H  |   J  |   K  |   L  | ;  : |  ' "  |
 * |--------+------------+------+------+------+------+--------.  ,-------------+------+------------+------+------+------+-|
 * | CTRL   |   Z  |   X  |   C  |   V  |   B  |A+SPACE| CAPS |  |WiTab |S-PSCR|   N  |   M  | ,  < | . >  | /  ? |  -_   |
 * `----------------------------+------+------+------+-------+|  |------+------+------+------------+------+---------------'
 *                        | TG(1)|Alt   | Win  | Space | ENTER|  |Enter | Space| Win  | Alt  | TG(2)|
 *                        |M LAY |      |      |       | LOWER|  |LOWER | RAISE|      |      |      |
 *                        `-----------------------------------'  `----------------------------------'
 */
   [_QWERTY] = LAYOUT(
     KC_ESC      , KC_1      , KC_2      , KC_3       , KC_4       , KC_5       , LCTL(KC_GRV)  ,                                               KC_ESC        , KC_6    , KC_7    , KC_8     , KC_9   , KC_0     , KC_BSPC   ,
     KC_TAB      , KC_Q      , KC_W      , KC_E       , KC_R       , KC_T       , _______       ,                                               _______       , KC_Y    , KC_U    , KC_I     , KC_O   , KC_P     , KC_LBRC   ,
     KC_LSFT     , KC_A      , KC_S      , KC_D       , KC_F       , KC_G       , _______       ,                                               _______       , KC_H    , KC_J    , KC_K     , KC_L   , KC_SCLN  , KC_QUOT   ,
     KC_LCTL     , KC_Z      , KC_X      , KC_C       , KC_V       , KC_B       , A(KC_SPC)     , KC_CAPS                , LGUI(KC_TAB)       , S(KC_PSCR)    , KC_N    , KC_M    , KC_COMM  , KC_DOT , KC_SLSH  , KC_MINS   ,
                                     TD(MUTE_LAYER2)  , KC_LALT    , KC_LGUI    , KC_SPC        , LT(_LOWER, KC_ENT)     , LT(_LOWER, KC_ENT) , LT(_RAISE, KC_SPC), KC_RGUI , KC_RALT , TD(ENCODER_LAYER)
  ),
/*
 * Base Layer: 2. QWERTY (Mac)
 *
 * ,---------------------------------------------------.                ,-------------------------------------------------.
 * | ESC    |   1  |   2  |   3  |   4  |   5  |       |                |      |   6  |   7  |   8  |   9  |   0  |  BKSP |
 * |--------+------------+------+------+------+--------|                |------+------------+------+------+------+--------|
 * | TAB    |   Q  |   W  |   E  |   R  |   T  |       |                |      |   Y  |   U  |   I  |   O  |   P  |       |
 * |--------+------------+------+------+------+--------|                |------+------------+------+------+------+--------|
 * | LSHIFT |   A  |   S  |  D   |   F  |   G  |       |                |      |   H  |   J  |   K  |   L  | ;  : |  ' "  |
 * |--------+------------+------+------+------+------+--------.  ,-------------+------+------------+------+------+------+-|
 * | CTRL   |   Z  |   X  |   C  |   V  |   B  |A+SPACE| CAPS |  | WiTab|S-PSCR|   N  |   M  | ,  < | . >  | /  ? |  -_   |
 * `----------------------------+------+------+------+-------+|  |------+------+------+------------+------+---------------'
 *                        | TG(1)|CTRL  | CMD  | Space | ENTER|  |Enter | Space| CMD  | Alt  | TG(2)|
 *                        |M LAY |      |      |       | LOWER|  |LOWER | RAISE|      |      |      |
 *                        `-----------------------------------'  `----------------------------------'
 */
   [_2WERTY] = LAYOUT(
     KC_ESC      , KC_1      , KC_2      , KC_3       , KC_4       , KC_5       , LCTL(KC_GRV)  ,                                               KC_ESC         , KC_6    , KC_7    , KC_8     , KC_9   , KC_0     , KC_BSPC   ,
     KC_TAB      , KC_Q      , KC_W      , KC_E       , KC_R       , KC_T       , _______       ,                                               _______        , KC_Y    , KC_U    , KC_I     , KC_O   , KC_P     , KC_LBRC   ,
     KC_LSFT     , KC_A      , KC_S      , KC_D       , KC_F       , KC_G       , _______       ,                                               _______        , KC_H    , KC_J    , KC_K     , KC_L   , KC_SCLN  , KC_QUOT   ,
     KC_LCTL     , KC_Z      , KC_X      , KC_C       , KC_V       , KC_B       , A(KC_SPC)     , KC_CAPS                , LGUI(KC_TAB)       , S(KC_PSCR)     , KC_N    , KC_M    , KC_COMM  , KC_DOT , KC_SLSH  , KC_MINS   ,
                                     TD(MUTE_LAYER2)  , KC_LCTL    , KC_LCMD    , KC_SPC        , LT(_LOWER, KC_ENT)     , LT(_LOWER, KC_ENT) , LT(_RAISE, KC_SPC), KC_RCMD , KC_RALT , TD(ENCODER_LAYER)
  ),

/*
 * LOWER Layer: 3. Symbols + Arrows
 *
 * ,---------------------------------------------------.                ,-------------------------------------------------.
 * |  ~     |   !  |   @  |   #  |   $  |   %  |       |                |      |  ^   |   &  |   *  |   (  |   )  | BSPC  |
 * |--------+------------+------+------+------+--------|                |------+------------+------+------+------+--------|
 * | TAB    |   `  |   -  |   _  |   =  |   +  |       |                |      |      |   {  |   }  |   [  |   ]  |       |
 * |--------+------------+------+------+------+--------|                |------+------------+------+------+------+--------|
 * | LSHIFT |      |      |      |      |      |       |                |      |      |      |      |      |      |       |
 * |--------+------------+------+------+------+------+--------.  ,-------------+------+------------+------+------+------+-|
 * | CTRL   |     |       |      |      |      |A+SPACE| CAPS |  | MO4  |      |  UP  |      |      |      |  \   |  =  + |
 * `----------------------------+------+------+------+-------+|  |------+------+------+------------+------+---------------'
 *                        | TG(1)|CTRL  | CMD  | Space | ENTER|  |Enter | LEFT | DOWN | RIGHT| TG(2)|
 *                        |M LAY |      |      |       | LOWER|  |LOWER |      |      |      |      |
 *                        `-----------------------------------'  `----------------------------------'
 */
   [_LOWER] = LAYOUT(
     KC_TILDE    , KC_EXLM   , KC_AT     , KC_HASH    , KC_DLR     , KC_PERC    , LCTL(KC_GRV)  ,                                          KC_ESC       , KC_CIRC , KC_AMPR , KC_ASTR  , KC_LPRN, KC_RPRN  , KC_BSPC      ,
     KC_TAB      , KC_GRV    , KC_MINS   , KC_UNDS    , KC_EQUAL   , KC_PLUS    , _______       ,                                          _______      , _______ , KC_LCBR , KC_RCBR  , KC_LBRC, KC_RBRC  , _______      ,
     KC_LSFT     , _______   , _______   , _______    , _______    , _______    , _______       ,                                          _______      , _______ , _______ , _______  , _______, _______  , _______      ,
     KC_LCTL     , _______   , _______   , _______    , _______    , _______    , A(KC_SPC)     , KC_CAPS                   , MO(_ADJUST), _______      , KC_UP   , _______ , _______  , _______, KC_BSLS  , KC_EQL       ,
                                     TD(MUTE_LAYER2)  , KC_LCTL    , KC_LCMD    , KC_SPC        , LT(_LOWER, KC_ENT)        , _______    , KC_LEFT      , KC_DOWN , KC_RGHT , TD(ENCODER_LAYER)
  ),

/*
 * RAISE Layer: 4. NUMBERS + MEDIA
 *
 * ,---------------------------------------------------.                ,-------------------------------------------------.
 * |        |      |      |      |      |      |       |                |      |NUMLock|  /  |   *  |   - |      |        |
 * |--------+------------+------+------+------+--------|                |------+------------+------+------+------+--------|
 * | TAB    |      | Prev | Play | Next | VolUp|       |                |      |   7  |   8  |   9  |   + |      |        |
 * |--------+------------+------+------+------+--------|                |------+------------+------+------+------+--------|
 * | LSHIFT |      |      |      | Mute | VolD |       |                |      |  4   |  5   |  6   |Enter|      |        |
 * |--------+------------+------+------+------+------+--------.  ,-------------+------+------------+------+------+------+-|
 * | CTRL   |      |      |      |      |      |       |      |  | MO4  |      |  1   |  2   |  3   |      |     |        |
 * `----------------------------+------+------+------+-------+|  |------+------+------+------------+------+---------------'
 *                        | TG(1)|      |      |       |      |  |      |      | 0    |      | TG(2)|
 *                        |M LAY |      |      |       |      |  |      |      |      |      |      |
 *                        `-----------------------------------'  `----------------------------------'
 */
   [_RAISE] = LAYOUT(
     _______    , _______   , _______   , _______    , _______   , _______    , LCTL(KC_GRV) ,                                              KC_ESC        , KC_NUM  , KC_SLSH , KC_ASTR , KC_MINS, _______  , KC_BSPC     ,
     KC_TAB     , _______   , KC_MPRV   , KC_MPLY    , KC_MNXT   , KC_VOLU    , _______      ,                                              _______       , KC_7    , KC_8    , KC_9    , KC_PLUS, _______  , _______     ,
     KC_LSFT    , _______   , _______   , _______    , KC_MUTE   , KC_VOLD    , _______      ,                                              _______       , KC_4    , KC_5    , KC_6    , KC_ENT , _______  , _______     ,
     KC_LCTL    , _______   , _______   , _______    , _______   , _______    , _______      , _______                     , MO(_ADJUST)  , _______       , KC_1    , KC_2    , KC_3    , _______, _______  , _______     ,
                                     TD(MUTE_LAYER2) , _______   , _______    , _______      , _______                     , _______      , _______       , KC_0    , _______ , TD(ENCODER_LAYER)
  ),


/*
 * RAISE Layer: 5. FunctionKeys, RGB
 *
 * ,---------------------------------------------------.                ,-------------------------------------------------.
 * |        | F1   |  F2  | F3   | F4   | F5   |       |                |ESC   |  F6 | F7   |  F8  | F9  | F10  |        |
 * |--------+------------+------+------+------+--------|                |------+------------+------+------+------+-------|
 * |   TOG  |VAI   | SAI  | HUI  | SPI  | MOD  |       |                |      |     |      |      | F11 | F12  |        |
 * |--------+------------+------+------+------+--------|                |------+------------+------+------+------+-------|
 * |        | VID  | SAD  | HUD  | SPD  | RMOD |       |                |      |     |      |      |     |      |        |
 * |--------+------------+------+------+------+------+--------.  ,-------------+------+------------+------+------+-----+-|
 * |        |      |      |      |      |      |       |      |  | MO4  |      |     |      |      |     |      |        |
 * `----------------------------+------+------+------+-------+|  |------+------+------+------------+------+--------------'
 *                        | TG(1)|      |      |       |      |  |      |      |     |      | TG(2)|
 *                        |M LAY |      |      |       |      |  |      |      |     |      |      |
 *                        `-----------------------------------'  `----------------------------------'
 */
   [_ADJUST] = LAYOUT(
     _______    , KC_F1     , KC_F2     , KC_F3      , KC_F4     , KC_F5      , LCTL(KC_GRV)  ,                                           KC_ESC        , KC_F6   , KC_F7   , KC_F8   , KC_F9  , KC_F10   , _______      ,
     RGB_TOG    , RGB_VAI   , RGB_SAI   , RGB_HUI    , RGB_SPI   , RGB_MOD    , _______       ,                                           _______       , _______ , _______ , _______ , KC_F11 , KC_F12   , _______      ,
     _______    , RGB_VAD   , RGB_SAD   , RGB_HUD    , RGB_SPD   , RGB_RMOD   , _______       ,                                           _______       , _______ , _______ , _______ , _______, _______  , _______      ,
     _______    , _______   , _______   , _______    , _______   , _______    , _______       , _______                  , MO(_ADJUST)  , _______       , _______ , _______ , _______ , _______, _______  , _______      ,
                                     TD(MUTE_LAYER2) , _______   , _______    , _______       , _______                  , _______      , _______       , _______ , _______ , TD(ENCODER_LAYER)
  ),

  // /*
//  * Layer template
//  *
//  * ,-------------------------------------------.                              ,-------------------------------------------.
//  * |        |      |      |      |      |      |                              |      |      |      |      |      |        |
//  * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
//  * |        |      |      |      |      |      |                              |      |      |      |      |      |        |
//  * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
//  * |        |      |      |      |      |      |      |      |  |      |      |      |      |      |      |      |        |
//  * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
//  *                        |      |      |      |      |      |  |      |      |      |      |      |
//  *                        |      |      |      |      |      |  |      |      |      |      |      |
//  *                        `----------------------------------'  `----------------------------------'
//  */
//     [_LAYERINDEX] = LAYOUT(
//       _______, _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______, _______,
//       _______, _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______, _______,
//       _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
//                                  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
//     ),
  [_GAME] = LAYOUT(
     KC_ESC         , KC_1          , KC_2          , KC_3          , KC_4          , KC_5          , LCTL(KC_GRV)  ,                                 KC_ESC        , KC_6          , KC_7          , KC_8          , KC_9          , KC_0          , KC_MINS     ,
     KC_TAB         , KC_LSFT       , KC_Q          , KC_X          , KC_E          , KC_R          , KC_T          ,                                 _______       , KC_Y          , KC_U          , KC_I          , KC_O          , KC_P          , KC_LBRC     ,
     KC_LSFT        , KC_LSFT       , KC_A          , KC_W          , KC_D          , KC_F          , KC_G          ,                                 _______       , KC_H          , KC_J          , KC_K          , KC_L          , KC_SCLN       , KC_QUOT     ,
     KC_LALT        , KC_LCTL       , KC_Z          , KC_S          , KC_C          , KC_V          , KC_SPC        , KC_BSPC       , KC_BSPC       , KC_ENT        , KC_ENT        , KC_N          , KC_M          , KC_COMM       , KC_DOT        , KC_SLSH     ,
                                                      _______       , KC_LCTL       , KC_SPC        , KC_SPC        , KC_LGUI       , KC_CAPS       , KC_ENT        , KC_SPC        , KC_LALT       , _______
  )
};

#if defined(OLED_ENABLE)

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    if (is_keyboard_master()) {
        return OLED_ROTATION_0;
    }
    return OLED_ROTATION_0;
}

static void render_status(void) {
    // Host Keyboard Layer Status
    oled_write_P(PSTR("Amon Types\n\nLAYER:\n "), false);
    switch (get_highest_layer(layer_state)) {
        case _QWERTY:
            oled_write_P(PSTR("QWERTY\n\n"), false);
            break;
        case _2WERTY:
            oled_write_P(PSTR("NUM/SYMBOLS\n\n"), false);
            break;
        case _LOWER:
            oled_write_P(PSTR("SYMB/ARROWS\n\n"), false);
            break;
        case _RAISE:
            oled_write_P(PSTR("NUM/MEDIA\n\n"), false);
            break;
        case _ADJUST:
            oled_write_P(PSTR("FUNCTION/RGB\n\n"), false);
            break;
        default:
            oled_write_P(PSTR("Undefined "), false);
            break;
    }
    oled_write_P(PSTR("Die beforeyou die,\nthere is\nno chance\nafter."), false);

    //Host Keyboard LED Status
    led_t led_state = host_keyboard_led_state();
    oled_write_P(led_state.num_lock ? PSTR("NUM -") : PSTR("       "), false);
    oled_write_P(led_state.caps_lock ? PSTR("CAPS ") : PSTR("       "), false);
    oled_write_P(led_state.scroll_lock ? PSTR("SCRL") : PSTR("       "), false);
}


/* KEYBOARD PET START */

/* settings */
#    define MIN_WALK_SPEED      10
#    define MIN_RUN_SPEED       40

/* advanced settings */
#    define ANIM_FRAME_DURATION 200  // how long each frame lasts in ms
#    define ANIM_SIZE           96   // number of bytes in array. If you change sprites, minimize for adequate firmware size. max is 1024

/* timers */
uint32_t anim_timer = 0;
uint32_t anim_sleep = 0;

/* current frame */
uint8_t current_frame = 0;

/* status variables */

int current_wpm = get_current_wpm();
led_t led_usb_state;

bool isSneaking = false;
bool isJumping  = false;
bool showedJump = true;

/* logic */
static void render_luna(int LUNA_X, int LUNA_Y) {
    /* Sit */
    static const char PROGMEM sit[2][ANIM_SIZE] = {/* 'sit1', 32x22px */
                                                   {
                                                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x1c, 0x02, 0x05, 0x02, 0x24, 0x04, 0x04, 0x02, 0xa9, 0x1e, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x10, 0x08, 0x68, 0x10, 0x08, 0x04, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x06, 0x82, 0x7c, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x04, 0x0c, 0x10, 0x10, 0x20, 0x20, 0x20, 0x28, 0x3e, 0x1c, 0x20, 0x20, 0x3e, 0x0f, 0x11, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                   },

                                                   /* 'sit2', 32x22px */
                                                   {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x1c, 0x02, 0x05, 0x02, 0x24, 0x04, 0x04, 0x02, 0xa9, 0x1e, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x90, 0x08, 0x18, 0x60, 0x10, 0x08, 0x04, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x0e, 0x82, 0x7c, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x04, 0x0c, 0x10, 0x10, 0x20, 0x20, 0x20, 0x28, 0x3e, 0x1c, 0x20, 0x20, 0x3e, 0x0f, 0x11, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}};

    /* Walk */
    static const char PROGMEM walk[2][ANIM_SIZE] = {/* 'walk1', 32x22px */
                                                    {
                                                        0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x40, 0x20, 0x10, 0x90, 0x90, 0x90, 0xa0, 0xc0, 0x80, 0x80, 0x80, 0x70, 0x08, 0x14, 0x08, 0x90, 0x10, 0x10, 0x08, 0xa4, 0x78, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x08, 0xfc, 0x01, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x18, 0xea, 0x10, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x1c, 0x20, 0x20, 0x3c, 0x0f, 0x11, 0x1f, 0x03, 0x06, 0x18, 0x20, 0x20, 0x3c, 0x0c, 0x12, 0x1e, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                    },

                                                    /* 'walk2', 32x22px */
                                                    {
                                                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x40, 0x20, 0x20, 0x20, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x10, 0x28, 0x10, 0x20, 0x20, 0x20, 0x10, 0x48, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x20, 0xf8, 0x02, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x03, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x10, 0x30, 0xd5, 0x20, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x20, 0x30, 0x0c, 0x02, 0x05, 0x09, 0x12, 0x1e, 0x02, 0x1c, 0x14, 0x08, 0x10, 0x20, 0x2c, 0x32, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                    }};

    /* Run */
    static const char PROGMEM run[2][ANIM_SIZE] = {/* 'run1', 32x22px */
                                                   {
                                                       0x00, 0x00, 0x00, 0x00, 0xe0, 0x10, 0x08, 0x08, 0xc8, 0xb0, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x40, 0x40, 0x3c, 0x14, 0x04, 0x08, 0x90, 0x18, 0x04, 0x08, 0xb0, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0xc4, 0xa4, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xc8, 0x58, 0x28, 0x2a, 0x10, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x09, 0x04, 0x04, 0x04, 0x04, 0x02, 0x03, 0x02, 0x01, 0x01, 0x02, 0x02, 0x04, 0x08, 0x10, 0x26, 0x2b, 0x32, 0x04, 0x05, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                   },

                                                   /* 'run2', 32x22px */
                                                   {
                                                       0x00, 0x00, 0x00, 0xe0, 0x10, 0x10, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x78, 0x28, 0x08, 0x10, 0x20, 0x30, 0x08, 0x10, 0x20, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x04, 0x08, 0x10, 0x11, 0xf9, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x10, 0xb0, 0x50, 0x55, 0x20, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x0c, 0x10, 0x20, 0x28, 0x37, 0x02, 0x1e, 0x20, 0x20, 0x18, 0x0c, 0x14, 0x1e, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                   }};

    /* Bark */
    static const char PROGMEM bark[2][ANIM_SIZE] = {/* 'bark1', 32x22px */
                                                    {
                                                        0x00, 0xc0, 0x20, 0x10, 0xd0, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x40, 0x3c, 0x14, 0x04, 0x08, 0x90, 0x18, 0x04, 0x08, 0xb0, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x04, 0x08, 0x10, 0x11, 0xf9, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xc8, 0x48, 0x28, 0x2a, 0x10, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x0c, 0x10, 0x20, 0x28, 0x37, 0x02, 0x02, 0x04, 0x08, 0x10, 0x26, 0x2b, 0x32, 0x04, 0x05, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                    },

                                                    /* 'bark2', 32x22px */
                                                    {
                                                        0x00, 0xe0, 0x10, 0x10, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x40, 0x40, 0x2c, 0x14, 0x04, 0x08, 0x90, 0x18, 0x04, 0x08, 0xb0, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x04, 0x08, 0x10, 0x11, 0xf9, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xc0, 0x48, 0x28, 0x2a, 0x10, 0x0f, 0x20, 0x4a, 0x09, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x0c, 0x10, 0x20, 0x28, 0x37, 0x02, 0x02, 0x04, 0x08, 0x10, 0x26, 0x2b, 0x32, 0x04, 0x05, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                    }};

    /* Sneak */
    static const char PROGMEM sneak[2][ANIM_SIZE] = {/* 'sneak1', 32x22px */
                                                     {
                                                         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x40, 0x40, 0x40, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x40, 0x40, 0x80, 0x00, 0x80, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x21, 0xf0, 0x04, 0x02, 0x02, 0x02, 0x02, 0x03, 0x02, 0x02, 0x04, 0x04, 0x04, 0x03, 0x01, 0x00, 0x00, 0x09, 0x01, 0x80, 0x80, 0xab, 0x04, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x1c, 0x20, 0x20, 0x3c, 0x0f, 0x11, 0x1f, 0x02, 0x06, 0x18, 0x20, 0x20, 0x38, 0x08, 0x10, 0x18, 0x04, 0x04, 0x02, 0x02, 0x01, 0x00, 0x00, 0x00, 0x00,
                                                     },

                                                     /* 'sneak2', 32x22px */
                                                     {
                                                         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x40, 0x40, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0xa0, 0x20, 0x40, 0x80, 0xc0, 0x20, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3e, 0x41, 0xf0, 0x04, 0x02, 0x02, 0x02, 0x03, 0x02, 0x02, 0x02, 0x04, 0x04, 0x02, 0x01, 0x00, 0x00, 0x00, 0x04, 0x00, 0x40, 0x40, 0x55, 0x82, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x20, 0x30, 0x0c, 0x02, 0x05, 0x09, 0x12, 0x1e, 0x04, 0x18, 0x10, 0x08, 0x10, 0x20, 0x28, 0x34, 0x06, 0x02, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                     }};

    /* animation */
    void animate_luna(void) {
        /* jump */
        if (isJumping || !showedJump) {
            /* clear */
            oled_set_cursor(LUNA_X, LUNA_Y + 2);
            oled_write("     ", false);

            oled_set_cursor(LUNA_X, LUNA_Y - 1);

            showedJump = true;
        } else {
            /* clear */
            oled_set_cursor(LUNA_X, LUNA_Y - 1);
            oled_write("     ", false);

            oled_set_cursor(LUNA_X, LUNA_Y);
        }

        /* switch frame */
        current_frame = (current_frame + 1) % 2;

        /* current status */
        if (led_usb_state.caps_lock) {
            oled_write_raw_P(bark[abs(1 - current_frame)], ANIM_SIZE);

        } else if (isSneaking) {
            oled_write_raw_P(sneak[abs(1 - current_frame)], ANIM_SIZE);

        } else if (current_wpm <= MIN_WALK_SPEED) {
            oled_write_raw_P(sit[abs(1 - current_frame)], ANIM_SIZE);

        } else if (current_wpm <= MIN_RUN_SPEED) {
            oled_write_raw_P(walk[abs(1 - current_frame)], ANIM_SIZE);

        } else {
            oled_write_raw_P(run[abs(1 - current_frame)], ANIM_SIZE);
        }
    }

    /* animation timer */
    if (timer_elapsed32(anim_timer) > ANIM_FRAME_DURATION) {
        anim_timer = timer_read32();
        animate_luna();
    }

    /* this fixes the screen on and off bug */
    if (current_wpm > 0) {
        oled_on();
        anim_sleep = timer_read32();
    } else if (timer_elapsed32(anim_sleep) > OLED_TIMEOUT) {
        oled_off();
    }
}

/* KEYBOARD PET END */

void render_display(void) {
    if (is_keyboard_master()) {
        render_status(); // Renders the current keyboard state (layer, lock, caps, scroll, etc)
    } else {
    render_luna(0, 13);

        /* wpm counter */
        uint8_t wpm = get_current_wpm();
        char wpm_str[4];
        oled_set_cursor(0,6);
        wpm_str[3] = '\0';
        wpm_str[2] = '0' + wpm % 10;
        wpm_str[1] = '0' + (wpm /= 10) % 10;
        wpm_str[0] = '0' + wpm / 10;
        oled_write(wpm_str, false);

        oled_set_cursor(0, 7);
        oled_write(" wpm", false);
    }
}
#endif

#ifdef ENCODER_ENABLE
bool encoder_update_user(uint8_t index, bool clockwise) {
    if (index == 0) { // Volume control
        if (clockwise) {
            tap_code16(KC_VOLD);
            clear_oneshot_layer_state(ONESHOT_OTHER_KEY_PRESSED);
        } else {
            tap_code16(KC_VOLU);
            clear_oneshot_layer_state(ONESHOT_OTHER_KEY_PRESSED);
        }
    } else if (index == 1) {
        if (is_page_scroll_layer) {
            if (clockwise) {
                tap_code(KC_PGUP);
            } else {
                tap_code(KC_PGDN);
            }
        } else if (is_alt_tab_layer) {
            if (clockwise) {
                if (!is_alt_tab_active) {
                    is_alt_tab_active = true;
                    register_code(KC_LALT);
                }
                alt_tab_timer = timer_read();
                tap_code16(S(KC_TAB));
            } else {
                tap_code16(KC_TAB);
            }
        } else {
            if (clockwise) {
                tap_code16(S(C(KC_TAB)));
            } else {
                tap_code16(C(KC_TAB));
            }
        }
    }

    return false;
}
void matrix_scan_user(void) {
    if (is_alt_tab_active) {
        if (timer_elapsed(alt_tab_timer) > 1250) {
            unregister_code(KC_LALT);
            is_alt_tab_active = false;
            }
        }
}
#endif

// Determine the current tap dance state
uint8_t cur_dance(qk_tap_dance_state_t *state) {
    if (state->count == 1) {
        if (!state->pressed) return SINGLE_TAP;
        else return SINGLE_HOLD;
    } else if (state->count == 2) return DOUBLE_TAP;
    else return 8;
}

// Initialize tap structure associated with example tap dance key
static tap spaceraise_tap_state = {
    .is_press_action = true,
    .state = 0
};

// Functions that control what our tap dance key does
void spaceraise_finished(qk_tap_dance_state_t *state, void *user_data) {
    spaceraise_tap_state.state = cur_dance(state);
    switch (spaceraise_tap_state.state) {
        case SINGLE_HOLD:
            layer_on(_RAISE);
            break;
        case SINGLE_TAP:
        default:
            tap_code(KC_SPACE);
            break;
    }
}

void spaceraise_reset(qk_tap_dance_state_t *state, void *user_data) {
    // If the key was held down and now is released then switch off the layer
    if (spaceraise_tap_state.state == SINGLE_HOLD) {
        layer_off(_RAISE);
    }
    spaceraise_tap_state.state = 0;
}

static tap encoder_tap_state = {
    .state = 0
};

void encoder_finished(qk_tap_dance_state_t *state, void *user_data) {
    encoder_tap_state.state = cur_dance(state);
    switch (encoder_tap_state.state) {
        case SINGLE_TAP:
            is_alt_tab_layer = false;
            is_page_scroll_layer = !is_page_scroll_layer;
            break;
        case DOUBLE_TAP:
            is_page_scroll_layer = false;
            is_alt_tab_layer = !is_alt_tab_layer;
            break;
    }
}

void encoder_reset(qk_tap_dance_state_t *state, void *user_data) {}

qk_tap_dance_action_t tap_dance_actions[] = {
    // Tap once for Mute, twice for Layer 1 _2WERTY
    [MUTE_LAYER2] = ACTION_TAP_DANCE_LAYER_TOGGLE(KC_MUTE, _2WERTY),
    // Tap one for Space, hold or twice for Layer 3 _RAISE
    [SPACE_RAISE] = ACTION_TAP_DANCE_FN_ADVANCED_TIME(NULL, spaceraise_finished, spaceraise_reset, 175),
    // Tap one for _3WERTY, twice for _4WERTY (encoder functions)
    [ENCODER_LAYER] = ACTION_TAP_DANCE_FN_ADVANCED_TIME(NULL, encoder_finished, encoder_reset, 275)

};
