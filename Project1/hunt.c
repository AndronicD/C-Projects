#include "hunt.h"
#include <stdio.h>

/* Task 1 - Axii */

uint16_t find_spell(uint64_t memory)
{ uint64_t count = 0, mask, i, j;
  uint16_t spell = 0;
  for(i = 0; i <= 63; i++)
    { 
         mask = ((unsigned long)1 << i);
        if((mask & memory) != 0)
        {
            count++;
            if(count == 5) break;
            
        } else count = 0;
    }
    memory = memory >> (i + 1);
    for(j = 0; j<=15; j++)
    {
        spell = ((memory % 2) << j) + spell;
        memory = memory >>  1;
    }
    return spell;
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

}


uint16_t find_key(uint64_t memory)
{   uint64_t count = 0, mask, i;
    uint16_t key;
    for(i = 0; i <= 63; i++)
    { 
         mask=((unsigned long)1 << i);
        if((mask & memory) != 0)
        {
            count++;
            if(count == 3) break;
            
        }
        else count = 0;
    }
    memory = memory << (66 - i);
    memory = memory >> 48;
    key = memory;
    return key;
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

}


uint16_t decrypt_spell(uint16_t spell, uint16_t key)
{
    /*
     * Find the spell knowing that
     * spell_encrypted = spell_plaintext ^ key
     */
    uint16_t res = -1;
    res = spell ^ key;
    return res;
    
}


/* Task 2 - Sword Play */

uint32_t choose_sword(uint16_t enemy)
{ uint32_t sword = 0, value, i, mask, count = 0, a = 1, b = 1;
  for(i = 0; i <= 31; i++)
  {
      mask = ((unsigned long long)1 << i);
      if((enemy & mask) != 0) count++;
     
  }
   if(count % 2 == 0)
      {
          value = enemy & (1 - enemy);
          sword = (a << 31) + (b << 28) + value;
      }
      else
      {
          value = - enemy;
          sword = 7;
          sword = sword << 28;
          sword = sword + value;
      }    

  return sword;
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
    uint32_t res=-1;
    res = cocktail << 16; 
    return res;
    
}


uint8_t trial_of_forrest_eyes(uint64_t map)
{   uint64_t mask, count = 0, i, c;
    uint8_t res = 3, count2 = 0, contor = 1;
    for(i = 0; i <= 63; i++)
     {
         mask = ((unsigned long long)1 << i); ;
         if ((map & mask) != 0) count++;
         if (count == 64) res = 2;
     }
        mask = ((unsigned long long)3 << 31);
        if(mask == map) res = 1;
    c = map;

    for(i = 0; i <= 63; i++)
    {   
        if(map == 0)
        {
            contor = 1;
            break;
        }
        if(map == UINT64_MAX)
        {
            contor = 0;
            break;
        }
        if((c % 2 == 1) && (count2 < 4)) count2++;
        if((c % 2 == 0) && (count2 > 0)){
            contor = 0;
            break;
        }
        if(count2 == 4) count2 = 0;
        c = c >> 1;
    }
    if (contor == 1) res = 0;
    return res;
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
  
}


uint8_t trial_of_the_dreams(uint32_t map)
{   uint32_t poz1, poz2, i, mask, c=0;
    for(i = 0; i <= 31; i++)
    {
        mask = ((unsigned long)1 << i);
        if((mask & map) != 0) c++;
        if(c == 1)
        {
            poz1 = i;
            break;
        }
    }
    c = 0;
    for(i = 0; i<= 31; i++)
    {
         mask = ((unsigned long)1 << i);
        if((mask & map) != 0) c++;
        if(c == 2)
        {
            poz2 = i;
            break;
        }
        
    }
        
    return poz2 - poz1;

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
    uint8_t res=-1;
    return res;
}
