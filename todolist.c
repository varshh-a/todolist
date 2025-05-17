#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define FILENAME "todo_list.dat"

struct Task {
    int id;
    char description[100];
    int completed;
    char date[11]; // Format: YYYY-MM-DD
};

void addTask();
void viewTasks();
void markTaskComplete();
void deleteTask();
void viewTasksByDate();

int main() {
    int choice;
    while (1) {
        printf("\nTo-Do List with Calendar Scheduling\n");
        printf("1. Add Task\n");
        printf("2. View Tasks\n");
        printf("3. Mark Task as Complete\n");
        printf("4. Delete Task\n");
        printf("6. View Tasks by Date\n");
        printf("7. Exit\n");
        printf("Choose an option: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                addTask();
                break;
            case 2:
                viewTasks();
                break;
            case 3:
                markTaskComplete();
                break;
            case 4:
                deleteTask();
                break;
            
            case 6:
                viewTasksByDate();
                break;
            case 7:
                printf("Exiting application...\n");
                return 0;
            default:
                printf("Invalid choice. Try again.\n");
        }
    }
}

void addTask() {
    struct Task newTask;
    FILE *file = fopen(FILENAME, "ab");
    if (!file) {
        printf("Error opening file!\n");
        return;
    }

    printf("Enter Task ID: ");
    scanf("%d", &newTask.id);
    getchar();
    printf("Enter Task Description: ");
    fgets(newTask.description, 100, stdin);
    strtok(newTask.description, "\n");
    printf("Enter Due Date (YYYY-MM-DD): ");
    scanf("%s", newTask.date);
    newTask.completed = 0;

    fwrite(&newTask, sizeof(struct Task), 1, file);
    fclose(file);
    printf("Task added successfully!\n");
}

void viewTasks() {
    struct Task task;
    FILE *file = fopen(FILENAME, "rb");
    if (!file) {
        printf("No tasks found!\n");
        return;
    }

    printf("\n%-10s %-50s %-15s %-10s\n", "Task ID", "Description", "Due Date", "Completed");
    while (fread(&task, sizeof(struct Task), 1, file)) {
        printf("%-10d %-50s %-15s %-10s\n", task.id, task.description, task.date, task.completed ? "Yes" : "No");
    }
    fclose(file);
}

void markTaskComplete() {
    int id;
    struct Task task;
    FILE *file = fopen(FILENAME, "rb+"), *tempFile = fopen("temp.dat", "wb");
    if (!file || !tempFile) {
        printf("Error updating task!\n");
        return;
    }

    printf("Enter Task ID to mark as complete: ");
    scanf("%d", &id);
    while (fread(&task, sizeof(struct Task), 1, file)) {
        if (task.id == id) {
            task.completed = 1;
            printf("Task marked as complete!\n");
        }
        fwrite(&task, sizeof(struct Task), 1, tempFile);
    }
    fclose(file);
    fclose(tempFile);
    remove(FILENAME);
    rename("temp.dat", FILENAME);
}

void deleteTask() {
    int id;
    struct Task task;
    FILE *file = fopen(FILENAME, "rb"), *tempFile = fopen("temp.dat", "wb");
    if (!file || !tempFile) {
        printf("Error deleting task!\n");
        return;
    }

    printf("Enter Task ID to delete: ");
    scanf("%d", &id);
    while (fread(&task, sizeof(struct Task), 1, file)) {
        if (task.id != id) {
            fwrite(&task, sizeof(struct Task), 1, tempFile);
        }
    }
    fclose(file);
    fclose(tempFile);
    remove(FILENAME);
    rename("temp.dat", FILENAME);
    printf("Task deleted successfully!\n");
}

void viewTasksByDate() {
    char date[11];
    struct Task task;
    FILE *file = fopen(FILENAME, "rb");
    if (!file) {
        printf("No tasks found!\n");
        return;
    }

    printf("Enter Date (YYYY-MM-DD) to view tasks: ");
    scanf("%s", date);
    printf("\n%-10s %-50s %-15s %-10s\n", "Task ID", "Description", "Due Date", "Completed");
    while (fread(&task, sizeof(struct Task), 1, file)) {
        if (strcmp(task.date, date) == 0) {
            printf("%-10d %-50s %-15s %-10s\n", task.id, task.description, task.date, task.completed ? "Yes" : "No");
        }
    }
    fclose(file);
}

