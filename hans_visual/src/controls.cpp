
#include "controls.hpp"

keys_controller * keys_controller::controls = nullptr;

keys_controller * keys_controller::get_instance()
{
    if (!controls)
        controls = new keys_controller();

    return controls;
}
