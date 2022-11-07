#include QMK_KEYBOARD_H
#include "bootloader.h"

#ifdef PROTOCOL_LUFA
#include "lufa.h"
#include "split_util.h"
#endif

#ifdef AUDIO_ENABLE
#include "audio.h"
#endif

#ifdef SSD1306OLED
#include "ssd1306.h"
#endif

// test

#ifdef RGBLIGHT_ENABLE
//Following line allows macro to read current RGB settings
extern rgblight_config_t rgblight_config;
#endif

extern uint8_t is_master;

enum layer_number {
	_QWERTY = 0,
	_COLEMAK,
	_LOWER,
	_SYMBL,
	_SYMBL2,
	_FN,
};

enum custom_keycodes {
	QWERTY = SAFE_RANGE,
	COLEMAK,
	RGBRST,
	CTRLK,
};

enum macro_keycodes {
	KC_SAMPLEMACRO,
};

#define CMD_EIS LGUI_T(KC_LANG2)
#define CMD_KNA RGUI_T(KC_LANG1)
#define LOWER  MO(_LOWER)
#define SYMBL  MO(_SYMBL)
#define SYMBL2 MO(_SYMBL2)
#define EMACS  MO(_EMACS)
#define FNCTN  MO(_FN)
#define COLEMK DF(_COLEMAK)
#define QWRTY  DF(_QWERTY)

//Macros
#define M_SAMPLE M(KC_SAMPLEMACRO)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	[_COLEMAK] = LAYOUT( \
		KC_ESC,  KC_Q,    KC_W,    KC_F,    KC_P,    KC_G,                      KC_J,    KC_L,    KC_U,    KC_Y,    _______, KC_BSPC, \
		KC_LCTL, KC_A,    KC_R,    KC_S,    KC_T,    KC_D,                      KC_H,    KC_N,    KC_E,    KC_I,    KC_O,    FNCTN, \
		KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                      KC_K,    KC_M,    _______, _______, SYMBL,   _______, \
		KC_LCTL, _______, KC_LALT, CMD_EIS, LOWER,   KC_SPC,  _______, _______, KC_SPC,  KC_RSFT, CMD_KNA, KC_RALT, _______, _______ \
	),

	[_QWERTY] = LAYOUT( \
		KC_ESC,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                      KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSPC, \
		KC_LCTL, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                      KC_H,    KC_J,    KC_K,    KC_L,    _______, FNCTN, \
		KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                      KC_N,    KC_M,    _______, _______, SYMBL,   _______, \
		KC_LCTL, _______, KC_LALT, CMD_EIS, LOWER,   KC_SPC,  _______, _______, KC_SPC,  KC_RSFT, CMD_KNA, KC_RALT, _______, _______ \
	),

	[_LOWER] = LAYOUT( \
		_______, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                      KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_TAB, \
		_______, KC_9,    KC_0,    KC_LBRC, KC_RBRC, KC_BSLS,                   KC_BSPC, KC_ENT,  KC_UP,   KC_MINS, KC_EQL,  _______, \
		_______, KC_SCLN, KC_SLSH, KC_COMM, KC_DOT,  KC_GRV,                    KC_QUOT, KC_LEFT, KC_DOWN, KC_RGHT, _______, _______, \
		_______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______ \
	),

	[_SYMBL] = LAYOUT( \
		_______, KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC,                   KC_CIRC, KC_AMPR, KC_ASTR, _______, _______, _______, \
		_______, KC_LPRN, KC_RPRN, KC_LCBR, KC_RCBR, KC_PIPE,                   KC_PLUS, KC_UNDS, _______, _______, _______, _______, \
		_______, KC_COLN, KC_QUES, KC_LABK, KC_RABK, KC_TILD,                   KC_DQUO, _______, _______, _______, _______, _______, \
		_______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______ \
	),

	[_FN] = LAYOUT( \
		_______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                     _______, COLEMK,  QWRTY,   _______, RESET,   _______, \
		_______, KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,                    _______, KC_VOLD, KC_VOLU, KC_MUTE, KC_MPLY, _______, \
		_______, KC_F11,  KC_F12,  _______, _______, _______,                   _______, KC_BRID, KC_BRIU, KC_MPRV, KC_MNXT, _______, \
		_______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______ \
	),
};


#ifdef AUDIO_ENABLE
float tone_qwerty[][2]     = SONG(QWERTY_SOUND);
float tone_dvorak[][2]     = SONG(DVORAK_SOUND);
float tone_colemak[][2]    = SONG(COLEMAK_SOUND);
float tone_plover[][2]     = SONG(PLOVER_SOUND);
float tone_plover_gb[][2]  = SONG(PLOVER_GOODBYE_SOUND);
float music_scale[][2]     = SONG(MUSIC_SCALE_SOUND);
#endif

// define variables for reactive RGB
bool TOG_STATUS = false;
int RGB_current_mode;

void persistent_default_layer_set(uint16_t default_layer) {
	eeconfig_update_default_layer(default_layer);
	default_layer_set(default_layer);
}

// Setting ADJUST layer RGB back to default
void update_tri_layer_RGB(uint8_t layer1, uint8_t layer2, uint8_t layer3) {
	if (IS_LAYER_ON(layer1) && IS_LAYER_ON(layer2)) {
		#ifdef RGBLIGHT_ENABLE
		//rgblight_mode(RGB_current_mode);
		#endif
		layer_on(layer3);
	} else {
		layer_off(layer3);
	}
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
	switch (keycode) {
		case QWERTY:
			if (record->event.pressed) {
				#ifdef AUDIO_ENABLE
				PLAY_SONG(tone_qwerty);
				#endif
				persistent_default_layer_set(1UL<<_QWERTY);
			}
			return false;
			break;
		case COLEMAK:
			if (record->event.pressed) {
				#ifdef AUDIO_ENABLE
				PLAY_SONG(tone_colemak);
				#endif
				persistent_default_layer_set(1UL<<_COLEMAK);
			}
			return false;
			break;
		case RGBRST:
			#ifdef RGBLIGHT_ENABLE
			if (record->event.pressed) {
				eeconfig_update_rgblight_default();
				rgblight_enable();
				RGB_current_mode = rgblight_config.mode;
			}
			#endif
			break;
		case CTRLK:
			if (record->event.pressed) {
				if (TOG_STATUS) { //TOG_STATUS checks is another reactive key currently pressed, only changes RGB mode if returns false
				} else {
					TOG_STATUS = !TOG_STATUS;
				}
				register_code(KC_LSFT);
				SEND_STRING(SS_TAP(X_END));
				unregister_code(KC_LSFT);
				SEND_STRING(SS_TAP(X_DEL));
			} else {
				TOG_STATUS = false;
			}
			return false;
			break;

	}
	return true;
}

void matrix_init_user(void) {
#ifdef AUDIO_ENABLE
	startup_user();
#endif
#ifdef RGBLIGHT_ENABLE
	RGB_current_mode = rgblight_config.mode;
#endif
	//SSD1306 OLED init, make sure to add #define SSD1306OLED in config.h
#ifdef SSD1306OLED
	iota_gfx_init(!has_usb());   // turns on the display
#endif
}


#ifdef AUDIO_ENABLE

void startup_user()
{
	_delay_ms(50); // gets rid of tick
}

void shutdown_user()
{
	_delay_ms(150);
	stop_all_notes();
}

void music_on_user(void)
{
	music_scale_user();
}

void music_scale_user(void)
{
	PLAY_SONG(music_scale);
}

#endif


//SSD1306 OLED update loop, make sure to add #define SSD1306OLED in config.h
#ifdef SSD1306OLED

void matrix_scan_user(void) {
	iota_gfx_task();  // this is what updates the display continuously
}

void matrix_update(struct CharacterMatrix *dest,
		const struct CharacterMatrix *source) {
	if (memcmp(dest->display, source->display, sizeof(dest->display))) {
		memcpy(dest->display, source->display, sizeof(dest->display));
		dest->dirty = true;
	}
}

//assign the right code to your layers for OLED display
#define L_BASE 0
#define L_LOWER (1<<_LOWER)
#define L_SYMBL (1<<_SYMBL)

static void render_logo(struct CharacterMatrix *matrix) {

	static char logo[]={
		0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,0x90,0x91,0x92,0x93,0x94,
		0xa0,0xa1,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,0xa8,0xa9,0xaa,0xab,0xac,0xad,0xae,0xaf,0xb0,0xb1,0xb2,0xb3,0xb4,
		0xc0,0xc1,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7,0xc8,0xc9,0xca,0xcb,0xcc,0xcd,0xce,0xcf,0xd0,0xd1,0xd2,0xd3,0xd4,
		0};
	matrix_write(matrix, logo);
	//matrix_write_P(&matrix, PSTR(" Split keyboard kit"));
}



void render_status(struct CharacterMatrix *matrix) {

	// Render to mode icon
	static char logo[][2][3]={{{0x95,0x96,0},{0xb5,0xb6,0}},{{0x97,0x98,0},{0xb7,0xb8,0}}};
	if (keymap_config.swap_lalt_lgui==false) {
		matrix_write(matrix, logo[0][0]);
		matrix_write_P(matrix, PSTR("\n"));
		matrix_write(matrix, logo[0][1]);
	} else {
		matrix_write(matrix, logo[1][0]);
		matrix_write_P(matrix, PSTR("\n"));
		matrix_write(matrix, logo[1][1]);
	}

	// Define layers here, Have not worked out how to have text displayed for each layer. Copy down the number you see and add a case for it below
	char buf[40];
	snprintf(buf,sizeof(buf), "Undef-%ld", layer_state);
	matrix_write_P(matrix, PSTR("\nLayer: "));
	switch (layer_state) {
		case L_BASE:
			matrix_write_P(matrix, PSTR("Default"));
			break;
		case L_RAISE:
			matrix_write_P(matrix, PSTR("Raise"));
			break;
		case L_LOWER:
			matrix_write_P(matrix, PSTR("Lower"));
			break;
		default:
			matrix_write(matrix, buf);
	}

	// Host Keyboard LED Status
	char led[40];
	snprintf(led, sizeof(led), "\n%s  %s  %s",
			(host_keyboard_leds() & (1<<USB_LED_NUM_LOCK)) ? "NUMLOCK" : "       ",
			(host_keyboard_leds() & (1<<USB_LED_CAPS_LOCK)) ? "CAPS" : "    ",
			(host_keyboard_leds() & (1<<USB_LED_SCROLL_LOCK)) ? "SCLK" : "    ");
	matrix_write(matrix, led);
}


void iota_gfx_task_user(void) {
	struct CharacterMatrix matrix;

#if DEBUG_TO_SCREEN
	if (debug_enable) {
		return;
	}
#endif

	matrix_clear(&matrix);
	if (is_master) {
		render_status(&matrix);
	} else {
		render_logo(&matrix);
	}
	matrix_update(&display, &matrix);
}

#endif
