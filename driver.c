#include "Implementation.h"

void generateDOT(BucketNode *root, FILE *fp) {
    if (root == NULL) {
        return;
    }
    fprintf(fp, "node%d [shape = record, label=\"{", (int)root);
    fprintf(fp, "Bucket ID = %d\\n"
                "BEG ETA INTERVAL - %02d : %02d\\n"
                "END ETA INTERVAL - %02d : %02d\\n",
            root->data[0].bucketID,
            root->data[0].beginningETA.hour,
            root->data[0].beginningETA.min,
            root->data[0].endETA.hour,
            root->data[0].endETA.min);
    for (int i = 1; i < root->count; i++) {
        fprintf(fp, "| Bucket ID = %d\\n"
                    "BEG ETA INTERVAL = %02d : %02d\\n"
                    "END ETA INTEVAL + %02d : %02d\\n",
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
    fprintf(fp, "Flight ID - %d\\n"
                "DEPARTURE - %02d : %02d\\n"
                "ETA - %02d : %02d\\n",
            root->data[0].flightID,
            root->data[0].departure.hour,
            root->data[0].departure.min,
            root->data[0].ETA.hour,
            root->data[0].ETA.min);
    for (int i = 1; i < root->count; i++) {
        fprintf(fp, "| Bucket ID = %d\\n"
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

    if(reply1 == 'y' && reply2 == 'y')
    {
        SHOW_STATUS(root, ID, departure, ETA);
    }
    else if (reply1 == 'y')
    {
        ETA.hour = -1;
        SHOW_STATUS(root, ID, departure, ETA);
    }
    else if(reply2 == 'y')
    {
        departure.hour = -1;
        SHOW_STATUS(root, ID, departure, ETA);
    }
    else
    {
        departure.hour = -1;
        ETA.hour = -1;
        SHOW_STATUS(root, ID, departure, ETA);
    }

}
void main()
{
    BucketNode  *root;
    FILE *fptr = fopen("tree.dot","w");

    Bucket x;
    root = NULL;
    int pos;
    root = READ_FLIGHT_PLAN_INTO_BUCKET(root);
    x.beginningETA.hour = 15;
    x.beginningETA.min = 0;
    root = DELETE_TREE_BUCKET(x, root);


//    int flightID;
    fprintf(fptr, "digraph G {\n");
    fprintf(fptr, "node [shape=circle];\n");
    generateDOT(root, fptr);
    fprintf(fptr,"graph [layout=\"dot\", size=\"200,200\"]\n");
    fprintf(fptr, "}\n");


//    departure.hour = -16;
//    departure.min = 35;
//    ETA.hour =-19;
//    ETA.min = 45;
//
//    bucket = SEARCH_BUCKET_TREE(departure, root, &pos);

    //PRINT_FLIGHT_PLAN_TREE(bucket->data[pos].f);
    //for(int i=0; i<50; i++)
//    {
//        SHOW_STATUS(root, i+1, departure, ETA);
//
//    }
}