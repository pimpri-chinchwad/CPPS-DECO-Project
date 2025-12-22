#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h> // Added for system() commands if needed

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
struct book books[20];
int total_books = -1;

/* ---------- Utility ---------- */
void clear() {
    printf("\e[1;1H\e[2J");
}

void get_current_date(int *d, int *m, int *y) {
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    *d = tm->tm_mday;
    *m = tm->tm_mon + 1;
    *y = tm->tm_year + 1900;
}

/* ---------- File Handling Functions ---------- */
void save_data() {
    FILE *fp; 
    int i, j;

    // 1. Save Books
    fp = fopen("books.csv", "w");
    if (fp != NULL) {
        for (i = 0; i <= total_books; i++) {
            fprintf(fp, "%d,%s\n", books[i].id, books[i].name);
        }
        fclose(fp);
    }

    // 2. Save Students
    fp = fopen("students.csv", "w");
    if (fp != NULL) {
        for (i = 0; i <= total_students; i++) {
            fprintf(fp, "%s,%s,%s,%d", 
                    student_array[i].name, 
                    student_array[i].PRN, 
                    student_array[i].password, 
                    student_array[i].n);
            
            for (j = 0; j < student_array[i].n; j++) {
                fprintf(fp, ",%d", student_array[i].array[j]);
            }

            fprintf(fp, ",%d,%d,%d\n", 
                    student_array[i].borrow_day, 
                    student_array[i].borrow_month, 
                    student_array[i].borrow_year);
        }
        fclose(fp);
    }
}

void load_data() {
    FILE *fp; 
    int result, j; 

    // 1. Load Books
    fp = fopen("books.csv", "r");
    if (fp != NULL) {
        total_books = -1; 
        while (1) {
            total_books++;
            result = fscanf(fp, "%d,%[^,\n]", 
                                &books[total_books].id, 
                                books[total_books].name);
            if (result != 2) { 
                total_books--; 
                break; 
            }
        }
        fclose(fp);
    }

    // 2. Load Students
    fp = fopen("students.csv", "r");
    if (fp != NULL) {
        total_students = -1; 
        while (1) {
            total_students++;
            result = fscanf(fp, "%[^,],%[^,],%[^,],%d", 
                                student_array[total_students].name,
                                student_array[total_students].PRN,
                                student_array[total_students].password,
                                &student_array[total_students].n);

            if (result != 4) {
                total_students--; 
                break;
            }

            for (j = 0; j < student_array[total_students].n; j++) {
                fscanf(fp, ",%d", &student_array[total_students].array[j]);
            }

            fscanf(fp, ",%d,%d,%d\n", 
                   &student_array[total_students].borrow_day,
                   &student_array[total_students].borrow_month,
                   &student_array[total_students].borrow_year);
        }
        fclose(fp);
    }
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
    
    save_data(); //Save everytime!!
    printf("Signup successful!\n");
}

/* ---------- Student Login ---------- */
int student_login() {
    char prn[20], pass[20];
    int i;
    printf("Enter PRN: ");
    scanf("%s", prn);
    printf("Enter password: ");
    scanf("%s", pass);
    for (i = 0; i <= total_students; i++) {
        if (strcmp(student_array[i].PRN, prn) == 0 &&
            strcmp(student_array[i].password, pass) == 0) {
            return i;
        }
    }
    return -1;
}

/* ---------- Borrow Books ---------- */
void borrow_books(int index) {
    int i;
    printf("Enter number of books to borrow: ");
    scanf("%d", &student_array[index].n);
    printf("Enter book IDs:\n");
    for (i = 0; i < student_array[index].n; i++) {
        scanf("%d", &student_array[index].array[i]);
    }
    get_current_date(&student_array[index].borrow_day,
                     &student_array[index].borrow_month,
                     &student_array[index].borrow_year);

    save_data(); // Save everytime!
    printf("Books borrowed successfully!\n");
}

/* ---------- Check Due Date ---------- */
void check_due_date(int index) {
    if (student_array[index].n == 0) {
        printf("No books borrowed.\n");
        return;
    }
    int today_d, today_m, today_y;
    get_current_date(&today_d, &today_m, &today_y);
    // Simple logic: assume due within same month for demo
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
    int i;
    if (student_array[index].n == 0) {
        printf("No books borrowed.\n");
        return;
    }
    printf("Books borrowed:\n");
    for (i = 0; i < student_array[index].n; i++) {
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
    
    save_data(); // Save everytime
    printf("Book added successfully!\n");
}

/* ---------- Display Books ---------- */
void books_present() {
    int i;
    if (total_books == -1) {
        printf("No books available.\n");
        return;
    }
    printf("Books in library:\n");
    for (i = 0; i <= total_books; i++) {
        printf("%d - %s\n", books[i].id, books[i].name);
    }
}

/* ---------- Display All Student Records (LIBRARIAN) ---------- */
void display_all_students() {
    int i, j;
    if (total_students == -1) {
        printf("No students registered.\n");
        return;
    }
    printf("\n--- Student Records ---\n");
    for (i = 0; i <= total_students; i++) {
        printf("\nName: %s\n", student_array[i].name);
        printf("PRN: %s\n", student_array[i].PRN);
        printf("Books Borrowed: %d\n", student_array[i].n);
        if (student_array[i].n > 0) {
            printf("Book IDs: ");
            for (j = 0; j < student_array[i].n; j++) {
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
    if (strcmp(user, "admin") == 0 && strcmp(pass, "admin123") == 0) {
        int ch;
        while (1) {
            printf("\n--- Librarian Menu ---\n");
            printf("1. Add Book\n");
            printf("2. View Books\n");
            printf("3. View Student Records\n");
            printf("4. Logout\n");
            printf("Enter choice: ");
            scanf("%d", &ch);
            if (ch == 1)
                add_book();
            else if (ch == 2)
                books_present();
            else if (ch == 3)
                display_all_students();
            else if (ch == 4) {
                printf("Librarian logged out.\n");
                return;
            } else
                printf("Invalid choice.\n");
        }
    } else {
        printf("Invalid librarian credentials!\n");
    }
}

/* ---------- Main ---------- */
int main() {
    load_data(); // Load data from CSV 
    
    int choice, index;
    while (1) {
        printf("\n1. Student Signup\n");
        printf("2. Student Login\n");
        printf("3. Librarian Login\n");
        printf("4. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        clear();
        switch (choice) {
        case 1:
            student_signup();
            break;
        case 2:
            index = student_login();
            if (index == -1) {
                printf("Invalid login!\n");
            } else {
                int ch;
                printf("\n1. Borrow Books\n");
                printf("2. View Borrowed Books\n");
                printf("3. Check Due Date\n");
                printf("Enter choice: ");
                scanf("%d", &ch);
                if (ch == 1)
                    borrow_books(index);
                else if (ch == 2)
                    display_borrowed(index);
                else if (ch == 3)
                    check_due_date(index);
            }
            break;
        case 3:
            librarian_login();
            break;
        case 4:
            printf("Program exited successfully.\n");
            return 0;
        default:
            printf("Invalid choice.\n");
        }
    }
}

        

