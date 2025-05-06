#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CANDIDATES 10
#define MAX_VOTERS 100

// Structure for voter linked to candidate
typedef struct Voter
{
    int voterId;
    char name[50];
    struct Voter *next;
} Voter;

// Registered Voter with credentials
typedef struct
{
    int voterId;
    char name[50];
    char password[20];
    int hasVoted;
} RegisteredVoter;

// Structure for candidate
typedef struct
{
    int candidateId;
    char name[50];
    int votes;
    Voter *voters; // Linked list of voters who voted for this candidate
} Candidate;

Candidate candidates[MAX_CANDIDATES];
int candidateCount = 0;

RegisteredVoter registeredVoters[MAX_VOTERS];
int registeredVoterCount = 0;

// Add a new candidate
void addCandidate(int id, const char *name)
{
    if (candidateCount >= MAX_CANDIDATES)
    {
        printf("Maximum number of candidates reached!\n");
        return;
    }

    candidates[candidateCount].candidateId = id;
    strcpy(candidates[candidateCount].name, name);
    candidates[candidateCount].votes = 0;
    candidates[candidateCount].voters = NULL;
    candidateCount++;

    printf("Candidate added successfully!\n");
}

// Register a new voter
void registerVoter(int id, const char *name, const char *password)
{
    for (int i = 0; i < registeredVoterCount; i++)
    {
        if (registeredVoters[i].voterId == id)
        {
            printf("Voter ID already exists!\n");
            return;
        }
    }

    registeredVoters[registeredVoterCount].voterId = id;
    strcpy(registeredVoters[registeredVoterCount].name, name);
    strcpy(registeredVoters[registeredVoterCount].password, password);
    registeredVoters[registeredVoterCount].hasVoted = 0;
    registeredVoterCount++;

    printf("Voter registered successfully!\n");
}

// Authenticate voter by ID and password
int authenticateVoter(int id, const char *password)
{
    for (int i = 0; i < registeredVoterCount; i++)
    {
        if (registeredVoters[i].voterId == id && strcmp(registeredVoters[i].password, password) == 0)
        {
            if (registeredVoters[i].hasVoted)
            {
                printf("You have already voted!\n");
                return -1;
            }
            return i; // return index of authenticated voter
        }
    }
    printf("Authentication failed! Invalid ID or password.\n");
    return -1;
}

// Add a vote after authentication
void addVote(int candidateId, int voterIndex)
{
    for (int i = 0; i < candidateCount; i++)
    {
        if (candidates[i].candidateId == candidateId)
        {
            Voter *newVoter = (Voter *)malloc(sizeof(Voter));
            newVoter->voterId = registeredVoters[voterIndex].voterId;
            strcpy(newVoter->name, registeredVoters[voterIndex].name);
            newVoter->next = candidates[i].voters;
            candidates[i].voters = newVoter;

            candidates[i].votes++;
            registeredVoters[voterIndex].hasVoted = 1;

            printf("Vote recorded successfully!\n");
            return;
        }
    }
    printf("Candidate not found!\n");
}

// Display candidates
void displayCandidates()
{
    printf("\nCandidates List:\n");
    printf("ID\tName\t\tVotes\n");
    printf("--------------------------------\n");
    for (int i = 0; i < candidateCount; i++)
    {
        printf("%d\t%s\t\t%d\n", candidates[i].candidateId, candidates[i].name, candidates[i].votes);
    }
}

// Display voters for a candidate
void displayVoters(int candidateId)
{
    for (int i = 0; i < candidateCount; i++)
    {
        if (candidates[i].candidateId == candidateId)
        {
            printf("\nVoters for %s:\n", candidates[i].name);
            printf("Voter ID\tName\n");
            printf("------------------------\n");

            Voter *current = candidates[i].voters;
            while (current != NULL)
            {
                printf("%d\t\t%s\n", current->voterId, current->name);
                current = current->next;
            }
            return;
        }
    }
    printf("Candidate not found!\n");
}

int main()
{
    int choice;

    printf("Welcome to the Secure Election System!\n");

    while (1)
    {
        printf("\nMenu:\n");
        printf("1. Add Candidate\n");
        printf("2. Register Voter\n");
        printf("3. Cast Vote (Login Required)\n");
        printf("4. Display Candidates\n");
        printf("5. Display Voters for a Candidate\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
        {
            int id;
            char name[50];
            printf("Enter candidate ID: ");
            scanf("%d", &id);
            printf("Enter candidate name: ");
            scanf("%s", name);
            addCandidate(id, name);
            break;
        }
        case 2:
        {
            int id;
            char name[50], password[20];
            printf("Enter voter ID: ");
            scanf("%d", &id);
            printf("Enter voter name: ");
            scanf("%s", name);
            printf("Set a password: ");
            scanf("%s", password);
            registerVoter(id, name, password);
            break;
        }
        case 3:
        {
            int voterId, candidateId;
            char password[20];
            printf("Enter your Voter ID: ");
            scanf("%d", &voterId);
            printf("Enter your Password: ");
            scanf("%s", password);
            int voterIndex = authenticateVoter(voterId, password);
            if (voterIndex != -1)
            {
                printf("Enter Candidate ID to vote for: ");
                scanf("%d", &candidateId);
                addVote(candidateId, voterIndex);
            }
            break;
        }
        case 4:
            displayCandidates();
            break;
        case 5:
        {
            int candidateId;
            printf("Enter candidate ID: ");
            scanf("%d", &candidateId);
            displayVoters(candidateId);
            break;
        }
        case 6:
            printf("Thank you for using the Secure Election System!\n");
            return 0;
        default:
            printf("Invalid choice! Please try again.\n");
        }
    }
}