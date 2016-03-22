/**HEADER*******************************************************************
 * *
 * * Copyright (c) 2008 Freescale Semiconductor;
 * * All Rights Reserved
 * *
 * * Copyright (c) 1989-2008 ARC International;
 * * All Rights Reserved
 * *
 * ****************************************************************************
 * *
 * * THIS SOFTWARE IS PROVIDED BY FREESCALE "AS IS" AND ANY EXPRESSED OR
 * * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * * IN NO EVENT SHALL FREESCALE OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * * THE POSSIBILITY OF SUCH DAMAGE.
 * *
 * ****************************************************************************
 * *
 * * Comments:
 * *
 * *   This file contains main initialization for your application
 * *   and infinite loop
 * *
 * *END************************************************************************/

#include "main.h"
#include <tfs.h>
#include <httpd.h>
#include <httpd_types.h>


#if defined(APPLICATION_HAS_SHELL) && (!SHELLCFG_USES_RTCS)
#error This application requires SHELLCFG_USES_RTCS defined non-zero in user_config.h. Please recompile libraries with this option if any Ethernet interface is available.
#endif

TASK_TEMPLATE_STRUCT MQX_template_list[] =
{
/*  Task number, Entry point, Stack, Pri, String, Auto? */
   {MAIN_TASK,   Main_task,   2000,  9,   "main", MQX_AUTO_START_TASK},
   {LED_TASK,    Led_task,   2000,  8,   "led", MQX_AUTO_START_TASK},
   {ALARM_TASK,  Alarm_task,   2000,  8,   "alarm", MQX_AUTO_START_TASK},
   {0,           0,           0,     0,   0,      0,                 }
};
HMI_CLIENT_STRUCT_PTR hmi_client;
HTTPD_STRUCT *http_server;

RTC_TIME_STRUCT curr_time;


unsigned char http_refresh_text[] = "<html><head><title>Main Control Center for testing</title><link rel=stylesheet href=https://ajax.googleapis.com/ajax/libs/jqueryui/1.11.4/themes/smoothness/jquery-ui.css><link rel=stylesheet href=https://cdnjs.cloudflare.com/ajax/libs/jquery-ui-timepicker-addon/1.6.1/jquery-ui-timepicker-addon.css><body><div><span>room 1:</span> <button id=1>loading</button> <span>turn on after</span> <input class=timey value=0:0:0 id=from1> <span>turn off after</span> <input class=timey value=0:0:0 id=to1> <button class=setAlarmOff id=time1 data-room=1>loading</button> <button class=setAlarm id=timeset1 data-room=1>loading</button></div><div><span>room 2:</span> <button id=2>loading</button> <span>turn on after</span> <input class=timey value=0:0:0 id=from2> <span>turn off after</span> <input class=timey value=0:0:0 id=to2> <button class=setAlarmOff id=time2 data-room=2>loading</button> <button class=setAlarm id=timeset2 data-room=2>loading</button></div><div><span>room 3:</span> <button id=3>loading</button> <span>turn on after</span> <input class=timey value=0:0:0 id=from3> <span>turn off after</span> <input class=timey value=0:0:0 id=to3> <button class=setAlarmOff id=time3 data-room=3>loading</button> <button class=setAlarm id=timeset3 data-room=3>loading</button></div><div><span>room 4:</span> <button id=4>loading</button> <span>turn on after</span> <input class=timey value=0:0:0 id=from4> <span>turn off after</span> <input class=timey value=0:0:0 id=to4> <button class=setAlarmOff id=time4 data-room=4>loading</button> <button class=setAlarm id=timeset4 data-room=4>loading</button></div><div class=time>current server time is <span id=curr_time></span></div><div>Refreshing data every 3 seconds</div><div>set up server clock</div><input class=timey value=0:0:0 id=set_clock> <button id=setClock data-status=set_clock>Set Up Clock</button><script src=https://ajax.googleapis.com/ajax/libs/jquery/2.2.0/jquery.min.js></script><script src=https://ajax.googleapis.com/ajax/libs/jqueryui/1.11.4/jquery-ui.min.js></script><script src=https://cdn.jsdelivr.net/jquery.ui.timepicker.addon/1.4.5/jquery-ui-timepicker-addon.min.js></script><script>$(function(){function t(t,a){$.ajax({url:t+a,method:'GET',dataType:'text'})}function a(){var t=$.ajax({url:app.init,method:'GET',dataType:'text'});t.done(function(t){console.log(JSON.parse(t)),app.data=JSON.parse(t),o(),setTimeout(a,3e3)})}function o(){app.data.rooms.forEach(function(t,a){var o=$('#'+(a+1));0==t?o.text('turn ON').data('status','off'):1==t?o.text('turn OFF').data('status','on'):2==t&&o.text('HUSH').data('status','blink')}),app.data.timeControl.forEach(function(t,a){var o=$('#time'+(a+1)),r=$('#timeset'+(a+1));0==t?(o.hide(),r.text('SET ON/OFF TIMES')):1==t&&(r.text('SET ON/OFF TIMES'),o.text('timeControl turn OFF').show())}),app.timeControl&&(app.data.zoneOn.forEach(function(t,a){var o=$('#from'+(a+1)),r=t%60,e=Math.floor(t/60)%60,p=Math.floor(t/3600),n=p.toString()+':'+e.toString()+':'+r.toString();o.val(n)}),app.data.zoneOff.forEach(function(t,a){var o=$('#to'+(a+1)),r=t%60,e=Math.floor(t/60)%60,p=Math.floor(t/3600),n=p.toString()+':'+e.toString()+':'+r.toString();o.val(n)}),app.timeControl=!1),$('#curr_time').text(app.data.time)}app={},app.timeControl=!0,app.room=$(this).attr('id'),app.on_url='http://192.168.105.190/room_on.cgi?',app.off_url='http://192.168.105.190/room_off.cgi?',app.set_date_url='http://192.168.105.190/setrtc.cgi?',app.set_alarm_url='http://192.168.105.190/setalarm.cgi?',app.set_alarm_off_url='http://192.168.105.190/resetalarm.cgi?',app.init='http://192.168.105.190/get_status.cgi',app.data={},$('.timey').timepicker({timeFormat:'H:m:s',stepHour:1,stepMinute:1,stepSecond:1}),a(),$('button').on('click',function(){var a=$(this),o=a.data('status'),r=$(this).attr('id'),e=a.data('room');'off'==o?(t(app.on_url,r),a.text('turn OFF').data('status','on')):'on'==o?(t(app.off_url,r),a.text('turn ON').data('status','off')):'blink'==o?(t(app.on_url,r),a.text('turn OFF').data('status','on')):'set_clock'==o?(t(app.set_date_url,$('#set_clock').val()),$('#curr_time').text('changed')):a.hasClass('setAlarm')?(value=e.toString()+','+$('#from'+e).val()+','+$('#to'+e).val(),t(app.set_alarm_url,value)):a.hasClass('setAlarmOff')&&t(app.set_alarm_off_url,e)})})</script>";

const TFS_DIR_ENTRY static_data[] = {
  { "/index.html", 0, http_refresh_text, sizeof(http_refresh_text) },
  {0,0,0,0}
  };

const short OFF   = 0;
const short ON    = 1;
const short ALARM = 2;

unsigned short roomAlarmState[4] = {OFF, OFF, OFF, OFF };
int buttons[] = {HMI_BUTTON_1, HMI_BUTTON_2, HMI_BUTTON_3, HMI_BUTTON_4};
int leds[] = {HMI_LED_1, HMI_LED_2, HMI_LED_3, HMI_LED_4};
int mapLeds[] = {0, 1, 2, 3};

unsigned short timingControl[] = {OFF, OFF, OFF, OFF};
unsigned short alarmControl[] = {OFF, OFF, OFF, OFF};
int zoneOn[] = {0, 0, 0, 0};
int zoneOff[] = {0, 0, 0, 0};

static HTTPD_ROOT_DIR_STRUCT http_root_dir[] = { {"", "tfs:"}, { 0,0 } };

// helper functions
void _toggle_alarms(void *ptr)
{
  int i, toggle_state;

  toggle_state = ON;

  // Check if any alarms are on/alarming, if so alarms to be toggled off
  // otherwise default to on

   for (i = 0; i < 4; i++) {
     // If someone is turning off all manual alarms, they will also be aware of
     // any current alarms going off -> They intend to turn alarms off
     if(roomAlarmState[i] == ON || roomAlarmState[i] == ALARM) {
       toggle_state = OFF;
       break;
     }
   }

   for (i = 0; i < 4; i++) {
     roomAlarmState[i] = toggle_state;
   }
}

void _hush_alarms(void *ptr)
{
  int i;
   for (i = 0; i < 4; i++) {
     if(roomAlarmState[i] == ALARM) {
       roomAlarmState[i] = ON;
     }
   }
}


// room toggle security ON
_mqx_int room_on_callback(HTTPD_SESSION_STRUCT *session)
{
  int room = atoi(session->request.urldata)-1;
  httpd_sendstr(session->sock, "turned security on");
  roomAlarmState[room] = ON;
  return session->request.content_len;
}

// room toggle security OFF
_mqx_int room_off_callback(HTTPD_SESSION_STRUCT *session)
{
  int room = atoi(session->request.urldata)-1;
  httpd_sendstr(session->sock, "turned security off");
  roomAlarmState[room] = OFF;
  return session->request.content_len;
}

//get JSON format of current status of the system

_mqx_int get_status_callback(HTTPD_SESSION_STRUCT *session){

  char buffer[256];
  int hours,remain,min,sec;

  _rtc_get_time(&curr_time);

  hours = curr_time.seconds / (60*60);
  remain = curr_time.seconds % (60*60);
  min = remain/60;
  sec = remain%60;


  sprintf(buffer, "{\"rooms\":[%d,%d,%d,%d],\"time\":\"%u:%u:%u\",\"timeControl\":[%d,%d,%d,%d],\"zoneOn\":[%d,%d,%d,%d],\"zoneOff\":[%d,%d,%d,%d]}", roomAlarmState[0], roomAlarmState[1],roomAlarmState[2],roomAlarmState[3], hours, min, sec, timingControl[0], timingControl[1],timingControl[2],timingControl[3],zoneOn[0], zoneOn[1],zoneOn[2],zoneOn[3],zoneOff[0], zoneOff[1],zoneOff[2],zoneOff[3]);

  httpd_sendstr(session->sock, buffer);
  return session->request.content_len;

}

//Set the clock using hours, minutes and seconds
_mqx_int set_clock(HTTPD_SESSION_STRUCT *session)
{
  RTC_TIME_STRUCT the_new_time;

  int hours, minutes, seconds, i;
  sscanf(session->request.urldata, "%u:%u:%u", &hours, &minutes, &seconds);

  //Time struct only has seconds
  the_new_time.seconds = (hours*60*60) + (minutes*60) + seconds;
  _rtc_set_time(&the_new_time);

  //sprintf(buffer, "Clock time set to%u:%u:%u\n", (curr_time.seconds / (60*60)), (curr_time.seconds / 60), curr_time.seconds);

  for(i = 0; i < 4; i++) {
    alarmControl[i] = OFF;
  }
  httpd_sendstr(session->sock, "Time set");
  return session->request.content_len;
}

//Set the clock using hours, minutes and seconds
_mqx_int set_alarms(HTTPD_SESSION_STRUCT *session)
{

  int room, start_hours, start_minutes, start_seconds, end_hours, end_minutes, end_seconds;


  sscanf(session->request.urldata, "%d,%u:%u:%u,%u:%u:%u", &room, &start_hours, &start_minutes, &start_seconds, &end_hours, &end_minutes, &end_seconds);

  // id's start from 0
  room--;

  //Time struct only has seconds
  zoneOn[room] = (start_hours*60*60) + (start_minutes*60) + start_seconds;
  zoneOff[room] = (end_hours*60*60) + (end_minutes*60) + end_seconds;

  timingControl[room] = ON;

  httpd_sendstr(session->sock, "Alarm Set");
  return session->request.content_len;
}

_mqx_int reset_alarms(HTTPD_SESSION_STRUCT *session)
{
  int room;

  sscanf(session->request.urldata, "%d", &room);

  timingControl[room - 1] = OFF;

  httpd_sendstr(session->sock, "Alarm Reset");
  return session->request.content_len;
}

// cgi endpoints
static HTTPD_CGI_LINK_STRUCT http_cgi_params[] = { { "get_status", get_status_callback }, { "room_on", room_on_callback },{ "room_off", room_off_callback },{ "setalarm", set_alarms },{ "resetalarm", reset_alarms },{ "setrtc", set_clock }, {0,0}};


//Task 4 Alarm sensors
void _fix_alarm_sync(){
  int i;
  for (i = 0; i < 4; i++){
    if(roomAlarmState[i] == ALARM){
      btnled_set_value(hmi_client, leds[i], HMI_VALUE_ON);
    }
  }
}

void button_push(void *ptr)
{
  if(roomAlarmState[*((int*)ptr)]==ON){
    roomAlarmState[*((int*)ptr)] = ALARM;
    _fix_alarm_sync();
  }
}

void button_release(void *ptr)
{
  btnled_set_value(hmi_client, *((int*)ptr), HMI_VALUE_OFF);
}

/*TASK*-----------------------------------------------------------------
 * *
 * * Function Name  : Main_task
 * * Comments       :
 * *    This task initializes MFS and starts SHELL and a web server
 * *
 * *END------------------------------------------------------------------*/
void Main_task(uint_32 initial_data)
{

  int i;

  hmi_client = _bsp_btnled_init();
  rtcs_init();

  _rtc_init(RTC_INIT_FLAG_ENABLE);


  _io_tfs_install("tfs:", static_data);

  http_server = httpd_server_init_af(http_root_dir, "\\index.html", AF_INET);
  httpd_server_run(http_server);


  HTTPD_SET_PARAM_CGI_TBL(http_server, http_cgi_params);


  for (i = 0; i < 4; i++) {

    btnled_add_clb(hmi_client, buttons[i], HMI_VALUE_PUSH, button_push, &mapLeds[i]);
    btnled_add_clb(hmi_client, buttons[i], HMI_VALUE_RELEASE, button_release, &mapLeds[i]);

  }

  //ON OFF BUTTONS
  btnled_add_clb(hmi_client, HMI_BUTTON_5, HMI_VALUE_PUSH, _toggle_alarms, NULL);
  btnled_add_clb(hmi_client, HMI_BUTTON_6, HMI_VALUE_PUSH, _hush_alarms, NULL);


  while(1) {
    btnled_poll(hmi_client);
    ipcfg_task_poll();
  }
}




/*TASK*-----------------------------------------------------------------
 * *
 * * Function Name  : Led_task
 * * Comments       :
 * *    checks the current state of the alarm system and turns on/off/flash LEDs accordingly
 * *
 * *END------------------------------------------------------------------*/

void do_led_work()
{
int i;

for(i = 0; i<4; i++){

  if(roomAlarmState[i]==OFF)
  {
    btnled_set_value(hmi_client, leds[i], HMI_VALUE_OFF);
  }
  else if(roomAlarmState[i]==ON) {
    btnled_set_value(hmi_client, leds[i], HMI_VALUE_ON);
  }
  else if(roomAlarmState[i]==ALARM) {
      btnled_toogle(hmi_client, leds[i]);
  }
}

}

void Led_task(uint_32 initial_data)
{

  while(1){
    do_led_work();
    _time_delay_ticks(40);
  }
}



/*TASK*-----------------------------------------------------------------
 * *
 * * Function Name  : Alarm_task
 * * Comments       :
 * *    Task that deals with timingControl
 * *
 * *END------------------------------------------------------------------*/


/* function for checking alarm interaction with user when time control is set
*  - it controls time in a 24 hour period. so you can set alarm on at 3AM and off at 6PM and vice versa
*  - it accounts for logical interactions. If burglar sets alarm off at 2:59. It is not going to turn off at 3
*  - interacting with the web system and on off buttons will not break the timingControl
*  - each room has separate timing control
*/
void do_alarm_work()
{
  int i, time_secs, rollover_alarm;

  _rtc_get_time(&curr_time);

  //get seconds relative to the day
  time_secs = curr_time.seconds % (60*60*24);

  for (i = 0; i < 4; i++) {
    if(timingControl[i] == ON) {
      rollover_alarm = zoneOn[i] > zoneOff[i];

      switch (rollover_alarm) {
        case 0:  // When our alarm does not roll over a day. e.g. 18:30:0 -> 19:0:0
          if(time_secs >= zoneOn[i] && time_secs <= zoneOff[i] && alarmControl[i] == OFF) {
            if(roomAlarmState[i] != ALARM) {
              alarmControl[i]   = ON;
              roomAlarmState[i] = ON;
            }
          } else if(time_secs > zoneOff[i] && alarmControl[i] == ON) {
            if(roomAlarmState[i] != ALARM) {
              alarmControl[i]   = OFF;
              roomAlarmState[i] = OFF;
            } else {
              alarmControl[i]   = OFF;
            }
          }
        break;

        case 1: // When our alarm does roll over a day. e.g. 23:30:0 -> 0:30:0
          if((time_secs >= zoneOn[i] || time_secs <= zoneOff[i]) && alarmControl[i] == OFF) {
            if(roomAlarmState[i] != ALARM) {
              alarmControl[i]   = ON;
              roomAlarmState[i] = ON;
            }
          }
           else if(time_secs > zoneOff[i] && time_secs < zoneOn[i] && alarmControl[i] == ON) {
            if(roomAlarmState[i] != ALARM) {
              alarmControl[i]   = OFF;
              roomAlarmState[i] = OFF;
            } else {
              alarmControl[i]   = OFF;
            }
          }
        break;
      }

    }
  }
}

void Alarm_task(uint_32 initial_data)
{

  while(1){
    do_alarm_work();
    _time_delay_ticks(40);
  }
}

/* EOF */
