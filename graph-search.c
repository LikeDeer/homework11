#include <stdio.h>
#include <stdlib.h>

#define FALSE 0
#define TRUE 1
#define MAX_NODE_COUNT 10

/*****************************************
 * file name    graph-search.c
 * author       정준호
 * last update  2022. 05. 29
 * description 
     2022 자료구조 homework11. 그래프 및 연산(DFS, BFS) 구현.
     종류는 무방향 그래프를 선택하였고, 표현은 인접 리스트 형태이다.
     노드와 간선의 구조는 아래 참고.
     생성 방법
      노드 : command = v 를 입력시 -> 0 ~ 9로 순차적으로 생성
      간선 : command = e, 활성화된 노드 2개를 입력시 -> 양방향으로 간선 2개 생성
     DFS, BFS 는 강의자료의 이론을 참고하여 임의로 구현했습니다. text-book을 참고하지 않은 점 양해바랍니다.
 *****************************************/


/* 정점(노드)의 자료구조 */
typedef struct node {
    int index;                      // 노드의 번호 (0 ~ 9)
    int flag;                       // TRUE or FALSE (DFS, BFS 에서 사용)
    struct edge* firstEdge;         // 노드에서 첫 연결
} Node;


/* 간선의 자료구조 */
typedef struct edge {
    struct node* toNodeAddress;     // 해당 간선의 목적지 노드
    struct edge* nextEdge;          // 노드에 연결된 또 다른 간선
    // int weight;
} Edge;


Node* InitializeGraph(Node*);       // 그래프 초기화
void InsertVertex(Node*, int);      // 노드 삽입
void InsertEdge(Node*);             // 간선 삽입
void DFS(Node*);                    // 깊이 우선 탐색
void BFS(Node*);                    // 너비 우선 탐색
void PrintGraph(Node*, int);        // 그래프 출력 (인접 리스트 형태)
void FreeGraph(Node*);              // 노드, 간선에 할당된 메모리 free

/* DFS 용 스택 관련 함수 */
 // 스택은 DFS 연산 도중 생성
void Push(Node**, int*, Node*);
Node* Pop(Node**, int*);

/* BFS 용 큐 관련 함수 */
 // 큐는 BFS 연산 도중 생성
void enQueue(int*, int*, int);
int deQueue(int*, int*);

/* DFS, BFS 후 flag 다시 FALSE로 초기화 */
void InitFlags(Node*, int);


int main() {
    char command;
    int countVertices = 0;          // 생성된 노드의 개수 카운트
    Node* graph = NULL;             // graph : 노드 구조체 10개 배열 (InitializeGraph 참고)
    

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

        /* 잘못된 command 입력 시 오류 처리 */
        if (getchar() != '\n') {
            while (getchar() != '\n');
            printf("Wrong Input\n");
            continue;
        }

        
        switch (command) {
        case 'z':
        case 'Z':
            graph = InitializeGraph(graph);
            countVertices = 0;
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
            InsertEdge(graph);
            break;
        case 'd':
        case 'D':
            DFS(graph);
            InitFlags(graph, countVertices);
            break;
        case 'b':
        case 'B':
            BFS(graph);
            InitFlags(graph, countVertices);
            break;
        case 'p':
        case 'P':
            PrintGraph(graph, countVertices);
            break;
        case 'q':
        case 'Q':
            FreeGraph(graph);
            break;
        default:
            printf("\'%c\' is invalid command.\n", command);        // ">>>>  Concentration !!  <<<<"
            break;
        }
    } while (command != 'q' && command != 'Q');

    printf("Good Bye.\n");          // 스윗한 작별인사 멘트
    return 0;
}


Node* InitializeGraph(Node* graph) {
    if (graph != NULL) {            // 공백 그래프가 아니면 free 먼저
        FreeGraph(graph);
    }

    graph = (Node*) malloc(MAX_NODE_COUNT * sizeof(Node));  // graph : 노드 10개 배열 + 나중에 각 노드에 연결된 간선 리스트
    for (int i = 0; i < MAX_NODE_COUNT; i++) {
        graph[i].index= -1;        // index 가 -1 인 노드는 비활성화된 노드
    }

    return graph;
};


void InsertVertex(Node* graph, int index) {
    graph[index].index = index;
    graph[index].flag = FALSE;
    graph[index].firstEdge = NULL;
    printf("Vertex[%d] successfully inserted.\n", index);
};


void InsertEdge(Node* graph) {
    int node1, node2;
    Edge* newEdge1,* newEdge2;
    Edge* ptr;
    int i;


    /* 간선 입력 부분 (u, v) */
    printf("INPUT two nodes to be connected (0 ~ 9) : ");
    scanf("%d %d", &node1, &node2);
    while (getchar() != '\n');


    /* 오류 확인 부분 */
    if (node1 == node2) {       // 입력 간선이 self-loop 인지 확인 (허용되지 않음)
        printf("!! Self-loop is not permitted. !!\n");
        return;
    }

    if (((node1 < 0) || (node1 > 9)) || ((node2 < 0) || (node2 > 9))) {     // 범위를 벗어난 입력 확인
        printf("!! Wrong Input. Please input 2 numbers ranged 0 ~ 9. !!\n");
        return;
    }

    if ((graph[node1].index == -1) || (graph[node2].index == -1)) {         // 노드 존재 여부 확인
        printf("!! There a node(s) that doesn't exist !!\n");
        return;
    }


    /* 생성 부분 */
     // 이 프로그램의 그래프는 undirected-graph
    newEdge1 = (Edge*) malloc(sizeof(Edge));
    newEdge1->toNodeAddress = &graph[node2];
    newEdge1->nextEdge = NULL;


    newEdge2 = (Edge*) malloc(sizeof(Edge));
    newEdge2->toNodeAddress = &graph[node1];
    newEdge2->nextEdge = NULL;


    /* 삽입 부분 */
     // 삽입은 해당 노드의 리스트열의 끝에 삽입되어야 함.
    /* newEdge1 삽입 */
    if (graph[node1].firstEdge == NULL) {
        graph[node1].firstEdge = newEdge1;
    }
    else {
        ptr = graph[node1].firstEdge;                       // (0) 첫 간선으로 가서,

        do {
            if (ptr->toNodeAddress->index == node2) {       // (1) 이미 존재하는 간선인지 확인하고,
                printf("!! The edge already exists. !!\n");
                return;
            }
            if (ptr->nextEdge == NULL)                          // (끝에 다다르면 반복문 탈출)
                break;
            ptr = ptr->nextEdge;                            // (2) 다음 간선으로 이동
        } while (1);

        ptr->nextEdge = newEdge1;                           // (3) 리스트열의 끝에 삽입
    }
    /* newEdge2 삽입. newEdge1과 반대 방향 간선을 의미 */
    if (graph[node2].firstEdge == NULL) {
        graph[node2].firstEdge = newEdge2;
    }
    else {
        ptr = graph[node2].firstEdge;           // newEdge1 을 삽입할 때, 중복 간선이 없음을 확인했으므로
        while (ptr->nextEdge != NULL) {         //  여기선 확인할 필요 없음.
            ptr = ptr->nextEdge;
        }
        ptr->nextEdge = newEdge2;
    }
};


void DFS(Node* graph) {
    int start;
    Node* stack[10];            // 스택 : 노드 포인터의 배열
    int top = -1;
    Edge* edge_ptr;

    printf("Select a starting node : ");
    scanf("%d", &start);
    while(getchar() != '\n');

    /* 입력받은 graph[start] 가 생성되지 않은 노드면, */
    if (graph[start].index == -1) {
        printf("!! The [ %2d ] node doesn't exist. !!\n", start);
        return;
    }

    /* 시작 노드 스택에 push */
    graph[start].flag = TRUE;               // visit mark
    Push(stack, &top, &graph[start]);       // push

    while (top != -1) {
        edge_ptr = stack[top]->firstEdge;           // push 후엔 스택의 탑에서 그것의 간선의 끝을 찾을 때 까지 다시 시작
        while (edge_ptr != NULL) {  // 더 이상 방문하지 않은 인접 노드가 없을 때 까지
            if (edge_ptr->toNodeAddress->flag == FALSE) {   // 방문하지 않은 인접 노드가 있으면,
                edge_ptr->toNodeAddress->flag = TRUE;
                Push(stack, &top, edge_ptr->toNodeAddress);
                break;
            }
            else {                  // 다음 인접 노드 확인
                edge_ptr = edge_ptr->nextEdge;
            }
        }
        if (edge_ptr == NULL) {     // 방문하지 않은 인접 노드가 없거나, graph[start]가 연결을 갖고 있지 않았을 때 pop
            Pop(stack, &top);
        }
    }

    printf("\n");
    return;
}


void BFS(Node* graph) {
    int start;
    Node* queue[10];
    int front = -1, rear = -1;
    Edge* edge_ptr;

    printf("Select a starting node : ");
    scanf("%d", &start);
    while(getchar() != '\n');

    /* 입력받은 graph[start] 가 생성되지 않은 노드면, */
    if (graph[start].index == -1) {
        printf("!! The [ %2d ] node doesn't exist. !!\n", start);
        return;
    }

    
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
    Edge* edge_ptr,* edge_prev;     // prev 와 ptr 필요

    int i;

    for (i = 0; i < MAX_NODE_COUNT; i++) {
        edge_ptr = graph[i].firstEdge;
        while (edge_ptr != NULL) {
            edge_prev = edge_ptr;
            edge_ptr = edge_ptr->nextEdge;
            free(edge_prev);
        }
    }
    free(graph);            // 주의: 배열은 한 번에 free 하기. 배열 요소 하나하나 free 하는 것 아님!
}


void Push(Node** stack, int* top, Node* node) {
    stack[++(*top)] = node;
    printf("[ %2d ] ", node->index);
}


Node* Pop(Node** stack, int* top) {
    return stack[(*top)--];
}


void enQueue(int* queue, int* rear, int key) {

}


int deQueue(int* queue, int* front) {

}


void InitFlags(Node* graph, int count) {
    for (int i = 0; i < count; i++) {
        graph[i].flag = FALSE;
    }
}