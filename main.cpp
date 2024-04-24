//
// Created by Christopher Eichert on 4/5/2024.
//
#define SDL_MAIN_HANDLED true
#include "worm.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#ifdef __EMSCRIPTEN__
EM_JS(int, canvas_get_width, (), {
return canvas.width;
});

EM_JS(int, canvas_get_height, (), {
return canvas.height;
});
#endif

#include "worm.h"

int main(int argc, char* argv[]) {
    // Unused argc, argv
    (void) argc;
    (void) argv;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0) {
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }

    auto width = 1280;
    auto height = 720;

#ifdef __EMSCRIPTEN__
    width = canvas_get_width();
    height = canvas_get_height();
#endif

    // Setup window
    SDL_WindowFlags window_flags = (SDL_WindowFlags) (SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);
    SDL_Window *window = SDL_CreateWindow("Dear ImGui SDL2+SDL_Renderer example", SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED, width, height, window_flags);

    if (!window) {
        std::cout << "Window could not be created!" << std::endl
                  << "SDL_Error: " << SDL_GetError() << std::endl;
        abort();
    }

    // Setup SDL_Renderer instance
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        SDL_Log("Error creating SDL_Renderer!");
        abort();
    }
    //SDL_RendererInfo info;
    //SDL_GetRendererInfo(renderer, &info);
    //SDL_Log("Current SDL_Renderer: %s", info.name);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void) io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsClassic();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer_Init(renderer);

    // Main loop
    bool done = false;

    //Worm Vector
    WormManager wormManager;

    //Worm settings
    static int segmentSlider = 1,
    wormLength = 1,
    posX = 1,
    posY = 1,
    wormIndex = 0,
    segmentIndex = 1;
    bool createWorm = false;

    // Color Settings
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f),
    wormColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f),
    wormSplitColor = ImVec4(0, 0, 0, 0);

    // Event loop
    while (!done) {
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                done = true;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE &&
                event.window.windowID == SDL_GetWindowID(window))
                done = true;
        }

        // Start the Dear ImGui frame
        ImGui_ImplSDLRenderer_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        {
            ImGui::Begin("Worm Builder V3");

            if (ImGui::CollapsingHeader("Create Worm Settings")) {
                //Worm GUI
                ImGui::SliderInt("Segment Size", &segmentSlider, 1, 100);
                ImGui::SliderInt("X Position", &posX, 1, width);
                ImGui::SliderInt("Y Position", &posY, 1, height);
                ImGui::SliderInt("Worm Length", &wormLength, 1, 100);
                ImGui::ColorEdit3("Worm Color", (float *) &wormColor);
                ImGui::ColorEdit3("Background Color", (float *) &clear_color);

                //Create worm
                if (ImGui::Button("Create Worm")) {
                    createWorm = true;
                }
            }

            //Worm Split & Delete settings
            if (ImGui::CollapsingHeader("Edit Worm Settings")){
                //Worm Split GUI
                ImGui::InputInt("Worm Index", &wormIndex, 0, 100);
                ImGui::InputInt("Segment Index", &segmentIndex, 0, 100);
                ImGui::ColorEdit3("Split Worm Color", (float *) &wormSplitColor);

                //Split Worm
                if (ImGui::Button("Split Worm")) {

                    //Exceptions
                    if (wormIndex > wormManager.worms.size()){
                        throw std::invalid_argument("Error: Worm does not exist at index value");
                    }
                    if (wormManager.worms[wormIndex].wormLength < segmentIndex){
                        throw std::invalid_argument("Error: Worm segment does not exist at index value");
                    }

                    //Split at input point
                    wormManager.Split(wormIndex, segmentIndex, wormSplitColor);
                }

                ImGui::SameLine();

                //Re-Color Worm
                if (ImGui::Button("Re-Color Worm")){
                    wormManager.worms[wormIndex].wormColor = wormSplitColor;
                }
            }
            //FPS Debug
                //ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
                ImGui::End();
            }

            // Rendering
            ImGui::Render();

            //Generates background
            SDL_SetRenderDrawColor(renderer, (Uint8) (clear_color.x * 255), (Uint8) (clear_color.y * 255),(Uint8) (clear_color.z * 255), (Uint8) (clear_color.w * 255));
            SDL_RenderClear(renderer);

            // todo: add your game logic here to be drawn before the ui rendering

            //Create new worm
            if (createWorm) {
                wormManager.CreateWorm(segmentSlider, posX, posY, wormLength, wormColor);
                createWorm = false;
            }

            //Render worm
            for (int i = 0; i < wormManager.worms.size(); i++) {
                Worm wormInstance = wormManager.worms[i];

                //Get worm color
                ImVec4 color = wormInstance.wormColor;

                SDL_SetRenderDrawColor(renderer, (Uint8) (color.x * 255), (Uint8) (color.y * 255),(Uint8) (color.z * 255), (Uint8) (color.w * 255));

                for (int c = 0; c < wormInstance.wormLength; c++) {
                    WormNode *nodeInstance = wormInstance.nodes[c];
                    SDL_RenderFillRect(renderer, &nodeInstance->wormBody);
                }
            }

            // present ui on top of your drawings
            ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
            SDL_RenderPresent(renderer);

            SDL_Delay(0);
        }

        // Cleanup
        ImGui_ImplSDLRenderer_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext();

        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();

        return 0;
    }
