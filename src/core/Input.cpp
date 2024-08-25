#include "Input.h"
#include "Ntolen.h"
static const char *inputModule =
#include "Input.msc"
    ;

using namespace ntolen;

InputModuleProvider InputModuleProvider::provider = InputModuleProvider();
Input::Input(): InputProcessor()
{
    SDL_Log("Event created::");
}
const char** InputModuleProvider::moduleSource() const {
    return &inputModule;
}
Input::~Input()
{
}
void InputModuleProvider::provide(ModuleBuilder &builder)
{

    builder
        .clazz("Input")
        .method(true, "init_()", [](Djuru* djuru) {
            Ntolen* app = Ntolen::instance();
            app->input()->keybordClass = app->runtime()->makeHandle(djuru, 0, "input", "Keybord");
            app->input()->gamepadClass = app->runtime()->makeHandle(djuru, 0, "input", "Gamepad");
            app->input()->mouseClass = app->runtime()->makeHandle(djuru, 0, "input", "Mouse");
            std::cout<<"Init input"<<std::endl;
            MSCSetSlotBool(djuru, 0, true);
        })
        .end()
        
        // .constructor([]() {
            
        // })
        // .method(false,
        //         "processor=(_)",
        //         [](Djuru *djuru)
        //         {
        //             MSCHandle *ev = *(Event **)MSCGetSlotExtern(djuru, 0);
        //             MSCSetSlotBool(djuru, 0, ev->clicked(MSCGetSlotDouble(djuru, 1)));
        //         })
        .end();
}

bool Input::processKeybordEvent(const char* key, bool state) {
    Input* input = Ntolen::instance()->input();
     MVM* vm = Ntolen::runningVm();
     Djuru* djuru = MSCGetCurrentDjuru(vm);
     MSCEnsureSlots(djuru, 3);
     MSCSetSlotHandle(djuru, 0, input->keybordClass);
     MSCSetSlotString(djuru, 1, key);
     MSCSetSlotBool(djuru, 2, state);
     MSCInterpretResult result = MSCCall(djuru,  Ntolen::instance()->runtime()->makeCallHandle("handle_(_,_)"));
     return result == MSCInterpretResult::RESULT_SUCCESS;
}
bool Input::processGamepadEvent(const char* key, bool state) {
    Input* input = Ntolen::instance()->input();
     MVM* vm = Ntolen::runningVm();
     Djuru* djuru = MSCGetCurrentDjuru(vm);
     MSCEnsureSlots(djuru, 3);
     MSCSetSlotHandle(djuru, 0, input->gamepadClass);
     MSCSetSlotString(djuru, 1, key);
     MSCSetSlotBool(djuru, 2, state);
     MSCInterpretResult result = MSCCall(djuru,  Ntolen::instance()->runtime()->makeCallHandle("handle_(_,_)"));
     return result == MSCInterpretResult::RESULT_SUCCESS;
}
bool Input::processMouseEvent(const char* key, bool state) {
    Input* input = Ntolen::instance()->input();
     MVM* vm = Ntolen::runningVm();
     Djuru* djuru = MSCGetCurrentDjuru(vm);
     MSCEnsureSlots(djuru, 3);
     MSCSetSlotHandle(djuru, 0, input->mouseClass);
     MSCSetSlotString(djuru, 1, key);
     MSCSetSlotBool(djuru, 2, state);
     MSCInterpretResult result = MSCCall(djuru,  Ntolen::instance()->runtime()->makeCallHandle("handle_(_,_)"));
     return result == MSCInterpretResult::RESULT_SUCCESS;
}