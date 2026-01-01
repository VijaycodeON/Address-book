//header file and preprocessor directives
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "contact.h"
#include "file.h"
#include "populate.h"
#include<stdio_ext.h>

//function definition to find dynamic spacing length for the tables printed during the process
int spacing( AddressBook *addressBook )
{
    int len=0;

    //loop finds the length of the largest name in the structure addressbook
    for( int i=0; i<addressBook->contactCount; i++ )
    {
        if( strlen(addressBook->contacts[i].name) > len )
        {
            len = strlen( addressBook->contacts[i].name );  //length is stored in a variable for further use
        }
    }

    //length is incremented to match the word size ( 4 bytes of spacing) 
    while( len%4 !=0 )
    {
        len++; //increment len value
    }

    //return len value whenever function is called
    return len;
}

//function definition to print all the contacts from the structure addressbook 
void listContacts(AddressBook *addressBook) 
{
    int length = spacing( addressBook );   //length value updated to match the spacing while printing all the columns content

    FILE *fptr;
    fptr= fopen("contact.csv", "r+");   //file opened in read plus mode

    //validate file pointers
    if( fptr == NULL )
    {    
        printf("ERROR in file opening\n");
    }    


    //prints the heading of list conatct and all the contact contents
    printf("--------------------------------------------------------------------------------\n");
    printf("                                  LIST CONTACT                                    ");
    printf("\n--------------------------------------------------------------------------------\n");
    printf("SL  %-*s", length, "NAME");   //after name standard spacing is used to have alligned table while printing
    printf("    %-*s", 14, "PHONE NO");
    printf("    %s", "E-MAIL ID");
    printf("\n--------------------------------------------------------------------------------\n");

    //loop prints all the contents ( sl no, name, phone no, email id ) from the structure addressbook
    for( int i=0; i<addressBook->contactCount; i++ )
    {
        if( i<9)
        {
            printf("%d   ",i+1);
        }
        if( i>=9)
        {
            printf("%d  ",i+1);
        }
        printf("%-*s",length, addressBook->contacts[i].name);
        printf("    %-*s  ",12, addressBook->contacts[i].phone);
        printf("    %s\n", addressBook->contacts[i].email);
    }
    
    fclose(fptr); //close the file pointer
    printf("--------------------------------------------------------------------------------\n");
}


//function definition to initialize empty addressbook structure with contents from file or from dummy structure contacts
void initialize(AddressBook *addressBook)
{
    addressBook->contactCount = 0;
    //populateAddressBook(addressBook);    //populates addressbook structure with dummy conatcts structure

    // Load contacts from file during initialization (After files)
    loadContactsFromFile(addressBook);     //populates addressbook structure with contacts from file
}

//function definition to save contacts from addressbook structure to file
void saveAndExit(AddressBook *addressBook) 
{
    //function call
    saveContactsToFile(addressBook); // Save contacts to file
    exit(EXIT_SUCCESS); // Exit the program
}

//function definition to create new contacts into the addressbook structure
void createContact(AddressBook *addressBook)
{
    //prints the heading of create contact
    printf("--------------------------------------------------------------------------------\n");
    printf("                                CREATE CONTACT                                    ");
    printf("\n--------------------------------------------------------------------------------\n");

    //declare variables, arrays and flags 
    int i=0, j=0, k=0, flag=1;    
    char create_name[500];     
    char create_phone[100];
    char create_email[500];

    //super loop 
    while( flag )
    {
        __fpurge(stdin);  //clears i/p buffer
        printf("Enter the name      : ");
        scanf("%[^\n]",create_name);    //stores the name given by the user
        flag =  validatename( addressBook, create_name );  //validates name given ( name can be same or repeated in the file )
    }
    flag=1;

    //super loop
    while( flag )
    {
        __fpurge(stdin);   //clear i/p buffer
        printf("Enter the phone no  : ");
        scanf("%[^\n]",create_phone);  //store phone no given by the user
        flag= validatephone( addressBook, create_phone );  //validate the given phone no 

        //loop to match given phone no to existing phone no in the addressbook structure ( should be unique )
        for( i=0; i<addressBook->contactCount && i<MAX_CONTACTS; i++) 
        {
            //condition to through phone no already existing error
            if( strcmp( addressBook->contacts[i].phone, create_phone ) == 0 )
            {
                printf("\n--------------------------------------------------------------------------------\n");
                printf("                         ******* INVALID INPUT *******\n");
                printf("                        ENTERED PHONE NO ALREADY EXIST!"); 
                printf("\n--------------------------------------------------------------------------------\n");
                flag=1;
                break;
            }
        }
        if( flag )
            continue;   //continue super loop
        else
            break;      //break super loop

    }

    flag=1;
    //super loop
    while( flag )
    {
        __fpurge(stdin);
        printf("Enter the e-mail id : ");
        scanf("%[^\n]",create_email);
        flag = validateemail( addressBook, create_email );  //validate email given by the user

        //loop to find email id already exist in the addressbook structure
        for( i=0; i<addressBook->contactCount && i<MAX_CONTACTS; i++)
        {
            if( strcmp( addressBook->contacts[i].email, create_email ) == 0 )
            {
                printf("\n--------------------------------------------------------------------------------\n");
                printf("                         ******* INVALID INPUT *******\n");
                printf("                        ENTERED E-MAIL ID ALREADY EXIST!"); 
                printf("\n--------------------------------------------------------------------------------\n");
                flag=1;
                break;
            }
        }
        if( flag )
            continue;   //continue super loop
        else
            break;      //break super look

    }
    flag=1;

    //stores all the user given data into the addressbook structure ( new contact created )
    if( flag )
    {
        strcpy( addressBook->contacts[addressBook->contactCount].name, create_name ); 
        strcpy( addressBook->contacts[addressBook->contactCount].phone, create_phone ); 
        strcpy( addressBook->contacts[addressBook->contactCount].email, create_email ); 
        addressBook->contactCount = addressBook->contactCount + 1;   //increment contact count by 1

        //prints the success message in creating a new contact in the addressbook structure
        printf("\n--------------------------------------------------------------------------------\n");
        printf("                  ******* CONTACT CREATED SUCCESSFULLY *******");
        printf("\n--------------------------------------------------------------------------------\n");
    }

}

//function definition to search contact from the addressbook
void searchContact(AddressBook *addressBook) 
{
    int length = spacing( addressBook );
    //prints heading of search contact
    printf("--------------------------------------------------------------------------------\n");
    printf("                                SEARCH CONTACT                                    ");
    printf("\n--------------------------------------------------------------------------------\n");

    //declare variables, arrays and flags
    int i,count=0,set=1, flag=1;
    char search_name[500], search_phone[500], search_email[500], choice;

    //super loop
    while(flag)
    {
        __fpurge(stdin);  //clear i/p buffer

        //prints submenu 
        printf("\nSEARCH CONTACT MENU\n");
        printf("1. Name\n2. Phone no\n3. E-mail id\n4. Exit\n");
        printf("\nEnter your choice : ");
        scanf("%c", &choice);

        //validate choice number
        if( (choice >=49) && (choice <=52) )
        {
            //choice number perform different functionalities
            switch( choice )
            {
                //search based on user given name
                case 49:
                    flag=1;
                    count=0;
                    //super loop
                    while( flag)
                    {
                        __fpurge(stdin);
                        printf("Enter the name to search      : ");
                        scanf("%[^\n]", search_name); 
                        flag =  validatename( addressBook, search_name );  //validate user given name
                    }
                    flag=1;
                    //loop to find the given name againts the names in the addressbook
                    for( i=0; i<addressBook->contactCount; i++ )
                    {
                        //prints contact contents of all the matching names in the addressbook
                        if( strcmp( addressBook->contacts[i].name, search_name ) == 0 )  
                        {
                            flag=0;
                            if( set || (count==0) )
                            {
                                printf("\n--------------------------------------------------------------------------------\n");
                                printf("SL  %-*s", length, "NAME");
                                printf("    %-*s", 14, "PHONE NO");
                                printf("    %s", "E-MAIL ID");
                                printf("\n--------------------------------------------------------------------------------\n");
                                set=0;
                            }
                            count++;
                            if( count<9)
                            {
                                printf("%d   ",count);
                            }
                            if( count>=9)
                            {
                                printf("%d  ",count);
                            }
                            printf("%-*s",length, addressBook->contacts[i].name);
                            printf("    %-*s",14, addressBook->contacts[i].phone);
                            printf("    %s\n", addressBook->contacts[i].email);

                        } 
                    }
                    //prints error message ( search name no found )
                    if( flag )
                    {
                        printf("\n--------------------------------------------------------------------------------\n");
                        printf("                         ******* NO MATCH FOUND *******\n");
                        printf("--------------------------------------------------------------------------------\n");
                        continue;
                    }
                    printf("--------------------------------------------------------------------------------\n");
                    flag=1;
                    break;
               
                //search based on phone number
                case 50:
                    count=0;
                    flag=1;
                    //super loop
                    while( flag )
                    {
                        __fpurge(stdin);   //clear i/p buffer
                        printf("Enter the Phone no to search  : ");
                        scanf("%[^\n]", search_phone);
                        flag =validatephone( addressBook, search_phone );  //validate user given phone number
                    } 
                    flag=1;
                    //loop to find phone number againts the phone numbers in the addressbook structure
                    for( i=0; i<addressBook->contactCount; i++ )
                    {
                        //prints content of contact that matches the user given phone number
                        if( strcmp( addressBook->contacts[i].phone, search_phone ) == 0 )
                        {
                            flag=0;
                            count++;
                            printf("\n--------------------------------------------------------------------------------\n");
                            printf("SL  %-*s", length, "NAME");
                            printf("    %-*s", 14, "PHONE NO");
                            printf("    %s", "E-MAIL ID");
                            printf("\n--------------------------------------------------------------------------------\n");
                            if( count<9)
                            {
                                printf("%d   ",count);
                            }
                            if( count>=9)
                            {
                                printf("%d  ",count);
                            }
                            printf("%-*s",length, addressBook->contacts[i].name);
                            printf("    %-*s",14, addressBook->contacts[i].phone);
                            printf("    %s\n", addressBook->contacts[i].email);

                            break;
                        }
                    }
                    //prints error message ( phone number not found )
                    if( flag )
                    {
                        printf("\n--------------------------------------------------------------------------------\n");
                        printf("                         ******* NO MATCH FOUND *******\n");
                        printf("--------------------------------------------------------------------------------\n");
                        continue;
                    }
                    printf("--------------------------------------------------------------------------------\n");
                    flag=1;
                    break;

                //search based on email id
                case 51:
                    count=0;
                    flag=1;
                    //super loop
                    while(flag)
                    {
                        __fpurge(stdin);  //clear i.p buffer
                        printf("Enter the E-mail id to search : ");
                        scanf("%[^\n]", search_email); 
                        flag = validateemail( addressBook, search_email );  //validates user given email id
                    }
                    flag=1;
                    //loop to find email id against the email id in the addressbook structure
                    for( i=0; i<addressBook->contactCount; i++ )
                    {
                        //prints content of the contact that matches with the user given email id 
                        if( strcmp( addressBook->contacts[i].email, search_email ) == 0 )
                        {
                            flag=0;
                            count++;
                            printf("\n--------------------------------------------------------------------------------\n");
                            printf("SL  %-*s", length, "NAME");
                            printf("    %-*s", 14, "PHONE NO");
                            printf("    %s", "E-MAIL ID");
                            printf("\n--------------------------------------------------------------------------------\n");
                            if( count<9)
                            {
                                printf("%d   ",count);
                            }
                            if( count>=9)
                            {
                                printf("%d  ",count);
                            }
                            printf("%-*s",length, addressBook->contacts[i].name);
                            printf("    %-*s  ",14, addressBook->contacts[i].phone);
                            printf("    %s\n", addressBook->contacts[i].email);

                            break;
                        } 

                    }
                    //prints error message ( email id not found )
                    if( flag )
                    {
                        printf("\n--------------------------------------------------------------------------------\n");
                        printf("                         ******* NO MATCH FOUND *******\n");
                        printf("--------------------------------------------------------------------------------\n");
                        continue;
                    }
                    printf("--------------------------------------------------------------------------------\n");
                    flag=1;
                    break;

                //exit from the submenu
                case 52:
                    choice=1;  //reset value of choice
                    break;
            }
        }
        //prints error message for invalid choice
        else
        {
            printf("\n--------------------------------------------------------------------------------\n");
            printf("                         ******* INVALID INPUT *******\n");
            printf("                     ENTERED CHOICE SHOULD BE IN THE RANGE\n");
            printf("                                   { [0-4] }");
            printf("\n--------------------------------------------------------------------------------\n");
            continue; 
        }

        //exit from search submenu to main menu
        if( choice == 1 )
        {
            break;
        }

    }

}

//function deinition to edit contacts in the addressbook structure
void editContact(AddressBook *addressBook)
{
    int length = spacing( addressBook );

    //prints heading of edit contact
    printf("--------------------------------------------------------------------------------\n");
    printf("                                EDIT CONTACT                                    ");
    printf("\n--------------------------------------------------------------------------------\n");

    //declare variables, arrays and flags
    int i,set=1, flag=1,sl=0,choice=0, num[200], size=0;
    char edit_name[500], edit_phone[500], edit_email[500], replace[500], opt;

    //super loop
    while(1)
    {
        choice=0;
        __fpurge(stdin); //clear i/p buffer

        //prints edit submenu 
        printf("\nEDIT CONTACT MENU\n");
        printf("1. Name\n2. Phone no\n3. E-mail id\n4. Exit\n");
        printf("\nEnter your choice : ");
        scanf("%d", &choice);

        //validate choice
        if( (choice >=1) && (choice <=4) )
        {
            //choice perform different functionalities
            switch( choice )
            {
                //edit contact with name
                case 1:
                    set=1;
                    flag=1;
                    //super loop
                    while( flag)
                    {
                        __fpurge(stdin);  //clear i/p buffer
                        printf("Enter the name to edit           : ");
                        scanf("%[^\n]", edit_name); 
                        flag =  validatename( addressBook, edit_name );  //validate name given by user
                    }
                    flag=1;
                    //loop to match name against the names in the addressbook structure
                    for( i=0; i<addressBook->contactCount; i++ )
                    {
                        //prints options to edit particular contact content
                        if( strcmp( addressBook->contacts[i].name, edit_name ) == 0 )
                        {
                            num[size++] = i;
                            flag=0;
                            if( set || (i==0) )
                            {
                                printf("\n--------------------------------------------------------------------------------\n");
                                printf("SL  %-*s", length, "NAME");
                                printf("    %-*s", 14, "PHONE NO");
                                printf("    %s", "E-MAIL ID");
                                printf("\n--------------------------------------------------------------------------------\n");
                                set=0;
                            }
                            if( i<9)
                            {
                                printf("%d   ",i+1);
                            }
                            if( i>=9)
                            {
                                printf("%d  ",i+1);
                            }
                            printf("%-*s",length, addressBook->contacts[i].name);
                            printf("    %-*s",14, addressBook->contacts[i].phone);
                            printf("    %s\n", addressBook->contacts[i].email);

                        } 
                    }
                    //prints error message ( name not found )
                    if( flag )
                    {
                        printf("\n--------------------------------------------------------------------------------\n");
                        printf("                         ******* NO MATCH FOUND *******\n");
                        printf("--------------------------------------------------------------------------------\n");
                        break;
                    }
                    printf("--------------------------------------------------------------------------------\n");
                    flag=1;
                    //super loop
                    while( flag )
                    {
                        __fpurge(stdin); //clear i/p buffer
                        printf("Enter the SL number to be edited : ");
                        scanf("%d", &sl );  //selects particular contact to edit
                        for( i=0; i<size; i++)
                        {
                            if( (sl-1) == num[i]  )  //value of user should match the sl no
                            {
                                __fpurge(stdin);
                                printf("press y/n to confirm/cancel      : ");   //confirmation to edit contact contents
                                scanf("%c", &opt );

                                if( opt  == 89 || opt == 121 )   //validate user value
                                {
                                    while( flag)
                                    {
                                        __fpurge(stdin);  //clear i/p buffer
                                        printf("Enter the new name               : ");
                                        scanf("%[^\n]", replace); 
                                        flag =  validatename( addressBook, replace );  //validate user given name
                                        if( !flag )
                                        {
                                            //update name into the addressbook structure
                                            strcpy( addressBook->contacts[sl-1].name, replace );
                                            printf("\n--------------------------------------------------------------------------------\n");
                                            printf("                   ******* EDITED NAME SUCCESSFULLY *******");
                                            printf("\n--------------------------------------------------------------------------------\n");
                                            break;
                                        }

                                    }

                                }
                                else 
                                {
                                    if( opt  == 78 || opt == 110 )   //validate user value
                                    {
                                        flag=0;
                                    }
                                }
                            }
                            else
                            {
                                flag=1;
                            }
                            if( !flag )
                                break;
                        }
                        //print error message ( invalid user i/p : sl no not matching )
                        if( flag==1 )
                        {
                            printf("\n--------------------------------------------------------------------------------\n");
                            printf("                         ******* INVALID INPUT *******\n");
                            printf("                             SL NUMBER NOT MATCHING \n");
                            printf("--------------------------------------------------------------------------------\n");
                            flag=-1;
                            continue;
                        }

                        //prints error message ( invalid user i/p )
                        if( flag==2)
                        {
                            printf("\n--------------------------------------------------------------------------------\n");
                            printf("                        ******* INVALID INPUT *******\n");
                            printf("                        PRESS ONLY [ (Y,N) OR (y,n) ]");
                            printf("\n--------------------------------------------------------------------------------\n");

                        }
                    }
                    break;

                //edit contact with phone number
                case 2:
                    flag=1;
                    //super loop
                    while( flag )
                    {
                        __fpurge(stdin);   //clear i/p buffer
                        printf("Enter the phone no to edit       : ");
                        scanf("%[^\n]", edit_phone);
                        flag =validatephone( addressBook, edit_phone );   //validate user given phone number
                    } 
                    flag=1;
                    //loop to find user given phone number against phone number in the addressbook staructure
                    for( i=0; i<addressBook->contactCount; i++ )
                    {
                        //prints contact content of the matching phone number
                        if( strcmp( addressBook->contacts[i].phone, edit_phone ) == 0 )
                        {
                            flag=0;
                            printf("\n--------------------------------------------------------------------------------\n");
                            printf("SL  %-*s", length, "NAME");
                            printf("    %-*s", 14, "PHONE NO");
                            printf("    %s", "E-MAIL ID");
                            printf("\n--------------------------------------------------------------------------------\n");
                            if( i<9)
                            {
                                printf("%d   ",i+1);
                            }
                            if( i>=9)
                            {
                                printf("%d  ",i+1);
                            }
                            printf("%-*s",length, addressBook->contacts[i].name);
                            printf("    %-*s",14, addressBook->contacts[i].phone);
                            printf("    %s\n", addressBook->contacts[i].email);
                            set=i+1;
                            break;
                        }
                    }
                    //prints error message ( phone number not found )
                    if( flag )
                    {
                        printf("\n--------------------------------------------------------------------------------\n");
                        printf("                         ******* NO MATCH FOUND *******\n");
                        printf("--------------------------------------------------------------------------------\n");
                        break;
                    }
                    printf("--------------------------------------------------------------------------------\n");
                    flag=1;
                    //super loop
                    while( flag )
                    {
                        __fpurge(stdin);  //clear i/p buffer
                        printf("Enter the SL number to be edited : ");
                        scanf("%d", &sl );
                        if( sl == set  )   //user value should match sl no
                        {
                            __fpurge(stdin);
                            printf("press y/n to confirm/cancel      : ");  
                            scanf("%c", &opt );

                            if( opt  == 89 || opt == 121 ) //validate user value
                            {
                                //super loop
                                while( flag)
                                {
                                    __fpurge(stdin);
                                    printf("Enter the new phone no           : ");
                                    scanf("%[^\n]", replace); 
                                    flag =  validatephone( addressBook, replace );  //validate user given phone number
                                    for( i=0; i<addressBook->contactCount && i<MAX_CONTACTS; i++)
                                    {
                                        //given user phone number must be unique
                                        if( strcmp( addressBook->contacts[i].phone, replace ) == 0 )
                                        {
                                            printf("\n--------------------------------------------------------------------------------\n");
                                            printf("                         ******* INVALID INPUT *******\n");
                                            printf("                        ENTERED PHONE NO ALREADY EXIST!"); 
                                            printf("\n--------------------------------------------------------------------------------\n");
                                            flag=2;
                                            break;
                                        }
                                    }
                                    if( flag == 2 )
                                        break;
                                    if( flag==0 )
                                    {
                                        //update user given phone number into the addressbook structure
                                        strcpy( addressBook->contacts[sl-1].phone, replace );
                                        printf("\n--------------------------------------------------------------------------------\n");
                                        printf("                   ******* EDITED PHONE NO SUCCESSFULLY *******");
                                        printf("\n--------------------------------------------------------------------------------\n");
                                    }

                                }

                            }
                            else if( opt  == 78 || opt == 110 )   //validate user values
                            {
                                break;
                            }
                            //prints error message ( invalid user i/p )
                            else
                            {
                                printf("\n--------------------------------------------------------------------------------\n");
                                printf("                        ******* INVALID INPUT *******\n");
                                printf("                        PRESS ONLY [ (Y,N) OR (y,n) ]");
                                printf("\n--------------------------------------------------------------------------------\n");

                            }
                        }
                        //prints error message ( invalid user i/p : sl no not matching )
                        else
                        {
                            printf("\n--------------------------------------------------------------------------------\n");
                            printf("                         ******* INVALID INPUT *******\n");
                            printf("                             SL NUMBER NOT MATCHING \n");
                            printf("--------------------------------------------------------------------------------\n");
                            continue;
                        }
                        if( flag == 2 )
                            break;
                    }
                    break;

                //edit contact with email id
                case 3:
                    flag=1;
                    //super loop
                    while( flag)
                    {
                        __fpurge(stdin);  //clear i/p buffer
                        printf("Enter the E-mail id to edit       : ");
                        scanf("%[^\n]", edit_email); 
                        flag = validateemail( addressBook, edit_email );   //validate user given email id
                    }
                    flag=1;
                    //loop to find user given email id against the email id of addressbook structure
                    for( i=0; i<addressBook->contactCount; i++ )
                    {
                        //prints contact contents of matching email id
                        if( strcmp( addressBook->contacts[i].email, edit_email ) == 0 )
                        {
                            flag=0;
                            printf("\n--------------------------------------------------------------------------------\n");
                            printf("SL  %-*s", length, "NAME");
                            printf("    %-*s", 14, "PHONE NO");
                            printf("    %s", "E-MAIL ID");
                            printf("\n--------------------------------------------------------------------------------\n");
                            if( i<9)
                            {
                                printf("%d   ",i+1);
                            }
                            if( i>=9)
                            {
                                printf("%d  ",i+1);
                            }
                            printf("%-*s",length, addressBook->contacts[i].name);
                            printf("    %-*s  ",14, addressBook->contacts[i].phone);
                            printf("    %s\n", addressBook->contacts[i].email);
                            set=i+1;
                            break;
                        } 

                    }
                    //prints error message ( email id not found )
                    if( flag )
                    {
                        flag=1;
                        printf("\n--------------------------------------------------------------------------------\n");
                        printf("                         ******* NO MATCH FOUND *******\n");
                        printf("--------------------------------------------------------------------------------\n");
                        break;
                    }
                    printf("--------------------------------------------------------------------------------\n");
                    flag=1;
                    //super loop
                    while( flag )
                    {
                        __fpurge(stdin);  //clear i/p buffer
                        printf("Enter the SL number to be edited : ");
                        scanf("%d", &sl );
                        if( sl == set  )   //user values should match sl no
                        {
                            __fpurge(stdin);
                            printf("press y/n to confirm/cancel      : ");
                            scanf("%c", &opt );

                            if( opt  == 89 || opt == 121 )  //validate user i/p
                            {
                                //super loop
                                while( flag)
                                {
                                    __fpurge(stdin);  //clear i/p buffer 
                                    printf("Enter the new e-mail id          : ");
                                    scanf("%[^\n]", replace); 
                                    flag =  validateemail( addressBook, replace );  //validate user given email id
                                    
                                    //loop to find given user email id against the email id in the addressbook structure
                                    for( i=0; i<addressBook->contactCount && i<MAX_CONTACTS; i++)
                                    {
                                        //user given email id should be unique
                                        if( strcmp( addressBook->contacts[i].email, replace ) == 0 )
                                        {
                                            printf("\n--------------------------------------------------------------------------------\n");
                                            printf("                         ******* INVALID INPUT *******\n");
                                            printf("                        ENTERED E-MAIL ID ALREADY EXIST!"); 
                                            printf("\n--------------------------------------------------------------------------------\n");
                                            flag=2;
                                            break;
                                        }
                                    }
                                    if( flag == 2 )
                                        break;
                                    if( flag == 0 )
                                    {
                                        //update user given email id into the addressbook structure
                                        strcpy( addressBook->contacts[sl-1].email, replace );
                                        printf("\n--------------------------------------------------------------------------------\n");
                                        printf("                   ******* EDITED E-MAIL ID SUCCESSFULLY *******");
                                        printf("\n--------------------------------------------------------------------------------\n");
                                    }

                                }

                            }
                            else if( opt  == 78 || opt == 110 )  //validate user value
                            {
                                break;
                            }
                            //prints error message ( invalid user value )
                            else
                            {
                                printf("\n--------------------------------------------------------------------------------\n");
                                printf("                        ******* INVALID INPUT *******\n");
                                printf("                        PRESS ONLY [ (Y,N) OR (y,n) ]");
                                printf("\n--------------------------------------------------------------------------------\n");

                            }
                        }
                        //prints error message ( invalid user i/p : sl no not matching )
                        else
                        {
                            printf("\n--------------------------------------------------------------------------------\n");
                            printf("                         ******* INVALID INPUT *******\n");
                            printf("                             SL NUMBER NOT MATCHING \n");
                            printf("--------------------------------------------------------------------------------\n");
                            continue;
                        }
                        if( flag == 2 )
                            break;
                    }
                    break;

                //exit edit submenu
                case 4:
                    choice=4;  //reset choice value
                    break;
            }
        }
        //print error message ( invalid user choice i/p )
        else
        {
            printf("\n--------------------------------------------------------------------------------\n");
            printf("                         ******* INVALID INPUT *******\n");
            printf("                     ENTERED CHOICE SHOULD BE IN THE RANGE\n");
            printf("                                   { [0-4] }");
            printf("\n--------------------------------------------------------------------------------\n");
            continue;
        }

        //exit from edit submenu to main menu
        if( choice == 4 )
        {
            break;
        }

    }

}

//function definition to delete contact from addressbook structure
void deleteContact(AddressBook *addressBook)
{
    int length = spacing( addressBook );
    //prints heading of delete contact
    printf("--------------------------------------------------------------------------------\n");
    printf("                                DELETE CONTACT                                    ");
    printf("\n--------------------------------------------------------------------------------\n");

    //declare variables, arrays and flags
    int i,flag=1,num=0, choice=0, once=0;
    char del_name[500], del_phone[500], del_email[500], opt;

    //super loop
    while(1)
    {
        choice=0;
        __fpurge(stdin);  //clear i/p buffer

        //print delete submenu
        printf("\nDELETE CONTACT MENU\n");
        printf("1. Name\n2. Phone no\n3. E-mail id\n4. Exit\n");
        printf("\nEnter your choice : ");
        scanf("%d", &choice);

        //validate user choice value
        if( (choice >=1) && (choice <=4) )
        {
            //choice number perform different functionalities
            switch( choice )
            {
                //delete conatct with name
                case 1:
                    once=0;
                    flag=1;
                    //supr loop
                    while( flag)
                    {
                        __fpurge(stdin);  //clear i/p buffer
                        printf("Enter the name to delete         : ");
                        scanf("%[^\n]", del_name); 
                        flag =  validatename( addressBook, del_name );   //validate user given name

                        if( flag )
                            continue;
                        //loop to find user given name against the name in the addressbook structure
                        for( i=0; i<addressBook->contactCount; i++ )
                        {
                            //name not matching
                            if( strcmp( addressBook->contacts[i].name, del_name ) != 0 )
                            {
                                flag=0;
                            }
                            //name matching
                            else if( strcmp( addressBook->contacts[i].name, del_name ) == 0 )
                            {
                                flag=1;
                                //super loop
                                while( flag )
                                {
                                    if( once == 0 )
                                    {
                                        __fpurge(stdin);  //clear i/p buffer
                                        printf("Enter the phone no to delete     : ");
                                        scanf("%[^\n]", del_phone); 
                                        flag =  validatephone( addressBook, del_phone );  //validate user given phone number
                                        if( flag == 1 )
                                            once=0;
                                        else
                                            once=1;
                                    }

                                    if( once==0 )
                                        continue;

                                    //print contact contents of matching user given name
                                    if( strcmp( addressBook->contacts[i].phone, del_phone ) == 0 )
                                    {
                                        printf("\n--------------------------------------------------------------------------------\n");
                                        printf("SL  %-*s", length, "NAME");
                                        printf("    %-*s", 14, "PHONE NO");
                                        printf("    %s", "E-MAIL ID");
                                        printf("\n--------------------------------------------------------------------------------\n");
                                        if( i<9)
                                        {
                                            printf("%d   ",i+1);
                                        }
                                        if( i>=9)
                                        {
                                            printf("%d  ",i+1);
                                        }
                                        printf("%-*s",length, addressBook->contacts[i].name);
                                        printf("    %-*s",14, addressBook->contacts[i].phone);
                                        printf("    %s\n", addressBook->contacts[i].email);

                                        printf("--------------------------------------------------------------------------------\n");
                                        num=i;
                                        flag=1;
                                        break;
                                    }
                                    else
                                    {
                                        flag=0;
                                    } 
                                }
                            }
                            if( flag )
                                break;
                        }
                        //print error message ( invalid user i/p : name not found )
                        if( !flag )
                        {
                            printf("\n--------------------------------------------------------------------------------\n");
                            printf("                         ******* NO MATCH FOUND *******\n");
                            printf("--------------------------------------------------------------------------------\n");
                        }
                        else
                            break;
                    }

                    if( !flag )
                    {
                        flag=1;
                        continue;
                    }


                    //super loop
                    while( flag )
                    {
                        __fpurge(stdin);  //clear i/p buffer
                        printf("press y/n to confirm/cancel      : ");
                        scanf("%c", &opt );

                        if( opt  == 89 || opt == 121 )   //validate user given value
                        {
                            //loop to delete contact ( rearrange contents in the addressbook structure )
                            for( i=num; i<=addressBook->contactCount && i<MAX_CONTACTS; i++)
                            {
                                strcpy(addressBook->contacts[i].name, addressBook->contacts[i+1].name);
                                strcpy(addressBook->contacts[i].phone, addressBook->contacts[i+1].phone);
                                strcpy(addressBook->contacts[i].email, addressBook->contacts[i+1].email);
                            }
                            addressBook->contactCount = addressBook->contactCount-1;  //update contact count

                            //print delete contact success message
                            printf("\n--------------------------------------------------------------------------------\n");
                            printf("                  ******* DELETED CONTACT SUCCESSFULLY *******");
                            printf("\n--------------------------------------------------------------------------------\n");
                            flag=0;
                        }
                        else if( opt  == 78 || opt == 110 ) //validate user given value
                        {
                            break;
                        }
                        //prints error message ( invalid user i/p value )
                        else
                        {
                            printf("\n--------------------------------------------------------------------------------\n");
                            printf("                        ******* INVALID INPUT *******\n");
                            printf("                        PRESS ONLY [ (Y,N) OR (y,n) ]");
                            printf("\n--------------------------------------------------------------------------------\n");

                        }
                    }
                    break;
                //delete contact with phone number
                case 2:
                    flag=1;
                    //super loop
                    while( flag)
                    {
                        __fpurge(stdin);   //clear i/p buffer
                        printf("Enter the phone no to delete      : ");
                        scanf("%[^\n]", del_phone); 
                        flag =  validatephone( addressBook, del_phone );   //validate user given phone number

                        if( !flag )
                            break;
                    }
                    //loop to find user given phone number against the phone no in the addressbook structure
                    for( i=0; i<addressBook->contactCount; i++ )
                    {
                        //phone not number matching
                        if( strcmp( addressBook->contacts[i].phone, del_phone ) != 0 )
                        {
                            flag=0;
                        }
                        //phone number matching
                        else if( strcmp( addressBook->contacts[i].phone, del_phone ) == 0 )
                        {
                            //prints conatct content of the matching user given phone number
                            printf("\n--------------------------------------------------------------------------------\n");
                            printf("SL  %-*s", length, "NAME");
                            printf("    %-*s", 14, "PHONE NO");
                            printf("    %s", "E-MAIL ID");
                            printf("\n--------------------------------------------------------------------------------\n");
                            if( i<9)
                            {
                                printf("%d   ",i+1);
                            }
                            if( i>=9)
                            {
                                printf("%d  ",i+1);
                            }
                            printf("%-*s",length, addressBook->contacts[i].name);
                            printf("    %-*s",14, addressBook->contacts[i].phone);
                            printf("    %s\n", addressBook->contacts[i].email);
                            printf("--------------------------------------------------------------------------------\n");
                            flag=1;
                            num=i;
                            break;
                        }

                    }

                    //prints error message ( invalid user i/p : phone number not found )
                    if( !flag )
                    {
                        printf("\n--------------------------------------------------------------------------------\n");
                        printf("                         ******* NO MATCH FOUND *******\n");
                        printf("--------------------------------------------------------------------------------\n");
                    }

                    //super loop
                    while( flag )
                    {
                        __fpurge(stdin);  //clear i/p buffer
                        printf("press y/n to confirm/cancel      : ");
                        scanf("%c", &opt );

                        if( opt  == 89 || opt == 121 )   //validate user given input
                        {
                            //loop to delete contact ( rearrange contact contents in the addressbook structure )
                            for( i=num; i<=addressBook->contactCount && i<MAX_CONTACTS; i++)
                            {
                                strcpy(addressBook->contacts[i].name, addressBook->contacts[i+1].name);
                                strcpy(addressBook->contacts[i].phone, addressBook->contacts[i+1].phone);
                                strcpy(addressBook->contacts[i].email, addressBook->contacts[i+1].email);
                            }
                            addressBook->contactCount = addressBook->contactCount-1;  //update total contact count

                            //prints delete contact success message
                            printf("\n--------------------------------------------------------------------------------\n");
                            printf("                  ******* DELETED CONTACT SUCCESSFULLY *******");
                            printf("\n--------------------------------------------------------------------------------\n");
                            flag=0;
                        }
                        else if( opt  == 78 || opt == 110 )   //valiadte user given value
                        {
                            break;
                        }
                        //prints error message ( invalid user i/p value )
                        else
                        {
                            printf("\n--------------------------------------------------------------------------------\n");
                            printf("                        ******* INVALID INPUT *******\n");
                            printf("                        PRESS ONLY [ (Y,N) OR (y,n) ]");
                            printf("\n--------------------------------------------------------------------------------\n");

                        }
                    }
                    break;

                //delete conatct with email id
                case 3:
                    flag=1;
                    //super loop
                    while( flag)
                    {
                        __fpurge(stdin);  //clear i/p buffer
                        printf("Enter the e-mail id to delete     : ");
                        scanf("%[^\n]", del_email); 
                        flag =  validateemail( addressBook, del_email );  //validate user given email id

                        if( !flag )
                            break;
                    }
                    //loop to find user given email id against the email id in the adddressbook structure
                    for( i=0; i<addressBook->contactCount; i++ )
                    {
                        //email id nit matching
                        if( strcmp( addressBook->contacts[i].email, del_email ) != 0 )
                        {
                            flag=0;
                        }
                        //email id matching
                        else if( strcmp( addressBook->contacts[i].email, del_email ) == 0 )
                        {
                            //prints contact content of matching user given email id
                            printf("\n--------------------------------------------------------------------------------\n");
                            printf("SL  %-*s", length, "NAME");
                            printf("    %-*s", 14, "PHONE NO");
                            printf("    %s", "E-MAIL ID");
                            printf("\n--------------------------------------------------------------------------------\n");
                            if( i<9)
                            {
                                printf("%d   ",i+1);
                            }
                            if( i>=9)
                            {
                                printf("%d  ",i+1);
                            }
                            printf("%-*s",length, addressBook->contacts[i].name);
                            printf("    %-*s",14, addressBook->contacts[i].phone);
                            printf("    %s\n", addressBook->contacts[i].email);
                            printf("--------------------------------------------------------------------------------\n");
                            flag=1;
                            num=i;
                            break;
                        }

                    }

                    //prints error message ( invalid user i/p  : email id not found )
                    if( !flag )
                    {
                        printf("\n--------------------------------------------------------------------------------\n");
                        printf("                         ******* NO MATCH FOUND *******\n");
                        printf("--------------------------------------------------------------------------------\n");
                        break;
                    }

                    //super loop
                    while( flag )
                    {
                        __fpurge(stdin);  //clear i/p buffer
                        printf("press y/n to confirm/cancel      : ");
                        scanf("%c", &opt );

                        if( opt  == 89 || opt == 121 )   //validate user given value
                        {
                            //loop to delete contact ( rearrange of the contact content in the addressbook structure )
                            for( i=num; i<=addressBook->contactCount && i<MAX_CONTACTS; i++)
                            {
                                strcpy(addressBook->contacts[i].name, addressBook->contacts[i+1].name);
                                strcpy(addressBook->contacts[i].phone, addressBook->contacts[i+1].phone);
                                strcpy(addressBook->contacts[i].email, addressBook->contacts[i+1].email);
                            }
                            addressBook->contactCount = addressBook->contactCount-1;  //upadte toatl contact count
                            //prints delete contact success message
                            printf("\n--------------------------------------------------------------------------------\n");
                            printf("                  ******* DELETED CONTACT SUCCESSFULLY *******");
                            printf("\n--------------------------------------------------------------------------------\n");
                            flag=0;
                        }
                        else if( opt  == 78 || opt == 110 )  //validate user given value
                        {
                            break;
                        }
                        //prints error message ( invalide user i/p value )
                        else
                        {
                            printf("\n--------------------------------------------------------------------------------\n");
                            printf("                        ******* INVALID INPUT *******\n");
                            printf("                        PRESS ONLY [ (Y,N) OR (y,n) ]");
                            printf("\n--------------------------------------------------------------------------------\n");

                        }

                    }
                    break;

                //exit submenu
                case 4:
                    choice=4; //reset choice value
                    break;
            }
        }
        //prints error message ( invalid user i/p choice )
        else
        {
            printf("\n--------------------------------------------------------------------------------\n");
            printf("                         ******* INVALID INPUT *******\n");
            printf("                     ENTERED CHOICE SHOULD BE IN THE RANGE\n");
            printf("                                   { [0-4] }");
            printf("\n--------------------------------------------------------------------------------\n");
            continue;
        }

        //exit delete submenu and return to main menu
        if( choice == 4 )
        {
            break;
        }
    }

}

//function definition to validate user given name
int validatename( AddressBook *addressBook, char *create_name )
{
    //declare variables, arrays and flags
    int i=0, j=0, k=0, flag=1;    
    char create_dup[500];

    __fpurge(stdin);  //clear i/p buffer
    strcpy( create_dup, create_name );  //make a duplicate copy of user i/p
    while((create_dup[j] == ' ') || (create_dup[j] == '\t')) //skip spaces and tabs in start of the user i/p name
    {
        j++;
    }

    //copy name without spaces and tabs
    while( create_dup[j] != '\0' )
    {
        create_name[k++] = create_dup[j++] ;
    }

    create_name[k] = create_dup[j];  //adds null character

    //traverse through each character of user given i/p name
    while( create_name[i] != '\0'  )
    {
        //character should be upper or lower case only
        if((create_name[i]>=65 && create_name[i]<=90)||(create_name[i]>=97 && create_name[i]<=122));
        else if( create_name[i] == ' ' );
        else
        {
            printf("\n--------------------------------------------------------------------------------\n");
            printf("                         ******* INVALID INPUT *******\n");
            printf("                       ENTERED NAME SHOULD CONTAIN ONLY\n");
            printf("                                { [a-z] [A-Z] }");
            printf("\n--------------------------------------------------------------------------------\n");
            return 1; //return error value
        }
        i++;
    }
}

//function definition to validate user given phone number i/p
int validatephone( AddressBook *addressBook, char *create_phone )
{
    //declare variables and flags
    int i=0, j=0, k=0, flag=1;    

    //phone number should not exceed 10 numbers
    if( strlen( create_phone ) != 10 )
    {
        //prints error message
        printf("\n--------------------------------------------------------------------------------\n");
        printf("                         ******* INVALID INPUT *******\n");
        printf("                ENTERED PHONE NO SHOULD CONTAIN ONLY 10 NUMBERS");
        printf("\n--------------------------------------------------------------------------------\n");
        return 1; //return error value
    }
    //phone number is equal to 10 numbers
    else if( strlen( create_phone ) == 10 )
    {
        for( i=0; i<10; i++ )
        {
            //each character should be only digits
            if( create_phone[i] >= 48 && create_phone[i] <= 57 )
            {
                ;
            }
            //characters not digits
            else
            {
                //prints error message
                printf("\n--------------------------------------------------------------------------------\n");
                printf("                         ******* INVALID INPUT *******\n");
                printf("                     ENTERED PHONE NO SHOULD CONTAIN ONLY\n");
                printf("                                   { [0-9] }");
                printf("\n--------------------------------------------------------------------------------\n");
                return 1; //return error value
            }
        }
    }

    return 0; //return success value
}

//function definition to validate user given email id
int validateemail( AddressBook *addressBook, char *create_email )
{
    //declare variables and flags
    int i=0, j=0, k=0, flag=1;   

   //email id must have domain separator 
    if( strchr( create_email, '@' ) == NULL )
    {
        //prints error message
        printf("\n--------------------------------------------------------------------------------\n");
        printf("                         ******* INVALID INPUT *******\n");
        printf("                  ENTERED E-MAIL ID INVALID DOMAIN SEPARATOR\n");
        printf("                                   { ['@'] }");
        printf("\n--------------------------------------------------------------------------------\n");
        return 1; //return failed value
    }
    if( flag )
    {
        //email id must have domain extension
        if( strstr( create_email, ".com" ) == NULL )
        {
            //prints error message
            printf("\n--------------------------------------------------------------------------------\n");
            printf("                         ******* INVALID INPUT *******\n");
            printf("                  ENTERED E-MAIL ID INVALID DOMAIN EXTENSION\n");
            printf("                                   { [.com] }");
            printf("\n--------------------------------------------------------------------------------\n");
            return 1; //returns failed value
        }

        if(flag)
        {
            //email id must not start with digits
            if( ( create_email[0] >= 48 ) && ( create_email[0] <= 57 ) )
            {
                //prints error message
                printf("\n--------------------------------------------------------------------------------\n");
                printf("                         ******* INVALID INPUT *******\n");
                printf("                ENTERED E-MAIL ID SHOULD NOT START WITH NUMBERS\n");
                printf("                                   { [0-9] }");
                printf("\n--------------------------------------------------------------------------------\n");
                return 1; //returns failed value
            }
        }
    }

    if( flag )
    {
        i=0;
        //email id validation before domain separator
        while( create_email[i] != '@' )
        {
            //email id can be upper or lower case
            if((create_email[i]>=65 && create_email[i]<=90)||(create_email[i]>=97 && create_email[i]<=122))
            {
                ;
            }
            //email id can be digits ( not the first character )
            else if( ( create_email[i] >= 48 ) && ( create_email[i] <= 57 ) );

            //email id can have below sepcial characters
            else if( ( create_email[i] == '_' ) || ( create_email[i] == '.' ) );
            //above failed cases
            else
            {
                //prints error message
                printf("\n--------------------------------------------------------------------------------\n");
                printf("                         ******* INVALID INPUT *******\n");
                printf("                ENTERED E-MAIL ID USERNAME NAME SHOULD CONTAIN ONLY\n");
                printf("                        { [a-z] [A-Z] [0-9] [ '.' '_'] }");
                printf("\n--------------------------------------------------------------------------------\n");
                return 1; //returns failed value
            }
            i++;
        }
    }
    if( flag )
    {
        i++;
        //email id validation after domain separator
        while( create_email[i] != '\0' )
        {
            //email id must be lower case
            if( create_email[i]>=97 && create_email[i]<=122 );
            //email id can be digits
            else if( ( create_email[i] >= 48 ) && ( create_email[i] <= 57 ) );
            //email id can have below special character
            else if( create_email[i] == '.' );
            //above failed cases
            else
            {
                //prints error message
                printf("\n--------------------------------------------------------------------------------\n");
                printf("                         ******* INVALID INPUT *******\n");
                printf("                ENTERED E-MAIL ID DOMAIN NAME SHOULD CONTAIN ONLY\n");
                printf("                              { [a-z] [0-9] [.] }");
                printf("\n--------------------------------------------------------------------------------\n");

                return 1; //returns failed value
            }
            i++;
        }
    }
    return 0; //return success value
}
