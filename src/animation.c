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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <unistd.h>
#include <dirent.h>

#include <vconf.h>

#include <Elementary.h>

#include <boot.h>
#include <animation.h>

#include "log.h"

#define OVER_COUNT 19

static struct animation {
	Evas_Object *win;
	Evas_Coord w;
	Evas_Coord h;
	int t;
	Evas *evas;
	Ecore_Evas *ee;
	Evas_Object *layout;
	int state;
	Evas_Object *txt;
} s_animation = {
	.txt = NULL,};

static void win_del(void *data, Evas_Object * obj, void *event_info)
{
	_D("Window delete event received");
	elm_exit();
}

static Eina_Bool _end_cb(void *data)
{
	_D("_end_cb is invoked");
	printf("_end_cb is invoked\n");
	if (vconf_set_int(VCONFKEY_BOOT_ANIMATION_FINISHED, 1) != 0)
		_E("Failed to set finished set");
	elm_exit();
	return ECORE_CALLBACK_CANCEL;
}

static void _edje_cb(void *d, Evas_Object * obj, const char *e, const char *s)
{
	_D("edje callback is invoked");

	if (s_animation.state == TYPE_OFF || s_animation.state == TYPE_OFF_WITH_MSG) {
		_D("TYPE OFF");
		if (vconf_set_int(VCONFKEY_BOOT_ANIMATION_FINISHED, 1) != 0)
			_E("Failed to set finished set");
		if (s_animation.txt) {
			Evas_Coord w;
			Evas_Coord h;

			evas_object_size_hint_weight_set(s_animation.txt,
							 EVAS_HINT_EXPAND,
							 EVAS_HINT_EXPAND);
			evas_object_size_hint_fill_set(s_animation.txt,
						       EVAS_HINT_FILL,
						       EVAS_HINT_FILL);
			evas_object_resize(s_animation.txt, s_animation.w,
					   s_animation.h);
			evas_object_color_set(s_animation.txt, 255, 255, 255,
					      255);
			evas_object_text_font_set(s_animation.txt,
						  "SLP:style=medium", 30);
			evas_object_geometry_get(s_animation.txt, NULL, NULL,
						 &w, &h);
			evas_object_move(s_animation.txt,
					 (s_animation.w - w) >> 1,
					 (s_animation.h - h) >> 1);
			evas_object_show(s_animation.txt);
		}
		ecore_timer_add(1, _end_cb, NULL);
	} else {
		_D("TYPE_ON");
		_end_cb(NULL);
	}
}

#define DEFAULT_W 480
static void layout_file_set(int state)
{
	_D("Layout file set according to resolution");
	_D("Screen Width: %d, DEFAULT_WIDTH: %d", s_animation.w, DEFAULT_W);

	switch(s_animation.w)
	{
		case 360:
			_D("Resolution is %dx%d", s_animation.w, s_animation.h);
			if (state == TYPE_ON) {
				elm_layout_file_set(s_animation.layout, WEARABLE_POWER_ON, GRP_ON);
			} else {
				elm_layout_file_set(s_animation.layout, WEARABLE_POWER_OFF, GRP_OFF);
			}
			break;
		case 480:
			_D("Resolution is %dx%d", s_animation.w, s_animation.h);
			if (state == TYPE_ON) {
				elm_layout_file_set(s_animation.layout, WVGA_POWER_ON, GRP_ON);
			} else {
				elm_layout_file_set(s_animation.layout, WVGA_POWER_OFF, GRP_OFF);
			}
			break;
		case 720:
			_D("Resolution is %dx%d", s_animation.w, s_animation.h);
			if (state == TYPE_ON) {
				elm_layout_file_set(s_animation.layout, HD_POWER_ON, GRP_ON);
			} else {
				elm_layout_file_set(s_animation.layout, HD_POWER_OFF, GRP_OFF);
			}
			break;
		default:
			_D("This is Strange resolution, Plz check, %dx%d", s_animation.w, s_animation.h);
			if (state == TYPE_ON) {
				elm_layout_file_set(s_animation.layout, HD_POWER_ON, GRP_ON);
			} else {
				elm_layout_file_set(s_animation.layout, HD_POWER_ON, GRP_OFF);
			}
			break;
	}
}

static int init_layout(const char *msg)
{
	s_animation.layout = elm_layout_add(s_animation.win);
	if (!s_animation.layout) {
		_E("Failed to create layout");
		return EXIT_FAILURE;
	}

	layout_file_set(s_animation.state);

	evas_object_size_hint_weight_set(s_animation.layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	_D("[Boot-ani] Start animation: %d mode", s_animation.state);
	elm_win_resize_object_add(s_animation.win, s_animation.layout);
	edje_object_signal_callback_add(elm_layout_edje_get(s_animation.layout), "end", "animation", _edje_cb, NULL);
	evas_object_show(s_animation.layout);

	if (msg) {
		if (!s_animation.txt) {
			s_animation.txt = evas_object_text_add(s_animation.evas);
			if (!s_animation.txt) {
				_E("Failed to add text");
				evas_object_del(s_animation.layout);
				return EXIT_FAILURE;
			}
		}

		evas_object_text_text_set(s_animation.txt, msg);
		evas_object_hide(s_animation.txt);
	}

	return EXIT_SUCCESS;
}

static void fini_layout(void)
{
	if (s_animation.layout) {
		evas_object_del(s_animation.layout);
	}

	if (s_animation.txt) {
		evas_object_del(s_animation.txt);
		s_animation.txt = NULL;
	}
}

static int create_window(void)
{
	_D("Create Window");
	printf("Create Window\n");

	int x, y = 0;

	s_animation.win = elm_win_add(NULL, "BOOT_ANIMATION", ELM_WIN_NOTIFICATION);
	elm_win_role_set(s_animation.win, "alert");
	if (!s_animation.win) {
		_E("Failed to create a new window");
		printf("Failed to create a new window\n");
		return EXIT_FAILURE;
	}
	if (s_animation.state == TYPE_OFF || s_animation.state == TYPE_OFF_WITH_MSG) {
		_D("We are turning off the Tizen");
		elm_win_alpha_set(s_animation.win, EINA_TRUE);
	}
	evas_object_smart_callback_add(s_animation.win, "delete-request", win_del, NULL);

	s_animation.evas = evas_object_evas_get(s_animation.win);
	if (!s_animation.evas) {
		evas_object_del(s_animation.win);
		_E("Failed to get the evas object");
		return EXIT_FAILURE;
	}
	elm_win_screen_size_get(s_animation.win, &x, &y, &s_animation.w, &s_animation.h);
	_D("Window size is x: %d, y: %d, w: %d, h: %d", x, y, s_animation.w, s_animation.h);
	elm_win_borderless_set(s_animation.win, 1);
	elm_win_indicator_mode_set(s_animation.win, ELM_WIN_INDICATOR_HIDE);
	evas_object_move(s_animation.win, 0, 0);

/*	if (s_animation.w > s_animation.h) {
		int t;
		elm_win_rotation_with_resize_set(s_animation.win, 90);
		t = s_animation.w;
		s_animation.w = s_animation.h;
		s_animation.h = t;
	}*/

	evas_object_show(s_animation.win);

	s_animation.ee = ecore_evas_ecore_evas_get(s_animation.evas);
	if (!s_animation.ee) {
		evas_object_del(s_animation.win);
		_E("Failed to get the ecore evas object");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

int init_animation(int state, const char *msg)
{
	_D("Init animation");
	printf("Init animation\n");

	s_animation.state = state;

	if (create_window() == EXIT_FAILURE) {
		_E("Failed to create a new window");
		printf("Failed to create a new window\n");
		return EXIT_FAILURE;
	}

	if (init_layout(msg) == EXIT_FAILURE) {
		_E("Failed to init the layout object");
		if (msg) {
			evas_object_del(s_animation.txt);
		}
		evas_object_del(s_animation.win);
	}

	return EXIT_SUCCESS;
}

int fini_animation(void)
{
	fini_layout();
	evas_object_del(s_animation.win);
	fflush(stdout);
	close(1);
	return EXIT_SUCCESS;
}
