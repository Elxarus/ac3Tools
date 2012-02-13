#include <math.h>
#include <windows.h>
#include <commctrl.h>
#include "../ac3filter_intl.h"
#include "../ch_names.h"
#include "../resource_ids.h"
#include "control_iogains.h"

#define NGAINS 8

static const int controls[] =
{
  IDC_GRP_INPUT_GAINS, IDC_GRP_OUTPUT_GAINS,
  IDC_LBL_GAIN_IN1, IDC_LBL_GAIN_IN2, IDC_LBL_GAIN_IN3, IDC_LBL_GAIN_IN4, IDC_LBL_GAIN_IN5, IDC_LBL_GAIN_IN6, IDC_LBL_GAIN_IN7, IDC_LBL_GAIN_IN8, 
  IDC_EDT_GAIN_IN1, IDC_EDT_GAIN_IN2, IDC_EDT_GAIN_IN3, IDC_EDT_GAIN_IN4, IDC_EDT_GAIN_IN5, IDC_EDT_GAIN_IN6, IDC_EDT_GAIN_IN7, IDC_EDT_GAIN_IN8, 
  IDC_SLI_GAIN_IN1, IDC_SLI_GAIN_IN2, IDC_SLI_GAIN_IN3, IDC_SLI_GAIN_IN4, IDC_SLI_GAIN_IN5, IDC_SLI_GAIN_IN6, IDC_SLI_GAIN_IN7, IDC_SLI_GAIN_IN8, 
  IDC_LBL_GAIN_OUT1, IDC_LBL_GAIN_OUT2, IDC_LBL_GAIN_OUT3, IDC_LBL_GAIN_OUT4, IDC_LBL_GAIN_OUT5, IDC_LBL_GAIN_OUT6, IDC_LBL_GAIN_OUT7, IDC_LBL_GAIN_OUT8, 
  IDC_EDT_GAIN_OUT1, IDC_EDT_GAIN_OUT2, IDC_EDT_GAIN_OUT3, IDC_EDT_GAIN_OUT4, IDC_EDT_GAIN_OUT5, IDC_EDT_GAIN_OUT6, IDC_EDT_GAIN_OUT7, IDC_EDT_GAIN_OUT8, 
  IDC_SLI_GAIN_OUT1, IDC_SLI_GAIN_OUT2, IDC_SLI_GAIN_OUT3, IDC_SLI_GAIN_OUT4, IDC_SLI_GAIN_OUT5, IDC_SLI_GAIN_OUT6, IDC_SLI_GAIN_OUT7, IDC_SLI_GAIN_OUT8, 
  0
};

const int idc_lbl_gain_in[NGAINS]  = { IDC_LBL_GAIN_IN1, IDC_LBL_GAIN_IN2, IDC_LBL_GAIN_IN3, IDC_LBL_GAIN_IN4, IDC_LBL_GAIN_IN5, IDC_LBL_GAIN_IN6, IDC_LBL_GAIN_IN7, IDC_LBL_GAIN_IN8, };
const int idc_lbl_gain_out[NGAINS] = { IDC_LBL_GAIN_OUT1, IDC_LBL_GAIN_OUT2, IDC_LBL_GAIN_OUT3, IDC_LBL_GAIN_OUT4, IDC_LBL_GAIN_OUT5, IDC_LBL_GAIN_OUT6, IDC_LBL_GAIN_OUT7, IDC_LBL_GAIN_OUT8, };

const int idc_sli_gain_in[NGAINS]  = { IDC_SLI_GAIN_IN1, IDC_SLI_GAIN_IN2, IDC_SLI_GAIN_IN3, IDC_SLI_GAIN_IN4, IDC_SLI_GAIN_IN5, IDC_SLI_GAIN_IN6, IDC_SLI_GAIN_IN7, IDC_SLI_GAIN_IN8, };
const int idc_sli_gain_out[NGAINS] = { IDC_SLI_GAIN_OUT1, IDC_SLI_GAIN_OUT2, IDC_SLI_GAIN_OUT3, IDC_SLI_GAIN_OUT4, IDC_SLI_GAIN_OUT5, IDC_SLI_GAIN_OUT6, IDC_SLI_GAIN_OUT7, IDC_SLI_GAIN_OUT8, };

const int idc_edt_gain_in[NGAINS]  = { IDC_EDT_GAIN_IN1, IDC_EDT_GAIN_IN2, IDC_EDT_GAIN_IN3, IDC_EDT_GAIN_IN4, IDC_EDT_GAIN_IN5, IDC_EDT_GAIN_IN6, IDC_EDT_GAIN_IN7, IDC_EDT_GAIN_IN8, };
const int idc_edt_gain_out[NGAINS] = { IDC_EDT_GAIN_OUT1, IDC_EDT_GAIN_OUT2, IDC_EDT_GAIN_OUT3, IDC_EDT_GAIN_OUT4, IDC_EDT_GAIN_OUT5, IDC_EDT_GAIN_OUT6, IDC_EDT_GAIN_OUT7, IDC_EDT_GAIN_OUT8, };

static const double min_gain_level = -20.0;
static const double max_gain_level = +20.0;
static const int ticks = 5;

static const int ch_map322[NGAINS] = { CH_LFE, CH_BL, CH_SL, CH_L, CH_C, CH_R, CH_SR, CH_BR };

///////////////////////////////////////////////////////////////////////////////

ControlIOGains::ControlIOGains(HWND _dlg, IAudioProcessor *_proc):
Controller(_dlg, ::controls), proc(_proc)
{
  proc->AddRef();
}

ControlIOGains::~ControlIOGains()
{
  proc->Release();
}

void ControlIOGains::init()
{
  for (int i = 0; i < NGAINS; i++)
  {
    SetDlgItemText(hdlg, idc_lbl_gain_in[i], gettext(short_ch_names[ch_map322[i]]));
    SetDlgItemText(hdlg, idc_lbl_gain_out[i], gettext(short_ch_names[ch_map322[i]]));
    SendDlgItemMessage(hdlg, idc_sli_gain_in[i],  TBM_SETRANGE, TRUE, MAKELONG(min_gain_level, max_gain_level) * ticks);
    SendDlgItemMessage(hdlg, idc_sli_gain_out[i], TBM_SETRANGE, TRUE, MAKELONG(min_gain_level, max_gain_level) * ticks);
    SendDlgItemMessage(hdlg, idc_sli_gain_in[i],  TBM_SETTIC, 0, 0);
    SendDlgItemMessage(hdlg, idc_sli_gain_out[i], TBM_SETTIC, 0, 0);
    edt_in_gains[i].link(hdlg, idc_edt_gain_in[i]);
    edt_out_gains[i].link(hdlg, idc_edt_gain_out[i]);
  }
}

void ControlIOGains::update()
{
  proc->get_input_gains(input_gains);
  proc->get_output_gains(output_gains);

  for (int i = 0; i < NGAINS; i++)
  {
    SendDlgItemMessage(hdlg, idc_sli_gain_in[i],  TBM_SETPOS, TRUE, long(-value2db(input_gains[ch_map322[i]])  * ticks));
    SendDlgItemMessage(hdlg, idc_sli_gain_out[i], TBM_SETPOS, TRUE, long(-value2db(output_gains[ch_map322[i]]) * ticks));
    edt_in_gains[i].update_value(value2db(input_gains[ch_map322[i]]));
    edt_out_gains[i].update_value(value2db(output_gains[ch_map322[i]]));
  }
};

ControlIOGains::cmd_result ControlIOGains::command(int control, int message, LPARAM data)
{
  switch (control)
  {
    case IDC_SLI_GAIN_IN1:
    case IDC_SLI_GAIN_IN2:
    case IDC_SLI_GAIN_IN3:
    case IDC_SLI_GAIN_IN4:
    case IDC_SLI_GAIN_IN5:
    case IDC_SLI_GAIN_IN6:
    case IDC_SLI_GAIN_IN7:
    case IDC_SLI_GAIN_IN8:
      if (message == TB_THUMBPOSITION || message == TB_ENDTRACK)
      {
        proc->get_input_gains(input_gains);
        for (int i = 0; i < NGAINS; i++)
          input_gains[ch_map322[i]] = db2value(-double(SendDlgItemMessage(hdlg, idc_sli_gain_in[i], TBM_GETPOS, 0, 0))/ticks);

        proc->set_input_gains(input_gains);
        update();

        return cmd_ok;
      }
      return cmd_not_processed;

    case IDC_EDT_GAIN_IN1:
    case IDC_EDT_GAIN_IN2:
    case IDC_EDT_GAIN_IN3:
    case IDC_EDT_GAIN_IN4:
    case IDC_EDT_GAIN_IN5:
    case IDC_EDT_GAIN_IN6:
    case IDC_EDT_GAIN_IN7:
    case IDC_EDT_GAIN_IN8:
      if (message == CB_ENTER)
      {
        proc->get_input_gains(input_gains);
        for (int i = 0; i < NGAINS; i++)
          input_gains[ch_map322[i]] = db2value(edt_in_gains[i].value);

        proc->set_input_gains(input_gains);
        update();

        return cmd_ok;
      }
      return cmd_not_processed;

    case IDC_SLI_GAIN_OUT1:
    case IDC_SLI_GAIN_OUT2:
    case IDC_SLI_GAIN_OUT3:
    case IDC_SLI_GAIN_OUT4:
    case IDC_SLI_GAIN_OUT5:
    case IDC_SLI_GAIN_OUT6:
    case IDC_SLI_GAIN_OUT7:
    case IDC_SLI_GAIN_OUT8:
      if (message == TB_THUMBPOSITION || message == TB_ENDTRACK)
      {
        proc->get_output_gains(output_gains);
        for (int i = 0; i < NGAINS; i++)
          output_gains[ch_map322[i]] = db2value(-double(SendDlgItemMessage(hdlg, idc_sli_gain_out[i], TBM_GETPOS, 0, 0))/ticks);

        proc->set_output_gains(output_gains);
        update();

        return cmd_ok;
      }
      return cmd_not_processed;

    case IDC_EDT_GAIN_OUT1:
    case IDC_EDT_GAIN_OUT2:
    case IDC_EDT_GAIN_OUT3:
    case IDC_EDT_GAIN_OUT4:
    case IDC_EDT_GAIN_OUT5:
    case IDC_EDT_GAIN_OUT6:
    case IDC_EDT_GAIN_OUT7:
    case IDC_EDT_GAIN_OUT8:
      if (message == CB_ENTER)
      {
        proc->get_output_gains(output_gains);
        for (int i = 0; i < NGAINS; i++)
          output_gains[ch_map322[i]] = db2value(edt_out_gains[i].value);

        proc->set_output_gains(output_gains);
        update();

        return cmd_ok;
      }
      return cmd_not_processed;
  }
  return cmd_not_processed;
}
