#ifndef AC3FILTER_DLG_H
#define AC3FILTER_DLG_H

#include <streams.h>
#include "controls.h"
#include "translate.h"
#include "guids.h"


          
class AC3FilterDlg : public CBasePropertyPage, public AudioProcessorState
{
public:
  static CUnknown * WINAPI CreateMain  (LPUNKNOWN lpunk, HRESULT *phr);
  static CUnknown * WINAPI CreateMixer (LPUNKNOWN lpunk, HRESULT *phr);
  static CUnknown * WINAPI CreateGains (LPUNKNOWN lpunk, HRESULT *phr);
  static CUnknown * WINAPI CreateSPDIF (LPUNKNOWN lpunk, HRESULT *phr);
  static CUnknown * WINAPI CreateSystem(LPUNKNOWN lpunk, HRESULT *phr);
  static CUnknown * WINAPI CreateAbout (LPUNKNOWN lpunk, HRESULT *phr);

  int flags;

  void reload_state();

private:
  IAC3Filter      *filter;
  IDecoder        *dec;
  IAudioProcessor *proc;

  bool     visible;
  bool     refresh;
  int      refresh_time;

  Speakers in_spk;
  Speakers out_spk;
  Speakers user_spk;
  int      formats;
  bool     query_sink;
  bool     tray;

  // spdif
  bool     use_spdif;
  int      spdif_pt;
  bool     spdif_as_pcm;
  bool     spdif_encode;
  bool     spdif_stereo_pt;

  bool     spdif_check_sr;
  bool     spdif_allow_48;
  bool     spdif_allow_44;
  bool     spdif_allow_32;

  int      dts_mode;
  int      dts_conv;

  int      spdif_status;

  int      reinit;
  bool     spdif_no_pcm;
  bool     use_detector;

  // syncronization
  vtime_t  time_shift;
  vtime_t  time_factor;
  bool     dejitter;
  vtime_t  threshold;

  vtime_t  input_mean;
  vtime_t  input_stddev;
  vtime_t  output_mean;
  vtime_t  output_stddev;

  int      frames;
  int      errors;

  Speakers old_in_spk;
  int      old_spdif_status;
  matrix_t old_matrix;
  char     old_info[2048];
  char     old_jitter[128];
  char     old_lang[256];

  // Matrix
  DoubleEdit  edt_matrix[NCHANNELS][NCHANNELS];
  // AGC
  DoubleEdit  edt_attack;
  DoubleEdit  edt_release;
  // Gain control
  DoubleEdit  edt_master;
  DoubleEdit  edt_gain;
  DoubleEdit  edt_voice;
  DoubleEdit  edt_sur;
  DoubleEdit  edt_lfe;
  // I/O Gains
  DoubleEdit  edt_in_gains[NCHANNELS];
  DoubleEdit  edt_out_gains[NCHANNELS];
  // DRC
  DoubleEdit  edt_drc_power;
  DoubleEdit  edt_drc_level;
  // Bass redirection
  DoubleEdit  edt_bass_freq;
  // Delay
  DoubleEdit  edt_delay[NCHANNELS];
  DoubleEdit  edt_time_shift;
  // Links
  LinkButton  lnk_home;
  LinkButton  lnk_forum;
  LinkButton  lnk_email;
  LinkButton  lnk_donate;

  AC3FilterDlg(TCHAR *pName, LPUNKNOWN lpunk, HRESULT *phr, int DialogId, int TitleId, int flags);

  BOOL OnReceiveMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

  HRESULT OnConnect(IUnknown *pUnknown);
  HRESULT OnDisconnect();
  HRESULT OnActivate();
  HRESULT OnDeactivate();

  /////////////////////////////////////////////////////////////////////////////
  // Translation

  Translator trans;
  bool set_lang(const char *lang);
  void get_lang(char *buf, size_t size);

  /////////////////////////////////////////////////////////////////////////////
  // Interface update functions
  // 
  // * init()
  //   Top-level function to init the interface.
  //   Translate and init controls.
  //
  // * update()
  //   Top-level function to update controls.
  //   Reload state and update static and dynamic controls.
  //
  // * init_controls()
  //   Set initial controls parameters (range, color, ets), bind class members
  //   to controls, etc.
  //
  // * translate_controls()
  //   Translate control labels, create and translate tips.
  //
  // * update_dynamic_controls()
  //   Update only dynamic controls. Update the matrix if auto_matrix is on.
  //
  // * update_static_controls()
  //   Update only static controls. Update the matrix if auto_matrix is off.
  //
  // * update_matrix_controls()
  //   Update the matrix. It may be called from update_static_controls() and
  //   from update_dynamic_controls().
  //
  // * update_cpu_usage()
  //   Update CPU usage meter. It is updated independently from other controls
  //   because it needs more time to gather correct mean statistics.

  void init();
  void update();

  void init_controls();
  void translate_controls();

  void update_dynamic_controls();
  void update_static_controls();
  void update_matrix_controls();
  void update_cpu_usage();

  /////////////////////////////////////////////////////////////////////////////
  // Handle control notifications

  void command(int control, int message);
};

#endif
