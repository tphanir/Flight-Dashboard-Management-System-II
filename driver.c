#include "Implementation.h"

void generateDOT(BucketNode *root, FILE *fp) {
    if (root == NULL) {
        return;
    }
    fprintf(fp, "node%d [shape = record, label=\"{", (int)root);
    fprintf(fp, "Bucket ID : %d\\n"
                "BEG ETA INTERVAL - %02d : %02d\\n"
                "END ETA INTERVAL - %02d : %02d\\n",
            root->data[0].bucketID,
            root->data[0].beginningETA.hour,
            root->data[0].beginningETA.min,
            root->data[0].endETA.hour,
            root->data[0].endETA.min);
    for (int i = 1; i < root->count; i++) {
        fprintf(fp, "| Bucket ID : %d\\n"
                    "BEG ETA INTERVAL - %02d : %02d\\n"
                    "END ETA INTEVAL - %02d : %02d\\n",
                root->data[i].bucketID,
                root->data[i].beginningETA.hour,
                root->data[i].beginningETA.min,
                root->data[i].endETA.hour,
                root->data[i].endETA.min);

    }
    fprintf(fp, "}\"];\n");

    for (int i = 0; i <= root->count; i++) {
        if (root->children[i] != NULL) {
            fprintf(fp, "node%d:data -> node%d [arrowhead=\"normal\", color=\"black\", style=\"solid\", penwidth=1, tailport=\"s\", headport=\"n\"];\n", (int)root, (int)root->children[i]);
            generateDOT(root->children[i], fp);
        }
    }
}
void generateDOT2(FlightPlanNode *root, FILE *fp) {
    if (root == NULL) {
        return;
    }
    fprintf(fp, "node%d [shape = record, label=\"{", (int)root);
    fprintf(fp, "FLIGHT ID - %d\\n"
                "DEPARTURE - %02d : %02d\\n"
                "ETA - %02d : %02d\\n",
            root->data[0].flightID,
            root->data[0].departure.hour,
            root->data[0].departure.min,
            root->data[0].ETA.hour,
            root->data[0].ETA.min);
    for (int i = 1; i < root->count; i++) {
        fprintf(fp, "| FLIGHT ID - %d\\n"
                    "DEPARTURE - %02d : %02d\\n"
                    "ETA - %02d : %02d\\n",
                root->data[i].flightID,
                root->data[i].departure.hour,
                root->data[i].departure.min,
                root->data[i].ETA.hour,
                root->data[i].ETA.min);

    }
    fprintf(fp, "}\"];\n");

    for (int i = 0; i <= root->count; i++) {
        if (root->children[i] != NULL) {
            fprintf(fp, "node%d:data -> node%d [arrowhead=\"normal\", color=\"black\", style=\"solid\", penwidth=1, tailport=\"s\", headport=\"n\"];\n", (int)root, (int)root->children[i]);
            generateDOT2(root->children[i], fp);
        }
    }
}
BucketNode *func1(BucketNode *root)
{
    FlightPlan new;
    printf("Enter details of the FLIGHT PLAN : \n");
    printf("Enter FLIGHT ID - ");
    scanf("%d", &new.flightID);
    printf("Enter DEPARTURE TIME - ");
    scanf("%d %d", &new.departure.hour, &new.departure.min);
    printf("Enter ETA - ");
    scanf("%d %d", &new.ETA.hour, &new.ETA.min);
    root = INSERT_FLIGHT_PLAN_INTO_BUCKET(root, new);
    printf("FLIGHT PLAN INSERTED successfully.\n");
    return root;
}
void func2(BucketNode *root)
{
    char reply1, reply2;
    TIME departure, ETA;
    int ID;

    printf("Enter details of the FLIGHT PLAN - \n");
    printf("Enter FLIGHT ID : ");
    scanf("%d", &ID);
    while (getchar() != '\n');
    printf("Do you know DEPARTURE TIME [y/n] : ");
    scanf("%c", &reply1);
    if(reply1 == 'y')
    {
        printf("Enter DEPARTURE TIME - ");
        scanf("%d %d", &departure.hour, &departure.min);
    }
    while (getchar() != '\n');
    printf("Do you know ETA [y/n] : ");
    scanf("%c", &reply2);
    if(reply2 == 'y')
    {
        printf("Enter ETA - ");
        scanf("%d %d", &ETA.hour, &ETA.min);
    }

    if (reply1 == 'y')
    {
        ETA.hour = -1;
    }
    else if(reply2 == 'y')
    {
        departure.hour = -1;
    }
    else
    {
        departure.hour = -1;
        ETA.hour = -1;
    }
    SHOW_STATUS(root, ID, departure, ETA);

}
BucketNode *func3(BucketNode *root)
{
    char reply1, reply2;
    TIME departure, ETA;
    int ID;

    printf("Enter details of the FLIGHT PLAN - \n");
    printf("Enter FLIGHT ID : ");
    scanf("%d", &ID);
    while (getchar() != '\n');
    printf("Do you know DEPARTURE TIME [y/n] : ");
    scanf("%c", &reply1);
    if(reply1 == 'y')
    {
        printf("Enter DEPARTURE TIME - ");
        scanf("%d %d", &departure.hour, &departure.min);
    }
    while (getchar() != '\n');
    printf("Do you know ETA [y/n] : ");
    scanf("%c", &reply2);
    if(reply2 == 'y')
    {
        printf("Enter ETA - ");
        scanf("%d %d", &ETA.hour, &ETA.min);
    }

    if (reply1 == 'y')
    {
        ETA.hour = -1;
    }
    else if(reply2 == 'y')
    {
        departure.hour = -1;
    }
    else
    {
        departure.hour = -1;
        ETA.hour = -1;
    }
    root = DELETE_PLAN(root, ID, departure, ETA);
    printf("FLIGHT PLAN deleted successfully.\n");
    SHOW_STATUS(root,ID, departure,ETA);
    return root;
}

void graphMaker(FILE *fptr, BucketNode *root)
{
    fprintf(fptr, "digraph G {\n");
    fprintf(fptr, "node [shape=circle];\n");
    generateDOT(root, fptr);
    fprintf(fptr,"graph [layout=\"dot\", size=\"200,200\"]\n");
    fprintf(fptr, "}\n");
}
BucketNode *display(BucketNode *root)
{
    int done = 0;
    int input;

    printf("---------------------------------------------------------");
    printf("\n%43s\n", "WELCOME TO DIGITAL DASHBOARD");
    printf("---------------------------------------------------------\n");
    printf("\nCHOOSE from the following - \n");
    printf("\n1. INSERT NEW FLIGHT PLAN");
    printf("\n2. SHOW STATUS OF A FLIGHT PLAN");
    printf("\n3. CANCEL FLIGHT PLAN");
    printf("\n4. EXIT\n");
    printf("\n---------------------------------------------------------\n");

    while(!done)
    {
        printf("Enter chosen OPTION number : ");
        scanf("%d", &input);
        printf("\n");
        if(input == 1)
        {
            func1(root);
        }
        else if(input == 2)
        {
            func2(root);
        }
        else if(input == 3)
        {
            func3(root);
        }
        else if(input == 4)
        {
            done = 1;
            printf("---------------------------------------------------------\n");
            printf("%30s\n", "THANK YOU");
            printf("---------------------------------------------------------\n");
        }
        else
        {
            printf("INVALID OPTION SELECTED!\n");
            printf("Choose a valid OPTION.\n");
        }
        if(done != 1) {
            printf("\n---------------------------------------------------------\n");
            printf("\n1. INSERT NEW FLIGHT PLAN");
            printf("\n2. SHOW STATUS OF A FLIGHT PLAN");
            printf("\n3. CANCEL FLIGHT PLAN");
            printf("\n4. EXIT\n");
            printf("\n---------------------------------------------------------\n");
        }
    }
    return root;
}
void main()
{
    BucketNode  *root = NULL;
    FILE *fptr = fopen("tree.dot","w");
    root = READ_FLIGHT_PLAN_INTO_BUCKET(root);

    graphMaker(fptr, root);

}
