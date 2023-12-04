#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_USERS 100

// ANSI color codes
#define ANSI_COLOR_RESET "\x1b[0m"
#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_LIGHT_BLUE "\e[1;34m"
#define ANSI_COLOR_YELLOW "\e[0;33m"

typedef struct User {
    int id;
    char name[50];
    struct User* friends[MAX_USERS];
    int numFriends;
} User;

User* createUser(int id, const char* name) {
    User* user = (User*)malloc(sizeof(User));
    if (user == NULL) {
        printf("%sError: Memory allocation failed. Cannot create the user.%s\n", ANSI_COLOR_RED, ANSI_COLOR_RESET);
        exit(1);
    }
    user->id = id;
    strcpy(user->name, name);
    user->numFriends = 0;
    return user;
}

void addFriend(User* user1, User* user2) {
    if (user1->numFriends >= MAX_USERS || user2->numFriends >= MAX_USERS) {
        printf("%sError: Friend limit reached for one or both users.%s\n", ANSI_COLOR_RED, ANSI_COLOR_RESET);
        return;
    }

    for (int i = 0; i < user1->numFriends; i++) {
        if (user1->friends[i] == user2) {
            printf("%sError: User %s is already a friend of User %s.%s\n", ANSI_COLOR_RED, user2->name, user1->name, ANSI_COLOR_RESET);
            return;
        }
    }

    user1->friends[user1->numFriends++] = user2;
    user2->friends[user2->numFriends++] = user1;
    printf("%sFriendship established successfully between %s and %s.%s\n", ANSI_COLOR_GREEN, user1->name, user2->name, ANSI_COLOR_RESET);
}

bool isFriend(User* user1, User* user2) {
    for (int i = 0; i < user1->numFriends; i++) {
        if (user1->friends[i] == user2) {
            return true;
        }
    }
    return false;
}

void recommendFriends(User* user) {
    printf("\n+---------------------------------------+\n");
    printf("| Mutual Friend Recommendations for %s (User %d) |\n", user->name, user->id);
    printf("+---------------------------------------+\n");
    for (int i = 0; i < user->numFriends; i++) {
        User* friendOfUser = user->friends[i];
        for (int j = 0; j < friendOfUser->numFriends; j++) {
            User* mutualFriend = friendOfUser->friends[j];
            if (mutualFriend != user && !isFriend(mutualFriend, user)) {
                printf("| %s (User %d)                         |\n", mutualFriend->name, mutualFriend->id);
            }
        }
    }
    printf("+---------------------------------------+\n");
}

void listUsers(User* users[], int numUsers) {
    printf("\n+---------------------------+\n");
    printf("| List of Users             |\n");
    printf("+---------------------------+\n");
    for (int i = 0; i < numUsers; i++) {
        printf("| User ID: %-4d | Name: %-10s |\n", users[i]->id, users[i]->name);
    }
    printf("+---------------------------+\n");
}

void checkFriends(User* user) {
    printf("\n+-------------------------------+\n");
    printf("| Friends of %s (User %d)         |\n", user->name, user->id);
    printf("+-------------------------------+\n");
    for (int i = 0; i < user->numFriends; i++) {
        printf("| %s (User %d)                  |\n", user->friends[i]->name, user->friends[i]->id);
    }
    printf("+-------------------------------+\n");
}

int main() {
    User* users[MAX_USERS];
    int numUsers = 0;

    while (1) {
        printf("\n+---------------------------------------+\n");
        printf("%s|        FRIEND RECOMMENDER             |%s\n",ANSI_LIGHT_BLUE,ANSI_COLOR_RESET);
        printf("+---------------------------------------+\n");
        printf("| %sMenu:                                %s |\n",ANSI_COLOR_YELLOW,ANSI_COLOR_RESET);
        printf("| 1. Add user                           |\n");
        printf("| 2. Add friend                         |\n");
        printf("| 3. Recommend friends                  |\n");
        printf("| 4. List Users                         |\n");
        printf("| 5. Check Friends                      |\n");
        printf("| 6. Exit                               |\n");
        printf("+---------------------------------------+\n");
        printf("Enter your choice: ");
        int choice;
        scanf("%d", &choice);

        switch (choice) {
            case 1: {
                int id;
                char name[50];
                printf("Enter user ID: ");
                scanf("%d", &id);
                printf("Enter user name: ");
                scanf("%s", name);

                if (id < 1) {
                    printf("%sError: User ID must be a positive integer.%s\n", ANSI_COLOR_RED, ANSI_COLOR_RESET);
                    break;
                }

                for (int i = 0; i < numUsers; i++) {
                    if (users[i]->id == id) {
                        printf("%sError: User with the same ID already exists.%s\n", ANSI_COLOR_RED, ANSI_COLOR_RESET);
                        break;
                    }
                }

                users[numUsers++] = createUser(id, name);
                printf("%sUser added successfully.%s\n", ANSI_COLOR_GREEN, ANSI_COLOR_RESET);
                break;
            }
            case 2: {
                int id1, id2;
                printf("Enter user IDs to establish friendship: ");
                scanf("%d %d", &id1, &id2);

                User* user1 = NULL;
                User* user2 = NULL;

                for (int i = 0; i < numUsers; i++) {
                    if (users[i]->id == id1) {
                        user1 = users[i];
                    }
                    if (users[i]->id == id2) {
                        user2 = users[i];
                    }
                }

                if (!user1 || !user2) {
                    printf("%sError: One or both users not found.%s\n", ANSI_COLOR_RED, ANSI_COLOR_RESET);
                } else {
                    addFriend(user1, user2);
                }
                break;
            }
            case 3: {
                int id;
                printf("Enter user ID for friend recommendations: ");
                scanf("%d", &id);
                User* user = NULL;

                for (int i = 0; i < numUsers; i++) {
                    if (users[i]->id == id) {
                        user = users[i];
                        break;
                    }
                }

                if (!user) {
                    printf("%sError: User not found.%s\n", ANSI_COLOR_RED, ANSI_COLOR_RESET);
                } else {
                    recommendFriends(user);
                }
                break;
            }
            case 4:
                listUsers(users, numUsers);
                break;
            case 5: {
                int id;
                printf("Enter user ID to check friends: ");
                scanf("%d", &id);
                User* user = NULL;

                for (int i = 0; i < numUsers; i++) {
                    if (users[i]->id == id) {
                        user = users[i];
                        break;
                    }
                }

                if (!user) {
                    printf("%sError: User not found.%s\n", ANSI_COLOR_RED, ANSI_COLOR_RESET);
                } else {
                    checkFriends(user);
                }
                break;
            }
            case 6:
                // Clean up memory
                for (int i = 0; i < numUsers; i++) {
                    free(users[i]);
                }
                printf("\n%s+---------------------------------------+%s\n", ANSI_COLOR_GREEN, ANSI_COLOR_RESET);
                printf("%s|   Thank you for using the program.   |%s\n", ANSI_COLOR_GREEN, ANSI_COLOR_RESET);
                printf("%s+---------------------------------------+%s\n", ANSI_COLOR_GREEN, ANSI_COLOR_RESET);
                return 0;
            default:
                printf("%sError: Invalid choice. Please try again.%s\n", ANSI_COLOR_RED, ANSI_COLOR_RESET);
        }
    }
}
