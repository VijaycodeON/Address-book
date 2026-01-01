//header files and preprocessor directives
#include <stdio.h>
#include "file.h"

//function definition to load contacts from file to the structure
void loadContactsFromFile(AddressBook *addressBook) 
{
    
    FILE *fptr;   
    fptr= fopen("contact.csv", "r+");  //file opened in read plus mode
    
    //file pointer valiadtion
    if( fptr == NULL )
    {    
        printf("ERROR in file opening\n");
    }    
    else 
    {    
        //loop that copies contents from file to structure
        for( int i=0; fgetc(fptr)!=EOF; i++ )
        {
            fseek(fptr,-1, SEEK_CUR);
            fscanf(fptr, "%[^,],%[^,],%[^\n]\n",addressBook->contacts[i].name, addressBook->contacts[i].phone, addressBook->contacts[i].email ); 
            addressBook->contactCount= addressBook->contactCount +1; //increment the total count of the contacts
          
        }
    }    

    //close file pointer
    fclose(fptr);
}


//function definition to save structure contents into the file    
void saveContactsToFile(AddressBook *addressBook) 
{
    FILE *fptr;
    fptr= fopen("contact.csv", "w+"); //file opened in write plus mode
    
    //file pointer validation
    if( fptr == NULL )
    {    
        printf("ERROR in file opening\n");
    }    
    else 
    {    
        //loop that copies contents from structure to file
        for( int i=0; i<addressBook->contactCount && i<MAX_CONTACTS; i++ )
        {
            fprintf(fptr, "%s,%s,%s\n",addressBook->contacts[i].name, addressBook->contacts[i].phone, addressBook->contacts[i].email ); 
        }
    }    

    //close file pointer
    fclose(fptr);
}
