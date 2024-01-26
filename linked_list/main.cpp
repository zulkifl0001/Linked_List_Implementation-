#include <iostream>
#include <fstream>
using namespace std;

template <class T>
class Node
{

public :
    T data;//data at node
    Node<T>* xnode;//used to transverse the list

};

template <class T>
//given function to take the xor of two node
Node<T>* Xor(Node<T>* x, Node<T>* y)
{
    return reinterpret_cast<Node<T>*>(reinterpret_cast<uintptr_t>(x)^ reinterpret_cast<uintptr_t>(y));
}

template <class T>
//method to push to the front of the list
void push_f(Node<T>* &head_ref, T data)
{
    // Allocate memory for the new node
    Node<T>* new_node = new Node<T>();//creating new node to be added
    new_node -> data = data;//giving the new node data

    // Since the new node is inserted at the
    // start, xnode of new node will always be
    // Xor of current head and NULL
    new_node -> xnode = head_ref;//new node next node
    // If the linkedlist is not empty, then xnode of
    // present head node will be XOR of the new node
    // and node next to current head */
    if (head_ref != NULL)
    {
        // *(head_ref)->xnode is Xor of (NULL and next).
        // If we XOR Null with next, we get next
        head_ref->xnode = Xor(new_node, head_ref->xnode);
    }
    // Change head
    head_ref = new_node;
}

template <class T>
void push_r(Node<T> *&head,T data){

    Node<T>* new_node = new Node<T>();//creating new node
    new_node -> data = data;//assigning data to new node

    Node<T>* currPtr = head;
    Node<T>* prevPtr = NULL;
    Node<T>* nextPtr;


    if(head!=NULL) {
        while (currPtr != NULL) {
            //transversing to the end of the list
            nextPtr = Xor(prevPtr, currPtr->xnode);
            prevPtr = currPtr;
            currPtr = nextPtr;
        }
        new_node->xnode = prevPtr;//assigning our new nodes xnode which will be between the previous node and NULL
        prevPtr->xnode = Xor(prevPtr->xnode, new_node);//changing the previous nodes xnode statement as it is new between its previous node and the new node
    }
    else{
        //for cases when the list was empty
        new_node -> xnode = head;
        head=new_node;//updating head

    }

}

template<class T>

T pop_f(Node<T>*&head){
    Node<T>*currPtr=head;
    Node<T>*prevPtr=NULL;
    Node<T>*nextPtr;
    Node<T>*nextt;

    nextPtr= Xor(prevPtr,currPtr->xnode);
    if(nextPtr!=NULL){
        nextt=Xor(currPtr,nextPtr->xnode);//getting the next to next element in the list
        nextPtr->xnode=nextt;
        head->xnode=NULL;//xnode of head becomes null as it  must be dealocated
        T temp=head->data;//trasnfer data
        head=nextPtr;//change head to new head(next)
        return temp;}
    else{
        //if the list has only one element
        T temp=head->data;
        head=NULL;

        return temp;
    }

}

template<class T>
T pop_r(Node<T>*&head){
    Node<T> *next;
    Node<T>*prev=NULL;
    Node<T> *curr=head;
    next=Xor(prev,curr->xnode);//getting to the next node
    if (next!=NULL){
        //looping to the last node
        while (curr != NULL) {
            next = Xor(prev, curr->xnode);
            if(next==NULL){
                break;
            }
            prev = curr;
            curr = next;
        }
        // current node is the last  node before null
        Node<T>*prevv=Xor(prev->xnode,curr);//go to the node before the prev node
        prev->xnode=prevv;//change the second last nodes xnode to the node before it and
        curr->xnode=NULL;
        return curr->data;}
    else{
        T temp= head->data;
        head=NULL;

        return temp;
    }

}

template <class T>
T peek_f(Node<T>*head){
    if(head!=NULL) {
        return head->data;
    }

}


template <class T>
//prints the elements in reverse
void print_e(Node<T>* head){
    Node<T>* currPtr = head;
    Node<T>* prevPtr = NULL;
    Node<T>* nextPtr;
    Node<T> * check=head;//our starting pointer
    cout << "The nodes of Linked List are: \n";

    //go to the end of the list
    while (currPtr != NULL) {

        nextPtr = Xor(prevPtr, currPtr -> xnode);

        // update prevPtr and currPtr for next iteration
        prevPtr = currPtr;
        currPtr = nextPtr;

    }
    //start going back from the end and printing out the nodes
    while(currPtr!=check) {
        nextPtr=Xor(currPtr,prevPtr->xnode);
        currPtr=prevPtr;
        prevPtr=nextPtr;
        cout<<currPtr->data;

    }
    cout<<endl;
    }

//wrapper functions
//changing the number of inputs needed from 2 to 1
template <class T>
class LL_wrapper{
private :
    Node<T>* head=NULL;
//head will always be NULL in the beginning
public :
            //wrapper functions
    void push_front(T data);
    void push_rear(T data);
    T pop_front();
    T pop_rear();
    void print_elements();
    T peek_front();

};

//we input the head into our wrapper function so when we run it in our maze solver it only needs one input
template<class T>
void LL_wrapper<T>::push_front(T data) {
    push_f(head,data);
}

template<class T>
void LL_wrapper<T>::push_rear(T data) {
    push_r(head,data);
}

template<class T>
T LL_wrapper<T>::pop_front() {
    return pop_f(head);
}

template<class T>
T LL_wrapper<T>::pop_rear() {
    return pop_r(head);
}

template<class T>
void LL_wrapper<T>::print_elements() {
    print_e(head);
}

template<class T>
T LL_wrapper<T>::peek_front() {
    peek_f(head);
}


///////////////////////////////////
//maze class methods written in part 1

class Maze{
private:
    int Nrow; //total rows
    int Ncol; //total columns
    char Nstate[20][20];//max usable maze size 20
    char Cdir; //currect direction
    int Crow; //current row
    int Ccol; //currect column
    int Trow;
    int Tcol;
public:
    Maze();
    Maze(const Maze &cpy);

    bool can_move_left();
    bool can_move_right();
    bool can_move_forward();
    bool can_move_back();

    void move_left();
    void move_right();
    void move_forward();
    void move_back();

    bool is_solved();
    char print_state();

};

Maze::Maze() {
    //code given in the question

    ifstream input_file; /* input file stream */
    input_file.open ("input_maze.txt");
    int nrow, ncol; /* number of rows and columns */
    input_file >> nrow >> ncol; /* read the size from file */
    Nrow=nrow;
    Ncol=ncol;
    char state[nrow][ncol];
    for(int i=0; i<nrow; ++i){
        for(int j=0; j<ncol; ++j){
            input_file >> state[i][j];
            Nstate[i][j]=state[i][j];
            //checking to see where our player starts from
            if(state[i][j]!='#' and state[i][j]!= '.' and state[i][j]!='T'){
                Crow=i;
                Ccol=j;
                Cdir=state[i][j];
            }
            //saving the location of the goal
            if(state[i][j]=='T'){
                Trow=i;
                Tcol=j;
            }
        }
    }
    input_file.close();
}
Maze::Maze(const Maze &cpy) {
    //copy vals
    Crow=cpy.Crow;
    Ccol=cpy.Ccol;
    Cdir=cpy.Cdir;

    for (int a=0;a<Nrow;a++){
        for(int b=0;b<Ncol;a++){
            Nstate[a][b]=cpy.Nstate[a][b];//copy all things in Nstate to the new Nstate
        }
    }

}

bool Maze::is_solved()  {
    //checks to see if T is still at its old location; if not then it has been solved;
    bool x=true;
    for(int i=0;i<Nrow;i++){
        for(int j=0;j<Ncol;j++){
            if(Nstate[i][j]=='T'){
                x=false;
            }
        }
    }
    return x;
}

//can move methods
//apply cases to see if there is a wall in the direction our robot wants to move
//if not then it can move in that direction
bool Maze::can_move_forward() {

    switch(Cdir){
        case 'E':
            if(Nstate[Crow][Ccol+1]!='#'){
                return true;
            }
            else{
                return false;
            }

        case 'S':
            if(Nstate[Crow+1][Ccol]!='#'){
                return true;
            }
            else{
                return false;
            }

        case 'N':
            if(Nstate[Crow-1][Ccol]!='#'){
                return true;
            }
            else{
                return false;
            }

        case 'W':
            if(Nstate[Crow][Ccol-1]!='#'){
                return true;
            }
            else{
                return false;
            }
    }

}
bool Maze::can_move_back() {
    switch(Cdir){
        case 'E':
            if(Nstate[Crow][Ccol-1]!='#'){
                return true;
            }
            else{
                return false;
            }

        case 'S':
            if(Nstate[Crow-1][Ccol]!='#'){
                return true;
            }
            else{
                return false;
            }

        case 'N':
            if(Nstate[Crow+1][Ccol]!='#'){
                return true;
            }
            else{
                return false;
            }

        case 'W':
            if(Nstate[Crow][Ccol+1]!='#'){
                return true;
            }
            else{
                return false;
            }
    }
}
bool Maze::can_move_right() {
    switch(Cdir){
        case 'E':
            if(Nstate[Crow+1][Ccol]!='#'){
                return true;
            }
            else{
                return false;
            }

        case 'S':
            if(Nstate[Crow][Ccol-1]!='#'){
                return true;
            }
            else{
                return false;
            }

        case 'N':
            if(Nstate[Crow][Ccol+1]!='#'){
                return true;
            }
            else{
                return false;
            }

        case 'W':
            if(Nstate[Crow-1][Ccol]!='#'){
                return true;
            }
            else{
                return false;
            }
    }

}
bool Maze::can_move_left() {
    switch(Cdir){
        case 'E':
            if(Nstate[Crow-1][Ccol]!='#'){
                return true;
            }
            else{
                return false;
            }

        case 'S':
            if(Nstate[Crow][Ccol+1]!='#'){
                return true;
            }
            else{
                return false;
            }

        case 'N':
            if(Nstate[Crow][Ccol-1]!='#'){
                return true;
            }
            else{
                return false;
            }

        case 'W':
            if(Nstate[Crow+1][Ccol]!='#'){
                return true;
            }
            else{
                return false;
            }
    }

}

//actually moving methods
// call the respective can move functions and if they return true then move in the specified direction
//cases for the new direction
void Maze::move_forward() {
    if (this->can_move_forward()){
        switch(Cdir){
            case 'E':
                Nstate[Crow][Ccol]='.'; //change the old location to  .
                Ccol=Ccol+1;
                Nstate[Crow][Ccol]='E'; //put the robot at its new location
                break;
            case 'S':
                Nstate[Crow][Ccol]='.';
                Crow=Crow+1;
                Nstate[Crow][Ccol]='S';
                break;
            case 'N':
                Nstate[Crow][Ccol]='.';
                Crow=Crow-1;
                Nstate[Crow][Ccol]='N';
                break;

            case 'W':
                Nstate[Crow][Ccol]='.';
                Ccol=Ccol-1;
                Nstate[Crow][Ccol]='W';
                break;

        }
    }
    else{
        cout<<"cannot move forward"<<endl;
    }
}
void Maze::move_back() {
    if(this->can_move_back()){
        switch(Cdir){
            //reverse the directions before changing the position
            case 'E':
                Nstate[Crow][Ccol]='.';
                Ccol=Ccol-1;
                Cdir='W';
                Nstate[Crow][Ccol]=Cdir;

                break;
            case 'S':
                Nstate[Crow][Ccol]='.';
                Crow=Crow-1;
                Cdir='N';

                Nstate[Crow][Ccol]=Cdir;
                break;
            case 'N':
                Nstate[Crow][Ccol]='.';
                Crow=Crow+1;
                Cdir='S';
                Nstate[Crow][Ccol]=Cdir;
                break;


            case 'W':
                Nstate[Crow][Ccol]='.';
                Ccol=Ccol+1;
                Cdir='E';
                Nstate[Crow][Ccol]=Cdir;
                break;
        }
    }
    else{
        cout<<"cannot move back"<<endl;
    }
}
void Maze::move_right() {
    if (this->can_move_right()) {
        switch (Cdir) {
            //rotate the directions clockwise
            case 'E':
                Nstate[Crow][Ccol]='.';
                Crow = Crow + 1;
                Cdir = 'S';
                Nstate[Crow][Ccol]=Cdir;
                break;
            case 'S':
                Nstate[Crow][Ccol]='.';
                Ccol--;
                Cdir = 'W';
                Nstate[Crow][Ccol]=Cdir;
                break;

            case 'N':
                Nstate[Crow][Ccol]='.';
                Ccol++;
                Cdir = 'E';
                Nstate[Crow][Ccol]=Cdir;
                break;

            case 'W':
                Nstate[Crow][Ccol]='.';
                Crow--;
                Cdir = 'N';
                Nstate[Crow][Ccol]=Cdir;
                break;
        }
    }
    else{
        cout<<"cannot move right"<<endl;
    }
}
void Maze::move_left() {
    if(can_move_left()){
        switch(Cdir){
            //rotate the directions anti-clockwise
            case 'E':
                Nstate[Crow][Ccol]='.';
                Crow--;
                Cdir='N';
                Nstate[Crow][Ccol]=Cdir;
                break;

            case 'S':
                Nstate[Crow][Ccol]='.';
                Ccol++;
                Cdir='E';
                Nstate[Crow][Ccol]=Cdir;
                break;

            case 'N':
                Nstate[Crow][Ccol]='.';
                Ccol--;
                Cdir='W';
                Nstate[Crow][Ccol]=Cdir;
                break;

            case 'W':
                Nstate[Crow][Ccol]='.';
                Crow++;
                Cdir='S';
                Nstate[Crow][Ccol]=Cdir;
                break;
        }

    }
    else{
        cout<<"cannot move left";
    }
}

//cycle through the 2d array and print values at all locations
char Maze::print_state() {
    for(int i=0;i<Nrow;i++){
        for(int j=0;j<Ncol;j++){
            cout<<Nstate[i][j];
        }
        cout<<endl;
    }
}

//solver function
//the object created is a LL_wrapper object instead of a stackqueue object
//all other objects are the same
void solver(){
    //create a LL_wrappr object
    LL_wrapper<char> stack;

    Maze maze=Maze();
    cout<<"first pass"<<endl;
    //loop while maze is not solved
    while(!maze.is_solved()){
        cout<<"SQ: ";
        stack.print_elements();
        cout<<endl;
        maze.print_state();
        cout<<endl;
        //applying the steps given in the question
        if(maze.can_move_left()){

            maze.move_left();
            if(stack.peek_front()!='B'){

                stack.push_front('L');
            }else{

                stack.pop_front();
                switch(stack.peek_front()){
                    case 'L':
                        stack.pop_front();
                        stack.push_front('F');
                        break;
                    case 'F':
                        stack.pop_front();
                        stack.push_front('R');
                        break;
                    case 'R':
                        stack.pop_front();
                        stack.push_front('B');
                }

            }

        }
        else if(maze.can_move_forward()){

            maze.move_forward();
            if(stack.peek_front()!='B'){
                stack.push_front('F');
            }else{
                stack.pop_front();
                switch(stack.peek_front()){
                    case 'L':
                        stack.pop_front();
                        stack.push_front('R');
                        break;
                    case 'F':
                        stack.pop_front();
                        stack.push_front('B');
                        break;
                }
            }
        }else if(maze.can_move_right()){
            maze.move_right();
            if(stack.peek_front()!='B'){
                stack.push_front('R');
            }else{
                stack.pop_front();
                if(stack.peek_front()=='R'){
                    stack.push_front('B');
                }
            }
        }else{
            maze.move_back();
            stack.push_front('B');
        }

    }
    cout<<"SQ: ";
    stack.print_elements();
    cout<<endl;
    maze.print_state();
    cout<<endl;
    cout<<"maze is solved"<<endl;

    Maze maze2= Maze();
    cout<<"second pass:"<<endl;
    cout<<"SQ: ";
    stack.print_elements();
    cout<<endl;
    maze2.print_state();

    //second pass
    //we use the steps detailed in the question
    while (!maze2.is_solved()){
        switch(stack.pop_rear()){
            case 'F':
                maze2.move_forward();
                break;
            case 'B':
                maze2.move_back();
                break;
            case 'R':
                maze2.move_right();
                break;
            case 'L':
                maze2.move_left();
                break;
        }
        cout<<"SQ: ";
        stack.print_elements();
        cout<<endl;
        maze2.print_state();
        cout<<endl;
    }
    cout<<"maze is solved";
}
////////////////////////////////



int main (){
solver();
}