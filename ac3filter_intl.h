/*
  Internationalization support for AC3Filter
*/

#ifndef AC3FILTER_INTL
#define AC3FILTER_INTL

///////////////////////////////////////////////////////////////////////////////
// gettext standard definitions

#ifndef DISABLE_NLS
#  include "intl\include\libintl.h"
#  define _(s) gettext(s)
#  define gettext_noop(s) s
#  define N_(s) gettext_noop(s)

// gettext_id(const char *id, const char *str);
// Translate the string that have an id, return str if no translation found
// Used to translate dialog controls by an id, instead of translating by the
// text stored. This allows to translate different controls with the same text
// differently.

inline const char *gettext_id(const char *id, const char *str)
{
  const char *translated = gettext(id);
  return translated == id? str: translated;
}

// gettext_meta()
// Returns current translation info

inline const char *gettext_meta()
{
  return gettext("");
}

#else
#  define gettext(s) s
#  define _(s) gettext(s)
#  define gettext_noop(s) s
#  define N_(s) gettext_noop(s)
#  define gettext_id(id, s) s
#endif

///////////////////////////////////////////////////////////////////////////////
// Language codes conversion
//
// iso_langs - table of languages (ISO 639)
// First element of this table (index 0) is known to be English
//
// iso_countries - table of countries (ISO 3166)
//
// lang_index() - find an index of iso 639 code; -1 if not found; 0 is English
// lang_name() - convert 2 or 3-character code to the language name
//
// country_index() - find an index of iso 3166 code; -1 if not found; 0 is English
// country_name() - convert 2 or 3-character code to the country name

struct iso_lang_s
{ 
  const char *name;
  const char *iso6392;
  const char *iso6391;
};

struct iso_country_s
{
  const char *name;
  const char *alpha3;
  const char *alpha2;
  int code;
};

extern const iso_lang_s iso_langs[];
extern const iso_country_s iso_countries[];

#define LANG_LEN 256

int lang_index(const char *code);
const char *lang_name(const char *code);
const char *lang_name(int index);

int country_index(const char *code);
const char *country_name(const char *code);
const char *country_name(int index);

///////////////////////////////////////////////////////////////////////////////
// Language operations
//
// set_lang() - set global language to the language specified and init the
//   default package. You may omit package and set only language code. Code may
//   be either 2 or 3 character code. You may omit language code to set default
//   package only. set_lang("") cancels translation (now it just switches to
//   English)
//
//   set_lang(0, package, path) may be used to change the packade and path
//   without changing the language.
//
// get_lang() - return currently selected language. Returns zero if no language
//   selected.

void set_lang(const char *code, const char *package = 0, const char *path = 0);
const char *get_lang();

#endif
