#include <stdio.h>
#include <raylib.h>
#include "utils.h"
#include <math.h>
#include "easyTween.h"

#ifndef _DEBUG
    #pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

const char* vsCode = "#version 330\n"
"in vec3 vertexPosition;\n"
"in vec2 vertexTexCoord;\n"
"in vec4 vertexColor;\n"
"out vec3 fragPosition;\n"
"out vec2 fragTexCoord;\n"
"out vec4 fragColor;\n"
"uniform mat4 mvp;\n"
"void main() {\n"
"fragPosition = vertexPosition;\n"
"fragTexCoord = vertexTexCoord;\n"
"fragColor = vertexColor;\n"
"gl_Position = mvp*vec4(vertexPosition, 1.0);\n"
"};\n";

const char* fsCode = "#version 330\n"
"// Input vertex attributes (from vertex shader)\n"
"in vec3 fragPosition;\n"
"in vec2 fragTexCoord;\n"
"in vec4 fragColor;\n"
"// Input uniform values\n"
"uniform sampler2D texture0;\n"
"uniform float pocusSize;\n"
"uniform vec2 mousePos;\n"
"// Output fragment color\n"
"out vec4 finalColor;\n"
"// NOTE: Add here your custom variables\n"
"void main() {\n"
"    // Calculate final fragment color\n"
"float distanceToCenter = distance(fragPosition.xy , mousePos);\n"
"if (distanceToCenter < pocusSize) discard;\n" // Discard the fragment (transparent)
"else  finalColor = fragColor;\n" // Red color
"}\n";

bool exitWindow = false;
bool shouldExit = false;
bool shouldPocus = false;


//TODO :: If we try to launch 2 instance of FocusPocus, the second newly opened one closes, which is cool, but we would like at least to know why it do this
//FIXME:: We cannot close the app when clicking in the X button, or via the console, only CTRL+E works 
//FIXME:: Sometime when pocus is enabled, the screen may become all black ???
//FIXME:: Window do not cover the entire screen, in fact it cover the screen - window title borders, we should fix that.
//TODO :: It could be better to not show the FocusPocus icon in the task bar and have in the System Trail 
//TODO :: Do better shortcut ?? LCTRL x2 is better, currently not possible with RegisterKEy, use KEyBoardProc and SetKeyboardHook ???

int main() {


    SetConfigFlags(FLAG_WINDOW_TRANSPARENT); // Configures window to be transparent
    InitWindow(0, 0, "Focus Pocus");
    
    SetWindowProcHandle(GetWindowHandle());

    RegisterHotKeys(GetWindowHandle());

    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();
    printf("Size: %dx%d\n", screenWidth, screenHeight);
    SetWindowState(FLAG_WINDOW_ALWAYS_RUN | FLAG_WINDOW_TOPMOST | FLAG_WINDOW_MOUSE_PASSTHROUGH | FLAG_WINDOW_RESIZABLE);

    //this flag take into account the title bar height in the window size making our circle follow mouse code not acruate at the y position,
   //the quick fix we found is if we suppose that the title bar height is 20, to offset the mouse y pos by 20, we should find a better solution
    SetWindowState(FLAG_WINDOW_UNDECORATED); // Hide border/titlebar; omit if you want them there.
    //we need FLAG_BORDERLESS_WINDOWED_MODE but window is not transparent with this flag
    //SetWindowState(FLAG_BORDERLESS_WINDOWED_MODE); // Hide border/titlebar; omit if you want them there.

    SetWindowPosition(0, 19);

    AddIconToTaskBar(GetWindowHandle());

    Shader circleShader = LoadShaderFromMemory(vsCode, fsCode);
    int mousePosLoc = GetShaderLocation(circleShader, "mousePos");
    int pocusSizeLoc = GetShaderLocation(circleShader, "pocusSize");

   
    RenderTexture2D target = LoadRenderTexture(screenWidth, screenHeight);
    SetTargetFPS(60);

    const float pocusRadiusTarget = 80.f;
    const float pocusRadiusStart = pocusRadiusTarget * 11.f;
    const float pocusDuration = .5f; //in seconds

    
    SetExitKey(KEY_NULL);
    
    while (!exitWindow) {
        static float counter = 0;
        float pocusRadius = 0.f;
        Color overlayColor = BLACK;
        Color pocusColor = WHITE;
        if (shouldPocus) {
            pocusRadius = easeOutExp(counter, pocusRadiusStart, pocusRadiusTarget, pocusDuration);
            overlayColor.a = (unsigned char)easeOutExp(counter, 0, 245, pocusDuration);
            pocusColor.a = (unsigned char)easeOutExp(counter, 0, 25, pocusDuration);
            counter += GetFrameTime();
            if (counter > pocusDuration) {
                counter = pocusDuration;
            }
        }
        else {
            pocusRadius = easeInExp(counter, pocusRadiusStart, pocusRadiusTarget, pocusDuration);
            overlayColor.a = (unsigned char)easeOutSin(counter, 0, 245, pocusDuration);
            pocusColor.a = (unsigned char)easeOutSin(counter, 0, 25, pocusDuration);
            counter -= GetFrameTime();
            if (counter <= 0.f) {
                counter = 0.f;
                if (shouldExit) {
                    exitWindow = true;
                }

            }
        }
        //printf("Interpol:%f --> %f\n", counter, pocusRadius);

        int mx, my; CaptureMousePosition(&mx, &my);
        Vector2 mousePos = (Vector2){ (float)mx, (float)my };
        Vector2 pocusPos = { .x = mousePos.x, .y = screenHeight - mousePos.y};
        BeginTextureMode(target);
            ClearBackground(BLANK);
            
            SetShaderValue(circleShader, pocusSizeLoc, &pocusRadius, SHADER_UNIFORM_FLOAT);
            SetShaderValue(circleShader, mousePosLoc, &pocusPos, SHADER_ATTRIB_VEC2);

            BeginShaderMode(circleShader);
            //DrawRectangle(pocusPos.x - pocusRadius, pocusPos.y - pocusRadius, pocusRadius * 2.f, pocusRadius * 2.f, RED);


            DrawRectangle(0,0, screenWidth, screenHeight, overlayColor);
            // Activate our default shader for next drawings
            EndShaderMode();
            
        EndTextureMode();

        
        BeginDrawing();
            ClearBackground(BLANK);
            Rectangle source = (Rectangle){ 0.0f, 0.0f, (float)screenWidth, (float)screenHeight };
            Rectangle dest = (Rectangle){ 0.0f, 0, (float)screenWidth, (float)screenHeight };
            Vector2 origin = (Vector2){ 0.f, 0.f };

            DrawTexturePro(target.texture, source, dest, origin, 0.0f, WHITE);
        EndDrawing();

    }

    RemoveIconToTaskBar(GetWindowHandle());
    UnloadShader(circleShader);
    UnloadRenderTexture(target);
    CloseWindow();
    

    return 0;
}

bool* should_pocus() {
    return &shouldPocus;
}

bool* should_exit() {
    return &shouldExit;
}