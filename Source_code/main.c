#include <stdio.h>
#include <stdio_ext.h>
#include <unistd.h>
#include "contact.h"

//main function
int main()
{
    int choice=0, flag=1;
    AddressBook addressBook; // declare user defined structure
    initialize(&addressBook); // function call which Initialize the address book

    //prints project heading
    printf("\n--------------------------------------------------------------------------------\n");
    printf("***************************      ADDRESS BOOK      *****************************");
    printf("\n--------------------------------------------------------------------------------\n");

    //super loop : control stays inside the main menu before ( 7.exit )
    while( flag) 
    {
        choice=0; //reset the choice

        //prints the main menu
        printf("\nADDRESS BOOK MENU\n");
        printf("1. Create contact\n");
        printf("2. Search contact\n");
        printf("3. Edit contact\n");
        printf("4. Delete contact\n");
        printf("5. List all contacts\n");
        printf("6. Save contacts\n");		
        printf("7. Exit\n");
        printf("\nEnter your choice   : ");
        __fpurge(stdin);  //clear the i/p buffer
        scanf("%d", &choice);

        //validates the choice value
        if( choice >= 1 && choice <= 7 )
        {

            //each choice number calls differents functions as the main menu
            switch (choice) 
            {
                case 1:
                    createContact(&addressBook);  //function call to create new contacts
                    break;
                case 2:
                    searchContact(&addressBook);  //function call to search conatcts
                    break;
                case 3:
                    editContact(&addressBook);    //function call to edit contacts
                    break;
                case 4:
                    deleteContact(&addressBook);  //function call to delete contacts
                    break;
                case 5:
                    listContacts(&addressBook);   //function call to print all contacts
                    break;
                case 6:
                    printf("Saving and Exiting...\n");
                    saveContactsToFile(&addressBook);  //function call to save contacts to the file ( eg : .csv, .txt etc )
                    break;
                default:
                    flag = 0;  
                    printf("Exiting...\n");
            }
        }
        //prints error message on the screen for invalid choice number
        else
        {
            printf("\n--------------------------------------------------------------------------------\n");
            printf("***************************       INVALID INPUT     ******************************\n");
            printf("                      ENTERED CHOICE MUST BE IN THE RANGE                         \n");
            printf("                                   { [0-7] }                                        ");
            printf("\n--------------------------------------------------------------------------------\n");
            continue;
        }
    } 

    return 0;
}
