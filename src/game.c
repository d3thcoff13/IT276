#include <SDL.h>
#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "simple_logger.h"
#include "simple_json.h"
#include "entity.h"
#include "player.h"
#include "level.h"
#include "game.h"


void Save_Game(){
SJson* file, * array, * data;
file = sj_object_new();
array = sj_array_new();

data = sj_new_int(1);
sj_array_append(array, data);
sj_object_insert(file, "LoadFromFile", data);

data = sj_new_int(currentLevel);
sj_array_append(array, data);
sj_object_insert(file, "currentLevel", data);


sj_save(file, "../../savefiles/gameData.json");
savePlayerData(player_entity);
}

void Load_Game() {
    SJson* value, * file;
    int loadfromfile;
    currentLevel = 4;
    file = sj_load("../../savefiles/gameData.json");
    if (file == NULL) {
        slog("file not found");
        currentLevel = 1;
        return NULL;
    }
    loadfromfile = 0;
    value = sj_object_get_value(file, "LoadFromFile");
    sj_get_integer_value(value, &loadfromfile);

    if (loadfromfile == 1) {
        value = sj_object_get_value(file, "currentLevel");
        sj_get_integer_value(value, &currentLevel);
    }
    else currentLevel = 1;

}

int main(int argc, char * argv[])
{
    /*variable declarations*/
    int done = 0;
    const Uint8 * keys;
    Sprite *sprite;
    
    int mx,my;
    float mf = 0;
    Sprite *mouse;
    Vector4D mouseColor = {255,100,255,200};

    
    
    /*program initializtion*/
    init_logger("gf2d.log");
    slog("---==== BEGIN ====---");
    gf2d_graphics_initialize(
        "gf2d",
        1200,
        720,
        1200,
        720,
        vector4d(0,0,0,255),
        0);
    gf2d_graphics_set_frame_delay(16);
    gf2d_sprite_init(1024);
    SDL_ShowCursor(SDL_DISABLE);
    entity_manager_init(1024);
    
    gf2d_action_list_init(128);

    Load_Game();
    change_level(currentLevel);
    /*demo setup*/
    sprite = gf2d_sprite_load_image("../../images/backgrounds/bg_flat.png");
    mouse = gf2d_sprite_load_all("../../images/pointer.png",32,32,16);
    /*main game loop*/
    while(!done)
    {
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        /*update things here*/
        SDL_GetMouseState(&mx,&my);
        mf+=0.1;
        if (mf >= 16.0)mf = 0;
        
        entity_update_all();
        
        gf2d_graphics_clear_screen();// clears drawing buffers
        // all drawing should happen betweem clear_screen and next_frame
            //backgrounds drawn first
            gf2d_sprite_draw_image(sprite,vector2d(-10,0));
            

            draw_tiles(level);
            entity_draw_all();
            gf2d_sprite_draw(
                mouse,
                vector2d(mx,my),
                NULL,
                NULL,
                NULL,
                NULL,
                &mouseColor,
                (int)mf);
            
            //UI elements last
            
        gf2d_grahics_next_frame();// render current draw frame and skip to the next frame
        
        if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition
        //slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
        

    }
    slog("---==== END ====---");
    return 0;
}

/*eol@eof*/
