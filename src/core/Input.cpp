#include "Input.h"
#include "Ntolen.h"
#include "helper/logger.h"

static const char *inputModule =
#include "Input.msc.inc"
    ;

using namespace ntolen;

ModuleProvider* InputModuleProvider::provider = new InputModuleProvider();
Input::Input(): InputProcessor()
{
    Logger::debug("Event created::");
}
const char** InputModuleProvider::moduleSource() const {
    return &inputModule;
}
Input::~Input()
{
}

MSCHandle* Input::keybordClass = nullptr;
MSCHandle* Input::gamepadClass = nullptr;
MSCHandle* Input::mouseClass = nullptr;
MSCHandle* Input::updateFn = nullptr;

void InputModuleProvider::provide(ModuleBuilder &builder)
{

    builder
        .clazz("Input")
        .method(true, "init_()", [](Djuru* djuru) {
            Ntolen* app = Ntolen::instance();
            Input::keybordClass = app->runtime()->makeHandle(djuru, 0, "input", "Keybord");
            Input::gamepadClass = app->runtime()->makeHandle(djuru, 0, "input", "Gamepad");
            Input::mouseClass = app->runtime()->makeHandle(djuru, 0, "input", "Mouse");
            Input::updateFn =  app->runtime()->makeCallHandle("update_(_,_)");
            Logger::debug("Init input %p", Input::updateFn);
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

bool Input::updateInput(InputType type, const char* key, bool state) {
     MVM* vm = Ntolen::runningVm();
     Djuru* djuru = MSCGetCurrentDjuru(vm);
     if(type == gamepad) {
        return false;
     }
     MSCHandle* clazz = (type == keybord) ? Input::keybordClass : Input::mouseClass;
     Logger::debug("clazz:: %p Type:: %d", clazz, type);
     MSCEnsureSlots(djuru, 3);
     MSCSetSlotHandle(djuru, 0, clazz);
     MSCSetSlotString(djuru, 1, key);
     MSCSetSlotBool(djuru, 2, state);
     MSCInterpretResult result = MSCCall(djuru, Input::updateFn);
     return result == MSCInterpretResult::RESULT_SUCCESS;
}
// bool Input::processGamepadEvent(const char* key, bool state) {
//      MVM* vm = Ntolen::runningVm();
//      Djuru* djuru = MSCGetCurrentDjuru(vm);
//      MSCEnsureSlots(djuru, 3);
//      MSCSetSlotHandle(djuru, 0, Input::gamepadClass);
//      MSCSetSlotString(djuru, 1, key);
//      MSCSetSlotBool(djuru, 2, state);
//      MSCInterpretResult result = MSCCall(djuru, Input::updateFn);
//      return result == MSCInterpretResult::RESULT_SUCCESS;
// }
// bool Input::processMouseEvent(const char* key, bool state) {
//      MVM* vm = Ntolen::runningVm();
//      Djuru* djuru = MSCGetCurrentDjuru(vm);
//      MSCEnsureSlots(djuru, 3);
//      MSCSetSlotHandle(djuru, 0, Input::mouseClass);
//      MSCSetSlotString(djuru, 1, key);
//      MSCSetSlotBool(djuru, 2, state);
//      MSCInterpretResult result = MSCCall(djuru, Input::updateFn);
//      return result == MSCInterpretResult::RESULT_SUCCESS;
// }