#include "hunt.h"
#include "gates.c"
#include <stdio.h>

/* Task 1 - Axii */

uint16_t find_spell(uint64_t memory)
{
    /*
     * The spell is 16 bits and they are in the memory exactly after 5
     * consecutive bits of 1:
     *
     *            high                                             low
     *                [][][x]...[x][x][x][1][1][1][1][1][]...[][][]
     *                    |----16bits---|
     *                          Spell
     *
     * There is only 1 group of 5 consecutive bits of 1 and the other groups
     * have less than consecutive bits of 1
     * Steps:
     * - find 5 consecutive bits of 1
     * - the NEXT 16 bits == spell
     */

    uint16_t res = -1;

    /* TODO */
    int i, j = 0, bitsgroup = 0;
    for(i = 0; i < 64; i++)
    {
        if(bitsgroup != 5)
        {
            if(get_bit(memory, i) == 1)
                bitsgroup++;
            else
                bitsgroup = 0;
        }
        if(bitsgroup == 5)
        {
            if(get_bit(memory, i + 1) == 1)
                res = activate_bit(res, j);
            else
                res = clear_bit(res, j);
            j++;
        }
        if(j == 16)
            break;
    }
    return res;
}


uint16_t find_key(uint64_t memory)
{
    /*
     * The key is 16 bits and they are in the memory exactly before 3
     * consecutive bits of 1:
     *
     *                high                                           low
     *                     [][][][1][1][1][x][x]...[x][x][]...[][][]
     *                                    |----16bits---|
     *                                           Key
     * Steps:
     * - find 3 consecutive bits of 1
     * - the PREVIOUS 16 bits == key
     */

    uint16_t res = -1;

    /* TODO */
    int i, j = 15, bitsgroup = 0;
    for(i = 63; i >= 0; i--)
    {
        if(bitsgroup != 3)
        {
            if(get_bit(memory, i) == 1)
                bitsgroup++;
            else
                bitsgroup = 0;
        }
        if(bitsgroup == 3)
        {
            if(get_bit(memory, i - 1) == 1)
                res = activate_bit(res, j);
            else
                res = clear_bit(res, j);
            j--;
        }
        if(j < 0)
            break;
    }
    return res;
}


uint16_t decrypt_spell(uint16_t spell, uint16_t key)
{
    /*
     * Find the spell knowing that
     * spell_encrypted = spell_plaintext ^ key
     */

    uint16_t res = -1;

    /* TODO */
    res = (spell ^ key);
    return res;
}


/* Task 2 - Sword Play */

uint32_t choose_sword(uint16_t enemy)
{
    /*
     * Help Geralt construct a sword that will give him an advantage in battle.
     * The steps we will follow:
     * - choose the corect sword
     * - brand the sword with the correct runes such that the equation
     * for that specific enemy type is true
     *
     * How does the sword look (Sword template):
     *  [][][][] [0][0][0]...[0][0][0] [][][][][][][][][][][][][][][][]
     *  -------- --------------------- --------------------------------
     *     Type    Does not matter      The "Value" such that the equation
     *                               from 2 holds depending on the enemy type
     *    4 bits      12 bits                     16 bits
     *
     * 1. Type of the sword:
     * - 1001 - Silver Sword - for monsters
     * - 0110 - Steel Sword - for humans
     *
     *  If the number of active bits from variable "enemy" is even == Monster
     *  else == Human
     *
     * 2.
     * Monster Equation:
     *    Value ^ (Enemy & (1 - Enemy)) = 0
     *
     * Human Equation:
     *    Value + Enemy = 0
     */

    uint32_t res = -1;

    /* TODO */
    res = 0;
    int i, active_bits = 0, value;
    for(i = 0; i < 16; i++)
        if(get_bit(enemy, i) == 1)
            active_bits++;
    if(active_bits % 2 == 0)
    {
        res = activate_bit(res, 31);
        res = clear_bit(res, 30);
        res = clear_bit(res, 29);
        res = activate_bit(res, 28);
        value = (enemy & (1 - enemy));
    }
    else
    {
        res = clear_bit(res, 31);
        res = activate_bit(res, 30);
        res = activate_bit(res, 29);
        res = clear_bit(res, 28);
        value = (0 - enemy);
    }
    for(i = 0; i < 16; i++)
        if(get_bit(value, i) == 1)
            res = activate_bit(res, i);
    return res;
}


/* Task 3 - The Witcher Trials */

uint32_t trial_of_the_grasses(uint16_t cocktail)
{
    /*
     * To become a witcher one must survive a cocktail of different
     * herbs. The body should generate the correct antibodies to
     * neutralize the given cocktail.
     *
     *
     * The antibodies need to respect the following properties:
     *   (antibodies_high & cocktail) ^ (antibodies_low | cocktail) = 0
     *   antibodies_low & antibodies_high = 0
     *   antibodies_low | antibodies_high = cocktail
     *
     * Where:
     *  [][][]...[][][] | [][][]...[][][]
     *  ---------------   ---------------
     *  antibodies_high    antibodies_low
     *      16 bits           16 bits
     *      -------------------------
     *              antibodies
     */

    uint32_t res = -1;

    /* TODO */
    res = 0;
    int anti_high, anti_low, i;
    /*din relatiile din cerinta ajungem la concluzia ca
    antibodies_high este exact cocktail, iar antibodies_low este 0*/
    anti_high = cocktail;
    anti_low = 0;
    for(i = 0; i < 16; i++)
        if(get_bit(anti_low, i) == 1)
            res = activate_bit(res, i);
    for(i = 0; i < 16; i++)
        if(get_bit(anti_high, i) == 1)
            res = activate_bit(res, i + 16);
    return res;
}


uint8_t trial_of_forrest_eyes(uint64_t map)
{
    /*
     * For the next trail, the candidate is tied up and blindfolded.
     * They are also taken to a forest and they should return until
     * the next morning.
     *
     * The candidate knows that there are 4 possibilities for the forest:
     * Brokilon, Hindar, Wolven Glade and Caed Dhu.
     *
     * The candidate also knows those forests by heart, but first you
     * need to identify the forest.
     *
     * The characteristics for 3 of the forests are as follows:
     * - Brokilon - has ONLY groups of 4 trees (or a forest without any tree)
     *   Ex: a group of 4 trees: "...00111100..."
     * - Hindar - has ONLY 2 trees in the MIDDLE
     * - Wolven Glade - FULL of trees
     *
     * The "map" variable encodes the following information:
     * - 1 tree
     * - 0 patch of grass
     *
     * You should put in the "res" variable the index for the identified
     * forrest.
     * - Brokilon - index 0
     * - Hindar - index 1
     * - Wolven Glade - index 2
     * - Caed Dhu - index 3
     */

    uint8_t res = -1;

    /* TODO */
    int i, group = 0;
    /*verificam daca este brokilon*/
    if(map == 0)
        res = 0;
    for(i = 0; i < 64; i++)
    {
        if(get_bit(map, i) == 1)
            group++;
        else
            group = 0;
        if(group == 4)
            res = 0;
        if(group > 4)
        {
            res = -1;
            break;
        }
    }
    /*verificam daca este hindar*/
    uint64_t hindar = 0;
    hindar = activate_bit(hindar, 31);
    hindar = activate_bit(hindar, 32);
    if(map == hindar)
        res = 1;
    /*verificam daca este wolven glade*/
    if(map == UINT64_MAX)
        res = 2;
    /*daca nu e niciuna de mai sus, atunci e caed dhu*/
    if(res != 0 && res != 1 && res != 2)
        res = 3;
    return res;
}


uint8_t trial_of_the_dreams(uint32_t map)
{
    /*
     * For this final trial, the candidates go into a trance and when
     * they woke up they would be a fully fledged witcher.
     *
     * For this task, you need to help the witcher find the number
     * of steps that would take the candidate from the dream realm to
     * the real world.
     *
     * The "map" variable would have only 2 bits of 1:
     * - candidate location
     * - portal location
     *
     * You should put in the "res" variable the distance from the
     * candidate position to the portal.
     *
     *  Ex:
     *  [0][0][0][0][0][0]....[1][0][0][0][0][1][0]....[0][0][0]
     *
     *  res = 5
     */

    uint8_t res = -1;

    /* TODO */
    int i, j, distance = 0;
    for(i = 0; i < 32; i++)
    {
        if(get_bit(map, i) == 1)
        { 
            distance = 1;
            for(j = i + 1; j < 32; j++)
            {
                if(get_bit(map, j) == 0)
                    distance++;
                else
                    break;
            }
        }
        if(distance)
            break;         
    }
    res = distance;
    return res;
}
