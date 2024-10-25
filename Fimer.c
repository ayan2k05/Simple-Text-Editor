#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINES 1000
#define MAX_LINE_LENGTH 256


/*--------------------------------------------------------------------------------------------------------------------*/


void clear_console()
/*Function to clear the console*/
{
#ifdef _WIN32
    system("cls"); /*windows*/
#else
    system("clear"); /*linux*/
#endif
}


/*--------------------------------------------------------------------------------------------------------------------*/


void display_file(const char *filename)
/*Function to display content of the file*/
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
    perror("Could not open file");
    return;
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file))
    {
    printf("%s", line);
    }
    fclose(file);
}


/*--------------------------------------------------------------------------------------------------------------------*/


void write_file(const char *filename, char lines[MAX_LINES][MAX_LINE_LENGTH], int line_count)
/*Function to write content to file*/
{
    FILE *file = fopen(filename, "w");
    if (!file)
    {
    perror("Could not open file for writing");
    return;
    }

    for (int i = 0; i < line_count; i++)
    {
fprintf(file, "%s\n", lines[i]);
    }
    fclose(file);
}


/*--------------------------------------------------------------------------------------------------------------------*/


void edit_file(const char *filename)
/*Edit the file contenr*/
{
    char lines[MAX_LINES][MAX_LINE_LENGTH];
    int line_count = 0;

/*Opening and reading existing content*/
    FILE *file = fopen(filename, "r");
    if (file)
    {
        while (fgets(lines[line_count], sizeof(lines[line_count]), file))
        {
        lines[line_count][strcspn(lines[line_count], "\n")] = 0;
        line_count++;
        }
        fclose(file);
    }
    else
    {
    perror("Could not read file. Starting with an empty file.");
    }

    display_file(filename);/*Display existing content*/
    printf("\n--- Start editing (type 'remove line <n>' to delete, 'edit line <n>' to edit, 'search <text>' to search, 'clear' to clear screen, 'exit' to save and quit) ---\n");
    while (line_count < MAX_LINES)
    {
        printf("%d : ", line_count + 1);
        char input[MAX_LINE_LENGTH];
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = 0;
        if (strcmp(input, "exit") == 0)
        {
        break;
        }
        else if (strncmp(input, "remove line ", 12) == 0)
        {
            int line_to_remove = atoi(input + 12);
            if (line_to_remove > 0 && line_to_remove <= line_count)
            {
                for (int i = line_to_remove - 1; i < line_count - 1; i++)
                {
                strcpy(lines[i], lines[i + 1]);
                }
                line_count--;
                printf("Line %d removed.\n", line_to_remove);
            }
            else
            {
            printf("Invalid line number.\n");
            }
        }
        else if (strncmp(input, "edit line ", 10) == 0)
        {
            int line_to_edit = atoi(input + 10);
            if (line_to_edit > 0 && line_to_edit <= line_count)
            {
            printf("Current content: %s\n", lines[line_to_edit - 1]);
            printf("Enter new content: ");
            fgets(lines[line_to_edit - 1], sizeof(lines[line_to_edit - 1]), stdin);
            lines[line_to_edit - 1][strcspn(lines[line_to_edit - 1], "\n")] = 0;
            printf("Line %d updated.\n", line_to_edit);
            }
            else
            {
            printf("Invalid line number.\n");
            }
        }
        else if (strncmp(input, "search ", 7) == 0) {
            const char *search_term = input + 7;
            int found = 0;
            printf("Searching for: '%s'\n", search_term);
            for (int i = 0; i < line_count; i++)
            {
                if (strstr(lines[i], search_term))
                {
                printf("Found in line %d: %s\n", i + 1, lines[i]);
                found = 1;
                }
            }
            if (!found)
            {
            printf("No occurrences found.\n");
            }
        }
        else if (strcmp(input, "clear") == 0)
        {
        clear_console();
        display_file(filename);
        }
        else
        {
            if (line_count < MAX_LINES)
            {
            strcpy(lines[line_count], input);
            line_count++;
            }
            else
            {
            printf("Maximum line count reached! cannot add more lines.\n");
            }
        }
    }
    write_file(filename, lines, line_count);/*writng new content to the file*/
}


/*--------------------------------------------------------------------------------------------------------------------*/


int main(int argc, char *argv[])
{
    if (argc != 2)
    {
    fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
    return 1;
    }

    const char *filename = argv[1];
    edit_file(filename);
    return 0;
}
