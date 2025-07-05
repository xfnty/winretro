# Components

## UI
- **purpose**: pass input events from the user to the rest of the application and manage render 
  window and its device context.
- **lifetime**: gets initialized at the start and shut down at the end. Exists as long as the app is 
  running.
- **inputs**:
  - *Core status* – for enabling/disabling some menu options.
- **outputs**:
  - *Input events* – exit, load core/game, load/save game state, change render window size.
  - *Device context* – can be used to create OpenGL contexts.
- **actions**:
  - *(TODO)*

## OpenGL Context
- **purpose**: give the core a framebuffer to render to and present this framebuffer to the user.
- **lifetime**: as long as the game is loaded and running, but can be reinitialized multiple times
  with different parameters while the game is loading.
- **inputs**:
  - *Device context* – always the same, used to create OpenGL context.
  - *OpenGL profile* – Libretro allows for Compatibility, Core and ES profiles, but windows supports
    only the first two.
  - *OpenGL version* – OpenGL API version, major and minor.
  - *Maximum framebuffer size* – size of the framebuffer to create. Later the core can render to a 
    smaller portion of it.
  - *Window size* – size of the render window. Used for scaling and letterboxing.
  - *Render size* – portion of the framebuffer the core currently renders to.
- **outputs**:
  - *(None)*
- **actions**:
  - *(TODO)*

## Core
- **purpose**: manage Libretro core, supply it with everything it might need and pass its products 
  to other components.
- **lifetime**: can be reinitialized multiple times while the app is running. 
- **inputs**:
  - *(TODO)*
- **output**:
  - *(TODO)*
- **actions**:
  - *(TODO)*
