#include "communication.h"
#include "util_comm.h"

#include <stdio.h>

/* Task 1 - The Beginning */

void send_byte_message(void)
{   
    send_squanch(18);
    send_squanch(9);
    send_squanch(3);
    send_squanch(11);
    /* TODO
     * Send the encoding of the characters: R, I, C, K
     */
}


void recv_byte_message(void)
{   uint8_t mes1, mes2, mes3, mes4, mes5;
    mes1 = recv_squanch() + 64;
    mes2 = recv_squanch() + 64;
    mes3 = recv_squanch() + 64;
    mes4 = recv_squanch() + 64;
    mes5 = recv_squanch() + 64;
    fprintf(stdout, "%c", mes1);
    fprintf(stdout, "%c", mes2);
    fprintf(stdout, "%c", mes3);
    fprintf(stdout, "%c", mes4);
    fprintf(stdout, "%c", mes5);

    /* TODO
     * Receive 5 encoded characters, decode them and print
     * them to the standard output (as characters)
     *
     * ATTENTION!: Use fprintf(stdout, ...)
     */
}


void comm_byte(void)
{   uint8_t c1, c2, c3, c4, c5, c6, c7, c8, c9, c10;   
    c1 = recv_squanch();
    send_squanch(c1); send_squanch(c1);
    c2 = recv_squanch();
    send_squanch(c2); send_squanch(c2);
    c3 = recv_squanch();
    send_squanch(c3); send_squanch(c3);
    c4 = recv_squanch();
    send_squanch(c4); send_squanch(c4);
    c5 = recv_squanch();
    send_squanch(c5); send_squanch(c5);
    c6 = recv_squanch();
    send_squanch(c6); send_squanch(c6);
    c7 = recv_squanch();
    send_squanch(c7); send_squanch(c7);
    c8 = recv_squanch();
    send_squanch(c8); send_squanch(c8);
    c9 = recv_squanch();
    send_squanch(c9); send_squanch(c9);
    c10 = recv_squanch();
    send_squanch(c10); send_squanch(c10);
    /* TODO
     * Receive 10 encoded characters and send each character (the character is
     * still encoded) 2 times
     */
}


/* Task 2 - Waiting for the Message */

void send_message(void)
{   uint8_t length = (10 << 2);
    send_squanch(length);
    send_squanch(8);
    send_squanch(5);
    send_squanch(12);
    send_squanch(12);
    send_squanch(15);
    send_squanch(20);
    send_squanch(8);
    send_squanch(5);
    send_squanch(18);
    send_squanch(5);
    /* TODO
     * Send the message: HELLOTHERE
     * - send the encoded length
     * - send each character encoded
     */
}


void recv_message(void)
{   uint8_t length, message, mask = 60;
    length = recv_squanch();
    length = (length & mask) >> 2;
    fprintf(stdout, "%d", length / 10);
    fprintf(stdout, "%d", length % 10);
    for(;length > 0 ; length--)
    {
        message = recv_squanch() + 64;
        fprintf(stdout, "%c", message);
    }
    /* TODO
     * Receive a message:
     * - the first value is the encoded length
     * - length x encoded characters
     * - print each decoded character
     * 
     * ATTENTION!: Use fprintf(stdout, ...)
     */
}

void comm_message(void)
{   uint8_t length, mask = 60;
    uint32_t i;
    length = recv_squanch();
    length = (length & mask) >> 2;
    for(i = 1; i < length; i++)
    {
        recv_squanch();
    }
    if (recv_squanch() == 16)
        {  
            send_squanch(40);
            send_squanch(16);
            send_squanch(9);
            send_squanch(3);
            send_squanch(11);
            send_squanch(12);
            send_squanch(5);
            send_squanch(18);
            send_squanch(9);
            send_squanch(3);
            send_squanch(11);
        }
    else 
    {
    
        send_squanch(44);
        send_squanch(22);
        send_squanch(9);
        send_squanch(14);
        send_squanch(4);
        send_squanch(9);
        send_squanch(3);
        send_squanch(1);
        send_squanch(20);
        send_squanch(15);
        send_squanch(18);
        send_squanch(19);
    }
    /* TODO
     * Receive a message from Rick and do one of the following depending on the
     * last character from the message:
     * - 'P' - send back PICKLERICK
     * - anything else - send back VINDICATORS
     * You need to send the messages as you did at the previous tasks:
     * - encode the length and send it
     * - encode each character and send them
     */
}


/* Task 3 - In the Zone */

void send_squanch2(uint8_t c1, uint8_t c2)
{
    uint8_t mesaj, bit0, bit1, bit2, bit3, bit4, bit5, bit6, bit7;
    bit7 = ((c1 >> 3) % 2) << 7;
    bit6 = ((c2 >> 3) % 2) << 6;
    bit5 = ((c1 >> 2) % 2) << 5;
    bit4 = ((c2 >> 2) % 2) << 4;
    bit3 = ((c1 >> 1) % 2) << 3;
    bit2 = ((c2 >> 1) % 2) << 2;
    bit1 = (c1 % 2) << 1;
    bit0 = c2 % 2;
    mesaj = bit0 + bit1 + bit2 + bit3 + bit4 + bit5 + bit6 + bit7;
    return send_squanch(mesaj);

    /* TODO
     * Steps:
     * - "merge" the character encoded in c1 and the character encoded in c2
     * - use send_squanch to send the newly formed byte
     */
}


uint8_t decode_squanch2(uint8_t c)
{   uint8_t mesaj = c, mesaj2 = 0;
    uint8_t bit0, bit1, bit2, bit3, bit4, bit5, bit6, bit7;
    bit0 =  mesaj % 2;
    bit1 =  ((mesaj >> 2) % 2) << 1;
    bit2 =  ((mesaj >> 4) % 2) << 2;
    bit3 =  ((mesaj >> 6) % 2) << 3;
    bit4 =  ((mesaj >> 1) % 2) << 4;
    bit5 =  ((mesaj >> 3) % 2) << 5;
    bit6 =  ((mesaj >> 5) % 2) << 6;
    bit7 =  ((mesaj >> 7) % 2) << 7;
    mesaj2 = bit0 + bit1 + bit2 + bit3 + bit4 + bit5 + bit6 + bit7;
    return mesaj2;
    /*
     * Decode the given byte:
     * - split the two characters as in the image from ocw.cs.pub.ro
     */

}
