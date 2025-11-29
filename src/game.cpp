#include "game.h"

game::game() : Room(new ::Room), Stage(new ::Stage), Hero(new ::Hero){}

void game::start()
{
    delete Room;
    delete Stage;
    delete Hero;

    srand(time(0));
    init_set_of_rooms(norm);
    current_floor = 1;

    Room = new ::Room;
    Stage = new ::Stage;
    Hero = new ::Hero;

    Stage->initStage();
    Room->initRoom(4, a, current_floor);
}

void game::logic()
{
    Stage->check_of_rooms();

    Hero->SpearAttack();
    Hero->Move();
    Room->Moved(Hero->get_x(), Hero->get_y());
    Hero->DamageEnemy(Room->getArr());
    Hero->DamageMe(Room->getArr(), Room->get_num());
    Hero->Move();
    if(Hero->get_HP() < 1) current_floor = -10;
    Room->OpenRoom();
    if(Room->is_open()){
        stagef = Stage->StageMove(Hero->HeroDoor(Stage->get_adj_rooms(), Stage->getItemID(), Stage->getRoom()));
        if(stagef > 0){
            if(stagef == 10){
                current_floor++;
                Stage->initStage();
            }
            if(current_floor < 5){ Room->initRoom(Stage->getRoom(), a, current_floor); }
        }
    }

    if(current_floor < 0 || current_floor >= 5)
    {
        if(current_floor == -10) game_state = state::GAME_OVER;
        else game_state = state::WIN;
    }
}

void game::draw()
{
    putimage(0, 0, bmp_basic[17], TRANSPARENT_PUT);
    Stage->printDoorsAndItems(Room->is_open());
    Hero->DrawStats();
    Hero->DrawHero();
    Hero->DrawItems();
    Room->printEnemys();
    Stage->printMap();
}

void game::init_set_of_rooms(Enemy normal[])
{
    //¬раги 1 этажа
    a.enemies_location[0][0] = normal[ZOMBIE];   a.enemies_location[0][1] = normal[ZOMBIE];   a.enemies_location[0][2] = normal[ZOMBIE];                                               a.enemies_count[0] = 3;
    a.enemies_location[1][0] = normal[ZOMBIE];   a.enemies_location[1][1] = normal[ZOMBIE];                                                                                            a.enemies_count[1] = 2;
    a.enemies_location[2][0] = normal[ZOMBIE];   a.enemies_location[2][1] = normal[KNIGHT];   a.enemies_location[2][2] = normal[ZOMBIE];                                               a.enemies_count[2] = 3;
    a.enemies_location[3][0] = normal[BOSS1];                                                                                                                                          a.enemies_count[3] = 1;
    //¬раги 2 этажа
    a.enemies_location[4][0] = normal[KNIGHT];   a.enemies_location[4][1] = normal[KNIGHT];                                                                                            a.enemies_count[4] = 2;
    a.enemies_location[5][0] = normal[ZOMBIE];   a.enemies_location[5][1] = normal[KNIGHT];   a.enemies_location[5][2] = normal[KNIGHT];                                               a.enemies_count[5] = 3;
    a.enemies_location[6][0] = normal[NINJA];    a.enemies_location[6][1] = normal[KNIGHT];                                                                                            a.enemies_count[6] = 2;
    a.enemies_location[7][0] = normal[BOSS2];                                                                                                                                          a.enemies_count[7] = 1;
    //¬раги 3 этажа
    a.enemies_location[8][0] = normal[ZOMBIE];   a.enemies_location[8][1] = normal[ZOMBIE];   a.enemies_location[8][2] = normal[ZOMBIE];  a.enemies_location[8][3] = normal[ZOMBIE];   a.enemies_count[8] = 4;
    a.enemies_location[9][0] = normal[NINJA];    a.enemies_location[9][1] = normal[NINJA];                                                                                             a.enemies_count[9] = 2;
    a.enemies_location[10][0] = normal[KNIGHT];  a.enemies_location[10][1] = normal[KNIGHT];  a.enemies_location[10][2] = normal[NINJA];                                               a.enemies_count[10] = 3;
    a.enemies_location[11][0] = normal[BOSS3];                                                                                                                                         a.enemies_count[11] = 1;
    //¬раги 4 этажа
    a.enemies_location[12][0] = normal[KNIGHT];  a.enemies_location[12][1] = normal[KNIGHT];  a.enemies_location[12][2] = normal[KNIGHT]; a.enemies_location[12][3] = normal[KNIGHT];  a.enemies_count[12] = 4;
    a.enemies_location[13][0] = normal[KNIGHT];  a.enemies_location[13][1] = normal[NINJA];   a.enemies_location[13][2] = normal[NINJA];                                               a.enemies_count[13] = 3;
    a.enemies_location[14][0] = normal[ZOMBIE];  a.enemies_location[14][1] = normal[KNIGHT];  a.enemies_location[14][2] = normal[NINJA];                                               a.enemies_count[14] = 3;
    a.enemies_location[15][0] = normal[BOSS4];                                                                                                                                         a.enemies_count[15] = 1;

    a.enemies_location[0][0].x = 258;   a.enemies_location[0][2].x = 338;
    a.enemies_location[1][0].x = 570;   a.enemies_location[1][0].y = 0;     a.enemies_location[1][1].x = 57;   a.enemies_location[1][1].y = 340;
    a.enemies_location[2][0].x = 258;   a.enemies_location[2][2].x = 338;

    a.enemies_location[4][0].x = 258;   a.enemies_location[4][1].x = 338;
    a.enemies_location[5][1].x = 570;   a.enemies_location[5][1].y = 0;     a.enemies_location[5][2].x = 57;   a.enemies_location[5][2].y = 340;
    a.enemies_location[6][0].x = 338;

    a.enemies_location[8][0].x = 570;   a.enemies_location[8][0].y = 0;     a.enemies_location[8][1].x = 57;   a.enemies_location[8][1].y = 340;   a.enemies_location[8][2].x = 570;   a.enemies_location[8][2].y = 340;   a.enemies_location[8][3].x = 57;   a.enemies_location[8][3].y = 0;
    a.enemies_location[9][0].x = 258;   a.enemies_location[9][0].y = 205;   a.enemies_location[9][1].x = 338;  a.enemies_location[9][1].y = 205;
    a.enemies_location[10][0].x = 258;  a.enemies_location[10][1].x = 338;

    a.enemies_location[12][0].x = 570;  a.enemies_location[12][0].y = 0;    a.enemies_location[12][1].x = 57;  a.enemies_location[12][1].y = 340;  a.enemies_location[12][2].x = 570;  a.enemies_location[12][2].y = 340;  a.enemies_location[12][3].x = 57;  a.enemies_location[12][3].y = 0;
    a.enemies_location[13][0].x = 258;  a.enemies_location[13][1].x = 338;
    a.enemies_location[14][0].x = 258;  a.enemies_location[14][1].x = 338;
}
