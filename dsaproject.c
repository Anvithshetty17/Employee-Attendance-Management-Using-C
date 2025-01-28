#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


typedef struct Employee {
    int id;
    char name[50];
    struct Employee* next;
} Employee;


Employee* head = NULL;


void loadEmployees() {
    FILE* file = fopen("employees.csv", "r");
    if (!file) return;

    while (!feof(file)) {
        Employee* newEmployee = (Employee*)malloc(sizeof(Employee));
        if (fscanf(file, "%d,%49[^\n]\n", &newEmployee->id, newEmployee->name) == 2) {
            newEmployee->next = head;
            head = newEmployee;
        }
    }
    fclose(file);
}


void saveEmployees() {
    FILE* file = fopen("employees.csv", "w");
    if (!file) {
        printf("Could not open employees.csv for writing.\n");
        return;
    }

    Employee* current = head;
    while (current != NULL) {
        fprintf(file, "%d,%s\n", current->id, current->name);
        current = current->next;
    }
    fclose(file);
}


void addEmployee(int id, const char* name) {
    Employee* newEmployee = (Employee*)malloc(sizeof(Employee));
    newEmployee->id = id;
    strcpy(newEmployee->name, name);
    newEmployee->next = head;
    head = newEmployee;
    saveEmployees();
    printf("\n[Success] Employee added successfully!\n");
}

int employeeExists(int id) {
    Employee* current = head;
    while (current != NULL) {
        if (current->id == id) {
            return 1;
        }
        current = current->next;
    }
    return 0;
}

void updateEmployee(int id, const char* newName) {
    Employee* current = head;
    while (current != NULL) {
        if (current->id == id) {
            strcpy(current->name, newName);
            saveEmployees();
            printf("\n[Success] Employee details updated successfully!\n");
            return;
        }
        current = current->next;
    }
    printf("\n[Error] Employee with ID %d not found.\n", id);
}


void deleteEmployee(int id) {
    Employee* current = head;
    Employee* prev = NULL;

    while (current != NULL) {
        if (current->id == id) {
            if (prev == NULL) {
                head = current->next;
            } else {
                prev->next = current->next;
            }
            free(current);
            saveEmployees();
            printf("\n[Success] Employee with ID %d deleted successfully!\n", id);
            return;
        }
        prev = current;
        current = current->next;
    }
    printf("\n[Error] Employee with ID %d not found.\n", id);
}


void markAttendance(int id) {
    Employee* current = head;


    while (current != NULL) {
        if (current->id == id) {
            time_t now = time(NULL);
            struct tm* t = localtime(&now);

            if (t == NULL) {
                printf("Error: Unable to fetch local time.\n");
                return;
            }


            char filename[30];
            if (strftime(filename, sizeof(filename), "attendance_%Y-%m-%d.csv", t) == 0) {
                printf("Error: Filename generation failed.\n");
                return;
            }


            FILE* file = fopen(filename, "r");
            if (file) {
                char line[100];
                char date[11];
                strftime(date, sizeof(date), "%Y-%m-%d", t);

                while (fgets(line, sizeof(line), file)) {
                    int recordedId;
                    char recordedName[50], recordedDate[11], status[10];


                    if (sscanf(line, "%d,%49[^,],%10[^,],%9s", &recordedId, recordedName, recordedDate, status) == 4) {
                        if (recordedId == id && strcmp(recordedDate, date) == 0) {
                            printf("\n[Info] Attendance already marked for %s (ID: %d) today.\n", current->name, current->id);
                            fclose(file);
                            return;
                        }
                    }
                }
                fclose(file);
            }


            file = fopen(filename, "a");
            if (!file) {
                printf("Could not open %s for writing.\n", filename);
                perror("Error");
                return;
            }


            char date[11];
            strftime(date, sizeof(date), "%Y-%m-%d", t);


            fprintf(file, "%d,%s,%s,Present\n", current->id, current->name, date);
            fclose(file);

            printf("\n[Success] Attendance marked for %s (ID: %d) as Present.\n", current->name, current->id);
            return;
        }
        current = current->next;
    }

    printf("\n[Error] Employee with ID %d not found.\n", id);
}


void viewAttendance() {
    time_t now = time(NULL);
    struct tm* t = localtime(&now);

    if (t == NULL) {
        printf("Error: Unable to fetch local time.\n");
        return;
    }


    char filename[30];
    if (strftime(filename, sizeof(filename), "attendance_%Y-%m-%d.csv", t) == 0) {
        printf("Error: Filename generation failed.\n");
        return;
    }

    // Open the attendance file
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("\n[Info] No attendance records found for today (%s).\n", filename);
        return;
    }

    // Display the attendance records with proper formatting
    char line[100];
    printf("\n\t\tToday's Attendance Records:\n");
    printf("----------------------------------------------------------\n");
    printf("| %-5s | %-20s | %-10s | %-10s |\n", "ID", "Name", "Date", "Status");
    printf("----------------------------------------------------------\n");
    while (fgets(line, sizeof(line), file)) {
        int id;
        char name[50], date[11], status[10];


        if (sscanf(line, "%d,%49[^,],%10[^,],%9s", &id, name, date, status) == 4) {
            printf("| %-5d | %-20s | %-10s | %-10s |\n", id, name, date, status);
        }
    }
    printf("----------------------------------------------------------\n");
    fclose(file);
}

// Admin Dashboard
void adminDashboard() {
    int choice, id;
    char name[50];

    do {
        printf("\n\n==========================================================\n");
        printf("|\t\t   Admin Dashboard \t\t\t |\n");
        printf("==========================================================\n");
        printf("\n\t\t1. Add Employee\n");
        printf("\t\t2. View Employees\n");
        printf("\t\t3. Update Employee\n");
        printf("\t\t4. Delete Employee\n");
        printf("\t\t5. Logout\n\n");
        printf("==========================================================\n");
        printf("\tEnter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("\nEnter Employee ID: ");
                scanf("%d", &id);
                if (employeeExists(id)) {
                    printf("\n[Error] Employee ID %d already exists. Please use a different ID.\n", id);
                } else {
                    printf("Enter Employee Name: ");
                    scanf(" %[^\n]", name);
                    addEmployee(id, name);
                }
                break;

            case 2: {
                Employee* current = head;
                printf("\n\tEmployee List:\n");
                printf("------------------------------------------\n");
                printf("| %-5s | %-30s |\n", "ID", "Name");
                printf("------------------------------------------\n");
                while (current != NULL) {
                    printf("| %-5d | %-30s |\n", current->id, current->name);
                    current = current->next;
                }
                printf("------------------------------------------\n");
                break;
            }

            case 3:
                printf("\nEnter Employee ID to Update: ");
                scanf("%d", &id);
                if (employeeExists(id)) {
                    printf("Enter New Employee Name: ");
                    scanf(" %[^\n]", name);
                    updateEmployee(id, name);
                } else {
                    printf("\n[Error] Employee with ID %d not found.\n", id);
                }
                break;

            case 4:
                printf("\nEnter Employee ID to Delete: ");
                scanf("%d", &id);
                deleteEmployee(id);
                break;

            case 5:
                printf("\nLogging out...\n");
                return;

            default:
                printf("\n[Error] Invalid choice! Please try again.\n");
        }
    } while (choice != 5);
}

// User Dashboard
void userDashboard() {
    int choice, id;

    do {
        printf("\n\n==========================================================\n");
        printf("|\t\t    User Dashboard  \t\t\t |\n");
        printf("==========================================================\n");
        printf("\n\t\t1. Mark Attendance\n");
        printf("\t\t2. View Today's Attendance\n");
        printf("\t\t3. Exit\n");
        printf("\n==========================================================\n");
        printf("\n\tEnter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("\n\tEnter Your Employee ID: ");
                scanf("%d", &id);
                markAttendance(id);
                break;

            case 2:
                viewAttendance();
                break;

            case 3:
                printf("\nExiting...\n");
                return;

            default:
                printf("\n[Error] Invalid choice! Please try again.\n");
        }
    } while (choice != 3);
}

// Main Function
int main() {
    char username[20], password[20];
    const char* validUsername = "admin";
    const char* validPassword = "password";

    loadEmployees();

    printf("\n=============== Employee Attendance Tracker ===============\n");
    printf("\n\t\t1. User Dashboard\n");
    printf("\n\t\t2. Admin Login\n");
    printf("\n===========================================================\n");
    printf("\n\tEnter your choice: ");
    int choice;
    scanf("%d", &choice);

    if (choice == 2) {
        printf("\n\t****** Login ******\n");
        printf("\n\tEnter username: ");
        scanf("%s", username);
        printf("\tEnter password: ");
        scanf("%s", password);

        if (strcmp(username, validUsername) == 0 && strcmp(password, validPassword) == 0) {
            adminDashboard();
        } else {
            printf("\n[Error] Invalid credentials!\n");
        }
    } else {
        userDashboard();
    }

    return 0;
}
