/**********************************************************************
*          Copyright (c) 2014, Hogeschool voor de Kunsten Utrecht
*                      Hilversum, the Netherlands
*                          All rights reserved
***********************************************************************
*  This program is free software: you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with this program.
*  If not, see <http://www.gnu.org/licenses/>.
***********************************************************************
*
*  File name     : osc2midi.cpp
*  System name   : osc2midi
* 
*  Description   : OSC server forwarding OSC messages to MIDI
*
*  Author        : Marc_G
*  E-mail        : marcg@dinkum.nl
*
**********************************************************************/

#include "midi_io.h"
#include "osc.h"


MIDI_io midi_io;


// subclass OSC into a local class so we can provide our own callback
class localOSC : public OSC
{
  int realcallback(const char *path,const char *types,lo_arg **argv,int argc)
  {
  string msgpath=path;

    if(!msgpath.compare("/note-on")){
      int channel = argv[0]->i;
      int pitch = argv[1]->i;
      int velocity = argv[2]->i;
      event.message=Pm_Message(0x90+channel,pitch,velocity);
      midi_io.write_event(&event);
    } // if OSC message triggered

    if(!msgpath.compare("/note-off")){
      int channel = argv[0]->i;
      int pitch = argv[1]->i;
      int velocity = argv[2]->i;
      event.message=Pm_Message(0x80+channel,pitch,velocity);
      midi_io.write_event(&event);
    } // if OSC message triggered

    return 0;
  } // realcallback()

private:
  PmEvent event;
};



int main(int argc, char **argv)
{
int done = 0;
localOSC osc;
string serverport="7777";
bool use_default_devices=false;
int input_device=0,output_device=0;

  midi_io.list_devices();

  if(argc>1 && (string(argv[1]) == "-d")) {
    use_default_devices=true;
    cout << "Using default devices\n";
  }
  else cout << "For using default devices specify -d\n";

  if(!use_default_devices){
    cout << "\nGive input device number: ";
    cin >> input_device;
    midi_io.set_input_device(input_device);
    cout << "Give output device number: ";
    cin >> output_device;
    midi_io.set_output_device(output_device);
  }

  midi_io.initialise();

  osc.init(serverport);
  osc.set_callback("/note-on","iii");
  osc.set_callback("/note-off","iii");

  osc.start();

  cout << "Listening on port " << serverport << endl;

  while (!done) 
  {
    usleep(1000);
  }

  midi_io.finalise();

  return 0;
}

