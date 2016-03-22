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
   {0,           0,           0,     0,   0,      0,                 }
};

HMI_CLIENT_STRUCT_PTR hmi_client;
HTTPD_STRUCT *http_server;

RTC_TIME_STRUCT curr_time;


unsigned char http_refresh_text[] = "index page";

const TFS_DIR_ENTRY static_data[] = {
	{ "/index.html", 0, http_refresh_text, sizeof(http_refresh_text) },
	{0,0,0,0}
	};

static HTTPD_ROOT_DIR_STRUCT http_root_dir[] = { {"", "tfs:"}, { 0,0 } };

//CGI Function to toggle leds by ID
_mqx_int led_callback(HTTPD_SESSION_STRUCT *session)
{
  int led = atoi(session->request.urldata);
  httpd_sendstr(session->sock, "<html><body>LED toggled</body></html");
  btnled_toogle(hmi_client, HMI_GET_LED_ID(led));
  return session->request.content_len;
}

//Display the real time clock on webpage
_mqx_int real_time_clock(HTTPD_SESSION_STRUCT *session)
{
  char buffer[32];
  int hours,remain,min,sec;

  _rtc_get_time(&curr_time);

  hours = curr_time.seconds / (60*60);
  remain = curr_time.seconds % (60*60);
  min = remain/60;
  sec = remain%60;


  sprintf(buffer, "%u:%u:%u\n", hours, min, sec);

  httpd_sendstr(session->sock, buffer);
  return session->request.content_len;
}

//Set the clock using hours, minutes and seconds in format H:m:s
_mqx_int set_clock(HTTPD_SESSION_STRUCT *session)
{
  RTC_TIME_STRUCT the_new_time;

  int hours, minutes, seconds;
  sscanf(session->request.urldata, "%u:%u:%u", &hours, &minutes, &seconds);

  //Time struct only has seconds
  the_new_time.seconds = (hours*60*60) + (minutes*60) + seconds;
  _rtc_set_time(&the_new_time);

  httpd_sendstr(session->sock, "Time set");
  return session->request.content_len;
}

//Add to table for task 3
static HTTPD_CGI_LINK_STRUCT http_cgi_params[] = { { "led", led_callback },{ "rtc", real_time_clock },{ "setrtc", set_clock }, {0,0}};

void Main_task(uint_32 initial_data)
{
	int buttons[] = {HMI_BUTTON_1, HMI_BUTTON_2, HMI_BUTTON_3, HMI_BUTTON_4};
	int leds[] = {HMI_LED_1, HMI_LED_2, HMI_LED_3, HMI_LED_4};
	int i;

	hmi_client = _bsp_btnled_init();
	rtcs_init();

  _rtc_init(RTC_INIT_FLAG_ENABLE);

	_io_tfs_install("tfs:", static_data);
	http_server = httpd_server_init_af(http_root_dir, "\\index.html", AF_INET);
	httpd_server_run(http_server);


  HTTPD_SET_PARAM_CGI_TBL(http_server, http_cgi_params);


	while(1) ipcfg_task_poll();

}
/* EOF */