#include <stdio.h>
#include <stdlib.h>

#define FALSE 0
#define TRUE 1
#define MAX_NODE_COUNT 10


typedef struct node {
    int index;
    int flag;
    struct edge* firstEdge;
} Node;

typedef struct edge {
    struct node* toNodeAddress;
    struct edge* nextEdge;
    // int weight;
} Edge;


Node* InitializeGraph(Node*);
void InsertVertex();
void InsertEdge();
int DFS();
int BFS();
void PrintGraph(Node*, int);
void FreeGraph(Node*);



int main() {
    char command;
    int countVertices = 0;
    Node* graph = NULL;
    

    printf("[----- [정준호]  [2020069046] -----]\n");
    
    do
    {
        printf("\n----------------------------------------------------------------\n");
        printf("                         Graph Searches                         \n");
        printf("----------------------------------------------------------------\n");
        printf("  Initialize Graph = z\n");
        printf("  Insert Vertex = v\t\tInsert Edge = e\n");
        printf("  Depth First Search = d\tBreath First Search = b\n");
        printf("  Print Graph = p\t\tQuit = q\n");
        printf("----------------------------------------------------------------\n");
        printf("command >>> ");
        
        scanf("%c", &command);

        if (getchar() != '\n') {
            while (getchar() != '\n');
            printf("Wrong Input\n");
            continue;
        }

        
        switch (command) {
        case 'z':
        case 'Z':
            graph = InitializeGraph(graph);
            break;
        case 'v':
        case 'V':
            if (countVertices == 10) {
                printf("!! The graph is full. !!\n");
                break;
            }
            InsertVertex(graph, countVertices);
            countVertices++;
            break;
        case 'e':
        case 'E':
            InsertEdge();
            break;
        case 'd':
        case 'D':
            DFS();
            break;
        case 'b':
        case 'B':
            BFS();
            break;
        case 'p':
        case 'P':
            PrintGraph(graph, countVertices);
            break;
        case 'q':
        case 'Q':
            graph = InitializeGraph(graph);
            break;
        default:
            printf("\'%c\' is invalid command.\n", command);
            break;
        }
    } while (command != 'q' && command != 'Q');

    printf("Good Bye.\n");
    return 0;
}

Node* InitializeGraph(Node* graph) {
    if (graph != NULL) {
        FreeGraph(graph);
    }

    graph = (Node*) malloc(MAX_NODE_COUNT * sizeof(Node));
    for (int i = 0; i < MAX_NODE_COUNT; i++)
        graph[i].index = -1;        // node with indexed '-1' is inactive node;

    return graph;
};

void InsertVertex(Node* graph, int index) {
    graph[index].index = index;
    graph[index].flag = 0;
    graph[index].firstEdge = NULL;
    printf("Vertex[%d] successfully inserted.\n", index);
};

void InsertEdge(Node* graph) {
    int node1, node2;
    Edge* newEdge1,* newEdge2;
    Edge* ptr;
    int i;


    printf("INPUT two nodes to be connected (0 ~ 9) : ");
    scanf("%d %d", &node1, &node2);


    if (((node1 < 0) || (node1 > 9)) || ((node2 < 0) || (node2 > 9))) {
        printf("!! Wrong Input. Please input 2 numbers ranged 0 ~ 9. !!\n");
        return;
    }

    if ((graph[node1].index == -1) || (graph[node2].index == -1)) {
        printf("!! There a node(s) that doesn't exist !!\n");
        return;
    }

    newEdge1 = (Edge*) malloc(sizeof(Edge));
    newEdge1->toNodeAddress = &graph[node2];
    newEdge1->nextEdge = NULL;


    newEdge2 = (Edge*) malloc(sizeof(Edge));
    newEdge2->toNodeAddress = &graph[node1];
    newEdge2->nextEdge = NULL;


    if (graph[node1].firstEdge == NULL) {
        graph[node1].firstEdge = newEdge1;
    }
    else {
        ptr = graph[node1].firstEdge;
        while (ptr != NULL) {
            if (ptr->toNodeAddress->index == node2) {
                printf("!! The edge already exits. !!\n");
                return;
            }
            ptr = ptr->nextEdge;
        }
        ptr->nextEdge = newEdge1;
    }

    if (graph[node2].firstEdge == NULL) {
        graph[node2].firstEdge = newEdge2;
    }
    else {
        ptr = graph[node2].firstEdge;
        while (ptr != NULL) {
            ptr = ptr->nextEdge;
        }
        ptr->nextEdge = newEdge2;
    }
};

int DFS() {

};

int BFS() {

};

void PrintGraph(Node* graph, int count) {
    int i;
    Edge* edge_ptr;

    printf("--- Printing graph in Adjacent List representation.\n\n");
    for (i = 0; i < count; i++) {
        printf("[ %2d ]", graph[i].index);
        edge_ptr = graph[i].firstEdge;
        while (edge_ptr != NULL) {
            printf(" -> [ %2d ]", edge_ptr->toNodeAddress->index);
            edge_ptr = edge_ptr->nextEdge;
        }
        printf("\n");
    }
};

void FreeGraph(Node* graph) {
    Node* node_ptr;
    Edge* edge_ptr,* edge_prev;

    int i;

    for (i = 0; i < MAX_NODE_COUNT; i++) {
        node_ptr = graph + i;
        edge_ptr = node_ptr->firstEdge;
        while (edge_ptr != NULL) {
            edge_prev = edge_ptr;
            edge_ptr = edge_ptr->nextEdge;
            free(edge_prev);
        }
        free(node_ptr);
    }
}