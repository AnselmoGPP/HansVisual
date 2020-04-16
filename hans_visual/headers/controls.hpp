

class keys_controller           // Singleton class
{
    keys_controller() = default;

    static keys_controller *controls;

public:
    static keys_controller *get_instance();

    void update_key_states();

};
