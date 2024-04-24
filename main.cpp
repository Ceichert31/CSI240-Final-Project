//
// Created by newus on 4/5/2024.
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

    WormManager wormManager;

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

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);

    // Our state
    bool createWorm = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    ImVec4 wormColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

    // Main loop
    bool done = false;

    // Declare rect of square
    SDL_Rect squareRect;

    // Square dimensions: Half of the min(SCREEN_WIDTH, SCREEN_HEIGHT)
    squareRect.w = std::min(width, height) / 2;
    squareRect.h = std::min(width, height) / 2;

    // Square position: In the middle of the screen
    squareRect.x = width / 2 - squareRect.w / 2;
    squareRect.y = height / 2 - squareRect.h / 2;

    //
    SDL_Rect rect2;

    // Square dimensions: Half of the min(SCREEN_WIDTH, SCREEN_HEIGHT)
    rect2.w = width;
    rect2.h = height;

    // Square position: In the middle of the screen
    rect2.x = width / 2 - rect2.w / 2;
    rect2.y = height / 2 - rect2.h / 2;

    // Event loop
    while (!done) {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
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

        //Worm settings
        static int segmentSlider = 1, wormLength = 1, posX = 1, posY = 1, wormIndex = 0, segmentIndex = 1;
        {

            ImGui::Begin("Worm Test V2"); // Create a window called "Hello, world!" and append into it.

            //Worm Creation menu
            if (ImGui::CollapsingHeader("Create Worm", ImGuiTreeNodeFlags_DefaultOpen)){
                //Worm GUI
                ImGui::SliderInt("Segment Size", &segmentSlider, 1, 100);
                ImGui::SliderInt("X Position", &posX, 1, width);
                ImGui::SliderInt("Y Position", &posY, 1, height);
                ImGui::SliderInt("Worm Length", &wormLength, 1, 100);
                ImGui::ColorEdit3("Worm Color", (float *) &wormColor);


                ImGui::ColorEdit3("Background Color", (float *) &clear_color);// Edit 3 floats representing a color

                //Create worm
                if (ImGui::Button("Create Worm")) {
                    createWorm = true;
                }
            }

            //Worm Split & Delete settings
            if (ImGui::CollapsingHeader("Select Worm")){
                //Worm Split GUI
                ImGui::InputInt("Worm Index", &wormIndex, 0, 100);
                ImGui::InputInt("Segment Index", &segmentIndex, 0, 100);

                //Split Worm
                if (ImGui::Button("Split Worm")) {

                    //Exceptions
//                if (wormIndex > wormManager.size())
//                    throw std::string("Error: Input Worm does not exist");
//
//                if (wormManager[wormIndex]->wormLength < segmentIndex)
//                    throw std::string("Error: Input Worm does not have that many segments");
//
//                Worm* instance = wormManager[wormIndex];
//
//                Worm::WormNode* current = instance->head;
//                Worm::WormNode* previous;
//
//                //Iterate to desired segment
//                for (int i = 0; i < segmentIndex; i++){
//                    previous = current;
//                    Worm::WormNode* next = current->nextNode;
//                    current = next;
//                }
//
//                //Currently WIP
//                //Cleaning up old worm after data has been moved
//                //Resize old worm length
//                //Double check new worm length is the correct length
//
//                Worm::WormNode* splitPoint = current;
//
//                //Create new worm from this segment on
//                wormManager.push_back(instance->SplitAt(splitPoint));
//
//                //Delete data from old Worm
//                while (current != nullptr){
//                    //Store next node
//                    Worm::WormNode* next = current->nextNode;
//
//                    //Assign pointers null
//                    current->nextNode = nullptr;
//                    current = nullptr;
//
//                    //Delete node
//                    delete current;
//
//                    //Go to next node
//                    current = next;
//                }
//                current = instance->head;
//
//                //Remove reference to split point
//                previous = nullptr;
//            }

                    //Split at input point
                    //wormManager.split(wormIndex, segmentIndex);
                }

                ImGui::SameLine();

                //Delete Worm
                if (ImGui::Button("Delete Worm")){

                }
            }


                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate,
                            ImGui::GetIO().Framerate);
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
