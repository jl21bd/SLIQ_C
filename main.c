#include "msp.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

void UART0_init(void);

typedef struct entry
{
    int credit_s;
    bool married;
    bool debt;
    bool house;
    bool credit_c;
} entry;

typedef struct totals
{
    double married;
    double debt;
    double house;
    double credit_c;
} totals;

// function to analyze the input data (return type: entry structure)
entry* parse_input(double input[10][5]){
    entry* result = malloc(100);
    int i;
    for (i = 0; i < 10; i++)
    {
        result[i].credit_s = input[i][0];

        if (input[i][1] == 1)
        {
            result[i].married = true;
        }
        else
        {
            result[i].married = false;
        }

        if (input[i][2] == 1)
        {
            result[i].debt = true;
        }
        else
        {
            result[i].debt = false;
        }

        if (input[i][3] == 1)
        {
            result[i].house = true;
        }
        else
        {
            result[i].house = false;
        }

        if (input[i][4] == 1)
        {
            result[i].credit_c = true;
        }
        else
        {
            result[i].credit_c = false;
        }
    }

    return result;
}

// function to count the total number of 1s (yes) for each attribute
// of the data (return type: totals structure)
totals get_totals(entry* entries){
    totals result = {0};
    int i;
    for (i = 0; i < 10; ++i)
    {
        if (entries[i].married == true)
        {
            result.married += 1;
        }
        if (entries[i].debt == true)
        {
            result.debt += 1;
        }
        if (entries[i].house == true)
        {
            result.house += 1;
        }
        if (entries[i].credit_c == true)
        {
            result.credit_c += 1;
        }
    }

    return result;
}


void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;// stop watchdog timer

    double input[10][5] =
    {
     {302,1,1,0,0},
     {353,1,1,0,0},
     {420,0,0,1,1},
     {545,0,1,0,0},
     {610,1,0,0,0},
     {610,0,0,1,1},
     {710,0,0,0,1},
     {720,0,1,1,0},
     {780,1,1,1,1},
     {850,0,0,1,1}
    };

    entry* entries = parse_input(input);

    totals total = get_totals(entries);

    //calculate gini for married
    double prob_married_y = total.married / 10;
    double prob_married_n = 1 - prob_married_y;

    double married_y_credit_c_y = 0;
    double married_n_credit_c_y = 0;

    int i;
    for (i = 0; i < 10; ++i)
    {
        if (entries[i].married == true && entries[i].credit_c == true)
        {
            married_y_credit_c_y += 1;
        }

        if (entries[i].married == false && entries[i].credit_c == true)
        {
            married_n_credit_c_y += 1;
        }
    }

    double prob_married_y_credit_c_y = married_y_credit_c_y / total.married;
    double prob_married_y_credit_c_n = 1 - prob_married_y_credit_c_y;


    double prob_married_n_credit_c_y = married_n_credit_c_y / (10 - total.married);
    double prob_married_n_credit_c_n = 1 - prob_married_n_credit_c_y;

    double gini_married = ((prob_married_y) * (1 - (pow(prob_married_y_credit_c_y, 2) +
                     pow(prob_married_y_credit_c_n, 2)))) + ((prob_married_n) *
             (1 - (pow(prob_married_n_credit_c_y, 2) + pow(prob_married_n_credit_c_n, 2))));


    // calculate gini for debt
    double prob_debt_y = total.debt / 10;
    double prob_debt_n = 1 - prob_debt_y;

    double debt_y_credit_c_y = 0;
    double debt_n_credit_c_y = 0;

    for (i = 0; i < 10; ++i)
    {
        if (entries[i].debt == true && entries[i].credit_c == true)
        {
            debt_y_credit_c_y += 1;
        }

        if (entries[i].debt == false && entries[i].credit_c == true)
        {
            debt_n_credit_c_y += 1;
        }
    }

    double prob_debt_y_credit_c_y = debt_y_credit_c_y / total.married;
    double prob_debt_y_credit_c_n = 1 - prob_debt_y_credit_c_y;

    double prob_debt_n_credit_c_y = debt_n_credit_c_y / (10 - total.debt);
    double prob_debt_n_credit_c_n = 1 - prob_debt_n_credit_c_y;

    double gini_debt= ((prob_debt_y) *
            (1 - (pow(prob_debt_y_credit_c_y, 2)
                  + pow(prob_debt_y_credit_c_n, 2))))
        + ((prob_debt_n) * (1 -
                    (pow(prob_debt_n_credit_c_y, 2) +
                    pow(prob_debt_n_credit_c_n, 2))));

    // calculate gini for house
    double prob_house_y = total.house / 10;
    double prob_house_n = 1 - prob_house_y;

    double house_y_credit_c_y = 0;
    double house_n_credit_c_y = 0;

    for (i = 0; i < 10; i++)
    {
        if (entries[i].house == true && entries[i].credit_c == true)
        {
            house_y_credit_c_y += 1;
        }

        if (entries[i].house == false && entries[i].credit_c == true)
        {
            house_n_credit_c_y += 1;
        }
    }

    double prob_house_y_credit_c_y = house_y_credit_c_y / total.house;
    double prob_house_y_credit_c_n = 1 - prob_house_y_credit_c_y;

    double prob_house_n_credit_c_y = house_n_credit_c_y / (10 - total.house);
    double prob_house_n_credit_c_n = 1 - prob_house_n_credit_c_y;

    double gini_house = ((prob_house_y) * (1 - (pow(prob_house_y_credit_c_y, 2)
            + pow(prob_house_y_credit_c_n, 2))))  + ((prob_house_n) *
                (1 - (pow(prob_house_n_credit_c_y, 2) + pow(prob_house_n_credit_c_n, 2))));

    // integers to represents the decimal points for each gini index
    int a, b, c, d, e, f;
    char A, B, C, D, E, F;

    a = gini_married * 10;
    A = (char)a + '0';
    b = (gini_married * 100) - (a * 10);
    B = (char)b + '0';

    c = gini_debt *10;
    C = (char)c +'0';
    d = (gini_debt * 100) - (c * 10);
    D = (char)d +'0';

    e = gini_house * 10;
    E = (char)e +'0';
    f = (gini_house * 100) - (e * 10);
    F = (char)f +'0';

    int x = 0;

    UART0_init();

        // print out each gini index to the terminal to decide which attribute
        // to be used for splitting the decision tree

        // first attribute
        while(!(EUSCI_A0->IFG & 0x02)) { }  /* wait for transmit buffer empty */
        EUSCI_A0->TXBUF = '1';              /* send a char */
        while(!(EUSCI_A0->IFG & 0x02)) { }
        EUSCI_A0->TXBUF = ':';              /* send a char */
        while(!(EUSCI_A0->IFG & 0x02)) { }
        EUSCI_A0->TXBUF = ' ';              /* send a char */
        while(!(EUSCI_A0->IFG & 0x02)) { }
        EUSCI_A0->TXBUF = '0';              /* send a char */
        while(!(EUSCI_A0->IFG & 0x02)) { }
        EUSCI_A0->TXBUF = '.';              /* send a char */
        while(!(EUSCI_A0->IFG & 0x02)) { }
        EUSCI_A0->TXBUF = A;          /* send a char */
        while(!(EUSCI_A0->IFG & 0x02)) { }
        EUSCI_A0->TXBUF = B;
        while(!(EUSCI_A0->IFG & 0x02)) { }
        EUSCI_A0->TXBUF = '\t';



        // second attribute
        while(!(EUSCI_A0->IFG & 0x02)) { }  /* wait for transmit buffer empty */
        EUSCI_A0->TXBUF = '2';              /* send a char */
        while(!(EUSCI_A0->IFG & 0x02)) { }
        EUSCI_A0->TXBUF = ':';
        while(!(EUSCI_A0->IFG & 0x02)) { }
        EUSCI_A0->TXBUF = ' ';              /* send a char */
        while(!(EUSCI_A0->IFG & 0x02)) { }
        EUSCI_A0->TXBUF = '0';              /* send a char */
        while(!(EUSCI_A0->IFG & 0x02)) { }
        EUSCI_A0->TXBUF = '.';              /* send a char */
        while(!(EUSCI_A0->IFG & 0x02)) { }
        EUSCI_A0->TXBUF = C;          /* send a char */
        while(!(EUSCI_A0->IFG & 0x02)) { }
        EUSCI_A0->TXBUF = D;
        while(!(EUSCI_A0->IFG & 0x02)) { }
        EUSCI_A0->TXBUF = '\t';


        // third attribute
        while(!(EUSCI_A0->IFG & 0x02)) { }  /* wait for transmit buffer empty */
        EUSCI_A0->TXBUF = '3';              /* send a char */
        while(!(EUSCI_A0->IFG & 0x02)) { }
        EUSCI_A0->TXBUF = ':';              /* send a char */
        while(!(EUSCI_A0->IFG & 0x02)) { }
        EUSCI_A0->TXBUF = ' ';              /* send a char */
        while(!(EUSCI_A0->IFG & 0x02)) { }
        EUSCI_A0->TXBUF = '0';              /* send a char */
        while(!(EUSCI_A0->IFG & 0x02)) { }
        EUSCI_A0->TXBUF = '.';              /* send a char */
        while(!(EUSCI_A0->IFG & 0x02)) { }
        EUSCI_A0->TXBUF = E;          /* send a char */
        while(!(EUSCI_A0->IFG & 0x02)) { }
        EUSCI_A0->TXBUF = F;
        while(!(EUSCI_A0->IFG & 0x02)) { }
        EUSCI_A0->TXBUF = '\t';

        while(1){}
}

void UART0_init(void)
{
    EUSCI_A0->CTLW0 |= 1;       /* put in reset mode for config */
    EUSCI_A0->MCTLW = 0;        /* disable oversampling */
    EUSCI_A0->CTLW0 = 0x0081;   /* 1 stop bit, no parity, SMCLK, 8-bit data */
    EUSCI_A0->BRW = 26;         /* 3,000,000 / 115200 = 26 */
    P1->SEL0 |= 0x0C;           /* P1.3, P1.2 for UART */
    P1->SEL1 &= ~0x0C;
    EUSCI_A0->CTLW0 &= ~1;      /* take UART out of reset mode */
}
