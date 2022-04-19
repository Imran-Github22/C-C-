/*
 ============================================================================
 Name        : mortgage_deal.c
 Author      : Imran Farooqui
 Version     : 3.0
 Copyright   : (c) 2021. All rights strictly reserved.
 Description : Recommends the best mortgage deals, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <string.h>

#define SSIZE 20                                /* maximum SIZE of string, including NUL */
#define DB_SIZE 50                              /* maximum number of records */

//create the deal structure.
struct deal{
    char string[SSIZE];
    int duration;
    float iir;
    float rta;
    int ltv;
    int fees;
};

/**
 * The main() programme function.
 * @return returns an integer to mark the status.
 */
int main(void){
    void sort_Deals(struct deal *, int);        //the function to sort the deals structure.
    void print_promptHeader(void);              //the function to print the user prompt.
    int create_view_Deal(struct deal *, struct deal *, struct deal *, int);  //the function to create a view to the database.
    void print_Result(struct deal *, int);      //the function to print the search result.

    FILE *fin;                                  /* define a file pointer */
    struct deal db_Deal[DB_SIZE], *ptr_db_Deal;
    ptr_db_Deal = db_Deal;                      //the pointer is assigned to the database.

    int rec_Counter = 0;                        //The number of deals running counter.
    int total_Rec;                              //The total number of records loaded in the structure.

    fin = fopen("mortgage.txt", "r");           /* open data.txt file for read */
    while (!feof(fin)) {
        //load the data from file to the database structure.
        fscanf(fin, "%s %d %f %f %d %d", (ptr_db_Deal + rec_Counter)->string, &(ptr_db_Deal + rec_Counter)->duration,
               &(ptr_db_Deal + rec_Counter)->iir, &(ptr_db_Deal + rec_Counter)->rta, &(ptr_db_Deal + rec_Counter)->ltv,
               &(ptr_db_Deal + rec_Counter)->fees);

        if (rec_Counter < DB_SIZE-1) {
            rec_Counter++;
        }
        else {
            break;
        }
    }
    fclose(fin);								/* close file properly */

    total_Rec = rec_Counter+1;                  //the total number of records read and loaded in to the database.

    struct deal param_Deal[1], *ptr_Param_Deal; //create a structure for the user input parameters.
    ptr_Param_Deal = param_Deal;

    while (1) {
        char cmd;                               //a char variable to detect 'q' char.
        print_promptHeader();                   //prompt the user input for the search.
        scanf("%d %f %f %d %d", &(ptr_Param_Deal->duration), &(ptr_Param_Deal->iir),
              &(ptr_Param_Deal->rta), &(ptr_Param_Deal->ltv), &(ptr_Param_Deal->fees));
        cmd = getchar();                        //detect if 'q' char was entered,
        if (cmd == 'q') break;                  //terminate the programme if 'q' entered.
        struct deal vw_Deal[total_Rec];
        //create a database view based on user input requirements.
        rec_Counter = create_view_Deal(param_Deal, vw_Deal, db_Deal, total_Rec);
        if (rec_Counter > 1) sort_Deals(vw_Deal, rec_Counter); //sort the vw_Deal;
        print_Result(vw_Deal, rec_Counter);                     //print the search result.
    }

    return 0;                                   //The End.
}

/**
 * create_view_Deal() function,
 * creates a View to the Database
 * based on the customer requirements.
 * @param ptr_vw_Param a pointer to user prompt input structure.
 * @param ptr_cvw_Deal a pointer to the deal View structure.
 * @param ptr_cdb_Deal a pointer to the deals structure / database.
 * @param total_Rec total number of records in the database.
 * @return an integer informing the total number of records in the view.
 */
int create_view_Deal(struct deal *ptr_vw_Param, struct deal *ptr_cvw_Deal, struct deal *ptr_cdb_Deal, int total_Rec) {

        int i = 0;                              //a variable to keep track of the vw_Deal records.
        //the loop to populate the view deal structure.
        for (int rec_Counter = 0; rec_Counter < total_Rec-1; rec_Counter++)
        {
            if (((ptr_cdb_Deal + rec_Counter)->duration <= ptr_vw_Param->duration) &&
                ((ptr_cdb_Deal + rec_Counter)->iir <= ptr_vw_Param->iir) &&
                ((ptr_cdb_Deal + rec_Counter)->rta <= ptr_vw_Param->rta) &&
                ((ptr_cdb_Deal + rec_Counter)->ltv >= ptr_vw_Param->ltv) &&
                ((ptr_cdb_Deal + rec_Counter)->fees <= ptr_vw_Param->fees))
            {
                (ptr_cvw_Deal + i)->duration = (ptr_cdb_Deal + rec_Counter)->duration;
                (ptr_cvw_Deal + i)->fees = (ptr_cdb_Deal + rec_Counter)->fees;
                (ptr_cvw_Deal + i)->rta = (ptr_cdb_Deal + rec_Counter)->rta;
                (ptr_cvw_Deal + i)->iir = (ptr_cdb_Deal + rec_Counter)->iir;
                (ptr_cvw_Deal + i)->ltv = (ptr_cdb_Deal + rec_Counter)->ltv;
                strcpy((ptr_cvw_Deal + i)->string, (ptr_cdb_Deal + rec_Counter)->string);
                i++;
            }
        }
        return i;
}

/**
 *  The sort_Deals() function sorts a deal structure
 *  in increasing Initial Interest Rate (IIR) order and if equal
 *  it then sorts in increasing Product Fees order.
 * @param ptr_sort_Deal the pointer to deal structure.
 * @param totalRecords the total number of records in the structure.
 */
void sort_Deals(struct deal *ptr_sort_Deal, int totalRecords){
    struct deal temp_Deal;                      //a temporary deal record for the swap.

    for (int k = 1; k < totalRecords; k++)
        for (int j = 0; j < totalRecords - k; j++) {
            if ((ptr_sort_Deal+j)->iir > (ptr_sort_Deal + j + 1)->iir) {
                temp_Deal = *(ptr_sort_Deal+j);
                *(ptr_sort_Deal+j) = *(ptr_sort_Deal+j+1);
                *(ptr_sort_Deal+j+1) = temp_Deal;
            }
            else if ((ptr_sort_Deal+j)->iir == (ptr_sort_Deal + j + 1)->iir){  //sort by fees.
                if ((ptr_sort_Deal+j)->fees > (ptr_sort_Deal + j + 1)->fees) {
                    temp_Deal = *(ptr_sort_Deal+j);
                    *(ptr_sort_Deal+j) = *(ptr_sort_Deal+j+1);
                    *(ptr_sort_Deal+j+1) = temp_Deal;
                }
            }
        }
}

/**
 * print_promptHeader() prints the user prompt.
 */
void print_promptHeader(void){
    printf("Minimum duration of deal(year) | Maximum initial interest rate(%c) | Maximum rate thereafter(%c) |"
           " Minimum loan to value(%c) | Maximum product fees(%c)?\n",37,37,37,156);
    fflush(stdout);
}

/**
 * print_Result() function, prints the search results.
 * @param ptr_print_vw_Deal the pointer to the view.
 * @param i the number of records.
 */
void print_Result(struct deal *ptr_print_vw_Deal, int i){
    for (int j = 0; j < i; j++) {
        printf("%-s %d %.2f %.2f %d %d\n", (ptr_print_vw_Deal + j)->string, (ptr_print_vw_Deal + j)->duration,
               (ptr_print_vw_Deal + j)->iir, (ptr_print_vw_Deal + j)->rta,
               (ptr_print_vw_Deal + j)->ltv, (ptr_print_vw_Deal + j)->fees);  /* print values of variables to screen */
        fflush(stdout);
    }
    printf("\n");
    fflush(stdout);
}
