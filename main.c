#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define maxNameSize 20
#define SIZE 40

struct queue {
    int items[SIZE];
    int front;
    int rear;
};
typedef struct queue queue;

struct AdjacentVertex{
    char *name;
    struct AdjacentVertex *next;
};
typedef struct AdjacentVertex AdjacentVertex;


struct SourceVertex{
    int check;
    char name[maxNameSize];
    struct AdjacentVertex *head;
};
typedef struct SourceVertex SourceVertex;


struct Graph{
    int Vertex;
    SourceVertex * vertexList;
};
typedef struct Graph Graph;




int findVertexNumer();

AdjacentVertex * createAdjacent(char * dest);

SourceVertex  createVertex(char *name);

Graph * createGraph(int Vertex);

void getVertexNames();

void printGraph(Graph *graph);

void addEgde(Graph **graph ,char * source, char * dest);

void readFileAndAddEdge(Graph **graph);

void printAdjacencyMatrix(Graph *graph);

void DegreeCentrality(Graph *graph);

void ClosenessCentrality(Graph *graph);

queue* createQueue();

int isEmpty( queue* q);

void enqueue(queue* q, int value);

int dequeue( queue* q);

int findIdex(Graph *graph, char *name );

void clearCheck(Graph *graph);


int bfs(Graph *graph,char* source, char *dest);


char vertexNames[][maxNameSize];

int main() {

    int vertexNumber=findVertexNumer();
     getVertexNames();

     Graph *graph=createGraph(vertexNumber);


   readFileAndAddEdge(&graph);

   printGraph(graph);
   printf("\n***************************************************************************************\n\n");
   printAdjacencyMatrix(graph);
   printf("\n***************************************************************************************\n\n");
   DegreeCentrality(graph);
   printf("\n***************************************************************************************\n\n");
   ClosenessCentrality(graph);

    return 0;
}

AdjacentVertex * createAdjacent(char * dest){
    AdjacentVertex *newAdjacent=malloc(sizeof(AdjacentVertex));
    newAdjacent->name=malloc(sizeof(char)*strlen(dest));

    strcpy(newAdjacent->name,dest);
    newAdjacent->next=NULL;

    return newAdjacent;
}

SourceVertex  createVertex(char *name){
    SourceVertex *newVertex=malloc(sizeof(SourceVertex));


    strcpy(newVertex->name,name);
    newVertex->head=NULL;
    newVertex->check=0;

    return *newVertex;
}

Graph * createGraph(int Vertex){
    Graph *graph =malloc(sizeof(Graph));
    graph->Vertex=Vertex;

    graph->vertexList=malloc(Vertex *sizeof(SourceVertex));

    int i;
    for(i=0;i<Vertex;i++){
        graph->vertexList[i]=createVertex(vertexNames[i]);
    }

    return graph;
}

void addEgde(Graph **graph ,char * source, char * dest){
    int src,i;
    for ( i = 0; i <(*graph)->Vertex ; i++) {


        if(strcmp((*graph)->vertexList[i].name,source)==0){
            src=i;
        }

    }
    AdjacentVertex *newAdj=createAdjacent(dest);
    newAdj->next=(*graph)->vertexList[src].head;
    (*graph)->vertexList[src].head=newAdj;
    

}

void readFileAndAddEdge(Graph **graph){
    char name[maxNameSize];
    char srcVertexName[maxNameSize];
    char destVestexName[maxNameSize]="\0";
    FILE *file = fopen("input.txt", "r");

    while(fscanf(file,"%s",name)!=EOF){
       // srcVertexName[strlen(srcVertexName)-1]='\0';
      if(name[strlen(name)-1]==';'){
          name[strlen(name)-1]='\0';
          strcpy(srcVertexName,name);
          strcpy(destVestexName,"\0");
      }else{
          if(name[strlen(name)-1]==',') name[strlen(name)-1]='\0';
          strcpy(destVestexName,name);
      }
    if(destVestexName[0]!='\0') addEgde(graph,srcVertexName,destVestexName);


    }

    /*fscanf(file,"%s",name);
    name[strlen(name)-1]='\0';
    printf("%s",name);*/
    fclose(file);
}

void getVertexNames(){


        FILE *file = fopen("input.txt", "r");
        int c;
        int counter=0;
       int letterCounter=0;
        int checkSemiColon=0;
        while(1){
            c=fgetc(file);


            if(feof(file)) break;
            if(c==';') checkSemiColon=1;

            if(!checkSemiColon){
                vertexNames[counter][letterCounter]=(char)c;
                vertexNames[counter][letterCounter+1]='\0';
                letterCounter++;
            }

            if(c=='\n'){
                counter++;
                checkSemiColon=0;
                letterCounter=0;
            }
        }




    }

int findVertexNumer(){
    int c;
    int counter=1;
    FILE *file = fopen("input.txt", "r");

    while(1){
        c=fgetc(file);
        if(feof(file)) break;

        if(c=='\n'){
            counter++;
        }
    }


    return counter;
}

void printGraph(Graph *graph){
    int i;
   printf("\n\n");
    for (i = 0; i <graph->Vertex; ++i) {
        SourceVertex currentVertex=graph->vertexList[i];
        printf("Adjacency list of Vertex %s \n",currentVertex.name);
        printf("|%s|",currentVertex.name);

        AdjacentVertex *currentAdj=currentVertex.head;
        while(currentAdj!=NULL){

            printf(" --> %s",currentAdj->name);
            currentAdj=currentAdj->next;
        }
        printf("\n\n");
    }
}

void printAdjacencyMatrix(Graph *graph){
    int i,status=0;
    printf("\t\t\t");
    for (int i = 0; i <graph->Vertex ; ++i) {
        printf("%s   ",graph->vertexList[i].name);
    }
    puts("\n\n");
    for (int i = 0; i < graph->Vertex; i++) {
        printf("%s \t\t",graph->vertexList[i].name);
        for (int j = 0; j <graph->Vertex ; j++) {
            AdjacentVertex *currentAdj=graph->vertexList[i].head;
            status=0;
            while(currentAdj!=NULL){
                if(strcmp(currentAdj->name,vertexNames[j])==0) status=1;
                currentAdj=currentAdj->next;
            }
            printf("%d\t\t",status);
        }
        puts("\n\n");
    }
}

void DegreeCentrality(Graph *graph){
    int adjCounter=0;
    int numOfNodes=graph->Vertex;

    printf("\t\t\t\tDEGREE CENTRALİTY\n");
    printf("-------------------------------------------------\n");
    printf("|\tNode\t|\tScore\t|\tStandardized Score  |\n");
    printf("-------------------------------------------------\n");
    int i;
    for ( i = 0; i <numOfNodes ; ++i) {
        adjCounter=0;
        AdjacentVertex *currentAdj=graph->vertexList[i].head;

        while(currentAdj!=NULL){
            adjCounter++;
            currentAdj=currentAdj->next;
        }
        printf("|\t%s \t|\t  %d\t\t|\t\t   %d/%d\t\t\t|\n",graph->vertexList[i].name,adjCounter,adjCounter,numOfNodes-1);
        printf("-------------------------------------------------\n");

    }
}

void ClosenessCentrality(Graph *graph) {
    int score=0;
    int numOfNodes=graph->Vertex;

    printf("\t\t\t\tCLOSENESS CENTRALİTY\n");
    printf("-------------------------------------------------\n");
    printf("|\tNode\t|\tScore\t|\tStandardized Score  |\n");
    printf("-------------------------------------------------\n");

    for (int i = 0; i < graph->Vertex; ++i) {
        for (int j = 0; j <graph->Vertex ; ++j) {
            score+=bfs(graph,graph->vertexList[i].name,graph->vertexList[j].name);
        }
        printf("|\t%s \t|\t  %d\t|\t\t %d/%d\t\t\t|\n",graph->vertexList[i].name,score,numOfNodes-1,score);
        printf("-------------------------------------------------\n");
        score=0;

    }


}

int bfs(Graph *graph,char* source, char *dest){
    if(strcmp(source,dest)==0) return 0;

    queue *indexQ=createQueue();
    int layer[graph->Vertex]; //bunu düzelt

    int src=findIdex(graph,source);

    enqueue(indexQ,src);
    layer[src]=0;
    graph->vertexList[src].check=1;

    while (!isEmpty(indexQ)){
        int currentIndex=dequeue(indexQ);
        AdjacentVertex *currentAdj=graph->vertexList[currentIndex].head;
       while(currentAdj!=NULL){
           int index=findIdex(graph,currentAdj->name);
           if(strcmp(currentAdj->name,dest)==0){
               clearCheck(graph);
               return layer[currentIndex]+1;
           }else{
               if(graph->vertexList[index].check==0){
                   enqueue(indexQ,index);
                   layer[index]=layer[currentIndex]+1;
                   graph->vertexList[index].check=1;
               }
           }
           currentAdj=currentAdj->next;
       }
       graph->vertexList[currentIndex].check=2;
    }



}

void clearCheck(Graph *graph){
    int i;
    for ( i= 0; i < graph->Vertex; ++i) {
        graph->vertexList[i].check=0;
    }
}

int findIdex(Graph *graph, char *name ){
    int i;
    for ( i = 0; i <graph->Vertex ; i++) {


        if(strcmp(graph->vertexList[i].name,name)==0){
            return i;
        }

    }
}

queue* createQueue() {
    queue* q = malloc(sizeof(struct queue));
    q->front = -1;
    q->rear = -1;
    return q;
}

int isEmpty( queue* q) {
    if(q->rear == -1)
        return 1;
    else
        return 0;
}

void enqueue(queue* q, int value){
    if(q->rear == SIZE-1)
        printf("\nQueue is Full!!");
    else {
        if(q->front == -1)
            q->front = 0;
        q->rear++;
        q->items[q->rear] = value;
    }
}

int dequeue( queue* q){
    int item;
    if(isEmpty(q)){
        printf("Queue is empty");
        item = -1;
    }
    else{
        item = q->items[q->front];
        q->front++;
        if(q->front > q->rear){
            q->front = q->rear = -1;
        }
    }
    return item;
}