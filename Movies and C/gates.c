#include "gates.h"

#include <stdio.h>
#include <assert.h>

/* Task 1 - Bit by Bit */

uint8_t get_bit(uint64_t nr, uint8_t i)
{
    assert(i <= 8 * sizeof nr);
    uint64_t mask = ((unsigned long)1 << i);
    uint8_t res = -1;
    
    if ((mask & nr) != 0) res = 1;
    else res = 0;
    return res;
    /* TODO
     *
     * "res" should be 1 if the bit is active, else 0
     */
}


uint64_t flip_bit(uint64_t nr, uint8_t i)
{
    assert(i <= 8 * sizeof nr);
    uint64_t res = -1;
    uint64_t mask = ((unsigned long)1 << i);
    res = nr ^ mask;
    return res;
      /* TODO
     *
     * Return the "nr" with the ith bit flipped
     */
}


uint64_t activate_bit(uint64_t nr, uint8_t i)
{
    assert(i <= 8 * sizeof nr);
    uint64_t res = 0xFF;
    uint64_t mask = ((unsigned long)1 << i);
    res = nr | mask;
    return res;
      /* TODO
     *
     * Return the "nr" with the ith bit "1"
     */

}


uint64_t clear_bit(uint64_t nr, uint8_t i)
{
    assert(i <= 8 * sizeof nr);
    uint64_t res = -1;
    uint64_t mask = ((unsigned long)1 << i);
    res = nr & (~ mask);
    return res;
    /* TODO
     *
     * Return the "nr" with the ith bit "0"
     */
}


/* Task 2 - One Gate to Rule Them All */

uint8_t nand_gate(uint8_t a, uint8_t b)
{
    assert (a == 0 || a == 1);
    assert (b == 0 || b == 1);
    uint8_t res = !(a & b);
    return res;
}


uint8_t and_gate(uint8_t a, uint8_t b)
{
    assert (a == 0 || a == 1);
    assert (b == 0 || b == 1);
    uint8_t res = -1;
    res = nand_gate (nand_gate (a, b), nand_gate (a, b));
    return res;
    /* TODO - Use the nand gate to implement the and gate */
}


uint8_t not_gate(uint8_t a)
{
    assert (a == 0 || a == 1);
    uint8_t res = -1;
    res = nand_gate (a, a);
    return res;
    /* TODO - Use the nand gate to implement the not gate */
}


uint8_t or_gate(uint8_t a, uint8_t b)
{
    assert (a == 0 || a == 1);
    assert (b == 0 || b == 1);
    uint8_t res = -1;
    res = nand_gate (nand_gate (a, a), nand_gate (b, b));
    return res;
    /* TODO - Use the previously defined gates to implement the or gate */
}


uint8_t xor_gate(uint8_t a, uint8_t b)
{
    assert (a == 0 || a == 1);
    assert (b == 0 || b == 1);
    uint8_t res = -1;
    res = nand_gate (nand_gate (a, nand_gate (a, b)), nand_gate (b, nand_gate (a, b)));
    return res;
    /* TODO - Use the previously defined gates to implement the xor gate */
}


/* Task 3 - Just Carry the Bit */

uint8_t full_adder(uint8_t a, uint8_t b, uint8_t c)
{   uint8_t ressum, rescar;
    assert (a == 0 || a == 1);
    assert (b == 0 || b == 1);
    assert (c == 0 || c == 1);
    uint8_t res = -1;
    ressum = xor_gate(xor_gate(a,b), c);
    rescar = or_gate(or_gate(and_gate(a, b), and_gate(a, c)), and_gate(b,c));
    res = ressum + (rescar << 1);
     return res;
    /* TODO - implement the full_adder using the previous gates
     * Since the full_adder needs to provide 2 results, you should
     * encode the sum bit and the carry bit in one byte - you can encode
     * it in whatever way you like
     */
}


uint64_t ripple_carry_adder(uint64_t a, uint64_t b)
{   uint8_t bit1, bit2, sum = 0;
    uint64_t res = 0;
    uint64_t n, c=0;
    for(n = 0; n <= 63; n++)
        {
            bit1 = (a >> n) % 2;
            bit2 = (b >> n) % 2;
            sum = (full_adder(bit1, bit2, c) % 2);
            c = (full_adder(bit1, bit2, c) >> 1) ;
            if(sum == 1) 
            {
            res = res + (sum << n);
            }
        }
    if(c == 1) 
    {
        res = 0;
    }    
    return res;
    /* TODO
     * Use the full_adder to implement the ripple carry adder
     * If there is ANY overflow while adding "a" and "b" then the
     * result should be 0
     */
}
