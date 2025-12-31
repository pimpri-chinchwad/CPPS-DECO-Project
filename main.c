#include <stdio.h>
#include <string.h>
#include <time.h>

/* ---------- Structures ---------- */

struct student {
    char name[20];
    char PRN[20];
    char password[20];
    int n;
    int array[20];

    int borrow_day;
    int borrow_month;
    int borrow_year;
};

struct book {
    char name[30];
    int id;
};

/* ---------- Global Databases ---------- */

struct student student_array[20];
int total_students = -1;

// variables for login
int logged_in_as = -2;



struct book books[20];
int total_books = -1;

/* ---------- Utility ---------- */

void clear() {
    printf("\e[1;1H\e[2J");
}

/* ---------- Get Current Date ---------- */

void get_current_date(int *d, int *m, int *y) {
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);

    *d = tm->tm_mday;
    *m = tm->tm_mon + 1;
    *y = tm->tm_year + 1900;
}

/* ---------- Student Signup ---------- */

void student_signup() {
    total_students++;

    printf("Enter name: ");
    scanf("%s", student_array[total_students].name);

    printf("Enter PRN: ");
    scanf("%s", student_array[total_students].PRN);

    printf("Create password: ");
    scanf("%s", student_array[total_students].password);

    student_array[total_students].n = 0;
	
	clear();
    printf("Signup successful!\n");
}

/* ---------- Student Login ---------- */

void student_login() {
    char prn[20], pass[20];

    printf("Enter PRN: ");
    scanf("%s", prn);

    printf("Enter password: ");
    scanf("%s", pass);
	
	clear();
    for (int i = 0; i <= total_students; i++) {
        if (strcmp(student_array[i].PRN, prn) == 0 && strcmp(student_array[i].password, pass) == 0) {
        	logged_in_as=i;
        	return;
        }
    }
    printf("Invalid login!\n");

}



/* ---------- Borrow Books ---------- */

void borrow_books(int index) {
    printf("Enter number of books to borrow: ");
    scanf("%d", &student_array[index].n);
	clear();
    printf("You wish to borrow %d books.\n", student_array[index].n);
    printf("Please enter the book Id of the books you want to borrow\n");
    
    for (int i = 0; i < student_array[index].n; i++) {
    	printf("Enter %d book id: ", i+1);
        scanf("%d", &student_array[index].array[i]);
    }

    get_current_date(&student_array[index].borrow_day,
                     &student_array[index].borrow_month,
                     &student_array[index].borrow_year);
	
	clear();
    printf("%d books borrowed successfully!\n\n", student_array[index].n);
}

/* ---------- Check Due Date ---------- */

void check_due_date(int index) {
    if (student_array[index].n == 0) {
        printf("Cannot show due date, no books borrowed.\n");
        return;
    }

    int today_d, today_m, today_y;
    get_current_date(&today_d, &today_m, &today_y);

    int due_day = student_array[index].borrow_day + 7;
    int days_left = due_day - today_d;

    if (days_left < 0)
        printf("⚠️ OVERDUE\n");
    else if (days_left <= 2)
        printf("⚠️ Due date very close (%d days left)\n", days_left);
    else
        printf("✅ %d days left\n", days_left);
}

/* ---------- Display Borrowed Books ---------- */

void display_borrowed(int index) {
    if (student_array[index].n == 0) {
        printf("No books borrowed.\n");
        return;
    }

    printf("Books borrowed:\n");
    for (int i = 0; i < student_array[index].n; i++) {
        printf("%d\n", student_array[index].array[i]);
    }
}

/* ---------- Add Book ---------- */

void add_book() {
    total_books++;

    printf("Enter book ID: ");
    scanf("%d", &books[total_books].id);

    printf("Enter book name: ");
    scanf(" %[^\n]", books[total_books].name);

    printf("Book added successfully!\n");
}

/* ---------- Display Books ---------- */

void display_books() {
    if (total_books == -1) {
        printf("No books available.\n");
        return;
    }
	else {
		printf("Books in library:\n");
		for (int i = 0; i <= total_books; i++) {
		    printf("%d - %s\n", books[i].id, books[i].name);
		}
	}
}

/* ---------- Display All Student Records (LIBRARIAN) ---------- */

void display_all_students() {
    if (total_students == -1) {
        printf("No students registered.\n");
        return;
    }

    printf("\n--- Student Records ---\n");

    for (int i = 0; i <= total_students; i++) {
        printf("\nName: %s\n", student_array[i].name);
        printf("PRN: %s\n", student_array[i].PRN);
        printf("Books Borrowed: %d\n", student_array[i].n);

        if (student_array[i].n > 0) {
            printf("Book IDs: ");
            for (int j = 0; j < student_array[i].n; j++) {
                printf("%d ", student_array[i].array[j]);
            }
            printf("\nBorrow Date: %02d-%02d-%d\n",
                   student_array[i].borrow_day,
                   student_array[i].borrow_month,
                   student_array[i].borrow_year);
        }
    }
}

/* ---------- Librarian Login ---------- */

void librarian_login() {
    char user[20], pass[20];

    printf("Enter librarian username: ");
    scanf("%s", user);

    printf("Enter librarian password: ");
    scanf("%s", pass);
	
	clear();	
    if (strcmp(user, "admin") == 0 && strcmp(pass, "admin123") == 0) {
    	printf("Logged in as Librarian\n");
        logged_in_as=-1;
        return;

    }
    else {
        printf("Invalid librarian credentials!\n");
    }
}

/* ---------- Main ---------- */

int main() {
    int ch, index;
	clear();




    while (1) {

    
    
    
    
    
    	//user is currently logged out
    	if (logged_in_as==-2){
    		//give choices
    		printf("1.Login as Student\n");
    		printf("2. Sign up as student\n");
			printf("3. Login as Librarian\n");
			printf("Press Ctrl+C to exit\n");
			//input choice
			printf("\nEnter choice: ");
            scanf(" %d", &ch);
            clear();
            
            
            //perform choice
            switch(ch){
            case 1: //user wants to login as student
				student_login();
            	break;
            case 3:
            	//user wants to login as librarian
            	librarian_login();
            	break;
            case 2:
            	//user wants to signup as student
            	student_signup();
            	break;
            default://user did not choose a valid choice
            	printf("Invalid choice.\n");
    		}
    	}
    	
    	
    	
    	
    	
  	
    	
    	
    	//user is currently logged in as librarian
    	else if(logged_in_as==-1){
    		//give choices
    		printf("1.Add book\n");
			printf("2. View books\n");
			printf("3. Display all students\n");
			printf("Press Ctrl+C to exit\n");
			
			
			//input choice
			printf("\nEnter choice: ");
            scanf(" %d", &ch);
            clear();
           
            //perform choice
            switch(ch){
            case 1://librarian wants to add book
            	add_book();
            	break;   
            case 2://librarian wants to view books
            	display_books();
            	break;
            case 3://librarian wants to see all students
            	display_all_students();
            default://user did not choose a valid choice
            	printf("Invalid choice.\n");
    		}
    	
    	}
    	
    	
    	
    	
    	
    	
    	
    	
    	
    	
    	//user is logged in as any one of the students
    	else {
    		//give choices
    		printf("-->Logged in as %s  %s\n", student_array[logged_in_as].name, student_array[logged_in_as].PRN);
    		printf("1.View borrowed\n");
			printf("2. Borrow books\n");
			printf("3. Check due date\n");
			printf("4. Logout\n");
			printf("Press Ctrl+C to exit\n");
			
			//input choice
			printf("\nEnter choice: ");
            scanf(" %d", &ch);
            clear();
            
            
            //perform choice
            switch(ch){
            case 1://student wants to view borrowed books
            	display_borrowed(logged_in_as);
            	break;   
            case 2://student wants to borrow books
            	printf("->Borrowing books \n");
            	borrow_books(logged_in_as);
            	break;
            case 3://student wants to check due date
            	check_due_date(logged_in_as);
            	break;
            case 4://student wants to logout
            	logged_in_as=-2;
            	break;
            default://user did not choose a valid choice
            	printf("Invalid choice.\n");
    		}
			
    	
    	}
    	
    	
    	
    	
    	
    	
    	
    	
    	
    	
    	
    	
    	
    	
    	
    }
    	
    	
    	
    	
}

        
