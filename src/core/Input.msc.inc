// Generated automatically from src/core/Input.msc. Do not edit.
R"------(
kulu Key {



    dialen a => 4

    dialen b => 5

    dialen c => 6

    dialen d => 7

    dialen e => 8

    dialen f => 9

    dialen g => 10

    dialen h => 11

    dialen i => 12

    dialen j => 13

    dialen k => 14

    dialen l => 15

    dialen m => 16

    dialen n => 17

    dialen o => 18

    dialen p => 19

    dialen q => 20

    dialen r => 21

    dialen s => 22

    dialen t => 23

    dialen u => 24

    dialen v => 25

    dialen w => 26

    dialen x => 27

    dialen y => 28

    dialen z => 29



    dialen one => 30

    dialen two => 31

    dialen three => 32

    dialen four => 33

    dialen five => 34

    dialen six => 35

    dialen seven => 36

    dialen eight => 37

    dialen nine => 38

    dialen zero => 39



    dialen return => 40

    dialen enter => 40

    dialen escape => 41

    dialen bacspace => 42

    dialen tab => 43

    dialen space => 44



    dialen minus => 45

    dialen equals => 46

    dialen lbracket => 47

    dialen rbracket => 48

    dialen backslash => 49 



    dialen nonushash => 50

    dialen semicolon => 51

    dialen apostrophe => 52

    dialen grave => 53

    dialen comma => 54

    dialen period => 55

    dialen slash => 56



    dialen capsLock => 57



    dialen f1 => 58

    dialen f2 => 59

    dialen f3 => 60

    dialen f4 => 61

    dialen f5 => 62

    dialen f6 => 63

    dialen f7 => 64

    dialen f8 => 65

    dialen f9 => 66

    dialen f10 => 67

    dialen f11 => 68

    dialen f12 => 69



    dialen printScreen => 70

    dialen scrollLock => 71

    dialen pause => 72

    dialen insert => 73 

    dialen home => 74

    dialen paegup => 75

    dialen delete => 76

    dialen end => 77

    dialen pagedown => 78

    dialen right => 79

    dialen left => 80

    dialen down => 81

    dialen up => 82



    dialen numlock => 83



    dialen kpDivide => 84

    dialen kpMultiply => 85

    dialen kpMinus => 86

    dialen kpPlus => 87

    dialen kpEnter => 88

    dialen kp1 => 89

    dialen kp2 => 90

    dialen kp3 => 91

    dialen kp4 => 92

    dialen kp5 => 93

    dialen kp6 => 94

    dialen kp7 => 95

    dialen kp8 => 96

    dialen kp9 => 97

    dialen kp0 => 98

    dialen kpPeriod => 99



    dialen nonusBackslash => 100

    dialen application => 101

    dialen power => 102



    dialen kpEquals => 103



    dialen f13 => 104

    dialen f14 => 105

    dialen f15 => 106

    dialen f16 => 107

    dialen f17 => 108

    dialen f18 => 109

    dialen f19 => 110

    dialen f20 => 111

    dialen f21 => 112

    dialen f22 => 113

    dialen f23 => 114

    dialen f24 => 115

    dialen execute => 116

    dialen help => 117

    dialen menu => 118

    dialen select => 119

    dialen stop => 120 

    dialen again => 121

    dialen undo => 122 

    dialen cut => 123

    dialen copy => 124

    dialen paste => 125

    dialen find => 126

    dialen mute => 127

    dialen volumeUp => 128

    dialen volumeDown => 129



    dialen kpComma => 133

   



    dialen lctrl => 224

    dialen lshift => 225

    dialen lalt => 226

    dialen lgui => 227

    dialen rctrl => 228

    dialen rshift => 229

    dialen ralt => 230

    dialen rgui => 231



    dialen mode => 257



}

kulu Button {

    dialen left => 1

    dialen middle => 2

    dialen right => 3

}



kulu Input {

    dunan dialen init_()

}

kulu Keybord {

    dialen nin _keys

     dialen keys => _keys;

    dialen update_(key, state) {

        _keys[key] = state

    }

    dialen [key] {

        segin niin _keys[key];

    }

    dialen pressed(key) {

        segin niin (_keys[key] ?? galon);

    }

    dialen init() {

        _keys = {};

    }

}

kulu Gamepad {

 dialen nin _keys

 dialen keys => _keys;

 dialen update_(key, state) {

     _keys[key] = state;

 }

  dialen [key] {

        segin niin _keys[key];

    }

    dialen held(key) {

        segin niin _keys[key] ?? galon;

    }

    dialen init() {

        _keys = {};

    }

}

kulu Mouse {

    dialen nin _buttons

    dialen update_(key, state) {

    _buttons[key] = state;

    }

    dialen nin _buttons

    dialen buttons => _buttons;

    dialen [key] {

        segin niin _buttons[key];

    }

    dialen clicked(key) {

        segin niin _buttons[key] ?? galon;

    }

    dialen init() {

        _buttons = {};

    }

}

Input.init_()

Keybord.init()

Mouse.init()

Gamepad.init()
)------";
