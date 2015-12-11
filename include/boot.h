/*
 * Copyright (c) 2009-2015 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#ifndef PREFIX
#define PREFIX "/usr"
#endif

#define WEARABLE_POWER_ON PREFIX"/share/edje/360x480_PowerOn.edj"
#define WEARABLE_POWER_OFF PREFIX"/share/edje/360x480_PowerOff.edj"
#define WVGA_POWER_ON PREFIX"/share/edje/480x800_PowerOn.edj"
#define WVGA_POWER_OFF PREFIX"/share/edje/480x800_PowerOff.edj"
#define HD_POWER_ON PREFIX"/share/edje/720x1280_PowerOn.edj"
#define HD_POWER_OFF PREFIX"/share/edje/720x1280_PowerOff.edj"
#define EDJ_OFF "poweroff.edj"
#define EDJ_ON "poweron.edj"
#define GRP_OFF "poweroff"
#define GRP_ON "poweron"

#ifdef ARM_TYPE
#define DEFAULT_POWERON_WAV		PREFIX"/share/keysound/poweron.ogg"
#define DEFAULT_POWEROFF_WAV	PREFIX"/share/keysound/poweroff.ogg"

#define POWERON_WAV		"poweron.ogg"
#define POWEROFF_WAV	"poweroff.ogg"
#else
#define DEFAULT_POWERON_WAV		PREFIX"/share/keysound/poweron.wav"
#define DEFAULT_POWEROFF_WAV	PREFIX"/share/keysound/poweroff.wav"

#define POWERON_WAV		"poweron.wav"
#define POWEROFF_WAV	"poweroff.wav"
#endif


#define NICE_VALUE -20

#define TITLE "Password Application"

#define BA_DEFAULT_WINDOW_H	800	/* Default is HD(720 X 1280) */
#define BA_DEFAULT_WINDOW_W	1280	/* Default is WXGA(1280 X 800) */

#define FILE_PATH_MAX 128

enum {
	TYPE_UNKNOWN,
	TYPE_ON,
	TYPE_OFF,
	TYPE_OFF_NOEXIT,
	TYPE_OFF_WITH_MSG,
	TYPE_CLEAR,
	TYPE_MAX
};

enum {
	TYPE_CHECK_ANI,
	TYPE_CHECK_SOUND,
};
