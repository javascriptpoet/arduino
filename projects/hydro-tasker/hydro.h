#ifndef HYDRO_H
#define HYDRO_H
#ifndef debug
   #include <standardFirmataPlus.h>
   #include <Firmata.h>
#endif
#include <SimpleTimer.h>
#include <Debounce.h>
#include <Filters.h>
#include <tasker.h>
#include <timex.h>
#include <utils.h>
#include <protocol.h>
#include <Arduino.h>
#include <EEPROM.h>
#ifdef debug
     #include <stdarg.h>
     #include <stdio.h>
     #include <string.h>
#endif

#define debug

//define keepalive sysex command, same for both directions
#define KEEP_ALIVE_COMMAND           B00000001

//io configuration
//output/control/relay pins
//note: pins are zero based unlike their labels on the board
#define PUMP_RELAY_PIN               50 //pump
#define LIGHTS_RELAY_PIN             52 //lights, all on same relay
#define VRO_RELAY_PIN                24 //RO tank valve relay
#define VPMPZ1_RELAY_PIN             34 //zone 1 pump valve (sprinkler valves are directional, therefore, two for each zone)
#define VSCKZ1_RELAY_PIN             32 //zone 1 suck valve (sprinkler valves are directional, therefore, two for each zone)
#define VPMPZ2_RELAY_PIN             36 //zone 2 pump valve (sprinkler valves are directional, therefore, two for each zone)
#define VSCKZ2_RELAY_PIN             38 //zone 2 suck valve (sprinkler valves are directional, therefore, two for each zone)
#define VPMP1_RELAY_PIN              22 //valve 1 (closest to nut tank) of pump side
#define VPMP2_RELAY_PIN              28 //valve 2 of pump side
#define VSCK1_RELAY_PIN              24 //valve 1 (closest to nut tank) of suck side
#define VSCK2_RELAY_PIN              30 //valve 2 of suck side

//input/command/pushbuttons pins
//all buttons are debounced
#define CYCLE_COMMAND_PIN            23 //btn 1,starts main pump/suck cycle. If one is in progress, it is not disrupted
#define FLOOD1_COMMAND_PIN           25 //btn 2,floods zone 1, does abort first
#define FLOOD2_COMMAND_PIN           29 //btn 3,floods zone 2, does abort first
#define DRAIN1_COMMAND_PIN           27 //btn 4,drains zone 1, does abort first
#define DRAIN2_COMMAND_PIN           31 //btn 5,drains zone 2, does abort first
#define FILL_COMMAND_PIN             33 //btn 6,transfer from ro tank into nutrient tank, does abort first
#define EMPTY_COMMAND_PIN            35 //btn 7,empty the nutrient tank to waist,does abort first
#define ABORT_COMMAND_PIN            37 //btn 8,stops whatever is in progress, closes all valves, pump is kept running for short time to allow valves to close


//analog input channels
#define ZONE_1_LEVEL_CHAN            A0 //Water level sensor for zone 1
#define ZONE_2_LEVEL_CHAN            A1 //Water level sensor for zone 2

//action names
#define VALVE_RESET_ACTION F("valve reset") //closes all valves relays. valves will not close till pump is run for a while which are separate actions
#define FLOOD_ACTION        F("zone flood") //configures valves for filling buckets
#define DRAIN_ACTION        F("zone drain")//configures valves for draiing buckets
#define START_CYCLE_ACTION  F("start cycle")
#define STOP_CYCLE_ACTION   F("stop cycle")
#define START_PUMP_ACTION   F("start pump")
#define STOP_PUMP_ACTION    F("stop pump")
#define DELAY_ACTION        F("delay")
#define FILL_ACTION         F("fill")
#define EMPTY_ACTION        F("empty")

#define CONTROL_STATE_REMOTE 0
#define CONTROL_STATE_LOCAL  1

#ifdef debug
  extern char str_buffer[100];
#endif

extern struct Zone {
     FilterOnePole filteredLevel;//a 5hz low pass filter for water level sensor
     int pump_valve_pin;
     int suck_valve_pin;
} zones[2];

extern struct State {
     bool isMainCycle;
     int control;
} state;

//local control parameters
extern class Config {
     public:
     long fill_interval=300000;
     long flood_interval=2000;
     long drain_interval=2000;
     long main_interval=20000;
     long empty_interval=1800000;
     long pump_delay_interval=3000;
     long keep_alive_interval=3000;
     long debounce_interval=500;
     long level_setpoint=200;
     void setParam(int index,long val);
} *config;

extern Tasker *mainTasker;
extern Timex timex;
extern int zone1,zone2;
extern Utils utils;
extern Protocol *protocol;

//funcs
void config_setup(void);
void keep_alive_timer_setup(void);
void main_timer_setup(void);
void do_commands();
void main_tasker_setup(void);
void schedMainCycle(void);
void floodZone1(void);
void drainZone1(void);
void floodZone2(void);
void drainZone2(void);
void abort(void);
void fill(void);
void empty(void);
void setConfigParam(Cmd *cmd);
void configInfo(Cmd *cmd);
void resetConfig(Cmd *cmd);
void protocol_setup(void);

#endif
