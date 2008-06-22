#include "control_list.h"

ControlList::ControlList(HWND _dlg):
Controller(_dlg, 0)
{}

void ControlList::add(Controller *new_ctrl)
{
  ctrl.push_back(PCtrl(new_ctrl));
}

void ControlList::drop()
{
  ctrl.clear();
}

void ControlList::init()
{
  for (VCtrl::iterator iter = ctrl.begin(); iter < ctrl.end(); iter++)
    (*iter)->init();
};

void ControlList::update()
{
  for (VCtrl::iterator iter = ctrl.begin(); iter < ctrl.end(); iter++)
    (*iter)->update();
};

void ControlList::update_dynamic()
{
  for (VCtrl::iterator iter = ctrl.begin(); iter < ctrl.end(); iter++)
    (*iter)->update_dynamic();
};

bool ControlList::own_control(int control)
{
  for (VCtrl::iterator iter = ctrl.begin(); iter < ctrl.end(); iter++)
    if ((*iter)->own_control(control))
      return true;
  return false;
}

ControlList::cmd_result ControlList::command(int control, int message)
{
  for (VCtrl::iterator iter = ctrl.begin(); iter < ctrl.end(); iter++)
    if ((*iter)->own_control(control))
      return (*iter)->command(control, message);
  return cmd_not_processed;
}