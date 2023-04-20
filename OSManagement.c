
// Including preprocessors
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pthread.h>

// Declaring our global constants
#define EXIT 0
#define INVALID -1
#define MEMORY 1
#define FILES 2
#define THREAD 3
#define FIRST 0
#define BEST 1
#define WORST 2
#define NEXT 3
#define PAGE 4
#define FIFO 0
#define LRU 1
#define FRAMES 3
#define SIZE 7

// Function prototypes
int displayMenu();
void clearScreen();
void memoryManagement();
void nextFit(int blockSize[], int blocks, int processSize[], int processes);
void firstFit(int blockSize[], int blocks, int processSize[], int processes);
void bestFit(int blockSize[], int blocks, int processSize[], int processes);
void worstFit(int blockSize[], int blocks, int processSize[], int processes);
void displayProcess(int allocation[], int processes, int processSize[]);
void pageReplacement();
void fifo();
void lru();
int findLRU(int time[]);
void displayPages(int page, int allocation[]);
void fileAttributes();
void printAttributes(char name[], struct stat statBuff);
void multiThreads();
void *threadFunction(void *vargp);

// Main function
int main()
{
    // Declaring our variable
    int choice = INVALID;

    // Looping while choice != 0
    while (choice != EXIT)
    {
        // Setting choice equal to the return value of the displayMenu function
        choice = displayMenu();

        // Conditional statements determine which OS function to run
        // Calls the memory management function
        if (choice == MEMORY) {
            memoryManagement();
        }

        // Calls the file attributes function
        if (choice == FILES) {
            fileAttributes();
        }

        // Calls the multithreading function
        if (choice == THREAD) {
            multiThreads();
        }

        // Calls the page replacement function
        if (choice == PAGE) {
            pageReplacement();
        }
        // Calls exit function
        if (choice == EXIT) {
            exit(0);
        }
    }
    return 0;
}

// This function allows the user to enter their menu choice. It then returns that choice to main.
int displayMenu()
{
    // Declaring our variable
    int choice = INVALID;

    // Loops while the choice is -1
    while (choice == INVALID)
    {
        // These print statements print the menu system
        printf("\n*************** Operating Systems Management Menu ***************\n\n");
        printf("Select the OS program to run. Enter the number of your selection.\n");
        printf("1. Memory Management\n");
        printf("2. File Management\n");
        printf("3. Multithreading\n");
        printf("4. Page Replacement\n");
        printf("0. Exit\n");

        // Getting the input from the user
        printf("Enter your choice here: ");
        scanf("%d", &choice);

        // Making sure the user enters one of our values
        // If choice is 0-3 this conditional won't run
        if (choice < 0 || choice > 4) {
            choice = INVALID;
        }
    }
    return choice;
}
// This function simply clears the screen by prompting the user to hit the enter key
void clearScreen()
{
    // Printing our message
    printf("\nPress the enter key to move to the next screen.\n");

    // Declaring char input to store our input
    char input;

    // Inputting the user's input
    scanf("%c", &input);

    // Sending the system our clear keywords
    //system("cls");
    system("clear");
}

// This function clears the screen and determines which memory management algorithm to run.
void memoryManagement()
{
    // Calling the clearScreen function
    clearScreen();

    // Looping to simulate the four memory management algorithms
    for (int i = 0; i < 4; i++)
    {
        // Declaring our arrays
        int blockSize[5] = {80,10,65,35,70};
        int processSize[5] = {25,70,5,45,60};

        // Variable stores # of blocks
        int blocks = sizeof(blockSize) / sizeof(int);

        // Variable stores the # of processes
        int processes = sizeof(processSize) / sizeof(int);

        // Conditional statements execute an algorithm based on the i value.
        if (i == FIRST) {
            firstFit(blockSize, blocks, processSize, processes);
        }

        else if (i == BEST) {
            bestFit(blockSize, blocks, processSize, processes);
        }

        else if (i == WORST) {
            worstFit(blockSize, blocks, processSize, processes);
        }

        else if (i == NEXT) {
            nextFit(blockSize, blocks, processSize, processes);
        }
    }
}

// This function simulates the next fit algorithm
void nextFit(int blockSize[], int blocks, int processSize[], int processes)
{
    printf("\n*************** Next Fit ***************\n\n");

    // Array stores the block ID that a process is allocated to
    int allocation[processes];

    // Variable stores the block allocation for a process
    int id = 0;

    // Calling memset()
    memset(allocation, INVALID, sizeof(allocation));

    // Looping through the # of processes
    for (int i = 0; i < processes; i++)
    {
        while (id < blocks)
        {
            // if-statement updates the allocation array if the condition is true
            if (blockSize[id] >= processSize[i]) {
                allocation[i] = id;

                // Reduces the available memory
                blockSize[id]  = blockSize[id] - processSize[i];

                break;
            }

        // Updating the id
        id = (id + 1) % blocks;
        }
    }

    // Calling the displayProcess function
    displayProcess(allocation, processes, processSize);
}

// This function simulates the first fit algorithm
void firstFit(int blockSize[], int blocks, int processSize[], int processes)
{
    printf("\n*************** First Fit ***************\n\n");

    // Array stores the block ID that a process is allocated to
    int allocation[processes];

    // Calling the memset() function
    memset(allocation, INVALID, sizeof(allocation));

    // Looping through the number of processes
    for (int i = 0; i < processes; i++)
    {
        // Looping through the number of blocks
        for (int j = 0; j < blocks; j++)
        {
            // if-statement updates the allocation array if condition is true
            if (blockSize[j] >= processSize[i]) {
                allocation[i] = j;

                // Reducing available memory
                blockSize[j] = blockSize[j] - processSize[i];
                break;
            }
        }
    }

    // Calling the displayProcess function
    displayProcess(allocation, processes, processSize);
}

// This function simulates the best fit algorithm
void bestFit(int blockSize[], int blocks, int processSize[], int processes)
{
    printf("\n*************** Best Fit ***************\n\n");

    // Array stores the block ID that a process is allocated to
    int allocation[processes];

    // Calling the memset() function
    memset(allocation, INVALID, sizeof(allocation));

    // Looping through the number of processes
    for (int i = 0; i < processes; i++)
    {
        // Variable stores the current best fit value
        int bestIdx = INVALID;

        // Looping through the number of blocks
        for (int j = 0; j < blocks; j++)
        {
            if (blockSize[j] >= processSize[i])
            {
                // Checking if bestIdx is invalid
                if (bestIdx == INVALID)
                    bestIdx = j;

                else if (blockSize[bestIdx] > blockSize[j])
                    bestIdx = j;
            }
        }

        // Updating the allocation array if the condition is true
        if (bestIdx != INVALID) {
            allocation[i] = bestIdx;

            // Reducing available memory
            blockSize[bestIdx] = blockSize[bestIdx] - processSize[i];
        }
    }

    // Calling the displayProcess function
    displayProcess(allocation, processes, processSize);
}

// This function simulates the worst fit algorithm
void worstFit(int blockSize[], int blocks, int processSize[], int processes)
{
    printf("\n*************** Worst Fit ***************\n\n");

    // Array stores the block ID that a process is allocated to
    int allocation[processes];

    // Calling the memset() function
    memset(allocation, INVALID, sizeof(allocation));

    // Looping through the number of processes
    for (int i = 0; i < processes; i++)
    {
        // Variable stores the current worst fit value
        int wstIdx = INVALID;

        // Looping through the number of blocks
        for (int j = 0; j < blocks; j++)
        {
            if (blockSize[j] >= processSize[i])
            {
                if (wstIdx == INVALID)
                    wstIdx = j;

                else if (blockSize[wstIdx] < blockSize[j])
                    wstIdx = j;
            }
        }

        // Updating the allocation array if the condition is true
        if (wstIdx != INVALID) {
            allocation[i] = wstIdx;

            // Reducing available memory
            blockSize[wstIdx] = blockSize[wstIdx] - processSize[i];
        }
    }

    // Calling the displayProcess function
    displayProcess(allocation, processes, processSize);
}

// This function outputts the results of the above functions
void displayProcess(int allocation[], int processes, int processSize[])
{
    // Displaying our process information
    printf("\nProcess No. \tProcess Size\tBlock No.\n");

    // Looping through the processes
    for (int i = 0; i < processes; i++)
    {
        printf("%d\t\t %d\t\t", (i + 1), processSize[i]);

        // Checking if the allocation is invalid
        if (allocation[i] == INVALID) {
            printf("Not allocated\n");
        }

        // Printing the allocation if it is successful
        printf("%d\n", (allocation[i] + 1));
    }
    printf("\n");
}

// This function calls either the FIFO or LRU functions based on the value of the looping variable
void pageReplacement()
{
    // Calling function clearScreen
    clearScreen();

    printf("\n*************** Page Replacement: ***************\n\n");

    // Looping through the two page replacement algorithms
    for (int i = 0; i < 2; i++)
    {
        // Calling FIFO or LRU based on the looping variable's value
        if (i == FIFO)
            fifo();

        if (i == LRU)
            lru();
    }
}

// This function simulates the FIFO page replacement algorithm
void fifo()
{
    printf("\n\t*************** FIFO ***************\n\n");
    printf("\tPage   Frame 1   Frame 2   Frame 3\n");

    // This array stores the page requests
    int pageRequests[10] = {4, 1, 2, 4, 2, 5, 1, 3, 6};

    // This array stores the memory page a frame is allocated to
    int allocation[FRAMES];

    int pageFaults = 0; // Stores the page faults
    int present = 0; // Stores the number of page hits
    int pages = 9; // Stores the number of pages

    // Calling the memset function
    memset(allocation, INVALID, sizeof(allocation));

    // Looping through the number of pages
    for (int i = 0; i < pages; i++)
    {
        present = 0;

        // Looping through the number of frames
        for (int j = 0; j < FRAMES; j++)
        {
            if (pageRequests[i] == allocation[j]) {
                present++;
                pageFaults--;
            }
        }

            pageFaults++;

            // Updating the allocation array based on the conditions
            if (pageFaults <= FRAMES && present == 0) {
                allocation[i] = pageRequests[i];
            }

            // Replacing the front page if the condition is true
            else if(present == 0) {
                allocation[(pageFaults - 1) % FRAMES] = pageRequests[i];
            }

        // Calling the displayPages function
        displayPages(pageRequests[i], allocation);
    }

    // Displaying the total number of page faults
    printf("\n\nTotal Page Faults: %d\n", pageFaults);
}

// This function simulates the LRU page replacement algorithm
void lru()
{
    printf("\n\t*************** LRU ***************\n\n");
    printf("\tPage   Frame 1   Frame 2   Frame 3\n");

    int pageRequests[10] = {4, 1, 2, 4, 2, 5, 1, 3, 6}; // Stores the page requests
    int allocation[FRAMES]; // Store the memory frame a page is allocated to
    int time[10]; // Stores when a page is allocated

    // Variable stores the page faults
    int pageFaults = 0;

    // Variable stores the number of pages
    int pages = 9;

    // Variable stores the number of page hits
    int counter = 0;

    // These variables track page hits/misses
    int flag1 = 0;
    int flag2 = 0;

    // Stores the most recent request
    int position = 0;

    // Calling the memset function
    memset(allocation, INVALID, sizeof(allocation));

    // Looping through the number of pages
    for (int i = 0; i < pages; i++)
    {
        flag1 = 0;
        flag2 = 0;

        // Looping through the number of frames
        for (int j = 0; j < FRAMES; j++)
        {
            // Updating the time array if the conditions are met
            if (pageRequests[i] == allocation[j]) {
                counter++;
                time[j] = counter;

                flag1 = MEMORY;
                flag2 = MEMORY;

                break;
            }
        }

        if (flag1 == 0) {
            // Iterating through the number of frames
            for (int i = 0; i < FRAMES; i++)
            {
                if (allocation[i] == INVALID) {
                    counter++;
                    pageFaults++;

                    allocation[i] = pageRequests[i];
                    time[i] = counter;

                    flag2 = MEMORY;

                    break;
                }
            }
        }

        if (flag2 == 0) {
            position = findLRU(time);

            counter++;
            pageFaults++;
            allocation[position] = pageRequests[i];
            time[position] = counter;

        }

        // Calling the displayPages function
        displayPages(pageRequests[i], allocation);
    }

    // Displaying the number of page faults
    printf("\n\nTotal Page Faults: %d\n", pageFaults);
}

// This function finds the LRU
int findLRU(int time[])
{
    int position = 0;
    int minimum = time[0];

    // Iterating through the number of frames
    for (int i = 0; i < FRAMES; i++)
    {
        if (time[i] < minimum) {
            minimum = time[i];
            position = i;
        }
    }

    return position;
}

// This function displays the pages
void displayPages(int page, int allocation[])
{
    //printf("Page"); The header thing goes in fifo and lru
    printf("\n");
    printf("%10d", page);

    // Looping through the number of frames
    for (int i = 0; i < FRAMES; i++)
    {
        if (allocation[i] == INVALID)
            printf("%10s", "-");

        else {
            printf("%10d", allocation[i]);
        }
    }
}

// This function returns the file attributes of a specified file
void fileAttributes()
{
    // Declaring this variable to store the attribute structure
    struct stat statBuff;

    // Declaring this variable to store our error code
    int err = 0;

    // Declaring this variable as a pointer
    struct dirent *de;

    // Declaring this variable as an argument to the opendir() function
    DIR *dr = opendir(".");

    // Checking if dr is NULL
    if (dr == NULL)
        printf("Could not open current directory\n");

    // Looping while we still have contents in our directory
    while ((de = readdir(dr)) != NULL)
    {
        // Storing the result of the stat() function
        err = stat(de->d_name, &statBuff);

        // Checking if the file could not be accessed
        if (err == -1) {
            printf("Error in stat\n");
            printf("Press 'Enter' to continue\n");
            getchar();
        }

        // Calling our printAttributes function
        printAttributes(de->d_name, statBuff);
    }
}

// This function prints all the attributes of the files
void printAttributes(char name[], struct stat statBuff)
{
    // Declaring our time
    time_t t;

    // Stores the time in a string
    char timeStr[100];

    // Printing the file name
    printf("~~~~~~~~~~~~~~~ File = %s ~~~~~~~~~~~~~~~", name);

    // Outputting the file device id
    printf("\n\nDevice id = %i", statBuff.st_dev);

    // Outptting the final serial number
    printf("\nFile serial number = %llu", statBuff.st_ino);

    // Outputting the file user id
    printf("\nFile user id = %i", statBuff.st_uid);

    // Outputting the file group id
    printf("\nFile group id = %i", statBuff.st_gid);

    // Outputting the file mode
    printf("\nFile mode = %i", statBuff.st_mode);

    // Evaluating the file mode for owner permissions
    printf("\nOwner Permissions: ");

    if (statBuff.st_mode & S_IRGRP)
        printf ("read ");
    if (statBuff.st_mode & S_IWGRP)
        printf ("write ");
    if (statBuff.st_mode & S_IXGRP)
        printf ("execute");

    // Evaluating the file mode for group permissions
    printf("\nGroup Permissions: ");

    if (statBuff.st_mode & S_IROTH)
        printf ("read ");
    if (statBuff.st_mode & S_IWOTH)
        printf ("white");
    if (statBuff.st_mode & S_IXOTH)
        printf ("execute");

    // Outputting the creation date
    t = statBuff.st_ctime;
    strcpy(timeStr, ctime(&t));
    printf("\nDate created = %s", timeStr);

    // Outputting the last time the file was accessed
    t = statBuff.st_atime;
    strcpy(timeStr, ctime(&t));
    printf("Time file was last accessed = %s", timeStr);

    // Outputting the file size
    printf("File size = %lli bytes\n", statBuff.st_size);
}

// This function creates multiple threads using the pthread library
void multiThreads()
{
    // Stores the return value from the pthread_create function
    int i;
    int error = 0;
    pthread_t tid[SIZE];

    // Looping to create our threads
    for (i = 0; i < SIZE; i++)
    {
        // Storing the results of the pthread_create function
        error = pthread_create(&tid[i], NULL, &threadFunction, (void *) &tid[i]);

        // Checking if the thread was not created
        if (error != 0)
            printf("\nThread can't be created : [%s] Press 'Enter' to continue . . .\n", strerror(error));
    }
    
    // Looping to call our thread joining function
    for (i = 0; i < SIZE; i++)
    {
        pthread_join(tid[i], NULL);
    }
}

// This function prints messages depending on the status of a thread
void *threadFunction(void *vargp)
{
    // Loop control variable
    const int LOOP = 3;

    // This variable stores the thread id
    int *myid = (int *) vargp;

    // Outputting that the thread has started
    printf("\n------------------------\n");
    printf("Thread ID %d has started\n", *myid); 
    printf("------------------------\n");

    // Looping through the iterations
    for (int i = 0; i < LOOP; i++)
    {
        printf("Thread ID %d printing iteration %d\n", *myid, i);
    }
    printf("------------------------\n");

    // Outputting that the thread is complete
    printf("Thread ID %d has finished\n", *myid);
    printf("\n------------------------\n");

    return NULL;
}
