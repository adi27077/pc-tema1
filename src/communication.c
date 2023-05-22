#include "communication.h"
#include "util_comm.h"
#include "gates.c"

#include <stdio.h>


/* Task 1 - The Beginning */

void send_byte_message(void)
{
    /* TODO
     * Send the encoding of the characters: R, I, C, K
     */
    send_squanch(18); //r
    send_squanch(9); //i
    send_squanch(3); //c
    send_squanch(11); //k
}


void recv_byte_message(void)
{
    /* TODO
     * Receive 5 encoded characters, decode them and print
     * them to the standard output (as characters)
     *
     * ATTENTION!: Use fprintf(stdout, ...)
     */
    char c[5];
    for(int i = 0; i < 5; i++)
    {
        c[i] = recv_squanch();
        fprintf(stdout, "%c", c[i] + 64);
    }
}


void comm_byte(void)
{
    /* TODO
     * Receive 10 encoded characters and send each character (the character is
     * still encoded) 2 times
     */
    char c[10];
    for(int i = 0; i < 10; i++)
    {
        c[i] = recv_squanch();
        send_squanch(c[i]);
        send_squanch(c[i]);
    }
}


/* Task 2 - Waiting for the Message */

void send_message(void)
{
    /* TODO
     * Send the message: HELLOTHERE
     * - send the encoded length
     * - send each character encoded
     */
    char lenght = (10 << 2);
    send_squanch(lenght); //lungime codificata
    send_squanch(8); //h
    send_squanch(5); //e
    send_squanch(12); //l
    send_squanch(12); //l
    send_squanch(15); //o
    send_squanch(20); //t
    send_squanch(8); //h
    send_squanch(5); //e
    send_squanch(18); //r
    send_squanch(5); //e
}


void recv_message(void)
{
    /* TODO
     * Receive a message:
     * - the first value is the encoded length
     * - length x encoded characters
     * - print each decoded character
     * 
     * ATTENTION!: Use fprintf(stdout, ...)
     */
    uint8_t lenght = recv_squanch();
    lenght = clear_bit(lenght, 0);
    lenght = clear_bit(lenght, 1);
    lenght = clear_bit(lenght, 6);
    lenght = clear_bit(lenght, 7);
    lenght = (lenght >> 2);
    fprintf(stdout, "%d", lenght);
    char c[256];
    for(int i = 0; i < lenght; i++)
    {
        c[i] = recv_squanch();
        fprintf(stdout, "%c", c[i] + 64);
    }
}


void comm_message(void)
{
    /* TODO
     * Receive a message from Rick and do one of the following depending on the
     * last character from the message:
     * - 'P' - send back PICKLERICK
     * - anything else - send back VINDICATORS
     * You need to send the messages as you did at the previous tasks:
     * - encode the length and send it
     * - encode each character and send them
     */
    uint8_t lenght = recv_squanch();
    lenght = clear_bit(lenght, 0);
    lenght = clear_bit(lenght, 1);
    lenght = clear_bit(lenght, 6);
    lenght = clear_bit(lenght, 7);
    lenght = (lenght >> 2);
    char c[256];
    int i;
    for(i = 0; i < lenght; i++)
        c[i] = recv_squanch();
    if(c[i - 1] + 64 == 'P')
    {
        send_squanch(10 << 2); //lungime codificata
        send_squanch(16); //p
        send_squanch(9); //i
        send_squanch(3); //c
        send_squanch(11); //k
        send_squanch(12); //l
        send_squanch(5); //e
        send_squanch(18); //r
        send_squanch(9); //i
        send_squanch(3); //c
        send_squanch(11); //k
    }
    else
    {
        send_squanch(11 << 2); //lungime codificata
        send_squanch(22); //v
        send_squanch(9); //i
        send_squanch(14); //n
        send_squanch(4); //d
        send_squanch(9); //i
        send_squanch(3); //c
        send_squanch(1); //a
        send_squanch(20); //t
        send_squanch(15); //o
        send_squanch(18); //r
        send_squanch(19); //s
    }
    
}


/* Task 3 - In the Zone */

void send_squanch2(uint8_t c1, uint8_t c2)
{
    /* TODO
     * Steps:
     * - "merge" the character encoded in c1 and the character encoded in c2
     * - use send_squanch to send the newly formed byte
     */
    int i, j = 0;
    uint8_t code = 0;
    for(i = 0; i < 4; i++)
    {
        if(get_bit(c2, i) == 1)
            code = activate_bit(code, j);
        j++;
        if(get_bit(c1, i) == 1)
            code = activate_bit(code, j);
        j++;
    }
    send_squanch(code);
}


uint8_t decode_squanch2(uint8_t c)
{
    /*
     * Decode the given byte:
     * - split the two characters as in the image from ocw.cs.pub.ro
     */
    
    uint8_t res = -1;

    /* TODO */
    res = 0;
    int i, j = 0;
    for(i = 0; i < 8; i+=2)
    {
        if(get_bit(c, i) == 1)
            res = activate_bit(res, j);
        j++;
    }
    for(i = 1; i < 8; i+=2)
    {
        if(get_bit(c, i) == 1)
            res = activate_bit(res, j);
        j++;
    }
    return res;
}
