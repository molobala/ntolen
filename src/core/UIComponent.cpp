#include "UIComponent.h"
#include "Ntolen.h"

using namespace ntolen;

UIComponent::UIComponent() {
    Ntolen::instance()->registerComponent(this);
}
UIComponent::~UIComponent() {
    Ntolen::instance()->unregisterComponent(this);
}