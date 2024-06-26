#ifndef IMPLEMENTATION_H
#define IMPLEMENTATION_H

#include <stdio.h>
#include <stdlib.h>


typedef enum{FAILURE, SUCCESS} STATUS_CODE;
typedef enum{FALSE, TRUE} Boolean;

#define M 5
#define MAX 4
#define MIN 2


/* TIME structure */
typedef struct time_tag
{
    int min;
    int hour;
} TIME;


typedef struct flight_plan_tag
{
    int flightID;
    TIME departure;
    TIME ETA;
}FlightPlan;

typedef struct flight_plan_node_tag
{
    FlightPlan data[MAX];
    int count;
    struct flight_plan_node_tag *children[M];
}FlightPlanNode;

typedef struct bucket_tag
{
    FlightPlanNode *f;
    int bucketID;
    TIME beginningETA;
    TIME endETA;
}Bucket;

typedef struct bucket_node_tag
{
    Bucket data[MAX];
    int count;
    struct bucket_node_tag *children[M];
}BucketNode;



// TIME FUNCTIONS
int maxtime(TIME A, TIME B);
int timedeff(TIME A, TIME B);
void printTime(TIME t);


// INITIALIZE FUNCTIONS
void INIT_F_NODE(FlightPlanNode *root);

// SEARCH FLIGHT PLAN NODE FUNCTIONS
int BINARY_SEARCH_FLIGHT_PLAN(FlightPlan data[], TIME target, int start, int end);
Boolean SEARCH_FLIGHT_PLAN_NODE(TIME target, FlightPlanNode *current, int *pos);
FlightPlanNode *SEARCH_FLIGHT_PLAN_TREE(TIME target, FlightPlanNode *root, int *targetPosition);

// INSERT FLIGHT PLAN NODE FUNCTIONS
void PUSH_IN_FLIGHT_PLAN(FlightPlan median, FlightPlanNode *medianRight, FlightPlanNode *current, int pos);
void SPLIT_FLIGHT_PLAN_NODE(FlightPlan median, FlightPlanNode *medianRight, FlightPlanNode *current, int pos, FlightPlan *newMedian, FlightPlanNode **newMedianRight);
Boolean PUSH_DOWN_FLIGHT_PLAN_NODE(FlightPlan value, FlightPlanNode *current, FlightPlan *median, FlightPlanNode **medianRight);
FlightPlanNode *INSERT_FLIGHT_PLAN_TREE(FlightPlan value, FlightPlanNode *root);

// FLIGHT PLAN FILE HANDLING FUNCTIONS
FlightPlanNode *READ_FLIGHT_PLAN(FlightPlanNode *root);

// PRINT FLIGHT PLAN FUNCTIONS
void PRINT_FLIGHT_PLAN_NODE(FlightPlanNode *root);
void PRINT_FLIGHT_PLAN_TREE(FlightPlanNode *root);
void PRINT_FLIGHT_PLAN_INORDER(FlightPlanNode *root);

// DELETE FLIGHT PLAN FUNCTIONS
void MOVE_RIGHT_FLIGHT_PLAN(FlightPlanNode *current, int pos);
void MOVE_LEFT_FLIGHT_PLAN(FlightPlanNode *current, int pos);
void COMBINE_FLIGHT_PLAN(FlightPlanNode *current, int pos);
void RESTORE_FLIGHT_PLAN(FlightPlanNode *current, int pos);
void SUCCESSOR_FLIGHT_PLAN(FlightPlanNode *current, int pos);
void REMOVE_FLIGHT_PLAN(FlightPlanNode *current, int pos);
void REC_DELETE_TREE_FLIGHT_PLAN(FlightPlan target, FlightPlanNode *current);
FlightPlanNode *DELETE_TREE_FLIGHT_PLAN(FlightPlan target, FlightPlanNode *root);

// GRAPHVIZ

// INITIALIZE FUNCTIONS
void INIT_B_NODE(BucketNode *root);

// SEARCH BUCKET NODE FUNCTIONS
int BINARY_SEARCH_BUCKET(Bucket data[], TIME target, int start, int end);
Boolean SEARCH_BUCKET_NODE(TIME target, BucketNode *current, int *pos);
BucketNode *SEARCH_BUCKET_TREE(TIME target, BucketNode *root, int *targetPosition);

// INSERT BUCKET NODE FUNCTIONS
void SPLIT_BUCKET_NODE(Bucket median, BucketNode *medianRight, BucketNode *current, int pos, Bucket *newMedian, BucketNode **newMedianRight);
void PUSH_IN_BUCKET(Bucket median, BucketNode *medianRight, BucketNode *current, int pos);
Boolean PUSH_DOWN_BUCKET_NODE(Bucket value, BucketNode *current, Bucket *median, BucketNode **medianRight);
BucketNode *INSERT_BUCKET_TREE(Bucket value, BucketNode *root);

// BUCKET FILE HANDLING FUNCTIONS
BucketNode *READ_BUCKET(BucketNode *root);

// PRINT BUCKET FUNCTIONS
void PRINT_BUCKET_NODE(BucketNode *root);
void PRINT_BUCKET_TREE(BucketNode *root);
void PRINT_BUCKET_INORDER(BucketNode *root);

// DELETE BUCKET FUNCTIONS
void MOVE_RIGHT_BUCKET(BucketNode *current, int pos);
void MOVE_LEFT_BUCKET(BucketNode *current, int pos);
void COMBINE_BUCKET(BucketNode *current, int pos);
void RESTORE_BUCKET(BucketNode *current, int pos);
void SUCCESSOR_BUCKET(BucketNode *current, int pos);
void REMOVE_BUCKET(BucketNode *current, int pos);
void REC_DELETE_TREE_BUCKET(Bucket target, BucketNode *current);
BucketNode *DELETE_TREE_BUCKET(Bucket target, BucketNode *root);

// IMPORTANT FUNCTIONS
BucketNode *INSERT_FLIGHT_PLAN_INTO_BUCKET(BucketNode *root, FlightPlan plan);
BucketNode *READ_FLIGHT_PLAN_INTO_BUCKET(BucketNode *root);
void SEARCH_GIVEN_DEPARTURE(BucketNode *current, TIME departureTime, int *found, FlightPlanNode **node, BucketNode **bucket, int *planPosition, int *bucketPosition);
void SEARCH_GIVEN_ETA(FlightPlanNode *current, int ID, int *found, FlightPlan *plan);
void SEARCH_GIVEN_ID_PLAN(FlightPlanNode *root, int ID, FlightPlan *plan, int *found, FlightPlanNode **flight, int *flightPosition);
void SEARCH_GIVEN_ID(BucketNode *root, int ID, FlightPlan *plan, int *found, BucketNode **bucket, int *bucketPosition, FlightPlanNode **flight, int *flightPosition);
void SHOW_STATUS(BucketNode *root, int flightID, TIME departureTime, TIME ETA);
void SEARCH_FLIGHT_TIME_INORDER(FlightPlanNode *root, TIME minimum, TIME maximum, int *done);
void SEARCH_BUCKET_TIME_INORDER(BucketNode *root, TIME minimum, TIME maximum, int *done);
BucketNode *CANCEL_PLAN(BucketNode *root, int flightID, TIME departureTime, TIME ETA);

#endif