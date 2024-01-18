#define WLR_USE_UNSTABLE

#include <unistd.h>

#include <any>
#include <hyprland/src/Compositor.hpp>
#include <hyprland/src/Window.hpp>
//#include <hyprland/src/config/ConfigManager.hpp> we may need it in the feature add config for each sound
#include <SDL2/SDL.h>
#include "globals.hpp"
    SDL_AudioSpec wav_spec;
    Uint8 *wav_buffer;
    Uint32 wav_length;
    SDL_AudioDeviceID dev; //opening audio device
    //XDDD ^-^ Halano can't code ffs you should understand this gonna slow hyprland by 200% while open a window
// Do NOT change this function.
APICALL EXPORT std::string PLUGIN_API_VERSION() {
    return HYPRLAND_API_VERSION;
}
void sdlmoth(){
    SDL_Init(SDL_INIT_AUDIO);
    //Halano here i wanted to use this globaly so we don't keep loading it every time idk how could i do that :( 
    SDL_LoadWAV("/home/halano/.config/hypr/sounds/ow.wav", &wav_spec, &wav_buffer, &wav_length);

    dev = SDL_OpenAudioDevice(NULL, 0, &wav_spec, NULL, 0);
    // Play the sound
    SDL_QueueAudio(dev, wav_buffer, wav_length);
    SDL_PauseAudioDevice(dev, 0);   // unpause

    // Wait till there is some audio not yet sent to the soundcard
    while (SDL_GetQueuedAudioSize(dev) > 0) {
        SDL_Delay(0); //10 moth
    }
    // Clean up
    SDL_CloseAudioDevice(dev);
    SDL_FreeWAV(wav_buffer);
}
void onNewWindow(void* self, std::any data) {
    // data is guaranteed
    auto* const PWINDOW = std::any_cast<CWindow*>(data);

    //HyprlandAPI::addWindowDecoration(PHANDLE, PWINDOW, std::make_unique<CBordersPlusPlus>(PWINDOW));
    sdlmoth();
}

APICALL EXPORT PLUGIN_DESCRIPTION_INFO PLUGIN_INIT(HANDLE handle) {
    PHANDLE = handle;

    const std::string HASH = __hyprland_api_get_hash();

    if (HASH != GIT_COMMIT_HASH) {
        HyprlandAPI::addNotification(PHANDLE, "[Hyprbird] Failure in initialization: Version mismatch (headers ver is not equal to running hyprland ver)",
                                     CColor{1.0, 0.2, 0.2, 1.0}, 5000);
        throw std::runtime_error("[bb] Version mismatch");
    }
    HyprlandAPI::registerCallbackDynamic(PHANDLE, "openWindow", [&](void* self, SCallbackInfo& info, std::any data) { onNewWindow(self, data); });
    HyprlandAPI::addNotification(PHANDLE, "[Hyprbird] Initialized successfully!", CColor{0.2, 0.2, 1.0, 1.0}, 2000);

    return {"Hyprbird", "A plugin to play sounds while opening/closing a window.", "Halano/Vaxry", "0.1"};
}

APICALL EXPORT void PLUGIN_EXIT() {
  //moth >> We should free memory on plugin exit still idk how to do it probrly..
      //SDL_CloseAudioDevice(dev);
     // SDL_FreeWAV(wav_buffer);
    ;
}
