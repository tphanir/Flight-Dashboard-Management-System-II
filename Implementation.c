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
void SEARCH_GIVEN_DEPARTURE(BucketNode *current, TIME departureTime, int *found, FlightPlanNode **node, int *position)
{
    FlightPlanNode *plan;
    int pos;
    if(*found == 0 && current != NULL)
    {
        for(int i=0; i<current->count; i++)
        {
            SEARCH_GIVEN_DEPARTURE(current->children[i], departureTime, found, node, position);
            plan = SEARCH_FLIGHT_PLAN_TREE(departureTime, current->data[i].f, &pos);
            if(plan != NULL)
            {
                *position = pos;
                *found = 1;
                *node = plan;
            }
            SEARCH_GIVEN_DEPARTURE(current->children[i+1], departureTime, found, node, position);
        }
    }
}
void SEARCH_GIVEN_ETA(FlightPlanNode *current, int ID, int *found, FlightPlan *plan)
{
    if(*found == 0 && current != NULL)
    {
        for(int i=0; i<current->count; i++)
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
void SEARCH_GIVEN_ID_PLAN(FlightPlanNode *root, int ID, FlightPlan *plan, int *found)
{
    if(*found == 0 && root != NULL)
    {
        for(int i=0; i<root->count; i++)
        {
            SEARCH_GIVEN_ID_PLAN(root->children[i], ID, plan, found);
            if(root->data[i].flightID == ID)
            {
                *found = 1;
                *plan = root->data[i];
            }
            SEARCH_GIVEN_ID_PLAN(root->children[i+1], ID, plan, found);
        }
    }
}
void SEARCH_GIVEN_ID(BucketNode *root, int ID, FlightPlan *plan, int *found)
{
    if(*found == 0 && root != NULL)
    {
        for(int i=0; i<root->count; i++)
        {
            SEARCH_GIVEN_ID(root->children[i], ID, plan, found);
            SEARCH_GIVEN_ID_PLAN(root->data[i].f, ID, plan, found);
            SEARCH_GIVEN_ID(root->children[i+1], ID, plan, found);
        }
    }
}
Boolean SHOW_STATUS(BucketNode *root, int flightID, TIME departureTime, TIME ETA)
{
    BucketNode *bucket;
    FlightPlanNode *plan;
    int bucketPosition;
    int planPosition;
    int found = 0;
    TIME temp;

    if(departureTime.hour >= 0 && ETA.hour >= 1)
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
        SEARCH_GIVEN_DEPARTURE(root, departureTime, &found, &plan, &planPosition);
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
        SEARCH_GIVEN_ID(root, flightID, &p, &found);
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
    return TRUE;
}
