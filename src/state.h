#pragma once

typedef enum state_t state_t;
enum state_t {
    STATE_UNINITIALIZED = 0,
    STATE_INITIALIZED,
    STATE_ACTIVE,
};
