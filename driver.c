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
void main()
{
    BucketNode  *root;
    FILE *fptr = fopen("tree.dot","w");

    root = NULL;
    int pos;
    root = READ_FLIGHT_PLAN_INTO_BUCKET(root);
    //PRINT_BUCKET_TREE(root);
    BucketNode *bucket;

    TIME departure, ETA;
    int flightID;
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