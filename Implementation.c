#include "Implementation.h"

static int random = 1;

// TIME FUNCTIONS
int maxtime(TIME A, TIME B)
{
    int RETURN_VALUE;

    if(A.hour < B.hour)
    {
        RETURN_VALUE = 1;
    }
    else if(A.hour == B.hour)
    {
        if(A.min == B.min)
        {
            RETURN_VALUE = 0;
        }
        else if(A.min > B.min)
        {
            RETURN_VALUE = -1;
        }
        else
        {
            RETURN_VALUE = 1;
        }
    }
    else
    {
        RETURN_VALUE = -1;
    }

    return RETURN_VALUE;
}
int timedeff(TIME A, TIME B)
{
    int diff;
    diff = (A.hour - B.hour) * 60 + (A.min - B.min);
    return diff;
}
void printTime(TIME t)
{
    printf("%02d : %02d", t.hour, t.min);
}


// INITIALIZE FUNCTIONS
void INIT_F_NODE(FlightPlanNode *root)
{
    root->count = 0;
    for(int i=0; i<M; i++)
    {
        root->children[i] = NULL;
    }
}

// SEARCH FLIGHT PLAN NODE FUNCTIONS
int BINARY_SEARCH_FLIGHT_PLAN(FlightPlan data[], TIME target, int start, int end)
{
    int RETURN_VALUE;
    if(start <= end)
    {
        int mid = (start + end) / 2;
        if(maxtime(data[mid].departure, target) == 0)
        {
            RETURN_VALUE= mid;
        }
        else if(maxtime(data[mid].departure, target) == -1)
        {
            RETURN_VALUE= BINARY_SEARCH_FLIGHT_PLAN(data, target, start, mid - 1);
        }
        else
        {
            RETURN_VALUE= BINARY_SEARCH_FLIGHT_PLAN(data, target, mid + 1, end);
        }
    }
    else
    {
        RETURN_VALUE= start;
    }
    return RETURN_VALUE;
}
Boolean SEARCH_FLIGHT_PLAN_NODE(TIME target, FlightPlanNode *current, int *pos)
{
    Boolean RETURN_VALUE= FALSE;
    *pos = BINARY_SEARCH_FLIGHT_PLAN(current->data, target, 0, current->count -1);
    if(timedeff(current->data[*pos].departure,target) == 0)
    {
        RETURN_VALUE= TRUE;
    }
    return RETURN_VALUE;
}
FlightPlanNode *SEARCH_FLIGHT_PLAN_TREE(TIME target, FlightPlanNode *root, int *targetPosition)
{
    FlightPlanNode *RETURN_VALUE= NULL;
    if(root != NULL)
    {

        if(SEARCH_FLIGHT_PLAN_NODE(target, root, targetPosition))
        {
            RETURN_VALUE = root;
        }
        else
        {
            RETURN_VALUE = SEARCH_FLIGHT_PLAN_TREE(target, root->children[*targetPosition], targetPosition);
        }
    }
    return RETURN_VALUE;
}

// INSERT FLIGHT PLAN NODE FUNCTIONS
void PUSH_IN_FLIGHT_PLAN(FlightPlan median, FlightPlanNode *medianRight, FlightPlanNode *current, int pos)
{
    int i;

    for (i=current->count; i>pos; i--)
    {
        current->data[i] = current->data[i - 1];
        current->children[i + 1] = current->children[i];
    }

    current->data[pos] = median;
    current->children[pos + 1] = medianRight;
    current->count++;
}
void SPLIT_FLIGHT_PLAN_NODE(FlightPlan median, FlightPlanNode *medianRight, FlightPlanNode *current, int pos, FlightPlan *newMedian, FlightPlanNode **newMedianRight)
{
    int i;
    int medianIndex;
    if(pos <= MIN)
    {
        medianIndex = MIN;
    }
    else
    {
        medianIndex = MIN + 1;
    }

    *newMedianRight = (FlightPlanNode *)malloc(sizeof(FlightPlanNode));

    for(i = medianIndex; i < MAX; i++)
    {
        (*newMedianRight)->data[i - medianIndex] = current->data[i];
        (*newMedianRight)->children[i - medianIndex + 1] = current->children[i + 1];
    }

    (*newMedianRight)->count = MAX - medianIndex;
    current->count = medianIndex;

    if(pos <= MIN)
    {
        PUSH_IN_FLIGHT_PLAN(median, medianRight, current, pos);
    }
    else
    {
        PUSH_IN_FLIGHT_PLAN(median, medianRight, *newMedianRight, pos - medianIndex);
    }

    *newMedian = current->data[current->count - 1];
    (*newMedianRight)->children[0] = current->children[current->count];

    current->count--;
}
Boolean PUSH_DOWN_FLIGHT_PLAN_NODE(FlightPlan value, FlightPlanNode *current, FlightPlan *median, FlightPlanNode **medianRight)
{
    Boolean RETURN_VALUE= FALSE;
    int pos;
    if(current == NULL)
    {
        *median = value;
        *medianRight = NULL;
        RETURN_VALUE= TRUE;
    }
    else
    {
        SEARCH_FLIGHT_PLAN_NODE(value.departure, current, &pos);
        if(PUSH_DOWN_FLIGHT_PLAN_NODE(value, current->children[pos], median, medianRight))
        {
            if(current->count < MAX)
            {
                PUSH_IN_FLIGHT_PLAN(*median, *medianRight, current, pos);
                RETURN_VALUE= FALSE;
            }
            else
            {
                SPLIT_FLIGHT_PLAN_NODE(*median, *medianRight, current, pos, median, medianRight);
                RETURN_VALUE= TRUE;
            }
        }
    }
    return RETURN_VALUE;
}
FlightPlanNode *INSERT_FLIGHT_PLAN_TREE(FlightPlan value, FlightPlanNode *root)
{
    FlightPlan median;
    FlightPlanNode *medianRight;
    FlightPlanNode *newRoot;

    FlightPlanNode *RETURN_VALUE= root;

    if(PUSH_DOWN_FLIGHT_PLAN_NODE(value, root, &median, &medianRight))
    {
        newRoot = (FlightPlanNode *)malloc(sizeof(FlightPlanNode));
        newRoot->count = 1;
        newRoot->data[0] = median;
        newRoot->children[0] = root;
        newRoot->children[1] = medianRight;

        RETURN_VALUE= newRoot;
    }
    return  RETURN_VALUE;
}

// FLIGHT PLAN FILE HANDLING FUNCTIONS
FlightPlanNode *READ_FLIGHT_PLAN(FlightPlanNode *root)
{
    FILE *fptr;
    fptr = fopen("FlightPlans.dat", "r");
    while(!feof(fptr))
    {
        FlightPlan plan;
        fscanf(fptr, "%d, %d, %d, %d, %d",
               &plan.flightID, &plan.departure.hour, &plan.departure.min, &plan.ETA.hour, &plan.ETA.min);
        root = INSERT_FLIGHT_PLAN_TREE(plan, root);
    }
    fclose(fptr);
    return root;
}

// PRINT FLIGHT PLAN FUNCTIONS
void PRINT_FLIGHT_PLAN_NODE(FlightPlanNode *root)
{
    int i;
    for(i=0; i<root->count; i++)
    {
        printf("\nFLIGHT ID - %d",root->data[i].flightID);
        printf("\nDEPARTURE TIME - ");
        printTime(root->data[i].departure);
        printf("\nETA %10s - ", "");
        printTime(root->data[i].ETA);
    }
}
void PRINT_FLIGHT_PLAN_TREE(FlightPlanNode *root)
{
    if(root != NULL)
    {
        PRINT_FLIGHT_PLAN_NODE(root);
        printf("\n-----------------\n");
        for(int i=0; i<=root->count; i++)
        {
            PRINT_FLIGHT_PLAN_TREE(root->children[i]);
        }
    }
}
void PRINT_FLIGHT_PLAN_INORDER(FlightPlanNode *root)
{
    if(root != NULL)
    {
        for(int i=0; i<root->count; i++)
        {
            PRINT_FLIGHT_PLAN_INORDER(root->children[i]);
            printf("\nFLIGHT ID - %d",root->data[i].flightID);
            printf("\nDEPARTURE TIME - ");
            printTime(root->data[i].departure);
            printf("\nETA %10s - ", "");
            printTime(root->data[i].ETA);
            PRINT_FLIGHT_PLAN_INORDER(root->children[i+1]);
        }
    }
}

// DELETE FLIGHT PLAN FUNCTIONS
void MOVE_RIGHT_FLIGHT_PLAN(FlightPlanNode *current, int pos)
{
    int c;
    FlightPlanNode *t;
    t = current->children[pos];
    for(c = t->count; c>0; c--)
    {
        t->data[c] = t->data[c-1];
        t->children[c+1] = t->children[c];
    }
    t->children[1] = t->children[0];
    t->count++;
    t->data[0] = current->data[pos -1];
    t = current->children[pos - 1];
    current->data[pos - 1] = t->data[t->count - 1];
    current->children[pos]->children[0] = t->children[t->count];
    t->count--;
}
void MOVE_LEFT_FLIGHT_PLAN(FlightPlanNode *current, int pos)
{
    int c;
    FlightPlanNode *t;

    t=current->children[pos-1];
    t->count++;
    t->data[t->count-1] = current->data[pos -1];
    t->children[t->count] = (current->children[pos])->children[0];
    t = current->children[pos];
    current->data[pos-1] = t->data[0];
    t->children[0] = t->children[1];
    t->count--;
    for(c=1; c<=t->count; c++)
    {
        t->data[c-1] = t->data[c];
        t->children[c] = t->children[c+1];
    }
}
void COMBINE_FLIGHT_PLAN(FlightPlanNode *current, int pos)
{
    int c;
    FlightPlanNode *right;
    FlightPlanNode *left;
    right = current->children[pos];
    left = current->children[pos-1];
    left->count++;
    left->data[left->count-1] = current->data[pos-1];
    left->children[left->count] = right->children[0];
    for(c=1; c<=right->count; c++)
    {
        left->count++;
        left->data[left->count -1] = right->data[c-1];
        left->children[left->count] = right->children[c];
    }
    for(c=pos; c<current->count; c++)
    {
        current->data[c-1] = current->data[c];
        current->children[c] = current->children[c+1];
    }

    current->count--;
    free(right);
}
void RESTORE_FLIGHT_PLAN(FlightPlanNode *current, int pos)
{
    if(pos == 0)
    {
        if(current->children[1]->count > MIN)
        {
            MOVE_LEFT_FLIGHT_PLAN(current, 1);
        }
        else
        {
            COMBINE_FLIGHT_PLAN(current, 1);
        }
    }
    else if(pos == current->count)
    {
        if(current->children[pos -1]->count > MIN)
        {
            MOVE_RIGHT_FLIGHT_PLAN(current, pos);
        }
        else
        {
            COMBINE_FLIGHT_PLAN(current, pos);
        }
    }
    else if(current->children[pos - 1]->count > MIN)
    {
        MOVE_RIGHT_FLIGHT_PLAN(current, pos);
    }
    else if(current->children[pos + 1]->count > MIN)
    {
        MOVE_LEFT_FLIGHT_PLAN(current, pos+1);
    }
    else
    {
        COMBINE_FLIGHT_PLAN(current, pos);
    }
}
void SUCCESSOR_FLIGHT_PLAN(FlightPlanNode *current, int pos)
{
    FlightPlanNode *leaf;
    for(leaf = current->children[pos + 1]; leaf->children[0] != NULL; leaf = leaf->children[0])
    {

    }
    current->data[pos] = leaf->data[0];
}
void REMOVE_FLIGHT_PLAN(FlightPlanNode *current, int pos)
{
    int i;
    for(i=pos; i<current->count - 1; i++)
    {
        current->data[i] = current->data[i+1];
        current->children[i] = current->children[i+1];
    }
    current->children[i] = current->children[i+1];
    current->count--;
}
void REC_DELETE_TREE_FLIGHT_PLAN(FlightPlan target, FlightPlanNode *current)
{
    int pos;
    if(current != NULL)
    {
        if(SEARCH_FLIGHT_PLAN_NODE(target.departure, current, &pos))
        {
            if(current->children[pos] != NULL)
            {
                SUCCESSOR_FLIGHT_PLAN(current, pos);
                REC_DELETE_TREE_FLIGHT_PLAN(current->data[pos], current->children[pos+1]);
            }
            else
            {
                REMOVE_FLIGHT_PLAN(current, pos);
            }
        }
        else
        {
            REC_DELETE_TREE_FLIGHT_PLAN(target, current->children[pos]);
        }

        if(current->children[pos])
        {
            if(current->children[pos]->count < MIN)
            {
                RESTORE_FLIGHT_PLAN(current, pos);
            }
        }
        if (pos < current->count && current->children[pos+1])
        {
            if(current->children[pos+1]->count < MIN)
            {
                RESTORE_FLIGHT_PLAN(current, pos+1);
            }
        }

    }
}
FlightPlanNode *DELETE_TREE_FLIGHT_PLAN(FlightPlan target, FlightPlanNode *root)
{
    FlightPlanNode *oldRoot;
    REC_DELETE_TREE_FLIGHT_PLAN(target, root);
    if(root->count == 0)
    {
        oldRoot = root;
        root = root->children[0];
        free(oldRoot);
    }
    return  root;
}

// INITIALIZE FUNCTION
void INIT_B_NODE(BucketNode *root)
{
    root->count = 0;
    for(int i=0; i<M; i++)
    {
        root->children[i] = NULL;
    }
}

// SEARCH BUCKET NODE FUNCTIONS
int BINARY_SEARCH_BUCKET(Bucket data[], TIME target, int start, int end)
{
    int RETURN_VALUE;
    if(start <= end)
    {
        int mid = (start + end) / 2;
        if(maxtime(data[mid].beginningETA, target) == 0)
        {
            RETURN_VALUE= mid;
        }
        else if(maxtime(data[mid].beginningETA, target) == -1)
        {
            RETURN_VALUE= BINARY_SEARCH_BUCKET(data, target, start, mid - 1);
        }
        else
        {
            RETURN_VALUE= BINARY_SEARCH_BUCKET(data, target, mid + 1, end);
        }
    }
    else
    {
        RETURN_VALUE= start;
    }
    return RETURN_VALUE;
}
Boolean SEARCH_BUCKET_NODE(TIME target, BucketNode *current, int *pos)
{
    Boolean RETURN_VALUE= FALSE;
    *pos = BINARY_SEARCH_BUCKET(current->data, target, 0, current->count -1);
    if(timedeff(current->data[*pos].beginningETA,target) == 0)
    {
        RETURN_VALUE= TRUE;
    }
    return RETURN_VALUE;
}
BucketNode *SEARCH_BUCKET_TREE(TIME target, BucketNode *root, int *targetPosition)
{
    BucketNode *RETURN_VALUE= NULL;
    if(root != NULL)
    {

        if(SEARCH_BUCKET_NODE(target, root, targetPosition))
        {
            RETURN_VALUE = root;
        }
        else
        {
            RETURN_VALUE = SEARCH_BUCKET_TREE(target, root->children[*targetPosition], targetPosition);
        }
    }
    return RETURN_VALUE;
}

// INSERT BUCKET NODE FUNCTIONS
void PUSH_IN_BUCKET(Bucket median, BucketNode *medianRight, BucketNode *current, int pos)
{
    int i;

    for (i=current->count; i>pos; i--)
    {
        current->data[i] = current->data[i - 1];
        current->children[i + 1] = current->children[i];
    }

    current->data[pos] = median;
    current->children[pos + 1] = medianRight;
    current->count++;
}
void SPLIT_BUCKET_NODE(Bucket median, BucketNode *medianRight, BucketNode *current, int pos, Bucket *newMedian, BucketNode **newMedianRight)
{
    int i;
    int medianIndex;
    if(pos <= MIN)
    {
        medianIndex = MIN;
    }
    else
    {
        medianIndex = MIN + 1;
    }

    *newMedianRight = (BucketNode *)malloc(sizeof(BucketNode));

    for(i = medianIndex; i < MAX; i++)
    {
        (*newMedianRight)->data[i - medianIndex] = current->data[i];
        (*newMedianRight)->children[i - medianIndex + 1] = current->children[i + 1];
    }

    (*newMedianRight)->count = MAX - medianIndex;
    current->count = medianIndex;

    if(pos <= MIN)
    {
        PUSH_IN_BUCKET(median, medianRight, current, pos);
    }
    else
    {
        PUSH_IN_BUCKET(median, medianRight, *newMedianRight, pos - medianIndex);
    }

    *newMedian = current->data[current->count - 1];
    (*newMedianRight)->children[0] = current->children[current->count];

    current->count--;
}
Boolean PUSH_DOWN_BUCKET_NODE(Bucket value, BucketNode *current, Bucket *median, BucketNode **medianRight)
{
    Boolean RETURN_VALUE= FALSE;
    int pos;
    if(current == NULL)
    {
        *median = value;
        *medianRight = NULL;
        RETURN_VALUE= TRUE;
    }
    else
    {
        SEARCH_BUCKET_NODE(value.beginningETA, current, &pos);
        if(PUSH_DOWN_BUCKET_NODE(value, current->children[pos], median, medianRight))
        {
            if(current->count < MAX)
            {
                PUSH_IN_BUCKET(*median, *medianRight, current, pos);
                RETURN_VALUE= FALSE;
            }
            else
            {
                SPLIT_BUCKET_NODE(*median, *medianRight, current, pos, median, medianRight);
                RETURN_VALUE= TRUE;
            }
        }
    }
    return RETURN_VALUE;
}
BucketNode *INSERT_BUCKET_TREE(Bucket value, BucketNode *root)
{
    Bucket median;
    BucketNode *medianRight;
    BucketNode *newRoot;

    BucketNode *RETURN_VALUE = root;

    if(PUSH_DOWN_BUCKET_NODE(value, root, &median, &medianRight))
    {
        newRoot = (BucketNode *)malloc(sizeof(BucketNode));
        newRoot->count = 1;
        newRoot->data[0] = median;
        newRoot->children[0] = root;
        newRoot->children[1] = medianRight;

        RETURN_VALUE= newRoot;
    }
    return  RETURN_VALUE;
}

// BUCKET FILE HANDLING FUNCTIONS
BucketNode *READ_BUCKET(BucketNode *root)
{
    FILE *fptr;
    fptr = fopen("BucketList.dat", "r");
    while(!feof(fptr))
    {
        Bucket node;
        fscanf(fptr, "%d, %d, %d, %d, %d",
               &node.bucketID, &node.beginningETA.hour, &node.beginningETA.min, &node.endETA.hour, &node.endETA.min);
        node.f = NULL;
        root = INSERT_BUCKET_TREE(node, root);
    }
    fclose(fptr);
    return root;
}

// PRINT BUCKET FUNCTIONS
void PRINT_BUCKET_NODE(BucketNode *root)
{
    int i;
    for(i=0; i<root->count; i++)
    {
        printf("\nBUCKET ID : %d",root->data[i].bucketID);
        printf("\nBEG ETA INTERVAL - ");
        printTime(root->data[i].beginningETA);
        printf("\nEND ETA INTERVAL - ");
        printTime(root->data[i].endETA);
        //PRINT_FLIGHT_PLAN_TREE(root->data[i].f);
    }
}
void PRINT_BUCKET_TREE(BucketNode *root)
{
    if(root != NULL)
    {
        PRINT_BUCKET_NODE(root);
        printf("\n-----------------\n");
        for(int i=0; i<=root->count; i++)
        {
            PRINT_BUCKET_TREE(root->children[i]);
        }
    }
}
void PRINT_BUCKET_INORDER(BucketNode *root)
{
    if(root != NULL)
    {
        for(int i=0; i<root->count; i++)
        {
            PRINT_BUCKET_INORDER(root->children[i]);
            printf("\n--------------------------");
            printf("\nBUCKET ID : %d",root->data[i].bucketID);
            printf("\nBEG ETA INTERVAL - ");
            printTime(root->data[i].beginningETA);
            printf("\nEND ETA INTERVAL - ");
            printTime(root->data[i].endETA);
            printf("\n---------------------------");
            PRINT_FLIGHT_PLAN_INORDER(root->data[i].f);
            PRINT_BUCKET_INORDER(root->children[i+1]);
        }
    }
}

// DELETE BUCKET FUNCTIONS
void MOVE_RIGHT_BUCKET(BucketNode *current, int pos)
{
    int c;
    BucketNode *t;
    t = current->children[pos];
    for(c = t->count; c>0; c--)
    {
        t->data[c] = t->data[c-1];
        t->children[c+1] = t->children[c];
    }
    t->children[1] = t->children[0];
    t->count++;
    t->data[0] = current->data[pos -1];
    t = current->children[pos - 1];
    current->data[pos - 1] = t->data[t->count - 1];
    current->children[pos]->children[0] = t->children[t->count];
    t->count--;
}
void MOVE_LEFT_BUCKET(BucketNode *current, int pos)
{
    int c;
    BucketNode *t;

    t=current->children[pos-1];
    t->count++;
    t->data[t->count-1] = current->data[pos -1];
    t->children[t->count] = (current->children[pos])->children[0];
    t = current->children[pos];
    current->data[pos-1] = t->data[0];
    t->children[0] = t->children[1];
    t->count--;
    for(c=1; c<=t->count; c++)
    {
        t->data[c-1] = t->data[c];
        t->children[c] = t->children[c+1];
    }
}
void RESTORE_BUCKET(BucketNode *current, int pos)
{
    if(pos == 0)
    {
        if(current->children[1]->count > MIN)
        {
            MOVE_LEFT_BUCKET(current, 1);
        }
        else
        {
            COMBINE_BUCKET(current, 1);
        }
    }
    else if(pos == current->count)
    {
        if(current->children[pos -1]->count > MIN)
        {
            MOVE_RIGHT_BUCKET(current, pos);
        }
        else
        {
            COMBINE_BUCKET(current, pos);
        }
    }
    else if(current->children[pos - 1]->count > MIN)
    {
        MOVE_RIGHT_BUCKET(current, pos);
    }
    else if(current->children[pos + 1]->count > MIN)
    {
        MOVE_LEFT_BUCKET(current, pos+1);
    }
    else
    {
        COMBINE_BUCKET(current, pos);
    }
}
void COMBINE_BUCKET(BucketNode *current, int pos)
{
    int c;
    BucketNode *right;
    BucketNode *left;
    right = current->children[pos];
    left = current->children[pos-1];
    left->count++;
    left->data[left->count-1] = current->data[pos-1];
    left->children[left->count] = right->children[0];
    for(c=1; c<=right->count; c++)
    {
        left->count++;
        left->data[left->count -1] = right->data[c-1];
        left->children[left->count] = right->children[c];
    }
    for(c=pos; c<current->count; c++)
    {
        current->data[c-1] = current->data[c];
        current->children[c] = current->children[c+1];
    }

    current->count--;
    free(right);

}
void SUCCESSOR_BUCKET(BucketNode *current, int pos)
{
    BucketNode *leaf;
    for(leaf = current->children[pos + 1]; leaf->children[0] != NULL; leaf = leaf->children[0])
    {

    }
    current->data[pos] = leaf->data[0];
}
void REMOVE_BUCKET(BucketNode *current, int pos)
{
    int i;
    for(i=pos; i<current->count - 1; i++)
    {
        current->data[i] = current->data[i+1];
        current->children[i] = current->children[i+1];
    }
    current->children[i] = current->children[i+1];
    current->count--;
}
void REC_DELETE_TREE_BUCKET(Bucket target, BucketNode *current)
{
    int pos;
    if(current != NULL)
    {
        if(SEARCH_BUCKET_NODE(target.beginningETA, current, &pos))
        {
            if(current->children[pos] != NULL)
            {
                SUCCESSOR_BUCKET(current, pos);
                REC_DELETE_TREE_BUCKET(current->data[pos], current->children[pos+1]);
            }
            else
            {
                REMOVE_BUCKET(current, pos);
            }
        }
        else
        {
            REC_DELETE_TREE_BUCKET(target, current->children[pos]);
        }

        if(current->children[pos])
        {
            if(current->children[pos]->count < MIN)
            {
                RESTORE_BUCKET(current, pos);
            }
        }
        if (pos < current->count && current->children[pos+1])
        {
            if(current->children[pos+1]->count < MIN)
            {
                RESTORE_BUCKET(current, pos+1);
            }
        }

    }
}
BucketNode *DELETE_TREE_BUCKET(Bucket target, BucketNode *root)
{
    BucketNode *oldRoot;
    REC_DELETE_TREE_BUCKET(target, root);
    if(root->count == 0)
    {
        oldRoot = root;
        root = root->children[0];
        free(oldRoot);
    }
    return  root;
}

// IMPORTANT FUNCTIONS
BucketNode *INSERT_FLIGHT_PLAN_INTO_BUCKET(BucketNode *root, FlightPlan plan)
{
    TIME temp;
    BucketNode *current;
    int pos;

    temp.hour = plan.ETA.hour;
    temp.min = 0;

    current = SEARCH_BUCKET_TREE(temp, root, &pos);
    if(current == NULL)
    {
        Bucket node;
        node.bucketID = random++;
        node.beginningETA.hour = temp.hour;
        node.beginningETA.min = 0;
        node.endETA.hour = temp.hour;
        node.endETA.min = 59;
        node.f = NULL;
        node.f = INSERT_FLIGHT_PLAN_TREE(plan, node.f);
        root = INSERT_BUCKET_TREE(node, root);
    }
    else
    {
        current->data[pos].f =  INSERT_FLIGHT_PLAN_TREE(plan, current->data[pos].f);
    }
    return  root;
}
BucketNode *READ_FLIGHT_PLAN_INTO_BUCKET(BucketNode *root)
{
    FILE *fptr;
    fptr = fopen("FlightPlans.dat", "r");
    while(!feof(fptr))
    {
        FlightPlan plan;
        fscanf(fptr, "%d, %d, %d, %d, %d",
               &plan.flightID, &plan.departure.hour, &plan.departure.min, &plan.ETA.hour, &plan.ETA.min);
        root = INSERT_FLIGHT_PLAN_INTO_BUCKET(root, plan);
    }
    fclose(fptr);
    return root;
}
void SEARCH_GIVEN_DEPARTURE(BucketNode *current, TIME departureTime, int *found, FlightPlanNode **node, BucketNode **bucket, int *planPosition, int *bucketPosition)
{
    FlightPlanNode *plan;
    int pos;
    if(*found == 0 && current != NULL)
    {
        for(int i=0; i<current->count && *found == 0; i++)
        {
            SEARCH_GIVEN_DEPARTURE(current->children[i], departureTime, found, node, bucket, planPosition, bucketPosition);
            plan = SEARCH_FLIGHT_PLAN_TREE(departureTime, current->data[i].f, &pos);
            if(plan != NULL)
            {
                *bucket = current;
                *planPosition = pos;
                *found = 1;
                *bucketPosition = i;
                *node = plan;
            }
            SEARCH_GIVEN_DEPARTURE(current->children[i+1], departureTime, found, node, bucket, planPosition, bucketPosition);
        }
    }
}
void SEARCH_GIVEN_ETA(FlightPlanNode *current, int ID, int *found, FlightPlan *plan)
{
    if(*found == 0 && current != NULL)
    {
        for(int i=0; i<current->count && *found == 0; i++)
        {
            SEARCH_GIVEN_ETA(current->children[i], ID, found, plan);
            if(current->data[i].flightID == ID)
            {
                *found = 1;
                *plan = current->data[i];
            }
            SEARCH_GIVEN_ETA(current->children[i+1], ID, found, plan);
        }
    }
}
void SEARCH_GIVEN_ID_PLAN(FlightPlanNode *root, int ID, FlightPlan *plan, int *found, FlightPlanNode **flight, int *flightPosition)
{
    if(*found == 0 && root != NULL)
    {
        for(int i=0; i<root->count && *found == 0; i++)
        {
            SEARCH_GIVEN_ID_PLAN(root->children[i], ID, plan, found, flight, flightPosition);
            if(root->data[i].flightID == ID)
            {
                *found = 1;
                *plan = root->data[i];
                *flightPosition = i;
            }
            SEARCH_GIVEN_ID_PLAN(root->children[i+1], ID, plan, found, flight, flightPosition);
        }
    }
}
void SEARCH_GIVEN_ID(BucketNode *root, int ID, FlightPlan *plan, int *found, BucketNode **bucket, int *bucketPosition, FlightPlanNode **flight, int *flightPosition)
{
    if(*found == 0 && root != NULL)
    {
        for(int i=0; i<root->count && *found == 0; i++)
        {
            SEARCH_GIVEN_ID(root->children[i], ID, plan, found, bucket, bucketPosition, flight, flightPosition);
            SEARCH_GIVEN_ID_PLAN(root->data[i].f, ID, plan, found, flight, flightPosition);
            if(*found == 1)
            {
                *bucket = root;
                *bucketPosition = i;
                *flight = root->data[*bucketPosition].f;
            }
            SEARCH_GIVEN_ID(root->children[i+1], ID, plan, found,bucket,bucketPosition, flight, flightPosition);
        }
    }
}
void SHOW_STATUS(BucketNode *root, int flightID, TIME departureTime, TIME ETA)
{
    BucketNode *bucket;
    FlightPlanNode *plan;
    int bucketPosition;
    int planPosition;
    int found = 0;
    TIME temp;

    if(departureTime.hour >= 0 && ETA.hour >= 0)
    {
        temp.hour = ETA.hour;
        temp.min = 0;
        bucket = SEARCH_BUCKET_TREE(temp, root,&bucketPosition);

        if(bucket != NULL)
        {
            plan = SEARCH_FLIGHT_PLAN_TREE(departureTime, bucket->data[bucketPosition].f, &planPosition);
            if(plan != NULL && plan->data[planPosition].flightID == flightID)
            {
                printf("\nFLIGHT ID - %d",plan->data[planPosition].flightID);
                printf("\nDEPARTURE TIME - ");
                printTime(plan->data[planPosition].departure);
                printf("\nETA %10s - ", "");
                printTime(plan->data[planPosition].ETA);
            }
            else
            {
                printf("\nFLIGHT PLAN NOT FOUND.");
            }
        }
        else
        {
            printf("\nFLIGHT PLAN NOT FOUND.");
        }
    }
    else if (departureTime.hour >= 0)
    {
        SEARCH_GIVEN_DEPARTURE(root, departureTime, &found, &plan, &bucket,&planPosition, &bucketPosition);
        if(found && plan->data[planPosition].flightID == flightID)
        {
            printf("\nFLIGHT ID - %d",plan->data[planPosition].flightID);
            printf("\nDEPARTURE TIME - ");
            printTime(plan->data[planPosition].departure);
            printf("\nETA %10s - ", "");
            printTime(plan->data[planPosition].ETA);
        }
        else
        {
            printf("\nFLIGHT PLAN NOT FOUND.");
        }
    }
    else if (ETA.hour >= 0)
    {
        FlightPlan p;
        temp.hour = ETA.hour;
        temp.min = 0;

        bucket = SEARCH_BUCKET_TREE(temp, root, &bucketPosition);
        if(bucket != NULL)
        {
            SEARCH_GIVEN_ETA(bucket->data[bucketPosition].f, flightID, &found, &p);
            if(found)
            {
                printf("\nFLIGHT ID - %d",p.flightID);
                printf("\nDEPARTURE TIME - ");
                printTime(p.departure);
                printf("\nETA %10s - ", "");
                printTime(p.ETA);
            }
        }
        else
        {
            printf("\nFLIGHT PLAN NOT FOUND.");
        }
    }
    else
    {
        FlightPlan p;
        SEARCH_GIVEN_ID(root, flightID, &p, &found, &bucket, &bucketPosition, &plan, &planPosition);
        if(found)
        {
            printf("\nFLIGHT ID - %d",p.flightID);
            printf("\nDEPARTURE TIME - ");
            printTime(p.departure);
            printf("\nETA %10s - ", "");
            printTime(p.ETA);
        }
        else
        {
            printf("\nFLIGHT PLAN NOT FOUND.");
        }

    }
}
BucketNode *DELETE_PLAN(BucketNode *root, int flightID, TIME departureTime, TIME ETA)
{
    BucketNode *bucket;
    FlightPlanNode *plan;
    int bucketPosition;
    int planPosition;
    int found = 0;
    TIME temp;

    if(departureTime.hour >= 0 && ETA.hour >= 0)
    {
        temp.hour = ETA.hour;
        temp.min = 0;
        bucket = SEARCH_BUCKET_TREE(temp, root,&bucketPosition);

        if(bucket != NULL)
        {
            plan = SEARCH_FLIGHT_PLAN_TREE(departureTime, bucket->data[bucketPosition].f, &planPosition);
            if(plan != NULL && plan->data[planPosition].flightID == flightID)
            {
                bucket->data[bucketPosition].f = DELETE_TREE_FLIGHT_PLAN(plan->data[planPosition], plan);
                if(bucket->data[bucketPosition].f == NULL)
                {
                    root = DELETE_TREE_BUCKET(bucket->data[bucketPosition], root);
                }
            }
            else
            {
                printf("\nFLIGHT PLAN NOT FOUND.");
            }
        }
        else
        {
            printf("\nFLIGHT PLAN NOT FOUND.");
        }
    }
    else if (departureTime.hour >= 0)
    {
        SEARCH_GIVEN_DEPARTURE(root, departureTime, &found, &plan, &bucket, &planPosition, &bucketPosition);
        if(found && plan->data[planPosition].flightID == flightID)
        {
            if(plan != NULL && plan->data[planPosition].flightID == flightID)
            {
                bucket->data[bucketPosition].f = DELETE_TREE_FLIGHT_PLAN(plan->data[planPosition], plan);
                if(bucket->data[bucketPosition].f == NULL)
                {
                    root = DELETE_TREE_BUCKET(bucket->data[bucketPosition], root);
                }
            }
            else
            {
                printf("\nFLIGHT PLAN NOT FOUND.");
            }
        }
        else
        {
            printf("\nFLIGHT PLAN NOT FOUND.");
        }
    }
    else if (ETA.hour >= 0)
    {
        FlightPlan p;
        temp.hour = ETA.hour;
        temp.min = 0;

        bucket = SEARCH_BUCKET_TREE(temp, root, &bucketPosition);
        if(bucket != NULL)
        {
            SEARCH_GIVEN_ETA(bucket->data[bucketPosition].f, flightID, &found, &p);
            if(found)
            {
                bucket->data[bucketPosition].f = DELETE_TREE_FLIGHT_PLAN(p, bucket->data[bucketPosition].f);
                if(bucket->data[bucketPosition].f)
                {
                    root = DELETE_TREE_BUCKET(bucket->data[bucketPosition], root);
                }
            }
        }
        else
        {
            printf("\nFLIGHT PLAN NOT FOUND.");
        }
    }
    else
    {
        FlightPlan p;
        SEARCH_GIVEN_ID(root, flightID, &p, &found, &bucket, &bucketPosition, &plan, &planPosition);
        if(found)
        {

            bucket->data[bucketPosition].f = DELETE_TREE_FLIGHT_PLAN(p, bucket->data[bucketPosition].f);
            if(bucket->data[bucketPosition].f == NULL)
            {
                root = DELETE_TREE_BUCKET(bucket->data[bucketPosition], root);

            }
        }
        else
        {
            printf("\nFLIGHT PLAN NOT FOUND.");
        }

    }
    return  root;
}

