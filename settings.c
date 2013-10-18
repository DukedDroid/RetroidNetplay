/*  RetroArch - A frontend for libretro.
 *  Copyright (C) 2010-2013 - Hans-Kristian Arntzen
 * 
 *  RetroArch is free software: you can redistribute it and/or modify it under the terms
 *  of the GNU General Public License as published by the Free Software Found-
 *  ation, either version 3 of the License, or (at your option) any later version.
 *
 *  RetroArch is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 *  without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 *  PURPOSE.  See the GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along with RetroArch.
 *  If not, see <http://www.gnu.org/licenses/>.
 */

#include "general.h"
#include "conf/config_file.h"
#include "conf/config_file_macros.h"
#include "compat/strl.h"
#include "config.def.h"
#include "file.h"
#include "compat/posix_string.h"
#include "input/input_common.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <ctype.h>

struct settings g_settings;
struct global g_extern;

const char *config_get_default_audio(void)
{
   switch (AUDIO_DEFAULT_DRIVER)
   {
      case AUDIO_RSOUND:
         return "rsound";
      case AUDIO_OSS:
         return "oss";
      case AUDIO_ALSA:
         return "alsa";
      case AUDIO_ALSATHREAD:
         return "alsathread";
      case AUDIO_ROAR:
         return "roar";
      case AUDIO_COREAUDIO:
         return "coreaudio";
      case AUDIO_AL:
         return "openal";
      case AUDIO_SL:
         return "opensl";
      case AUDIO_SDL:
         return "sdl";
      case AUDIO_DSOUND:
         return "dsound";
      case AUDIO_XAUDIO:
         return "xaudio";
      case AUDIO_PULSE:
         return "pulse";
      case AUDIO_EXT:
         return "ext";
      case AUDIO_XENON360:
         return "xenon360";
      case AUDIO_PS3:
         return "ps3";
      case AUDIO_WII:
         return "gx";
      case AUDIO_RWEBAUDIO:
         return "rwebaudio";
      case AUDIO_NULL:
         return "null";
      default:
         return NULL;
   }
}

const char *config_get_default_video(void)
{
   switch (VIDEO_DEFAULT_DRIVER)
   {
      case VIDEO_GL:
         return "gl";
      case VIDEO_WII:
         return "gx";
      case VIDEO_XENON360:
         return "xenon360";
      case VIDEO_XDK_D3D:
         return "xdk_d3d";
      case VIDEO_D3D9:
         return "d3d9";
      case VIDEO_PSP1:
         return "psp1";
      case VIDEO_VITA:
         return "vita";
      case VIDEO_XVIDEO:
         return "xvideo";
      case VIDEO_SDL:
         return "sdl";
      case VIDEO_EXT:
         return "ext";
      case VIDEO_VG:
         return "vg";
      case VIDEO_NULL:
         return "null";
      case VIDEO_OMAP:
         return "omap";
      default:
         return NULL;
   }
}

const char *config_get_default_input(void)
{
   switch (INPUT_DEFAULT_DRIVER)
   {
      case INPUT_ANDROID:
         return "android_input";
      case INPUT_PS3:
         return "ps3";
      case INPUT_PSP:
         return "psp";
      case INPUT_SDL:
         return "sdl";
      case INPUT_DINPUT:
         return "dinput";
      case INPUT_X:
         return "x";
      case INPUT_XENON360:
         return "xenon360";
      case INPUT_XINPUT:
         return "xinput";
      case INPUT_WII:
         return "gx";
      case INPUT_LINUXRAW:
         return "linuxraw";
      case INPUT_APPLE:
         return "apple_input";
      case INPUT_QNX:
      	 return "qnx_input";
      case INPUT_RWEBINPUT:
      	 return "rwebinput";
      case INPUT_NULL:
         return "null";
      default:
         return NULL;
   }
}

void config_set_defaults(void)
{
   const char *def_video = config_get_default_video();
   const char *def_audio = config_get_default_audio();
   const char *def_input = config_get_default_input();

   if (def_video)
      strlcpy(g_settings.video.driver, def_video, sizeof(g_settings.video.driver));
   if (def_audio)
      strlcpy(g_settings.audio.driver, def_audio, sizeof(g_settings.audio.driver));
   if (def_input)
      strlcpy(g_settings.input.driver, def_input, sizeof(g_settings.input.driver));

   g_settings.video.xscale = xscale;
   g_settings.video.yscale = yscale;
   g_settings.video.fullscreen = g_extern.force_fullscreen ? true : fullscreen;
   g_settings.video.windowed_fullscreen = windowed_fullscreen;
   g_settings.video.monitor_index = monitor_index;
   g_settings.video.fullscreen_x = fullscreen_x;
   g_settings.video.fullscreen_y = fullscreen_y;
   g_settings.video.disable_composition = disable_composition;
   g_settings.video.vsync = vsync;
   g_settings.video.hard_sync = hard_sync;
   g_settings.video.hard_sync_frames = hard_sync_frames;
   g_settings.video.black_frame_insertion = black_frame_insertion;
   g_settings.video.swap_interval = swap_interval;
   g_settings.video.threaded = video_threaded;
   g_settings.video.smooth = video_smooth;
   g_settings.video.force_aspect = force_aspect;
   g_settings.video.scale_integer = scale_integer;
   g_settings.video.crop_overscan = crop_overscan;
   g_settings.video.aspect_ratio = aspect_ratio;
   g_settings.video.aspect_ratio_auto = aspect_ratio_auto; // Let implementation decide if automatic, or 1:1 PAR.
   g_settings.video.aspect_ratio_idx = aspect_ratio_idx;
   g_settings.video.shader_enable = shader_enable;
   g_settings.video.allow_rotate = allow_rotate;

   g_settings.video.font_enable = font_enable;
   g_settings.video.font_size = font_size;
   g_settings.video.font_scale = font_scale;
   g_settings.video.msg_pos_x = message_pos_offset_x;
   g_settings.video.msg_pos_y = message_pos_offset_y;
   
   g_settings.video.msg_color_r = ((message_color >> 16) & 0xff) / 255.0f;
   g_settings.video.msg_color_g = ((message_color >>  8) & 0xff) / 255.0f;
   g_settings.video.msg_color_b = ((message_color >>  0) & 0xff) / 255.0f;

   g_settings.video.refresh_rate = refresh_rate;
   g_settings.video.post_filter_record = post_filter_record;
   g_settings.video.gpu_record = gpu_record;
   g_settings.video.gpu_screenshot = gpu_screenshot;
   g_settings.video.rotation = ORIENTATION_NORMAL;

   g_settings.audio.enable = audio_enable;
   g_settings.audio.out_rate = out_rate;
   g_settings.audio.block_frames = 0;
   g_settings.audio.in_rate = out_rate;
   if (audio_device)
      strlcpy(g_settings.audio.device, audio_device, sizeof(g_settings.audio.device));
   g_settings.audio.latency = out_latency;
   g_settings.audio.sync = audio_sync;
   g_settings.audio.rate_control = rate_control;
   g_settings.audio.rate_control_delta = rate_control_delta;
   g_settings.audio.volume = audio_volume;

   g_settings.rewind_enable = rewind_enable;
   g_settings.rewind_buffer_size = rewind_buffer_size;
   g_settings.rewind_granularity = rewind_granularity;
   g_settings.slowmotion_ratio = slowmotion_ratio;
   g_settings.fastforward_ratio = fastforward_ratio;
   g_settings.pause_nonactive = pause_nonactive;
   g_settings.autosave_interval = autosave_interval;

   g_settings.block_sram_overwrite = block_sram_overwrite;
   g_settings.savestate_auto_index = savestate_auto_index;
   g_settings.savestate_auto_save  = savestate_auto_save;
   g_settings.savestate_auto_load  = savestate_auto_load;
   g_settings.network_cmd_enable   = network_cmd_enable;
   g_settings.network_cmd_port     = network_cmd_port;
   g_settings.stdin_cmd_enable     = stdin_cmd_enable;
   g_settings.game_history_size    = game_history_size;

   rarch_assert(sizeof(g_settings.input.binds[0]) >= sizeof(retro_keybinds_1));
   rarch_assert(sizeof(g_settings.input.binds[1]) >= sizeof(retro_keybinds_rest));
   memcpy(g_settings.input.binds[0], retro_keybinds_1, sizeof(retro_keybinds_1));
   for (unsigned i = 1; i < MAX_PLAYERS; i++)
      memcpy(g_settings.input.binds[i], retro_keybinds_rest, sizeof(retro_keybinds_rest));

   for (unsigned i = 0; i < MAX_PLAYERS; i++)
   {
      for (unsigned j = 0; j < RARCH_BIND_LIST_END; j++)
      {
         g_settings.input.autoconf_binds[i][j].joykey = NO_BTN;
         g_settings.input.autoconf_binds[i][j].joyaxis = AXIS_NONE;
      }
   }

   // Verify that binds are in proper order.
   for (int i = 0; i < MAX_PLAYERS; i++)
      for (int j = 0; j < RARCH_BIND_LIST_END; j++)
         if (g_settings.input.binds[i][j].valid)
            rarch_assert(j == g_settings.input.binds[i][j].id);

   g_settings.input.axis_threshold = axis_threshold;
   g_settings.input.netplay_client_swap_input = netplay_client_swap_input;
   g_settings.input.turbo_period = turbo_period;
   g_settings.input.turbo_duty_cycle = turbo_duty_cycle;
   g_settings.input.overlay_opacity = 1.0f;
   g_settings.input.overlay_scale = 1.0f;
   g_settings.input.debug_enable = input_debug_enable;
   g_settings.input.autodetect_enable = input_autodetect_enable;
#ifdef ANDROID
   g_settings.input.back_behavior = BACK_BUTTON_QUIT;
#endif

   for (int i = 0; i < MAX_PLAYERS; i++)
   {
      g_settings.input.joypad_map[i] = i;
      if (!g_extern.has_set_libretro_device[i])
         g_settings.input.libretro_device[i] = RETRO_DEVICE_JOYPAD;
   }

   g_extern.console.screen.viewports.custom_vp.width = 0;
   g_extern.console.screen.viewports.custom_vp.height = 0;
   g_extern.console.screen.viewports.custom_vp.x = 0;
   g_extern.console.screen.viewports.custom_vp.y = 0;

#ifdef RARCH_CONSOLE
   g_extern.lifecycle_mode_state |= ((1ULL << MODE_INFO_DRAW) | (1ULL << MODE_MENU));

   strlcpy(g_settings.system_directory, default_paths.system_dir, sizeof(g_settings.system_directory));

   g_settings.video.msg_pos_x = 0.05f;
   g_settings.video.msg_pos_y = 0.90f;
   g_settings.video.aspect_ratio = -1.0f;

   // g_extern
   strlcpy(g_extern.savefile_dir, default_paths.sram_dir, sizeof(g_extern.savefile_dir));
   g_extern.console.screen.gamma_correction = DEFAULT_GAMMA;
   g_extern.lifecycle_mode_state |= (1ULL << MODE_AUDIO_CUSTOM_BGM_ENABLE);
   g_extern.lifecycle_mode_state |= (1ULL << MODE_VIDEO_TRIPLE_BUFFERING_ENABLE);
   g_extern.lifecycle_mode_state |= (1ULL << MODE_VIDEO_SOFT_FILTER_ENABLE);
   g_extern.lifecycle_mode_state |= (1ULL << MODE_VIDEO_FLICKER_FILTER_ENABLE);

   g_extern.console.screen.resolutions.current.id = 0;
   strlcpy(g_extern.savestate_dir, default_paths.savestate_dir, sizeof(g_extern.savestate_dir));
#ifdef HAVE_RMENU
   strlcpy(g_extern.menu_texture_path, default_paths.menu_border_file, sizeof(g_extern.menu_texture_path));
#endif

   g_extern.state_slot = 0;
   g_extern.audio_data.mute = 0;
   g_extern.verbose = true;

   g_extern.console.sound.mode = SOUND_MODE_NORMAL;
#ifdef _XBOX1
   g_extern.console.sound.volume_level = 0;
#endif
#endif
   
#ifdef HAVE_OVERLAY
#if defined(__QNX__)
   strlcpy(g_extern.overlay_dir, "app/native/overlays/", sizeof(g_extern.overlay_dir));
   RARCH_LOG("Setting default overlay %s ...\n", "app/native/overlays/snes-landscape.cfg");
   strlcpy(g_settings.input.overlay, "app/native/overlays/snes-landscape.cfg", sizeof(g_settings.input.overlay));
#elif defined(IOS)
   strlcpy(g_extern.overlay_dir, "/Applications/RetroArch.app/overlays/", sizeof(g_extern.overlay_dir));
#elif defined(ANDROID)
   strlcpy(g_extern.overlay_dir, "/data/data/org.retroarch/overlays/", sizeof(g_extern.overlay_dir));
#endif
#endif

#if defined(HAVE_CG) || defined(HAVE_GLSL) || defined(HAVE_HLSL)
#if defined(__QNX__)
   strlcpy(g_settings.video.shader_dir, "/app/native/shaders_glsl/", sizeof(g_settings.video.shader_dir));
#elif defined(IOS)
   strlcpy(g_settings.video.shader_dir, "/Applications/RetroArch.app/shaders_glsl/", sizeof(g_settings.video.shader_dir));
#elif defined(ANDROID)
   strlcpy(g_settings.video.shader_dir, "/data/data/org.retroarch/shaders_glsl/", sizeof(g_settings.video.shader_dir));
#endif
#endif

   g_extern.config_save_on_exit = config_save_on_exit;

#if defined(HAVE_RMENU) || defined(HAVE_RGUI)
   /* Avoid reloading config on every ROM load */
   g_extern.block_config_read = true;
#endif

   rarch_init_msg_queue();
}

static void parse_config_file(void);

void config_load(void)
{
   if (!g_extern.block_config_read)
   {
      config_set_defaults();
      parse_config_file();
   }
}

static config_file_t *open_default_config_file(void)
{
   config_file_t *conf = NULL;

#if defined(_WIN32) && !defined(_XBOX)
   char conf_path[PATH_MAX];

   char app_path[PATH_MAX];
   fill_pathname_application_path(app_path, sizeof(app_path));
   fill_pathname_resolve_relative(conf_path, app_path, "retroarch.cfg", sizeof(conf_path));

   conf = config_file_new(conf_path);
   if (!conf)
   {
      const char *appdata = getenv("APPDATA");
      if (appdata)
      {
         fill_pathname_join(conf_path, appdata, "retroarch.cfg", sizeof(conf_path));
         conf = config_file_new(conf_path);
      }
   }

   if (conf)
      strlcpy(g_extern.config_path, conf_path, sizeof(g_extern.config_path));
#elif !defined(__CELLOS_LV2__) && !defined(_XBOX)
   char conf_path[PATH_MAX];
   const char *xdg  = getenv("XDG_CONFIG_HOME");
   const char *home = getenv("HOME");

   // XDG_CONFIG_HOME falls back to $HOME/.config.
   if (xdg)
      snprintf(conf_path, sizeof(conf_path), "%s/retroarch/retroarch.cfg", xdg);
   else if (home)
      snprintf(conf_path, sizeof(conf_path), "%s/.config/retroarch/retroarch.cfg", home);

   if (xdg || home)
   {
      RARCH_LOG("Looking for config in: \"%s\".\n", conf_path);
      conf = config_file_new(conf_path);
   }

   // Fallback to $HOME/.retroarch.cfg.
   if (!conf && home)
   {
      snprintf(conf_path, sizeof(conf_path), "%s/.retroarch.cfg", home);
      RARCH_LOG("Looking for config in: \"%s\".\n", conf_path);
      conf = config_file_new(conf_path);
   }

   // Try this as a last chance ...
   if (!conf)
   {
#ifndef GLOBAL_CONFIG_DIR
#define GLOBAL_CONFIG_DIR "/etc"
#endif
      fill_pathname_join(conf_path, GLOBAL_CONFIG_DIR, "retroarch.cfg", sizeof(conf_path));
      RARCH_LOG("Looking for config in: \"%s\".\n", conf_path);
      conf = config_file_new(conf_path);
   }

   if (conf)
      strlcpy(g_extern.config_path, conf_path, sizeof(g_extern.config_path));
#endif
   
   return conf;
}

static void config_read_keybinds_conf(config_file_t *conf);

static void parse_config_file(void)
{
   bool ret;
   if (*g_extern.config_path)
   {
      RARCH_LOG("Loading config from: %s.\n", g_extern.config_path);
      ret = config_load_file(g_extern.config_path);
   }
   else
   {
      RARCH_LOG("Loading default config.\n");
      ret = config_load_file(NULL);
      if (*g_extern.config_path)
         RARCH_LOG("Found default config: %s.\n", g_extern.config_path);
   }

   if (!ret)
   {
      RARCH_ERR("Couldn't find config at path: \"%s\"\n", g_extern.config_path);
#ifndef RARCH_CONSOLE
      rarch_fail(1, "parse_config_file()");
#endif
   }
}

bool config_load_file(const char *path)
{
   config_file_t *conf = NULL;

   if (path)
   {
      conf = config_file_new(path);
      if (!conf)
         return false;
   }
   else
      conf = open_default_config_file();

   if (conf == NULL)
      return true;

   char *save;
   char tmp_append_path[PATH_MAX]; // Don't destroy append_config_path.
   strlcpy(tmp_append_path, g_extern.append_config_path, sizeof(tmp_append_path));
   const char *extra_path = strtok_r(tmp_append_path, ",", &save);
   while (extra_path)
   {
      RARCH_LOG("Appending config \"%s\"\n", extra_path);
      bool ret = config_append_file(conf, extra_path);
      if (!ret)
         RARCH_ERR("Failed to append config \"%s\"\n", extra_path);
      extra_path = strtok_r(NULL, ";", &save);
   }

   if (g_extern.verbose)
   {
      RARCH_LOG_OUTPUT("=== Config ===\n");
      config_file_dump_all(conf, stderr);
      RARCH_LOG_OUTPUT("=== Config end ===\n");
   }

   char tmp_str[PATH_MAX];

   CONFIG_GET_FLOAT(video.xscale, "video_xscale");
   CONFIG_GET_FLOAT(video.yscale, "video_yscale");
   CONFIG_GET_INT(video.fullscreen_x, "video_fullscreen_x");
   CONFIG_GET_INT(video.fullscreen_y, "video_fullscreen_y");

   if (!g_extern.force_fullscreen)
      CONFIG_GET_BOOL(video.fullscreen, "video_fullscreen");

   CONFIG_GET_BOOL(video.windowed_fullscreen, "video_windowed_fullscreen");
   CONFIG_GET_INT(video.monitor_index, "video_monitor_index");
   CONFIG_GET_BOOL(video.disable_composition, "video_disable_composition");
   CONFIG_GET_BOOL(video.vsync, "video_vsync");
   CONFIG_GET_BOOL(video.hard_sync, "video_hard_sync");

   CONFIG_GET_INT(video.hard_sync_frames, "video_hard_sync_frames");
   if (g_settings.video.hard_sync_frames > 3)
      g_settings.video.hard_sync_frames = 3;

   CONFIG_GET_BOOL(video.black_frame_insertion, "video_black_frame_insertion");
   CONFIG_GET_INT(video.swap_interval, "video_swap_interval");
   g_settings.video.swap_interval = max(g_settings.video.swap_interval, 1);
   g_settings.video.swap_interval = min(g_settings.video.swap_interval, 4);
   CONFIG_GET_BOOL(video.threaded, "video_threaded");
   CONFIG_GET_BOOL(video.smooth, "video_smooth");
   CONFIG_GET_BOOL(video.force_aspect, "video_force_aspect");
   CONFIG_GET_BOOL(video.scale_integer, "video_scale_integer");
   CONFIG_GET_BOOL(video.crop_overscan, "video_crop_overscan");
   CONFIG_GET_FLOAT(video.aspect_ratio, "video_aspect_ratio");
   CONFIG_GET_INT(video.aspect_ratio_idx, "aspect_ratio_index");
   CONFIG_GET_BOOL(video.aspect_ratio_auto, "video_aspect_ratio_auto");
   CONFIG_GET_FLOAT(video.refresh_rate, "video_refresh_rate");

   CONFIG_GET_PATH(video.shader_path, "video_shader");
   CONFIG_GET_BOOL(video.shader_enable, "video_shader_enable");

   CONFIG_GET_BOOL(video.allow_rotate, "video_allow_rotate");

   CONFIG_GET_PATH(video.font_path, "video_font_path");
   CONFIG_GET_FLOAT(video.font_size, "video_font_size");
   CONFIG_GET_BOOL(video.font_enable, "video_font_enable");
   CONFIG_GET_BOOL(video.font_scale, "video_font_scale");
   CONFIG_GET_FLOAT(video.msg_pos_x, "video_message_pos_x");
   CONFIG_GET_FLOAT(video.msg_pos_y, "video_message_pos_y");
   CONFIG_GET_INT(video.rotation, "video_rotation");

#ifdef RARCH_CONSOLE
   /* TODO - will be refactored later to make it more clean - it's more 
    * important that it works for consoles right now */

   for (unsigned i = 0; i < MAX_PLAYERS; i++)
   {
      char cfg[64];
      snprintf(cfg, sizeof(cfg), "input_dpad_emulation_p%u", i + 1);
      CONFIG_GET_INT(input.dpad_emulation[i], cfg);
      snprintf(cfg, sizeof(cfg), "input_device_p%u", i + 1);
      CONFIG_GET_INT(input.device[i], cfg);
   }

   CONFIG_GET_BOOL_EXTERN(console.screen.gamma_correction, "gamma_correction");

   bool msg_enable = false;
   bool triple_buffering_enable = false;
   bool custom_bgm_enable = false;
   bool flicker_filter_enable = false;
   bool soft_filter_enable = false;

#ifdef HAVE_RMENU
   if (config_get_path(conf, "menu_texture_path", tmp_str, sizeof(tmp_str)))
      strlcpy(g_extern.menu_texture_path, tmp_str, sizeof(g_extern.menu_texture_path));
#endif

   if (config_get_bool(conf, "info_msg_enable", &msg_enable))
   {
      if (msg_enable)
         g_extern.lifecycle_mode_state |= (1ULL << MODE_INFO_DRAW);
      else 
         g_extern.lifecycle_mode_state &= ~(1ULL << MODE_INFO_DRAW);
   }

   if (config_get_bool(conf, "triple_buffering_enable", &triple_buffering_enable))
   {
      if (triple_buffering_enable)
         g_extern.lifecycle_mode_state |= (1ULL << MODE_VIDEO_TRIPLE_BUFFERING_ENABLE);
      else
         g_extern.lifecycle_mode_state &= ~(1ULL << MODE_VIDEO_TRIPLE_BUFFERING_ENABLE);
   }

   if (config_get_bool(conf, "custom_bgm_enable", &custom_bgm_enable))
   {
      if (custom_bgm_enable)
         g_extern.lifecycle_mode_state |= (1ULL << MODE_AUDIO_CUSTOM_BGM_ENABLE);
      else
         g_extern.lifecycle_mode_state &= ~(1ULL << MODE_AUDIO_CUSTOM_BGM_ENABLE);
   }

   if (config_get_bool(conf, "flicker_filter_enable", &flicker_filter_enable))
   {
      if (flicker_filter_enable)
         g_extern.lifecycle_mode_state |= (1ULL << MODE_VIDEO_FLICKER_FILTER_ENABLE);
      else 
         g_extern.lifecycle_mode_state &= ~(1ULL << MODE_VIDEO_FLICKER_FILTER_ENABLE);
   }

   if (config_get_bool(conf, "soft_filter_enable", &soft_filter_enable))
   {
      if (soft_filter_enable)
         g_extern.lifecycle_mode_state |= (1ULL << MODE_VIDEO_SOFT_FILTER_ENABLE);
      else 
         g_extern.lifecycle_mode_state &= ~(1ULL << MODE_VIDEO_SOFT_FILTER_ENABLE);
   }

   CONFIG_GET_INT_EXTERN(console.screen.flicker_filter_index, "flicker_filter_index");
   CONFIG_GET_INT_EXTERN(console.screen.soft_filter_index, "soft_filter_index");
#ifdef _XBOX1
   CONFIG_GET_INT_EXTERN(console.sound.volume_level, "sound_volume_level");
#endif
   CONFIG_GET_INT_EXTERN(console.screen.resolutions.current.id, "current_resolution_id");
   CONFIG_GET_INT_EXTERN(state_slot, "state_slot");
   CONFIG_GET_INT_EXTERN(audio_data.mute, "audio_mute");
   CONFIG_GET_INT_EXTERN(console.sound.mode, "sound_mode");
#endif

   CONFIG_GET_INT_EXTERN(console.screen.viewports.custom_vp.x, "custom_viewport_x");
   CONFIG_GET_INT_EXTERN(console.screen.viewports.custom_vp.y, "custom_viewport_y");
   CONFIG_GET_INT_EXTERN(console.screen.viewports.custom_vp.width, "custom_viewport_width");
   CONFIG_GET_INT_EXTERN(console.screen.viewports.custom_vp.height, "custom_viewport_height");

   unsigned msg_color = 0;
   if (config_get_hex(conf, "video_message_color", &msg_color))
   {
      g_settings.video.msg_color_r = ((msg_color >> 16) & 0xff) / 255.0f;
      g_settings.video.msg_color_g = ((msg_color >>  8) & 0xff) / 255.0f;
      g_settings.video.msg_color_b = ((msg_color >>  0) & 0xff) / 255.0f;
   }

   CONFIG_GET_BOOL(video.post_filter_record, "video_post_filter_record");
   CONFIG_GET_BOOL(video.gpu_record, "video_gpu_record");
   CONFIG_GET_BOOL(video.gpu_screenshot, "video_gpu_screenshot");

#ifdef HAVE_DYLIB
   CONFIG_GET_PATH(video.filter_path, "video_filter");
#endif

   CONFIG_GET_PATH(video.shader_dir, "video_shader_dir");
   if (!strcmp(g_settings.video.shader_dir, "default"))
      *g_settings.video.shader_dir = '\0';

   CONFIG_GET_FLOAT(input.axis_threshold, "input_axis_threshold");
   CONFIG_GET_BOOL(input.netplay_client_swap_input, "netplay_client_swap_input");

   for (unsigned i = 0; i < MAX_PLAYERS; i++)
   {
      char buf[64];
      snprintf(buf, sizeof(buf), "input_player%u_joypad_index", i + 1);
      CONFIG_GET_INT(input.joypad_map[i], buf);

      if (!g_extern.has_set_libretro_device[i])
      {
         snprintf(buf, sizeof(buf), "input_libretro_device_p%u", i + 1);
         CONFIG_GET_INT(input.libretro_device[i], buf);
      }
   }

   // Audio settings.
   CONFIG_GET_BOOL(audio.enable, "audio_enable");
   CONFIG_GET_INT(audio.out_rate, "audio_out_rate");
   CONFIG_GET_INT(audio.block_frames, "audio_block_frames");
   CONFIG_GET_STRING(audio.device, "audio_device");
   CONFIG_GET_INT(audio.latency, "audio_latency");
   CONFIG_GET_BOOL(audio.sync, "audio_sync");
   CONFIG_GET_BOOL(audio.rate_control, "audio_rate_control");
   CONFIG_GET_FLOAT(audio.rate_control_delta, "audio_rate_control_delta");
   CONFIG_GET_FLOAT(audio.volume, "audio_volume");

   CONFIG_GET_STRING(video.driver, "video_driver");
   CONFIG_GET_STRING(video.gl_context, "video_gl_context");
   CONFIG_GET_STRING(audio.driver, "audio_driver");
   CONFIG_GET_PATH(audio.dsp_plugin, "audio_dsp_plugin");
   CONFIG_GET_STRING(input.driver, "input_driver");
   CONFIG_GET_STRING(input.joypad_driver, "input_joypad_driver");

   if (!*g_settings.libretro)
      CONFIG_GET_PATH(libretro, "libretro_path");

   CONFIG_GET_PATH(core_options_path, "core_options_path");
   CONFIG_GET_PATH(screenshot_directory, "screenshot_directory");
   if (*g_settings.screenshot_directory)
   {
      if (!strcmp(g_settings.screenshot_directory, "default"))
         *g_settings.screenshot_directory = '\0';
      else if (!path_is_directory(g_settings.screenshot_directory))
      {
         RARCH_WARN("screenshot_directory is not an existing directory, ignoring ...\n");
         *g_settings.screenshot_directory = '\0';
      }
   }

#ifdef HAVE_RGUI
   CONFIG_GET_PATH(rgui_browser_directory, "rgui_browser_directory");
   if (!strcmp(g_settings.rgui_browser_directory, "default"))
      *g_settings.rgui_browser_directory = '\0';
#endif

#ifdef HAVE_OVERLAY
   CONFIG_GET_PATH_EXTERN(overlay_dir, "overlay_directory");
   if (!strcmp(g_extern.overlay_dir, "default"))
      *g_extern.overlay_dir = '\0';

   CONFIG_GET_PATH(input.overlay, "input_overlay");
   CONFIG_GET_FLOAT(input.overlay_opacity, "input_overlay_opacity");
   CONFIG_GET_FLOAT(input.overlay_scale, "input_overlay_scale");
#endif

   CONFIG_GET_BOOL(rewind_enable, "rewind_enable");

   int buffer_size = 0;
   if (config_get_int(conf, "rewind_buffer_size", &buffer_size))
      g_settings.rewind_buffer_size = buffer_size * UINT64_C(1000000);

   CONFIG_GET_INT(rewind_granularity, "rewind_granularity");
   CONFIG_GET_FLOAT(slowmotion_ratio, "slowmotion_ratio");
   if (g_settings.slowmotion_ratio < 1.0f)
      g_settings.slowmotion_ratio = 1.0f;

   CONFIG_GET_FLOAT(fastforward_ratio, "fastforward_ratio");

   CONFIG_GET_BOOL(pause_nonactive, "pause_nonactive");
   CONFIG_GET_INT(autosave_interval, "autosave_interval");

   CONFIG_GET_PATH(cheat_database, "cheat_database_path");
   CONFIG_GET_PATH(cheat_settings_path, "cheat_settings_path");

   CONFIG_GET_BOOL(block_sram_overwrite, "block_sram_overwrite");
   CONFIG_GET_BOOL(savestate_auto_index, "savestate_auto_index");
   CONFIG_GET_BOOL(savestate_auto_save, "savestate_auto_save");
   CONFIG_GET_BOOL(savestate_auto_load, "savestate_auto_load");

   CONFIG_GET_BOOL(network_cmd_enable, "network_cmd_enable");
   CONFIG_GET_INT(network_cmd_port, "network_cmd_port");
   CONFIG_GET_BOOL(stdin_cmd_enable, "stdin_cmd_enable");

   CONFIG_GET_PATH(game_history_path, "game_history_path");
   CONFIG_GET_INT(game_history_size, "game_history_size");

   CONFIG_GET_INT(input.turbo_period, "input_turbo_period");
   CONFIG_GET_INT(input.turbo_duty_cycle, "input_duty_cycle");

   CONFIG_GET_BOOL(input.debug_enable, "input_debug_enable");

   CONFIG_GET_BOOL(input.autodetect_enable, "input_autodetect_enable");
   CONFIG_GET_PATH(input.autoconfig_dir, "joypad_autoconfig_dir");

#ifdef ANDROID
   CONFIG_GET_INT(input.back_behavior, "input_back_behavior");
   CONFIG_GET_INT(input.icade_profile[0], "input_autodetect_icade_profile_pad1");
   CONFIG_GET_INT(input.icade_profile[1], "input_autodetect_icade_profile_pad2");
   CONFIG_GET_INT(input.icade_profile[2], "input_autodetect_icade_profile_pad3");
   CONFIG_GET_INT(input.icade_profile[3], "input_autodetect_icade_profile_pad4");
#endif

#ifdef HAVE_NETPLAY
   CONFIG_GET_BOOL_EXTERN(netplay_enable, "netplay_enable");
   CONFIG_GET_BOOL_EXTERN(netplay_is_client, "isclient");
   CONFIG_GET_INT_EXTERN(netplay_port, "port");
   CONFIG_GET_STRING_EXTERN(netplay_server, "host");
   CONFIG_GET_STRING_EXTERN(netplay_nick, "nickname");
   CONFIG_GET_INT_EXTERN(netplay_sync_frames, "frame");
   CONFIG_GET_BOOL_EXTERN(netplay_is_spectate, "is_spectate");
   
#endif
   CONFIG_GET_BOOL_EXTERN(config_save_on_exit, "config_save_on_exit");

   if (!g_extern.has_set_save_path && config_get_path(conf, "savefile_directory", tmp_str, sizeof(tmp_str)))
   {
      if (!strcmp(tmp_str, "default"))
         *g_extern.savefile_dir = '\0';
      else if (path_is_directory(tmp_str))
      {
         strlcpy(g_extern.savefile_dir, tmp_str, sizeof(g_extern.savefile_dir));
         strlcpy(g_extern.savefile_name_srm, tmp_str, sizeof(g_extern.savefile_name_srm));
         fill_pathname_dir(g_extern.savefile_name_srm, g_extern.basename, ".srm", sizeof(g_extern.savefile_name_srm));
      }
      else
         RARCH_WARN("savefile_directory is not a directory, ignoring ...\n");
   }

   if (!g_extern.has_set_state_path && config_get_path(conf, "savestate_directory", tmp_str, sizeof(tmp_str)))
   {
      if (!strcmp(tmp_str, "default"))
         *g_extern.savestate_dir = '\0';
      else if (path_is_directory(tmp_str))
      {
         strlcpy(g_extern.savestate_dir, tmp_str, sizeof(g_extern.savestate_dir));
         strlcpy(g_extern.savestate_name, tmp_str, sizeof(g_extern.savestate_name));
         fill_pathname_dir(g_extern.savestate_name, g_extern.basename, ".state", sizeof(g_extern.savestate_name));
      }
      else
         RARCH_WARN("savestate_directory is not a directory, ignoring ...\n");
   }

   if (!config_get_path(conf, "system_directory", g_settings.system_directory, sizeof(g_settings.system_directory)))
   {
      RARCH_WARN("system_directory is not set in config. Assuming system directory is same folder as game: \"%s\".\n",
            g_settings.system_directory);
   }

   if (!strcmp(g_settings.system_directory, "default"))
      *g_settings.system_directory = '\0';

   config_read_keybinds_conf(conf);

   config_file_free(conf);
   return true;
}

static void read_keybinds_keyboard(config_file_t *conf, unsigned player, unsigned index,
      struct retro_keybind *bind)
{
   if (input_config_bind_map[index].valid && input_config_bind_map[index].base)
   {
      const char *prefix = input_config_get_prefix(player, input_config_bind_map[index].meta);
      if (prefix)
         input_config_parse_key(conf, prefix, input_config_bind_map[index].base, bind);
   }
}

static void read_keybinds_button(config_file_t *conf, unsigned player, unsigned index,
      struct retro_keybind *bind)
{
   if (input_config_bind_map[index].valid && input_config_bind_map[index].base)
   {
      const char *prefix = input_config_get_prefix(player, input_config_bind_map[index].meta);
      if (prefix)
         input_config_parse_joy_button(conf, prefix, input_config_bind_map[index].base, bind);
   }
}

static void read_keybinds_axis(config_file_t *conf, unsigned player, unsigned index,
      struct retro_keybind *bind)
{
   if (input_config_bind_map[index].valid && input_config_bind_map[index].base)
   {
      const char *prefix = input_config_get_prefix(player, input_config_bind_map[index].meta);
      if (prefix)
         input_config_parse_joy_axis(conf, prefix, input_config_bind_map[index].base, bind);
   }
}

static void read_keybinds_player(config_file_t *conf, unsigned player)
{
   for (unsigned i = 0; input_config_bind_map[i].valid; i++)
   {
      struct retro_keybind *bind = &g_settings.input.binds[player][i];
      if (!bind->valid)
         continue;

      read_keybinds_keyboard(conf, player, i, bind);
      read_keybinds_button(conf, player, i, bind);
      read_keybinds_axis(conf, player, i, bind);
   }
}

static void config_read_keybinds_conf(config_file_t *conf)
{
   for (unsigned i = 0; i < MAX_PLAYERS; i++)
      read_keybinds_player(conf, i);
}

bool config_read_keybinds(const char *path)
{
   config_file_t *conf = config_file_new(path);
   if (!conf)
      return false;
   config_read_keybinds_conf(conf);
   config_file_free(conf);
   return true;
}

static void save_keybind_key(config_file_t *conf, const char *prefix, const char *base,
      const struct retro_keybind *bind)
{
   char key[64];
   snprintf(key, sizeof(key), "%s_%s", prefix, base);

   char ascii[2] = {0};
   const char *btn = ascii;

   if (bind->key >= RETROK_a && bind->key <= RETROK_z)
      ascii[0] = 'a' + (bind->key - RETROK_a);
   else
   {
      for (unsigned i = 0; input_config_key_map[i].str; i++)
      {
         if (input_config_key_map[i].key == bind->key)
         {
            btn = input_config_key_map[i].str;
            break;
         }
      }
   }

   config_set_string(conf, key, btn);
}

#ifndef RARCH_CONSOLE
static void save_keybind_hat(config_file_t *conf, const char *key, const struct retro_keybind *bind)
{
   unsigned hat = GET_HAT(bind->joykey);
   const char *dir = NULL;

   switch (GET_HAT_DIR(hat))
   {
      case HAT_UP_MASK:
         dir = "up";
         break;

      case HAT_DOWN_MASK:
         dir = "down";
         break;

      case HAT_LEFT_MASK:
         dir = "left";
         break;

      case HAT_RIGHT_MASK:
         dir = "right";
         break;

      default:
         rarch_assert(0);
   }

   char config[16];
   snprintf(config, sizeof(config), "h%u%s", hat, dir);
   config_set_string(conf, key, config);
}
#endif

static void save_keybind_joykey(config_file_t *conf, const char *prefix, const char *base,
      const struct retro_keybind *bind)
{
   char key[64];
   snprintf(key, sizeof(key), "%s_%s_btn", prefix, base);

   if (bind->joykey == NO_BTN)
      config_set_string(conf, key, "nul");
#ifndef RARCH_CONSOLE // Consoles don't understand hats.
   else if (GET_HAT_DIR(bind->joykey))
      save_keybind_hat(conf, key, bind);
#endif
   else
      config_set_uint64(conf, key, bind->joykey);
}

static void save_keybind_axis(config_file_t *conf, const char *prefix, const char *base,
      const struct retro_keybind *bind)
{
   char key[64];
   snprintf(key, sizeof(key), "%s_%s_axis", prefix, base);

   unsigned axis = 0;
   char dir = '\0';

   if (bind->joyaxis == AXIS_NONE)
      config_set_string(conf, key, "nul");
   else if (AXIS_NEG_GET(bind->joyaxis) != AXIS_DIR_NONE)
   {
      dir = '-';
      axis = AXIS_NEG_GET(bind->joyaxis);
   }
   else if (AXIS_POS_GET(bind->joyaxis) != AXIS_DIR_NONE)
   {
      dir = '+';
      axis = AXIS_POS_GET(bind->joyaxis);
   }

   if (dir)
   {
      char config[16];
      snprintf(config, sizeof(config), "%c%u", dir, axis);
      config_set_string(conf, key, config);
   }
}

static void save_keybind(config_file_t *conf, const char *prefix, const char *base,
      const struct retro_keybind *bind)
{
   if (!bind->valid)
      return;

   save_keybind_key(conf, prefix, base, bind);
   save_keybind_joykey(conf, prefix, base, bind);
   save_keybind_axis(conf, prefix, base, bind);
}

static void save_keybinds_player(config_file_t *conf, unsigned player)
{
   for (unsigned i = 0; input_config_bind_map[i].valid; i++)
   {
      const char *prefix = input_config_get_prefix(player, input_config_bind_map[i].meta);
      if (prefix)
         save_keybind(conf, prefix, input_config_bind_map[i].base, &g_settings.input.binds[player][i]);
   }
}

bool config_save_file(const char *path)
{
   config_file_t *conf = config_file_new(path);
   if (!conf)
      conf = config_file_new(NULL);
   if (!conf)
      return false;

   RARCH_LOG("Saving config at path: \"%s\"\n", path);

   config_set_string(conf, "libretro_path", g_settings.libretro);
   config_set_string(conf, "cheat_database_path", g_settings.cheat_database);
   config_set_bool(conf, "rewind_enable", g_settings.rewind_enable);
   config_set_int(conf, "rewind_granularity", g_settings.rewind_granularity);
   config_set_string(conf, "video_shader", g_settings.video.shader_path);
   config_set_bool(conf, "video_shader_enable", g_settings.video.shader_enable);
   config_set_float(conf, "video_aspect_ratio", g_settings.video.aspect_ratio);
   config_set_float(conf, "video_xscale", g_settings.video.xscale);
   config_set_float(conf, "video_yscale", g_settings.video.yscale);
   config_set_bool(conf, "video_crop_overscan", g_settings.video.crop_overscan);
   config_set_bool(conf, "video_scale_integer", g_settings.video.scale_integer);
   config_set_bool(conf, "video_smooth", g_settings.video.smooth);
   config_set_float(conf, "video_refresh_rate", g_settings.video.refresh_rate);
   config_set_bool(conf, "video_vsync", g_settings.video.vsync);
   config_set_bool(conf, "video_hard_sync", g_settings.video.hard_sync);
   config_set_int(conf, "video_hard_sync_frames", g_settings.video.hard_sync_frames);
   config_set_bool(conf, "video_black_frame_insertion", g_settings.video.black_frame_insertion);
   config_set_int(conf, "video_swap_interval", g_settings.video.swap_interval);
   config_set_bool(conf, "video_gpu_screenshot", g_settings.video.gpu_screenshot);
   config_set_int(conf, "video_rotation", g_settings.video.rotation);
   config_set_string(conf, "screenshot_directory", *g_settings.screenshot_directory ? g_settings.screenshot_directory : "default");
   config_set_int(conf, "aspect_ratio_index", g_settings.video.aspect_ratio_idx);
   config_set_string(conf, "audio_device", g_settings.audio.device);
   config_set_bool(conf, "audio_rate_control", g_settings.audio.rate_control);
   config_set_float(conf, "audio_rate_control_delta", g_settings.audio.rate_control_delta);

   if (*g_settings.system_directory)
      config_set_string(conf, "system_directory", g_settings.system_directory);
   else
      config_set_string(conf, "system_directory", "default");

   if (*g_extern.savefile_dir)
      config_set_string(conf, "savefile_directory", g_extern.savefile_dir);
   else
      config_set_string(conf, "savefile_directory", "default");

   if (*g_extern.savestate_dir)
      config_set_string(conf, "savestate_directory", g_extern.savestate_dir);
   else
      config_set_string(conf, "savestate_directory", "default");

   if (*g_settings.video.shader_dir)
      config_set_string(conf, "video_shader_dir", g_settings.video.shader_dir);
   else
      config_set_string(conf, "video_shader_dir", "default");

#if defined(HAVE_RGUI) || defined(HAVE_RMENU)
   if (*g_settings.rgui_browser_directory)
      config_set_string(conf, "rgui_browser_directory", g_settings.rgui_browser_directory);
   else
      config_set_string(conf, "rgui_browser_directory", "default");
#endif

#ifdef HAVE_OVERLAY
   if (*g_extern.overlay_dir)
      config_set_string(conf, "overlay_directory", g_extern.overlay_dir);
   else
      config_set_string(conf, "overlay_directory", "default");
   
   config_set_string(conf, "input_overlay", g_settings.input.overlay);
   config_set_float(conf, "input_overlay_opacity", g_settings.input.overlay_opacity);
   config_set_float(conf, "input_overlay_scale", g_settings.input.overlay_scale);
#endif

#ifdef ANDROID
   config_set_int(conf, "input_back_behavior", g_settings.input.back_behavior);
   config_set_int(conf, "input_autodetect_icade_profile_pad1", g_settings.input.icade_profile[0]);
   config_set_int(conf, "input_autodetect_icade_profile_pad2", g_settings.input.icade_profile[1]);
   config_set_int(conf, "input_autodetect_icade_profile_pad3", g_settings.input.icade_profile[2]);
   config_set_int(conf, "input_autodetect_icade_profile_pad4", g_settings.input.icade_profile[3]);
#endif

   config_set_bool(conf, "gamma_correction", g_extern.console.screen.gamma_correction);
#ifdef _XBOX1
   config_set_int(conf, "sound_volume_level", g_extern.console.sound.volume_level);
#endif
   if (g_extern.lifecycle_mode_state & (1ULL << MODE_VIDEO_TRIPLE_BUFFERING_ENABLE))
      config_set_bool(conf, "triple_buffering_enable", true);
   else
      config_set_bool(conf, "triple_buffering_enable", false);

   if (g_extern.lifecycle_mode_state & (1ULL << MODE_INFO_DRAW))
      config_set_bool(conf, "info_msg_enable", true);
   else
      config_set_bool(conf, "info_msg_enable", false);

   if (g_extern.lifecycle_mode_state & (1ULL << MODE_VIDEO_SOFT_FILTER_ENABLE))
      config_set_bool(conf, "soft_filter_enable", true);
   else
      config_set_bool(conf, "soft_filter_enable", false);

   if (g_extern.lifecycle_mode_state & (1ULL << MODE_VIDEO_FLICKER_FILTER_ENABLE))
      config_set_bool(conf, "flicker_filter_enable", true);
   else
      config_set_bool(conf, "flicker_filter_enable", false);

   config_set_int(conf, "flicker_filter_index", g_extern.console.screen.flicker_filter_index);
   config_set_int(conf, "soft_filter_index", g_extern.console.screen.soft_filter_index);
   config_set_int(conf, "current_resolution_id", g_extern.console.screen.resolutions.current.id);
   config_set_int(conf, "custom_viewport_width", g_extern.console.screen.viewports.custom_vp.width);
   config_set_int(conf, "custom_viewport_height", g_extern.console.screen.viewports.custom_vp.height);
   config_set_int(conf, "custom_viewport_x", g_extern.console.screen.viewports.custom_vp.x);
   config_set_int(conf, "custom_viewport_y", g_extern.console.screen.viewports.custom_vp.y);
#ifdef HAVE_RMENU
   config_set_string(conf, "menu_texture_path", g_extern.menu_texture_path);
#endif
   config_set_float(conf, "video_font_size", g_settings.video.font_size);

   // g_extern
   config_set_bool(conf, "config_save_on_exit", g_extern.config_save_on_exit);
   config_set_int(conf, "sound_mode", g_extern.console.sound.mode);
   config_set_int(conf, "state_slot", g_extern.state_slot);
   config_set_int(conf, "audio_mute", g_extern.audio_data.mute);

   if (g_extern.lifecycle_mode_state & (1ULL << MODE_AUDIO_CUSTOM_BGM_ENABLE))
      config_set_bool(conf, "custom_bgm_enable", true);
   else
      config_set_bool(conf, "custom_bgm_enable", false);

   for (unsigned i = 0; i < MAX_PLAYERS; i++)
   {
      char cfg[64];
      snprintf(cfg, sizeof(cfg), "input_dpad_emulation_p%u", i + 1);
      config_set_int(conf, cfg, g_settings.input.dpad_emulation[i]);
      snprintf(cfg, sizeof(cfg), "input_device_p%u", i + 1);
      config_set_int(conf, cfg, g_settings.input.device[i]);
      snprintf(cfg, sizeof(cfg), "input_player%u_joypad_index", i + 1);
      config_set_int(conf, cfg, g_settings.input.joypad_map[i]);
      snprintf(cfg, sizeof(cfg), "input_libretro_device_p%u", i + 1);
      config_set_int(conf, cfg, g_settings.input.libretro_device[i]);
   }

   config_file_write(conf, path);  
   config_file_free(conf);

   return true;
}

bool config_save_keybinds(const char *path)
{
   config_file_t *conf = config_file_new(path);
   if (!conf)
      conf = config_file_new(NULL);
   if (!conf)
      return NULL;

   for (unsigned i = 0; i < MAX_PLAYERS; i++)
      save_keybinds_player(conf, i);

   config_file_write(conf, path);
   config_file_free(conf);
   return true;
}

void settings_set(uint64_t settings)
{
#ifdef HAVE_OVERLAY
   if (settings & (1ULL << S_INPUT_OVERLAY_OPACITY_DECREMENT))
   {
      g_settings.input.overlay_opacity -= 0.01f;

      if (g_settings.input.overlay_opacity < 0.0f)
         g_settings.input.overlay_opacity = 0.0f;
   }

   if (settings & (1ULL << S_INPUT_OVERLAY_OPACITY_INCREMENT))
   {
      g_settings.input.overlay_opacity += 0.01f;

      if (g_settings.input.overlay_opacity > 1.0f)
         g_settings.input.overlay_opacity = 1.0f;
   }

   if (settings & (1ULL << S_DEF_INPUT_OVERLAY_OPACITY))
      g_settings.input.overlay_opacity = 1.0f;

   if (settings & (1ULL << S_INPUT_OVERLAY_SCALE_DECREMENT))
   {
      g_settings.input.overlay_scale -= 0.01f;

      if (g_settings.input.overlay_scale < 0.01f) // Avoid potential divide by zero.
         g_settings.input.overlay_scale = 0.01f;
   }

   if (settings & (1ULL << S_INPUT_OVERLAY_SCALE_INCREMENT))
   {
      g_settings.input.overlay_scale += 0.01f;

      if (g_settings.input.overlay_scale > 2.0f)
         g_settings.input.overlay_scale = 2.0f;
   }

   if (settings & (1ULL << S_DEF_INPUT_OVERLAY_SCALE))
      g_settings.input.overlay_opacity = 1.0f;
#endif

   if (settings & (1ULL << S_REWIND_GRANULARITY_INCREMENT))
      g_settings.rewind_granularity++;

   if (settings & (1ULL << S_REWIND_GRANULARITY_DECREMENT))
   {
      if (g_settings.rewind_granularity > 1)
         g_settings.rewind_granularity--;
   }

   if (settings & (1ULL << S_DEF_REWIND_GRANULARITY))
      g_settings.rewind_granularity = 1;

   if (settings & (1ULL << S_VIDEO_VSYNC_TOGGLE))
      g_settings.video.vsync = !g_settings.video.vsync;

   if (settings & (1ULL << S_VIDEO_FULLSCREEN_TOGGLE))
      g_settings.video.fullscreen = !g_settings.video.fullscreen;

   if (settings & (1ULL << S_DEF_VIDEO_VSYNC))
      g_settings.video.vsync = true;

   if (settings & (1ULL << S_ASPECT_RATIO_DECREMENT))
   {
      if (g_settings.video.aspect_ratio_idx > 0)
         g_settings.video.aspect_ratio_idx--;
   }

   if (settings & (1ULL << S_ASPECT_RATIO_INCREMENT))
   {
      if (g_settings.video.aspect_ratio_idx < LAST_ASPECT_RATIO)
         g_settings.video.aspect_ratio_idx++;
   }

   if (settings & (1ULL << S_SCALE_INTEGER_TOGGLE))
      g_settings.video.scale_integer = !g_settings.video.scale_integer;

   if (settings & (1ULL << S_AUDIO_MUTE))
      g_extern.audio_data.mute = !g_extern.audio_data.mute;

   if (settings & (1ULL << S_AUDIO_CONTROL_RATE_DECREMENT))
   {
      if (g_settings.audio.rate_control_delta > 0.0)
         g_settings.audio.rate_control_delta -= 0.001;

      if (g_settings.audio.rate_control_delta < 0.0005)
      {
         g_settings.audio.rate_control = false;
         g_settings.audio.rate_control_delta = 0.0;
      }
      else
         g_settings.audio.rate_control = true;
   }

   if (settings & (1ULL << S_AUDIO_CONTROL_RATE_INCREMENT))
   {
      if (g_settings.audio.rate_control_delta < 0.2)
         g_settings.audio.rate_control_delta += 0.001;
      g_settings.audio.rate_control = true;
   }

   if (settings & (1ULL << S_FRAME_ADVANCE))
   {
      g_extern.lifecycle_state |= (1ULL << RARCH_FRAMEADVANCE);
      g_extern.lifecycle_mode_state |= (1ULL << MODE_GAME);
   }

   if (settings & (1ULL << S_HW_TEXTURE_FILTER))
      g_settings.video.smooth = !g_settings.video.smooth;

   if (settings & (1ULL << S_RESOLUTION_PREVIOUS))
   {
      if (g_extern.console.screen.resolutions.current.idx)
      {
         g_extern.console.screen.resolutions.current.idx--;
         g_extern.console.screen.resolutions.current.id = 
            g_extern.console.screen.resolutions.list[g_extern.console.screen.resolutions.current.idx];
      }
   }

   if (settings & (1ULL << S_RESOLUTION_NEXT))
   {
      if (g_extern.console.screen.resolutions.current.idx + 1 < 
            g_extern.console.screen.resolutions.count)
      {
         g_extern.console.screen.resolutions.current.idx++;
         g_extern.console.screen.resolutions.current.id = 
            g_extern.console.screen.resolutions.list[g_extern.console.screen.resolutions.current.idx];
      }
   }

   if (settings & (1ULL << S_ROTATION_DECREMENT))
   {
      if (g_settings.video.rotation > 0)
         g_settings.video.rotation--;
   }

   if (settings & (1ULL << S_ROTATION_INCREMENT))
   {
      if (g_settings.video.rotation < LAST_ORIENTATION)
         g_settings.video.rotation++;
   }

   if (settings & (1ULL << S_REWIND))
      g_settings.rewind_enable = !g_settings.rewind_enable;

   if (settings & (1ULL << S_SAVESTATE_DECREMENT))
   {
      if (g_extern.state_slot != 0)
         g_extern.state_slot--;
   }

   if (settings & (1ULL << S_SAVESTATE_INCREMENT))
      g_extern.state_slot++;

   if (settings & (1ULL << S_TRIPLE_BUFFERING))
   {
      if (g_extern.lifecycle_mode_state & (1ULL << MODE_VIDEO_TRIPLE_BUFFERING_ENABLE))
         g_extern.lifecycle_mode_state &= ~(1ULL << MODE_VIDEO_TRIPLE_BUFFERING_ENABLE);
      else
         g_extern.lifecycle_mode_state |= (1ULL << MODE_VIDEO_TRIPLE_BUFFERING_ENABLE);
   }

   if (settings & (1ULL << S_REFRESH_RATE_DECREMENT))
      g_settings.video.refresh_rate -= 0.01f;

   if (settings & (1ULL << S_REFRESH_RATE_INCREMENT))
      g_settings.video.refresh_rate += 0.01f;

   if (settings & (1ULL << S_INFO_DEBUG_MSG_TOGGLE))
   {
      if (g_extern.lifecycle_mode_state & (1ULL << MODE_FPS_DRAW))
         g_extern.lifecycle_mode_state &= ~(1ULL << MODE_FPS_DRAW);
      else
         g_extern.lifecycle_mode_state |= (1ULL << MODE_FPS_DRAW);
   }

   if (settings & (1ULL << S_INFO_MSG_TOGGLE))
   {
      if (g_extern.lifecycle_mode_state & (1ULL << MODE_INFO_DRAW))
         g_extern.lifecycle_mode_state &= ~(1ULL << MODE_INFO_DRAW);
      else
         g_extern.lifecycle_mode_state |= (1ULL << MODE_INFO_DRAW);
   }

   if (settings & (1ULL << S_DEF_ASPECT_RATIO))
      g_settings.video.aspect_ratio_idx = aspect_ratio_idx;

   if (settings & (1ULL << S_DEF_SCALE_INTEGER))
      g_settings.video.scale_integer = scale_integer;

   if (settings & (1ULL << S_DEF_AUDIO_MUTE))
      g_extern.audio_data.mute = false;

   if (settings & (1ULL << S_DEF_AUDIO_CONTROL_RATE))
   {
      g_settings.audio.rate_control_delta = rate_control_delta;
      g_settings.audio.rate_control = rate_control;
   }

   if (settings & (1ULL << S_DEF_HW_TEXTURE_FILTER))
      g_settings.video.smooth = video_smooth;

   if (settings & (1ULL << S_DEF_ROTATION))
      g_settings.video.rotation = ORIENTATION_NORMAL;

   if (settings & (1ULL << S_DEF_TRIPLE_BUFFERING))
      g_extern.lifecycle_mode_state |= (1ULL << MODE_VIDEO_TRIPLE_BUFFERING_ENABLE);

   if (settings & (1ULL << S_DEF_SAVE_STATE))
      g_extern.state_slot = 0;

   if (settings & (1ULL << S_DEF_REFRESH_RATE))
      g_settings.video.refresh_rate = refresh_rate;

   if (settings & (1ULL << S_DEF_INFO_DEBUG_MSG))
      g_extern.lifecycle_mode_state &= ~(1ULL << MODE_FPS_DRAW);

   if (settings & (1ULL << S_DEF_INFO_MSG))
      g_extern.lifecycle_mode_state |= (1ULL << MODE_INFO_DRAW);
}
