# Lift Off: Beyond Glaxium

## Command line options

  liftoff -m[2|4|8|16]
  
    Enables multisampling at 2, 4, 8 or 16 samples. If no number is given,
    the highest multisampling mode available on your hardware will be used.
    Not available when using low quality mode.
    
  liftoff -l
  
    Low quality mode. Half resolution for background texture, disable
    mip-mapping and multisampling and reduce number of background stars and
    particles to 50%.
      
  liftoff -s
  
    Log engine messages to "stdout.txt" for debugging purposes (Windows only).
    
  liftoff -h
  
    Show command line parameters (Linux console only).
      
## Controls

- JOYSTICKS and GAMEPADS are supported.
- MOUSE works only for menus.
- CURSOR KEYS or W/A/S/D will move your ship.
- CTRL will let your ship fire it's little guns.
- ESCAPE quits the game at any time.
- F12 toggles displaying current frame rate.
- CTRL+C can be used as "boss key".
