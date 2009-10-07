#include <math.h>
#include "../resource_ids.h"
#include "control_bass.h"

static const int controls[] =
{
  IDC_GRP_BASS,
  IDC_CHK_BASS_ENABLE,
  IDC_LBL_BASS_FREQ,
  IDC_EDT_BASS_FREQ,
  IDC_LBL_BASS_GAIN,
  IDC_EDT_BASS_GAIN,
  IDC_SLI_BASS_GAIN,
  IDC_LBL_BASS_ROUTE,
  IDC_RBT_BASS_FRONT,
  IDC_RBT_BASS_SUB,
  IDC_CHK_BASS_HPF,
  0
};

///////////////////////////////////////////////////////////////////////////////

ControlBass::ControlBass(HWND _dlg, IAudioProcessor *_proc):
Controller(_dlg, ::controls), proc(_proc)
{
  proc->AddRef();
}

ControlBass::~ControlBass()
{
  proc->Release();
}

void ControlBass::init()
{
  edt_bass_freq.link(hdlg, IDC_EDT_BASS_FREQ);
  edt_bass_gain.link(hdlg, IDC_EDT_BASS_GAIN);
}

void ControlBass::update()
{
  proc->get_bass_redir(&bass_redir);
  proc->get_bass_freq(&bass_freq);
  proc->get_bass_gain(&bass_gain);
  proc->get_bass_hpf(&bass_hpf);
  proc->get_bass_channels(&bass_channels);
  bool front = (bass_channels & (CH_MASK_L | CH_MASK_R)) != 0;
  bool sub   = (bass_channels & CH_MASK_LFE) != 0;

  CheckDlgButton(hdlg, IDC_CHK_BASS_ENABLE, bass_redir? BST_CHECKED: BST_UNCHECKED);
  CheckDlgButton(hdlg, IDC_CHK_BASS_HPF, bass_hpf? BST_CHECKED: BST_UNCHECKED);
  CheckDlgButton(hdlg, IDC_RBT_BASS_FRONT, front? BST_CHECKED: BST_UNCHECKED);
  CheckDlgButton(hdlg, IDC_RBT_BASS_SUB, sub? BST_CHECKED: BST_UNCHECKED);
  edt_bass_freq.update_value(bass_freq);
  edt_bass_gain.update_value(value2db(bass_gain));
};

ControlBass::cmd_result ControlBass::command(int control, int message)
{
  switch (control)
  {
    case IDC_CHK_BASS_ENABLE:
    {
      bass_redir = IsDlgButtonChecked(hdlg, IDC_CHK_BASS_ENABLE) == BST_CHECKED;
      proc->set_bass_redir(bass_redir);
      update();
      return cmd_ok;
    }

    case IDC_EDT_BASS_FREQ:
      if (message == CB_ENTER)
      {
        bass_freq = (int)edt_bass_freq.value;
        proc->set_bass_freq(bass_freq);
        update();
        return cmd_ok;
      }
      return cmd_not_processed;

    case IDC_EDT_BASS_GAIN:
      if (message == CB_ENTER)
      {
        bass_gain = db2value(edt_bass_gain.value);
        proc->set_bass_gain(bass_gain);
        update();
        return cmd_ok;
      }
      return cmd_not_processed;

    case IDC_CHK_BASS_HPF:
    {
      bass_hpf = IsDlgButtonChecked(hdlg, IDC_CHK_BASS_HPF) == BST_CHECKED;
      proc->set_bass_hpf(bass_hpf);
      update();
      return cmd_ok;
    }

    case IDC_RBT_BASS_FRONT:
    case IDC_RBT_BASS_SUB:
    {
      bool front = IsDlgButtonChecked(hdlg, IDC_RBT_BASS_FRONT) == BST_CHECKED;
      bool sub   = IsDlgButtonChecked(hdlg, IDC_RBT_BASS_SUB) == BST_CHECKED;
      if (front) bass_channels = CH_MASK_L | CH_MASK_R;
      if (sub) bass_channels = CH_MASK_LFE;
      proc->set_bass_channels(bass_channels);
      update();
      return cmd_ok;
    }
  }
  return cmd_not_processed;
}
