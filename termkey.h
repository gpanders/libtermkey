#ifndef __TERMKEY_H__
#define __TERMKEY_H__

#include <stdint.h>
#include <stdlib.h>

typedef enum {
  TERMKEY_SYM_UNKNOWN = -1,
  TERMKEY_SYM_NONE = 0,

  // Special names in C0
  TERMKEY_SYM_BACKSPACE,
  TERMKEY_SYM_TAB,
  TERMKEY_SYM_ENTER,
  TERMKEY_SYM_ESCAPE,

  // Special names in G0
  TERMKEY_SYM_SPACE,
  TERMKEY_SYM_DEL,

  // CSI keys
  TERMKEY_SYM_UP,
  TERMKEY_SYM_DOWN,
  TERMKEY_SYM_LEFT,
  TERMKEY_SYM_RIGHT,
  TERMKEY_SYM_BEGIN,

  // CSI function keys
  TERMKEY_SYM_INSERT,
  TERMKEY_SYM_DELETE,
  TERMKEY_SYM_HOME,
  TERMKEY_SYM_END,
  TERMKEY_SYM_PAGEUP,
  TERMKEY_SYM_PAGEDOWN,

  TERMKEY_SYM_F1,
  TERMKEY_SYM_F2,
  TERMKEY_SYM_F3,
  TERMKEY_SYM_F4,
  TERMKEY_SYM_F5,
  TERMKEY_SYM_F6,
  TERMKEY_SYM_F7,
  TERMKEY_SYM_F8,
  TERMKEY_SYM_F9,
  TERMKEY_SYM_F10,
  TERMKEY_SYM_F11,
  TERMKEY_SYM_F12,

  // Numeric keypad special keys
  TERMKEY_SYM_KP0,
  TERMKEY_SYM_KP1,
  TERMKEY_SYM_KP2,
  TERMKEY_SYM_KP3,
  TERMKEY_SYM_KP4,
  TERMKEY_SYM_KP5,
  TERMKEY_SYM_KP6,
  TERMKEY_SYM_KP7,
  TERMKEY_SYM_KP8,
  TERMKEY_SYM_KP9,
  TERMKEY_SYM_KPENTER,
  TERMKEY_SYM_KPPLUS,
  TERMKEY_SYM_KPMINUS,
  TERMKEY_SYM_KPMULT,
  TERMKEY_SYM_KPDIV,
  TERMKEY_SYM_KPCOMMA,
  TERMKEY_SYM_KPPERIOD,
  TERMKEY_SYM_KPEQUALS,

  // et cetera ad nauseum
} termkey_sym;

typedef enum {
  TERMKEY_RES_NONE,
  TERMKEY_RES_KEY,
  TERMKEY_RES_EOF,
  TERMKEY_RES_AGAIN,
} termkey_result;

enum {
  TERMKEY_KEYFLAG_SPECIAL = 0x01, // 'code' is a special keycode, not a unicode codepoint
};

enum {
  TERMKEY_KEYMOD_SHIFT = 0x01,
  TERMKEY_KEYMOD_ALT   = 0x02,
  TERMKEY_KEYMOD_CTRL  = 0x04,
};

typedef int termkey_keysym;

typedef struct {
  int modifiers;
  termkey_keysym code; // Or Unicode codepoint
  int flags;

  /* Any Unicode character can be UTF-8 encoded in no more than 6 bytes, plus
   * terminating NUL */
  char utf8[7];
} termkey_key;

typedef struct termkey termkey_t;

enum {
  TERMKEY_FLAG_NOINTERPRET = 0x01, // Do not interpret C0//G1 codes if possible
  TERMKEY_FLAG_CONVERTKP   = 0x02, // Convert KP codes to regular keypresses
  TERMKEY_FLAG_RAW         = 0x04, // Input is raw bytes, not UTF-8
  TERMKEY_FLAG_UTF8        = 0x08, // Input is definitely UTF-8
};

termkey_t *termkey_new(int fd, int flags);
void       termkey_free(termkey_t *tk);

void       termkey_setwaittime(termkey_t *tk, int msec);
int        termkey_getwaittime(termkey_t *tk);

termkey_result termkey_getkey(termkey_t *tk, termkey_key *key);
termkey_result termkey_getkey_force(termkey_t *tk, termkey_key *key);
termkey_result termkey_waitkey(termkey_t *tk, termkey_key *key);

void       termkey_pushinput(termkey_t *tk, unsigned char *input, size_t inputlen);

termkey_result termkey_advisereadable(termkey_t *tk);

// Registration of keys and names
termkey_keysym termkey_register_keyname(termkey_t *tk, termkey_keysym code, const char *name);

termkey_keysym termkey_register_c0(termkey_t *tk, termkey_keysym code, unsigned char ctrl, const char *name);
termkey_keysym termkey_register_csi_ss3(termkey_t *tk, termkey_keysym code, unsigned char cmd, const char *name);
termkey_keysym termkey_register_ss3kpalt(termkey_t *tk, termkey_keysym code, unsigned char cmd, const char *name, char kpalt);
termkey_keysym termkey_register_csifunc(termkey_t *tk, termkey_keysym code, int number, const char *name);

termkey_keysym termkey_register_c0_full(termkey_t *tk, termkey_keysym code, int modifier_set, int modifier_mask, unsigned char ctrl, const char *name);
termkey_keysym termkey_register_csi_ss3_full(termkey_t *tk, termkey_keysym code, int modifier_set, int modifier_mask, unsigned char cmd, const char *name);
termkey_keysym termkey_register_ss3kpalt_full(termkey_t *tk, termkey_keysym code, int modifier_set, int modifier_mask, unsigned char cmd, const char *name, char kpalt);
termkey_keysym termkey_register_csifunc_full(termkey_t *tk, termkey_keysym code, int modifier_set, int modifier_mask, int number, const char *name);

const char *termkey_describe_sym(termkey_t *tk, termkey_keysym code);

#endif
