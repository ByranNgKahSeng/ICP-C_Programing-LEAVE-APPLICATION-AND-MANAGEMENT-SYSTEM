//Name:Byran Ng Kah Seng
//TP068478

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

struct Staff; 
void admin_menu(struct Staff *);
void staff_menu(struct Staff *);
void superior_menu(struct Staff *);
void generate_report(void);
void show_profile(struct Staff *);
void add_staff(struct Staff *);
void update_staff(struct Staff *);
void check_leave(struct Staff *);
void search_leave_by_date(struct Staff *);
void apply_leave(struct Staff *);
void cancel_leave(struct Staff *);
void leave_status(struct Staff *);
void leave_information(struct Staff *);

struct application{
    char Name[50];
    int Day;
    char Type[50];
    char From[20];
    char To[20];
    char Status[50];
    char Apply_date[20];
    char Reason[100];
};

struct Staff{
    char staff_id[20];
    char password[20];
    char name[50];
    char gender[10];
    int age;
    char role[50];
    char dept[50];
    int annual_leave;
    int compassionate_leave;
    int emergency_leave;
    int medical_leave;
};

int main(void) {
    struct Staff staffs[20];
    char entered_staff_id[100];
    char entered_password[100];
    int logged_in=0;
    FILE *fp;
    
    //login
    printf("Welcome to APspace\n");
    int attempt=3;
    while(attempt>0 && logged_in==0){
        printf("Enter your staff id: ");
        scanf("%s",entered_staff_id);
        printf("Enter your password: ");
        scanf("%s",entered_password);
        
        fp=fopen("users.txt","r");
        if(fp==NULL){
            printf("Could not open the file.\n");
        }
        else {
            while (fscanf(fp,"%s %s %s %s %d %s %s %d %d %d %d\n",staffs[0].staff_id,staffs[0].password,staffs[0].name,
            staffs[0].gender,&staffs[0].age,staffs[0].role,staffs[0].dept,
            &staffs[0].annual_leave,&staffs[0].compassionate_leave,&staffs[0].emergency_leave,&staffs[0].medical_leave) == 11) {
                if(strcmp(staffs[0].staff_id,entered_staff_id)==0 && strcmp(staffs[0].password,entered_password)==0){
                    logged_in=1;
                    break; 
                }
            }
        }
        if(logged_in==0){
            attempt=attempt-1;
            printf("Incorrect username or password!\n");
        }
    }
    if(attempt==0){
        printf("You have exceeded the maximum number of wrong attempts. Goodbye!\n");
        return 0;
    }
    //logged in
    if(logged_in==1){
        printf("Login Successful!\n");
        //login to admin menu
        if(strcmp(staffs[0].role,"admin")==0){
            printf("Welcome,Admin %s!\n",staffs[0].name);
            admin_menu(&staffs[0]);
        }
        else if(strcmp(staffs[0].role,"superior")==0){
            printf("Welcome,Superior %s!\n",staffs[0].name);
            superior_menu(&staffs[0]);
        //login to staff menu
        }else{
            printf("Welcome,%s\n",staffs[0].name);
            staff_menu(&staffs[0]);
        }
    }else{
        printf("Incorrect password or username\n");
    }
    fclose(fp);
    return 0;
}

void admin_menu(struct Staff *staffs){
    //admin menu
    int choice;
    printf("\n===Admin menu===\n");
    printf("1. Add staff\n");
    printf("2. Update staff's leave balance\n");
    printf("3. Search staff\n");
    printf("4. Generate report\n");
    printf("5. Exit\n");
    printf("Enter your choice:");
    scanf("%d",&choice);
    
    switch (choice){
        case 1:
            add_staff(&staffs[0]);
            break;
        case 2:
            update_staff(&staffs[0]);
            break;
        case 3:
            show_profile(&staffs[0]);
            break;
        case 4:
            generate_report();
            break;
        case 5:
            printf("Logged Out!");
            exit(0);
            
        default:
            printf("Invalid choice. Please try again.\n");
            admin_menu(&staffs[0]);
            break;
    }
}

void staff_menu(struct Staff *staffs){
    //staff menu
    int choice;
    printf("\n===Staff menu===\n");
    printf("1. Apply leave\n");
    printf("2. Cancel leave\n");
    printf("3. Check leave status\n");
    printf("4. Check summary of leave information based on apply date\n");
    printf("5. Exit\n");
    printf("Enter your choice:");
    scanf("%d",&choice);
    
    switch (choice){
        case 1:
            apply_leave(&staffs[0]);
            break;
        case 2:
            cancel_leave(&staffs[0]);
            break;
        case 3:
            leave_status(&staffs[0]);
            break;
        case 4:
            leave_information(&staffs[0]);
            break;
        case 5:
            printf("Logged Out!");
            exit(0);
            
        default:
            printf("Invalid choice. Please try again.\n");
            staff_menu(&staffs[0]);
            break;
    }
}

void leave_information(struct Staff *staffs){
    int annual_approved_days=0, annual_pending_days=0 ;
    int compassionate_approved_days=0, compassionate_pending_days=0 ;
    int emergency_approved_days=0, emergency_pending_days=0 ;
    int medical_approved_days=0, medical_pending_days=0 ;
    char month_year[10];
    printf("Enter the month and year (mm/yyyy): ");
    scanf("%s", month_year);
    if(strlen(month_year)!=7){
        printf("Incorrect date format!\n");
        leave_information(&staffs[0]);
    }
    
    FILE *fp = fopen("leave_application.txt", "r");
    if (fp == NULL) {
        printf("Error: Unable to open file\n");
        return;
    }
    char temp_name[50], temp_month_year[20], type[20], status[20], date[8];
    int day;
    while (fscanf(fp, "%s %d %s %*s %*s %s %s %*s\n", temp_name, &day, type, status, temp_month_year) == 5) {
        // Extract month and year from the date string
        strncpy(date, temp_month_year+3, 7); // get the date in mm/yyyy format
        if (strcmp(staffs[0].name, temp_name) == 0 && strcmp(month_year, date) == 0) {
            if (strcmp(type, "annual_leave") == 0) {
                if (strcmp(status, "Approved") == 0) {
                    annual_approved_days += day;
                } else if (strcmp(status, "Pending") == 0) {
                    annual_pending_days += day;
                } 
            } else if (strcmp(type, "compassionate_leave") == 0) {
                if (strcmp(status, "Approved") == 0) {
                    compassionate_approved_days += day;
                } else if (strcmp(status, "Pending") == 0) {
                    compassionate_pending_days += day;
                } 
            } else if (strcmp(type, "emergency_leave") == 0) {
                if (strcmp(status, "Approved") == 0) {
                    emergency_approved_days += day;
                } else if (strcmp(status, "Pending") == 0) {
                    emergency_pending_days += day;
                } 
            } else if (strcmp(type, "medical_leave") == 0) {
                if (strcmp(status, "Approved") == 0) {
                    medical_approved_days += day;
                } else if (strcmp(status, "Pending") == 0) {
                    medical_pending_days += day;
                } 
            } 
        }
    }
    fclose(fp);
    printf("\nSummary of Leave Information apply in %s:\n", month_year);
    printf("-------------------------------------------------------------------\n");
    printf("Type           Leave Balance    Approved Days    Pending Days\n");
    printf("-------------------------------------------------------------------\n");
    printf("Annual         %d \t\t %d \t\t %d\n", staffs[0].annual_leave, annual_approved_days, annual_pending_days);
    printf("Compassionate  %d \t\t %d \t\t %d\n", staffs[0].compassionate_leave, compassionate_approved_days, compassionate_pending_days);
    printf("Emergency      %d \t\t %d \t\t %d\n", staffs[0].emergency_leave, emergency_approved_days, emergency_pending_days);
    printf("Medical        %d \t\t %d \t\t %d\n", staffs[0].medical_leave, medical_approved_days, medical_pending_days);
    char input[100];
    printf("\nPress any key to return to the staff menu...");
    scanf("%s", input);
    staff_menu(&staffs[0]);
}

void leave_status(struct Staff *staffs){
    struct application leave[20];
    int leave_id, i, j;
    char choice;
    char search_date[11];
    
    printf("Enter date (dd/mm/yyyy): ");
    scanf("%s", search_date);    
    FILE *fp;
    fp = fopen("leave_application.txt", "r");
    printf("\nLeave status on %s\n",search_date);
    printf("Number\tName\tDay\tType\tFrom\tTo\tStatus\tApplyDate\tReason\n");
    printf("==============================================================\n");

    // Read leave records from file and display them
    i = 0;
    int found=0;
    while (fscanf(fp,"%s %d %s %s %s %s %s %s\n",leave[i].Name,&leave[i].Day,leave[i].Type,leave[i].From, leave[i].To, leave[i].Status, leave[i].Apply_date, leave[i].Reason) == 8) {
        if(strcmp(leave[i].Name,staffs[0].name)==0){
            if (strcmp(leave[i].From, search_date) <= 0 && strcmp(leave[i].To, search_date) >= 0) {
                printf("%d\t%s\t%d\t%s\t%s\t%s\t%s\t%s\t%s\n", i, leave[i].Name, leave[i].Day, leave[i].Type, leave[i].From, leave[i].To, leave[i].Status, leave[i].Apply_date, leave[i].Reason);
                found=1;
            }
        }
        i++;
    }
    if (!found) {
        printf("No leave records found on %s\n", search_date);
        staff_menu(&staffs[0]);
        return; // exit the function
    }
    //wait user input anything before return to menu
    char input[100];
    printf("\nPress any key to return to the staff menu...");
    scanf("%s", input);
    staff_menu(&staffs[0]);
    return;
}

void get_current_date(char date[]) {
    // Get the current time
    time_t t = time(NULL);
    // Convert the current time to a struct representing the local time
    struct tm tm = *localtime(&t);
     // Format the date string as "dd/mm/yyyy" and store it in the date array
    sprintf(date, "%02d/%02d/%d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
}


void cancel_leave(struct Staff *staffs) {
    struct application leave[100];
    int leave_id, i, j;
    char choice;

    FILE *fp;
    fp = fopen("leave_application.txt", "r");
    if (fp==NULL){
        printf("Error");
        staff_menu(&staffs[0]);
    }

    printf("Number\tName\tDay\tType\tFrom\tTo\tStatus\tApplyDate\tReason\n");
    printf("==============================================================\n");

    // Read leave records from file and display them
    i = 0;
    int found=0;
    
    while (fscanf(fp,"%s %d %s %s %s %s %s %s\n",leave[i].Name,&leave[i].Day,leave[i].Type,leave[i].From, leave[i].To, leave[i].Status, leave[i].Apply_date, leave[i].Reason) == 8) {
        if(strcmp(leave[i].Name,staffs[0].name)==0){
            printf("%d\t%s\t%d\t%s\t%s\t%s\t%s\t%s\t%s\n", i, leave[i].Name, leave[i].Day, leave[i].Type, leave[i].From, leave[i].To, leave[i].Status, leave[i].Apply_date, leave[i].Reason);
            found=1;
        }
        i++;
    }
    if (!found) {
        printf("No leave records found for %s\n", staffs[0].name);
        staff_menu(&staffs[0]);
        return; // exit the function
    }

    // Get the leave ID from the user
    printf("\nEnter the leave ID to cancel: ");
    scanf("%d", &leave_id);

    // Check if the leave ID is valid
    if (leave_id >= i) {
        printf("Invalid leave ID\n");
        printf("Return to menu\n");
        staff_menu(&staffs[0]);
        return;
    }

    // Check if the leave status is pending or approved
    if (strcmp(leave[leave_id].Status, "Pending") != 0 && strcmp(leave[leave_id].Status, "Approved") != 0) {
        printf("Leave cannot be cancelled because it has already been %s\n", leave[leave_id].Status);
        return;
    }

    // Check if the on-leave date has passed
    char current_date[11];
    get_current_date(current_date);
    if (strcmp(current_date, leave[leave_id].From) < 0 ) {
        printf("Leave cannot be cancelled because the on-leave date has already passed\n");
        return;
    }

    // Confirm cancellation with user
    printf("Are you sure you want to cancel this leave? (y/n) ");
    scanf(" %c", &choice);

    if (choice == 'y' || choice == 'Y') {
        // Update the leave status to cancelled
        strcpy(leave[leave_id].Status, "Cancelled");
        // Write the updated leave records to file
        fp = fopen("leave_application.txt", "w");
        for (j = 0; j < i; j++) {
            fprintf(fp, "%s %d %s %s %s %s %s %s\n", leave[j].Name, leave[j].Day, leave[j].Type, leave[j].From, leave[j].To, leave[j].Status, leave[j].Apply_date, leave[j].Reason);
        }
        fclose(fp);
        printf("Leave has been cancelled\n");
        staff_menu(&staffs[0]);
    } else {
        printf("Leave cancellation has been cancelled\n");
        staff_menu(&staffs[0]);
    }
}

void apply_leave(struct Staff *staffs){
    int total_day;
    char leave_type[20], from_date[20], to_date[20], current_date[11], reason[100];
    printf("\n====== Apply Leave Form ======\n");
    printf("Name:%s\n",staffs[0].name);
    
    printf("Total Days: ");
    scanf("%d", &total_day);
    if(total_day<=0){
        printf("Invalid Days!\n");
        staff_menu(&staffs[0]);
    }
    
    printf("Leave Type(annual_leave/compassionate_leave/emergency_leave/medical_leave): ");
    scanf("%s", leave_type);
    if (strcmp(leave_type, "annual_leave") != 0 && strcmp(leave_type, "compassionate_leave") != 0 && strcmp(leave_type, "emergency_leave") != 0 && strcmp(leave_type, "medical_leave") != 0) {
        printf("Invalid leave type\n");
        staff_menu(&staffs[0]);
    }
    //check the leave balance
    if (strcmp(leave_type,"annual_leave")==0 && staffs[0].annual_leave-total_day < 0){
        printf("Not enough leave balance!\n");
        staff_menu(&staffs[0]);
    } else if (strcmp(leave_type,"compassionate_leave")==0 && staffs[0].compassionate_leave-total_day < 0){
        printf("Not enough leave balance!\n");
        staff_menu(&staffs[0]);
    } else if (strcmp(leave_type,"emergency_leave")==0 && staffs[0].emergency_leave-total_day < 0){
        printf("Not enough leave balance!\n");
        staff_menu(&staffs[0]);
    } else if (strcmp(leave_type,"medical_leave")==0 && staffs[0].medical_leave-total_day < 0){
        printf("Not enough leave balance!\n");
        staff_menu(&staffs[0]);
    }
    
    get_current_date(current_date);
    
    printf("From (dd/mm/yyyy): ");
    scanf("%s", from_date);
    if (strlen(from_date) != 10){
        printf("Invalid date format\n");
        staff_menu(&staffs[0]);
    }
    
    printf("To (dd/mm/yyyy): ");
    scanf("%s", to_date);
    if (strlen(to_date) != 10){
        printf("Invalid date format\n");
        staff_menu(&staffs[0]);
    }
    
    printf("ApplyDate (dd/mm/yyyy): %s\n",current_date);
    
    printf("Reason: ");
    scanf(" %[^\n]s", reason);  //read entire string until newline character (\n) is encountered
    
    // Replace whitespaces with underscores in the reason string
    for (int i = 0; i < strlen(reason); i++) {
        if (reason[i] == ' ') {
            reason[i] = '_';
        }
    }
    
    // Write leave information to file
    FILE *fp;
    fp = fopen("leave_application.txt", "a");
    if(fp == NULL){
        printf("Could not open the file.\n");
    }else{
        fprintf(fp, "%s %d %s %s %s %s %s %s\n", staffs[0].name, total_day, leave_type, from_date, to_date, "Pending", current_date, reason);
        fclose(fp);
        printf("\nLeave application submitted successfully.\n");
    }
    
    staff_menu(&staffs[0]);
}

void superior_menu(struct Staff *staffs){
    //leave approver menu
    int choice;
    printf("\n===Superior menu===\n");
    printf("1. Check all leave application\n");
    printf("2. Search leave application on specified date\n");
    printf("3. Exit\n");
    printf("Enter your choice:");
    scanf("%d",&choice);
    
    switch (choice){
        case 1:
            check_leave(&staffs[0]);
            break;
        case 2:
            search_leave_by_date(&staffs[0]);
            break;
        case 3:
            printf("Program end, thank you!\n");
            exit(0);
            
        default:
            printf("Invalid choice. Please try again.\n");
            superior_menu(&staffs[0]);
            break;
    }
}

void add_staff(struct Staff *staffs){
    char newstaff_name[50]; 
    char newstaff_gender[50];
    int newstaff_age; 
    char newstaff_role[50];
    char newstaff_dept[50];

    //get new staff information
    printf("Enter new staff name:");
    scanf("%s",newstaff_name);
    for (int i = 0; i < strlen(newstaff_name); i++) {
        if (newstaff_name[i] == ' ') {
            newstaff_name[i] = '_';
        }   //replace whitespace to underscore
    }
    
    printf("Enter gender:");
    scanf("%s",newstaff_gender);
    
    printf("Enter age:");
    scanf("%d",&newstaff_age);
    
    printf("Enter role(admin/staff/superior):");
    scanf("%s",newstaff_role);
    
    printf("Enter department(academic/administration/management/technical):");
    scanf("%s",newstaff_dept);

    FILE *fp;
    char line[100];
    char *staff_id;
    int new_id;
    char new_staff_id[10];
    char new_password[50];
    char default_leave[]="15 3 2 7";
    fp=fopen("users.txt","r");
    if(fp==NULL){
        printf("Could not open the file.\n");
    }else{
        while(fgets(line,100,fp)!=NULL){
            //extract the staff ID
            staff_id=strtok(line," ");
            
            //increase the ID number
            //skip the first five characters "staff"
            //atoi convert string to integer
            new_id=atoi(staff_id+5)+1;
            //generate new staff ID
            sprintf(staff_id,"staff%03d",new_id);
            //generate new password
            sprintf(new_password,"password%d",new_id);
        }
    }
    fclose(fp);
    fp=fopen("users.txt","a");
    if(fp==NULL){
        printf("Could not open the file.\n");
    }else{
        fprintf(fp,"%s %s %s %s %d %s %s %s\n",staff_id,new_password,newstaff_name,newstaff_gender,newstaff_age,newstaff_role,newstaff_dept,default_leave);
    }
    printf("\nNew staff profile added successfully!\n");
    printf("Staff ID:%s\n",staff_id);
    printf("password:%s\n",new_password);
    fclose(fp);
}


void update_staff(struct Staff *staffs){
    int choice;
    printf("====Choose Number to Modify====\n");
    printf("***Attention the leave balance will update to all staff\n");
    printf("1.Annual leave\n");
    printf("2.Compassionate leave\n");
    printf("3.Emergency leave\n");
    printf("4.Medical leave\n");
    printf("5.Reset the leave balance\n");
    printf("6.Exit\n");
    printf("================================\n");
    FILE *fp;
    int i=0;
    fp=fopen("users.txt","r");
    if(fp==NULL){
        printf("Could not open the file.\n");
    }else{
        while(!feof(fp)){   //copy each value 
            int n=fscanf(fp,"%s %s %s %s %d %s %s %d %d %d %d\n",staffs[i].staff_id,staffs[i].password,staffs[i].name,
            staffs[i].gender,&staffs[i].age,staffs[i].role,staffs[i].dept,&staffs[i].annual_leave,
            &staffs[i].compassionate_leave,&staffs[i].emergency_leave,&staffs[i].medical_leave);
            if (n!=11){ //send error while the array is wrong
                printf("Error\n");
                break;
            }
            i++;
        }
    }
    fclose(fp);
    printf("Enter number:");
    scanf("%d",&choice);
    int leave_balance;
    int j;
    fp=fopen("users.txt","w");
    switch (choice){
        case 1:
            printf("Modify leave balance for Annual leave:");
            scanf("%d",&leave_balance);   //get new leave balance
            for(j=0;j<i;j++){
                staffs[j].annual_leave=leave_balance;    //addition of balance
                fprintf(fp,"%s %s %s %s %d %s %s %d %d %d %d\n",staffs[j].staff_id,staffs[j].password,staffs[j].name,
            staffs[j].gender,staffs[j].age,staffs[j].role,staffs[j].dept,staffs[j].annual_leave,
            staffs[j].compassionate_leave,staffs[j].emergency_leave,staffs[j].medical_leave);
            }
            break;
        case 2:
            printf("Modify leave balance for Compassionate leave:");
            scanf("%d",&leave_balance);   //get new leave balance
            for(j=0;j<i;j++){
                staffs[j].compassionate_leave=leave_balance;    //addition of balance
                fprintf(fp,"%s %s %s %s %d %s %s %d %d %d %d\n",staffs[j].staff_id,staffs[j].password,staffs[j].name,
            staffs[j].gender,staffs[j].age,staffs[j].role,staffs[j].dept,staffs[j].annual_leave,
            staffs[j].compassionate_leave,staffs[j].emergency_leave,staffs[j].medical_leave);
            }
            break;
        case 3:
            printf("Modify leave balance for Emergency leave:");
            scanf("%d",&leave_balance);   //get new leave balance
            for(j=0;j<i;j++){
                staffs[j].emergency_leave=leave_balance;    //addition of balance
                fprintf(fp,"%s %s %s %s %d %s %s %d %d %d %d\n",staffs[j].staff_id,staffs[j].password,staffs[j].name,
            staffs[j].gender,staffs[j].age,staffs[j].role,staffs[j].dept,staffs[j].annual_leave,
            staffs[j].compassionate_leave,staffs[j].emergency_leave,staffs[j].medical_leave);
            }
            break;
        case 4:
            printf("Modify leave balance for Medical leave:");
            scanf("%d",&leave_balance);   //get new leave balance
            for(j=0;j<i;j++){
                staffs[j].medical_leave=leave_balance;    //addition of balance
                fprintf(fp,"%s %s %s %s %d %s %s %d %d %d %d\n",staffs[j].staff_id,staffs[j].password,staffs[j].name,
            staffs[j].gender,staffs[j].age,staffs[j].role,staffs[j].dept,staffs[j].annual_leave,
            staffs[j].compassionate_leave,staffs[j].emergency_leave,staffs[j].medical_leave);
            }
            break;
        case 5:
            printf("Reset all leave balance to:\n");    //reset all leave balance
            printf("\tAnnual leave:15\n\tCompassionate leave:3\n\tEmergency leave:2\n\tMedical leave:7\n");
            for(j=0;j<i;j++){
                //reset the value
                staffs[j].annual_leave=15;
                staffs[j].compassionate_leave=3;
                staffs[j].emergency_leave=2;
                staffs[j].medical_leave=7;
                fprintf(fp,"%s %s %s %s %d %s %s %d %d %d %d\n",staffs[j].staff_id,staffs[j].password,staffs[j].name,
            staffs[j].gender,staffs[j].age,staffs[j].role,staffs[j].dept,staffs[j].annual_leave,
            staffs[j].compassionate_leave,staffs[j].emergency_leave,staffs[j].medical_leave);
            }
            break;
        case 6:
            admin_menu(&staffs[0]); //exit to menu
            break;
        default:
            printf("Invalid choice,please try again!\n");
            update_staff(&staffs[0]);
            break;
    }
    fclose(fp);
    printf("\nUpdated Successful!\n");
    admin_menu(&staffs[0]);
}


void show_profile(struct Staff *staffs){
    char search_staff[50];
    int found=0;
    int i=0;
    FILE *fp;
    printf("Enter staff name or staff ID:");
    scanf("%s",search_staff);
    fp=fopen("users.txt","r"); 
    if(fp==NULL){
        printf("Could not open the file.\n");
    }else{
        while (!feof(fp)){ //scan through the file
            fscanf(fp,"%s %s %s %s %d %s %s %d %d %d %d\n",staffs[i].staff_id,staffs[i].password,staffs[i].name,
            staffs[i].gender,&staffs[i].age,staffs[i].role,staffs[i].dept,
            &staffs[i].annual_leave,&staffs[i].compassionate_leave,&staffs[i].emergency_leave,&staffs[i].medical_leave);
            if(strcmp(staffs[i].staff_id,search_staff)==0 || strcmp(staffs[i].name,search_staff)==0){
                //if the staff id or name is found
                printf("\n========Staff Profile========\n");
                printf("Staff ID:%s\n", staffs[i].staff_id);
                printf("Name:%s\n", staffs[i].name);
                printf("Gender:%s\n", staffs[i].gender);
                printf("Age:%d\n", staffs[i].age);
                printf("Role:%s\n", staffs[i].role);
                printf("Department:%s\n", staffs[i].dept);
                printf("===Leave Balance===\n");
                printf("Annual Leave:%d\n", staffs[i].annual_leave);
                printf("Compassionate Leave:%d\n", staffs[i].compassionate_leave);
                printf("Emergency Leave:%d\n", staffs[i].emergency_leave);
                printf("Medical Leave:%d\n", staffs[i].medical_leave);
                found=1;
                break;  //exit the loop
            }
            i++;
        }
    }
    if(found==0){   //if the staff is not found
        printf("Staff not found!\n");
        admin_menu(&staffs[0]);
    }
    fclose(fp);
    admin_menu(&staffs[0]);
}

void check_leave(struct Staff *staffs){
    struct application leave[20]; //declaration
    int choice;
    FILE *fp;
    fp=fopen("leave_application.txt","r");
    printf("Number\tName\tDay\tType\tFrom\tTo\tStatus\tApplyDate\n");
    printf("=========================================================\n");
    int i=0;
    //print out all the leave application
    while(!feof(fp)){
        int n=fscanf(fp,"%s %d %s %s %s %s %s %s\n",leave[i].Name,&leave[i].Day,leave[i].Type,leave[i].From,leave[i].To,leave[i].Status,leave[i].Apply_date,leave[i].Reason);
        if (n!=8){
            printf("Error");
            break;
        }else{
            printf("%d\t%s\t%d\t%s\t%s\t%s\t%s\t%s\n",i+1,leave[i].Name,leave[i].Day,leave[i].Type,leave[i].From,leave[i].To,leave[i].Status,leave[i].Apply_date);
            i++;
        }
    }
    int j=i;
    fclose(fp);
    printf("=========================================================\n");
    printf("Choose number to make decision:");
    scanf("%d",&i);
    //show detail on selected leave application
    printf("Staff name     :%s\n",leave[i-1].Name);
    printf("Total day leave:%d\n",leave[i-1].Day);
    printf("Type of leave  :%s\n",leave[i-1].Type);
    printf("From           :%s\n",leave[i-1].From);
    printf("To             :%s\n",leave[i-1].To);
    printf("Status         :%s\n",leave[i-1].Status);
    printf("Apply date     :%s\n",leave[i-1].Apply_date);
    printf("Reason         :%s\n",leave[i-1].Reason);
    char respon[10];
    int token=0;
    if (strcmp(leave[i-1].Status, "Approved") == 0 || strcmp(leave[i-1].Status, "Rejected") == 0 || strcmp(leave[i-1].Status, "Cancelled") == 0) {
        token=2;
        printf("=========================================================\n");
        printf("Press anything to continue...");
        scanf("%s",respon);
        superior_menu(&staffs[0]);
    }
    //ask user approve or reject the leave application
    while(token==0){
        printf("\nApprove/Reject the application:");
        scanf("%s",respon);
        if (strcmp(respon, "Approve") == 0 || strcmp(respon, "approve") == 0) {   //if approve
            strcpy(leave[i-1].Status, "Approved");
            printf("Application Approved!\n");
            token=1;
            
            //update the leave balance to the file
            fp=fopen("users.txt","r");
            if (fp == NULL) {
                printf("Error opening file\n");
                return;
            }
            int k=0;
            while(!feof(fp)){
                fscanf(fp,"%s %s %s %s %d %s %s %d %d %d %d\n",staffs[k].staff_id,staffs[k].password,
                staffs[k].name,staffs[k].gender,&staffs[k].age,staffs[k].role,staffs[k].dept,
                &staffs[k].annual_leave,&staffs[k].compassionate_leave,&staffs[k].emergency_leave,&staffs[k].medical_leave);
                k++;
            }
            for(int m=0;m<k;m++){
                if (strcmp(staffs[m].name,leave[i-1].Name)==0){
                    if (strcmp(leave[i-1].Type,"annual_leave")==0){
                        staffs[m].annual_leave -= leave[i-1].Day;
                        staffs[i].annual_leave = staffs[m].annual_leave;
                    } else if (strcmp(leave[i-1].Type,"compassionate_leave")==0){
                        staffs[m].compassionate_leave -= leave[i-1].Day;
                        staffs[i].compassionate_leave = staffs[m].compassionate_leave;
                    } else if (strcmp(leave[i-1].Type,"emergency_leave")==0){
                        staffs[m].emergency_leave -= leave[i-1].Day;
                        staffs[i].emergency_leave = staffs[m].emergency_leave;
                    } else if (strcmp(leave[i-1].Type,"medical_leave")==0){
                        staffs[m].medical_leave -= leave[i-1].Day;
                        staffs[i].medical_leave = staffs[m].medical_leave;
                    } 
                }
            }
            fclose(fp);
            fp=fopen("users.txt","w");
            for (i=0;i<k;i++){
                fprintf(fp,"%s %s %s %s %d %s %s %d %d %d %d\n",staffs[i].staff_id,staffs[i].password,
                staffs[i].name,staffs[i].gender,staffs[i].age,staffs[i].role,staffs[i].dept,
                staffs[i].annual_leave,staffs[i].compassionate_leave,staffs[i].emergency_leave,staffs[i].medical_leave);
            }
            fclose(fp);
            
        }
        else if (strcmp(respon, "Reject") == 0 || strcmp(respon, "reject") == 0) {   //if reject
            strcpy(leave[i-1].Status, "Rejected");
            printf("Application Rejected!\n");
            token=1;
        }
        else {
            printf("Invalid response!\n");
            printf("Return To Menu...\n");
            superior_menu(&staffs[0]);
        }
    }
    if (token==1){
        fp=fopen("leave_application.txt","w");
        for(i=0;i<j;i++){
            fprintf(fp,"%s %d %s %s %s %s %s %s\n",leave[i].Name,leave[i].Day,leave[i].Type,leave[i].From,leave[i].To,leave[i].Status,leave[i].Apply_date,leave[i].Reason);
        }
        token=0;
        superior_menu(&staffs[0]);
    }
    fclose(fp);
    
}


void search_leave_by_date(struct Staff *staffs){
    struct application leave[20];
    char search_date[20];
    int found = 0;
    FILE *fp;
    
    printf("Enter date (dd/mm/yyyy): ");
    scanf("%s", search_date);
    
    fp=fopen("leave_application.txt","r");
    printf("Leave applications on %s:\n", search_date);
    printf("=========================================================\n");
    while(!feof(fp)){
        int i;
        int n=fscanf(fp,"%s %d %s %s %s %s %s %s\n",leave[i].Name,&leave[i].Day,leave[i].Type,leave[i].From,leave[i].To,leave[i].Status,leave[i].Apply_date,leave[i].Reason);
        if (n!=8){
            printf("Error");
            break;
        }else{        
            if (strcmp(leave[i].From, search_date) <= 0 && strcmp(leave[i].To, search_date) >= 0) {
                printf("Name: %s\n", leave[i].Name);
                printf("Day: %d\n", leave[i].Day);
                printf("Type: %s\n", leave[i].Type);
                printf("From: %s\n", leave[i].From);
                printf("To: %s\n", leave[i].To);
                printf("Status: %s\n", leave[i].Status);
                printf("Apply date: %s\n", leave[i].Apply_date);
                printf("Reason: %s\n", leave[i].Reason);
                printf("=========================================================\n");
                found = 1;
            }
        }
        i++;
    }
    do{
        int respon;
        printf("Press 1 to continue, 2 to end the program.\n");
        scanf("%d",&respon);
        switch (respon){
            case 1:
                found=2;
                search_leave_by_date(&staffs[0]);
                break;
            case 2:
                found=2;
                superior_menu(&staffs[0]);
                break;
            default:
                printf("Invalid respond, please try again!\n");
        }   
    }while(found=1);
    if (found=0) {
        printf("No leave applications found for %s.\n", search_date);
        search_leave_by_date(&staffs[0]);
    }
}
    
void generate_report() {
    // Open the input file
    FILE *fp = fopen("users.txt", "r");
    if (fp == NULL) {
        printf("Error opening file.\n");
        return;
    }
    // Read the users from the input file
    struct Staff staffs[100];
    int num_staffs = 0;
    while (fscanf(fp, "%s %s %s %s %d %s %s %d %d %d %d\n", staffs[num_staffs].staff_id, staffs[num_staffs].password,
                staffs[num_staffs].name, staffs[num_staffs].gender, &staffs[num_staffs].age, staffs[num_staffs].role,
                staffs[num_staffs].dept, &staffs[num_staffs].annual_leave, &staffs[num_staffs].compassionate_leave,
                &staffs[num_staffs].emergency_leave, &staffs[num_staffs].medical_leave) == 11) {
        num_staffs++;
    }
    int i=num_staffs;
    // Calculate the leave statistics for each department
    int academic_annual_leave=0, academic_compassionate_leave=0, academic_emergency_leave=0, academic_medical_leave=0;
    int administration_annual_leave=0, administration_compassionate_leave=0, administration_emergency_leave=0, administration_medical_leave=0;
    int management_annual_leave=0, management_compassionate_leave=0, management_emergency_leave=0, management_medical_leave=0;
    int technical_annual_leave=0, technical_compassionate_leave=0, technical_emergency_leave=0, technical_medical_leave=0;
    for (int num_staffs = 0; num_staffs<i ; num_staffs++) {
        if (strcmp(staffs[num_staffs].dept,"academic")==0){
            academic_annual_leave += staffs[num_staffs].annual_leave;
            academic_compassionate_leave += staffs[num_staffs].compassionate_leave;
            academic_emergency_leave += staffs[num_staffs].emergency_leave;
            academic_medical_leave += staffs[num_staffs].medical_leave;
            
        } else if (strcmp(staffs[num_staffs].dept,"administration")==0){
            administration_annual_leave += staffs[num_staffs].annual_leave;
            administration_compassionate_leave += staffs[num_staffs].compassionate_leave;
            administration_emergency_leave += staffs[num_staffs].emergency_leave;
            administration_medical_leave += staffs[num_staffs].medical_leave;
        }  else if (strcmp(staffs[num_staffs].dept,"management")==0){
            management_annual_leave += staffs[num_staffs].annual_leave;
            management_compassionate_leave += staffs[num_staffs].compassionate_leave;
            management_emergency_leave += staffs[num_staffs].emergency_leave;
            management_medical_leave += staffs[num_staffs].medical_leave;
        }  else if (strcmp(staffs[num_staffs].dept,"technical")==0){
            technical_annual_leave += staffs[num_staffs].annual_leave;
            technical_compassionate_leave += staffs[num_staffs].compassionate_leave;
            technical_emergency_leave += staffs[num_staffs].emergency_leave;
            technical_medical_leave += staffs[num_staffs].medical_leave;
        }  
    } 
    fclose(fp);
    
    // Create the output file
    FILE *report_fp = fopen("monthly_report.txt", "w");
    if (report_fp == NULL) {
        printf("Error creating report file.\n");
        fclose(report_fp);
        return;
    }
    // Write the report to the output file
    fprintf(report_fp, "Monthly Report - Leave Statistics\n");
    fprintf(report_fp,"=====Leave balance allocated=====\n");
    fprintf(report_fp, "Department: academic\n");
    fprintf(report_fp, "Annual Leave: %d\n", academic_annual_leave);
    fprintf(report_fp, "Compassionate Leave: %d\n", academic_compassionate_leave);
    fprintf(report_fp, "Emergency Leave: %d\n", academic_emergency_leave);
    fprintf(report_fp, "Medical Leave: %d\n", academic_medical_leave);
        
    fprintf(report_fp, "\nDepartment: administration\n");
    fprintf(report_fp, "Annual Leave: %d\n", administration_annual_leave);
    fprintf(report_fp, "Compassionate Leave: %d\n", administration_compassionate_leave);
    fprintf(report_fp, "Emergency Leave: %d\n", administration_emergency_leave);
    fprintf(report_fp, "Medical Leave: %d\n", administration_medical_leave);
    
    fprintf(report_fp, "\nDepartment: management\n");
    fprintf(report_fp, "Annual Leave: %d\n", management_annual_leave);
    fprintf(report_fp, "Compassionate Leave: %d\n", management_compassionate_leave);
    fprintf(report_fp, "Emergency Leave: %d\n", management_emergency_leave);
    fprintf(report_fp, "Medical Leave: %d\n", management_medical_leave);
    
    fprintf(report_fp, "\nDepartment: technical\n");
    fprintf(report_fp, "Annual Leave: %d\n", technical_annual_leave);
    fprintf(report_fp, "Compassionate Leave: %d\n", technical_compassionate_leave);
    fprintf(report_fp, "Emergency Leave: %d\n", technical_emergency_leave);
    fprintf(report_fp, "Medical Leave: %d\n", technical_medical_leave);
    fclose(report_fp);
    
    printf("Monthy report generated!\n");
}


