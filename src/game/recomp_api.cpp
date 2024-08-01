#include <vector>
#include <map>
#include <string>
#include <cmath>
#include <fstream>

#include "librecomp/recomp.h"
#include "librecomp/overlays.hpp"
#include "zelda_config.h"
#include "recomp_input.h"
#include "recomp_ui.h"
#include "zelda_render.h"
#include "zelda_sound.h"
#include "../../lib/mm-decomp/include/z64item.h"
#include "librecomp/helpers.hpp"
#include "../patches/input.h"
#include "../patches/graphics.h"
#include "../patches/sound.h"
#include "../patches/misc_funcs.h"
#include "ultramodern/ultramodern.hpp"
#include "ultramodern/config.hpp"

#include "Archipelago.h"

std::vector<u32> items;
std::vector<u32> locations;

std::vector<int> item_i_to_player;

extern "C" void recompf(const char* zc_format, ...) {
    va_list va_args;
    va_start(va_args, zc_format);
    
    va_list va_args_copy;
    va_copy(va_args_copy, va_args);
    const int i_len = std::vsnprintf(NULL, 0, zc_format, va_args_copy);
    va_end(va_args_copy);
    
    std::vector<char> zc(i_len + 1);
    std::vsnprintf(zc.data(), zc.size(), zc_format, va_args);
    va_end(va_args);
    std::string str_text(zc.data(), i_len);
    
    OutputDebugStringA(str_text.c_str());
}

extern "C" void apClearItems() {
    
}

extern "C" void apRecvItem(int64_t id, int sending_player_id, bool notify) {
    items.push_back(id & 0xFFFFFF);
    item_i_to_player.push_back(sending_player_id);
}

extern "C" void apCheckLocation(int64_t id) {
    locations.push_back(id & 0xFFFFFF);
}

extern "C" void apGetItemId(uint8_t* rdram, recomp_context* ctx) {
    u32 arg = _arg<0, u32>(rdram, ctx);

    if (arg == 0) {
        _return(ctx, 0);
        return;
    }

    int64_t location = 0x34769420000000 | arg;

    if (getLocationHasLocalItem(location)) {
        int64_t item = getItemAtLocation(location) & 0xFFFFFF;

        if ((item & 0xFF0000) == 0x000000) {
            _return(ctx, (u32) (item & 0xFF));
            return;
        } else {
            switch (item & 0xFF0000) {
                case 0x010000:
                    _return(ctx, (u32) GI_B2);
                    return;
                case 0x020000:
                    _return(ctx, (u32) GI_MAGIC_JAR_SMALL);
                    return;
                case 0x040000:
                    switch (item & 0xFF) {
                        case ITEM_SONG_TIME:
                            _return(ctx, (u32) GI_A6);
                            break;
                        case ITEM_SONG_HEALING:
                            _return(ctx, (u32) GI_AF);
                            break;
                        case ITEM_SONG_EPONA:
                            _return(ctx, (u32) GI_A5);
                            break;
                        case ITEM_SONG_SOARING:
                            _return(ctx, (u32) GI_A3);
                            break;
                        case ITEM_SONG_STORMS:
                            _return(ctx, (u32) GI_A2);
                            break;
                        case ITEM_SONG_SONATA:
                            _return(ctx, (u32) GI_AE);
                            break;
                        case ITEM_SONG_LULLABY:
                            _return(ctx, (u32) GI_AD);
                            break;
                        case ITEM_SONG_NOVA:
                            _return(ctx, (u32) GI_AC);
                            break;
                        case ITEM_SONG_ELEGY:
                            _return(ctx, (u32) GI_A8);
                            break;
                        case ITEM_SONG_OATH:
                            _return(ctx, (u32) GI_A7);
                            break;
                    }
                    return;
                case 0x090000:
                    switch (item & 0xFF) {
                        case ITEM_DUNGEON_MAP:
                            _return(ctx, (u32) GI_MAP);
                            break;
                        case ITEM_COMPASS:
                            _return(ctx, (u32) GI_COMPASS);
                            break;
                        case ITEM_KEY_BOSS:
                            _return(ctx, (u32) GI_KEY_BOSS);
                            break;
                        case ITEM_KEY_SMALL:
                            _return(ctx, (u32) GI_KEY_SMALL);
                            break;
                    }
                    return;
            }
        }
    }

    _return(ctx, 0x0000B3);
}

extern "C" void apSay(uint8_t* rdram, recomp_context* ctx) {
    AP_Say(std::string((char*) ctx->r4));
}

extern "C" void recomp_get_items_size(uint8_t* rdram, recomp_context* ctx) {
    _return(ctx, ((u32) items.size()));
}

extern "C" void recomp_get_item(uint8_t* rdram, recomp_context* ctx) {
    u32 items_i = _arg<0, u32>(rdram, ctx);
    _return(ctx, ((u32) items[items_i]));
}

extern "C" void recomp_get_item_foreign(uint8_t* rdram, recomp_context* ctx) {
    u32 items_i = _arg<0, u32>(rdram, ctx);
    _return(ctx, item_i_to_player[items_i] != AP_GetPlayerID());
}

extern "C" void recomp_get_locations_size(uint8_t* rdram, recomp_context* ctx) {
    _return(ctx, ((u32) locations.size()));
}

extern "C" void recomp_get_location(uint8_t* rdram, recomp_context* ctx) {
    u32 locations_i = _arg<0, u32>(rdram, ctx);
    _return(ctx, ((u32) locations[locations_i]));
}

extern "C" void recomp_has_item(uint8_t* rdram, recomp_context* ctx) {
    u32 id = _arg<0, u32>(rdram, ctx);
    u32 count = 0;
    for (u32 i = 0; i < items.size(); ++i) {
        if (items[i] == id) {
            count += 1;
        }
    }
    _return(ctx, count);
}

extern "C" void recomp_send_location(uint8_t* rdram, recomp_context* ctx) {
    u32 arg = _arg<0, u32>(rdram, ctx);
    if (std::find(locations.begin(), locations.end(), arg) == locations.end()) {
        int64_t id = ((int64_t) (((int64_t) 0x34769420000000) | ((int64_t) arg)));
        AP_SendItem(id);
    }
}

extern "C" void recomp_location_is_checked(uint8_t* rdram, recomp_context* ctx) {
    u32 id = _arg<0, u32>(rdram, ctx);
    _return(ctx, std::find(locations.begin(), locations.end(), id) != locations.end());
}

extern "C" void recomp_get_items_index(uint8_t* rdram, recomp_context* ctx) {
    u8 file_no = _arg<0, u8>(rdram, ctx);
    u32 items_index = _arg<1, u32>(rdram, ctx);

    std::ifstream read_index_file("ap_save_indices.txt");

    if (!read_index_file.good()) {
        _return(ctx, 0);
        return;
    }

    std::string line;

    getline(read_index_file, line);
    if (file_no == 0) {
        _return(ctx, (u32) stol(line));
        return;
    }

    getline(read_index_file, line);
    _return(ctx, (u32) stol(line));
}

extern "C" void recomp_save_items_index(uint8_t* rdram, recomp_context* ctx) {
    u8 file_no = _arg<0, u8>(rdram, ctx);
    u32 items_index = _arg<1, u32>(rdram, ctx);

    std::ifstream read_index_file("ap_save_indices.txt");

    u32 index0 = 0;
    u32 index1 = 0;

    if (read_index_file.good()) {
        std::string line;

        getline(read_index_file, line);
        if (file_no == 1) {
            index0 = stol(line);
        }

        getline(read_index_file, line);
        if (file_no == 0) {
            index1 = stol(line);
        }
    }

    if (file_no == 0) {
        index0 = items_index;
    } else {
        index1 = items_index;
    }

    read_index_file.close();

    std::ofstream write_index_file("ap_save_indices.txt", std::ofstream::trunc);

    write_index_file << index0 << std::endl << index1 << std::endl;
    write_index_file.close();
}

extern "C" void recomp_complete_goal(uint8_t* rdram, recomp_context* ctx) {
    AP_StoryComplete();
}

extern "C" void recomp_update_inputs(uint8_t* rdram, recomp_context* ctx) {
    recomp::poll_inputs();
}

extern "C" void recomp_puts(uint8_t* rdram, recomp_context* ctx) {
    PTR(char) cur_str = _arg<0, PTR(char)>(rdram, ctx);
    u32 length = _arg<1, u32>(rdram, ctx);

    for (u32 i = 0; i < length; i++) {
        fputc(MEM_B(i, (gpr)cur_str), stdout);
    }
}

extern "C" void recomp_exit(uint8_t* rdram, recomp_context* ctx) {
    ultramodern::quit();
}

extern "C" void recomp_get_gyro_deltas(uint8_t* rdram, recomp_context* ctx) {
    float* x_out = _arg<0, float*>(rdram, ctx);
    float* y_out = _arg<1, float*>(rdram, ctx);

    recomp::get_gyro_deltas(x_out, y_out);
}

extern "C" void recomp_get_mouse_deltas(uint8_t* rdram, recomp_context* ctx) {
    float* x_out = _arg<0, float*>(rdram, ctx);
    float* y_out = _arg<1, float*>(rdram, ctx);

    recomp::get_mouse_deltas(x_out, y_out);
}

extern "C" void recomp_powf(uint8_t* rdram, recomp_context* ctx) {
    float a = _arg<0, float>(rdram, ctx);
    float b = ctx->f14.fl; //_arg<1, float>(rdram, ctx);

    _return(ctx, std::pow(a, b));
}

extern "C" void recomp_get_target_framerate(uint8_t* rdram, recomp_context* ctx) {
    int frame_divisor = _arg<0, u32>(rdram, ctx);

    _return(ctx, ultramodern::get_target_framerate(60 / frame_divisor));
}

extern "C" void recomp_get_aspect_ratio(uint8_t* rdram, recomp_context* ctx) {
    ultramodern::renderer::GraphicsConfig graphics_config = ultramodern::renderer::get_graphics_config();
    float original = _arg<0, float>(rdram, ctx);
    int width, height;
    recompui::get_window_size(width, height);

    switch (graphics_config.ar_option) {
        case ultramodern::renderer::AspectRatio::Original:
        default:
            _return(ctx, original);
            return;
        case ultramodern::renderer::AspectRatio::Expand:
            _return(ctx, std::max(static_cast<float>(width) / height, original));
            return;
    }
}

extern "C" void recomp_get_targeting_mode(uint8_t* rdram, recomp_context* ctx) {
    _return(ctx, static_cast<int>(zelda64::get_targeting_mode()));
}

extern "C" void recomp_get_bgm_volume(uint8_t* rdram, recomp_context* ctx) {
    _return(ctx, zelda64::get_bgm_volume() / 100.0f);
}

extern "C" void recomp_get_low_health_beeps_enabled(uint8_t* rdram, recomp_context* ctx) {
    _return(ctx, static_cast<u32>(zelda64::get_low_health_beeps_enabled()));
}

extern "C" void recomp_time_us(uint8_t* rdram, recomp_context* ctx) {
    _return(ctx, static_cast<u32>(std::chrono::duration_cast<std::chrono::microseconds>(ultramodern::time_since_start()).count()));
}

extern "C" void recomp_autosave_enabled(uint8_t* rdram, recomp_context* ctx) {
    _return(ctx, static_cast<s32>(zelda64::get_autosave_mode() == zelda64::AutosaveMode::On));
}

extern "C" void recomp_load_overlays(uint8_t * rdram, recomp_context * ctx) {
    u32 rom = _arg<0, u32>(rdram, ctx);
    PTR(void) ram = _arg<1, PTR(void)>(rdram, ctx);
    u32 size = _arg<2, u32>(rdram, ctx);

    load_overlays(rom, ram, size);
}

extern "C" void recomp_high_precision_fb_enabled(uint8_t * rdram, recomp_context * ctx) {
    _return(ctx, static_cast<s32>(zelda64::renderer::RT64HighPrecisionFBEnabled()));
}

extern "C" void recomp_get_resolution_scale(uint8_t* rdram, recomp_context* ctx) {
    _return(ctx, ultramodern::get_resolution_scale());
}

extern "C" void recomp_get_inverted_axes(uint8_t* rdram, recomp_context* ctx) {
    s32* x_out = _arg<0, s32*>(rdram, ctx);
    s32* y_out = _arg<1, s32*>(rdram, ctx);

    zelda64::CameraInvertMode mode = zelda64::get_camera_invert_mode();

    *x_out = (mode == zelda64::CameraInvertMode::InvertX || mode == zelda64::CameraInvertMode::InvertBoth);
    *y_out = (mode == zelda64::CameraInvertMode::InvertY || mode == zelda64::CameraInvertMode::InvertBoth);
}

extern "C" void recomp_get_analog_inverted_axes(uint8_t* rdram, recomp_context* ctx) {
    s32* x_out = _arg<0, s32*>(rdram, ctx);
    s32* y_out = _arg<1, s32*>(rdram, ctx);

    zelda64::CameraInvertMode mode = zelda64::get_analog_camera_invert_mode();

    *x_out = (mode == zelda64::CameraInvertMode::InvertX || mode == zelda64::CameraInvertMode::InvertBoth);
    *y_out = (mode == zelda64::CameraInvertMode::InvertY || mode == zelda64::CameraInvertMode::InvertBoth);
}

extern "C" void recomp_analog_cam_enabled(uint8_t* rdram, recomp_context* ctx) {
    _return<s32>(ctx, zelda64::get_analog_cam_mode() == zelda64::AnalogCamMode::On);
}

extern "C" void recomp_get_camera_inputs(uint8_t* rdram, recomp_context* ctx) {
    float* x_out = _arg<0, float*>(rdram, ctx);
    float* y_out = _arg<1, float*>(rdram, ctx);

    // TODO expose this in the menu
    constexpr float radial_deadzone = 0.05f;

    float x, y;

    recomp::get_right_analog(&x, &y);

    float magnitude = sqrtf(x * x + y * y);

    if (magnitude < radial_deadzone) {
        *x_out = 0.0f;
        *y_out = 0.0f;
    }
    else {
        float x_normalized = x / magnitude;
        float y_normalized = y / magnitude;

        *x_out = x_normalized * ((magnitude - radial_deadzone) / (1 - radial_deadzone));
        *y_out = y_normalized * ((magnitude - radial_deadzone) / (1 - radial_deadzone));
    }
}

extern "C" void recomp_set_right_analog_suppressed(uint8_t* rdram, recomp_context* ctx) {
    s32 suppressed = _arg<0, s32>(rdram, ctx);

    recomp::set_right_analog_suppressed(suppressed);
}
