//-----------------------------------------
// Tim Zhang
// CSCI 385
// HW 7
//-----------------------------------------
#include <iostream>
#include <fstream>
#include <list>
#include <cctype>

using namespace std;

//-----------------------------------------
// AVL tree data structure implementation.
//-----------------------------------------
class AVLTree
{
  private:
    //-----------------------------------------
    // Inner class of AVLTree.
    // Represents a node in the tree with data
    // on the word and freqeuncy count.
    //-----------------------------------------
    class Node
    {
      private:
        string word;    //Unique word in text file
        int frequency;  //Frequency of word in text file
        Node* parent;   //Node parent 
        Node* left;     //Left child
        Node* right;    //Right child
        int height;     //Height of tree rooted at node
        int balance;    //Balance factor of tree rooted at node
                
      public:
        //---------------------------------------
        // Parameterized constructor.
        // AVL container class provides position
        // and value for object instantiation.
        //---------------------------------------
        Node(string _word, Node* _parent){
          word = _word;
          frequency = 1;
          parent = _parent;
          left = right = NULL;
          height = 0;
          balance = 0;
        }
           
        //---------------------------------------
        // Balances node w.r.t children nodes
        // Tracks root of tree.
        //---------------------------------------
        void balanceNode(Node** headPtr){
          if (balance == 2){
            //Condition met for R rotation
            if (left->getBalance() == 1){
              rightRotation(headPtr);
            }
            //Condition met for L-R rotation
            else if (left->getBalance() == -1){
              leftRightRotation(headPtr);
            }
          }
          else if (balance == -2){
            //Condition met for L rotation
            if (right->getBalance() == -1){
              leftRotation(headPtr);
            }
            //Condition met for R-L rotation
            else if (right->getBalance() == 1){
              rightLeftRotation(headPtr);
            }
          }
          calculateHeight();
        }
              
        //---------------------------------------
        // AVL right rotation transformation.
        //---------------------------------------
        void rightRotation(Node** headPtr){
          Node* l = left;
          
          l->setParent(parent);
          
          //Change root of AVL tree
          if (parent == NULL){
            *headPtr = left;
          }
          else{ 
            if (parent->getRightChild() == this){
              parent->setRightChild(l);
            }
            else{
              parent->setLeftChild(l);
            }
          }

          this->setLeftChild(l->getRightChild());
          if (this->getLeftChild() != NULL){
            this->getLeftChild()->setParent(this);
          }
          
          this->setParent(l);
          l->setRightChild(this);
          
          calculateHeight();
          parent->calculateHeight();
        }
        
        //---------------------------------------
        // AVL left rotation transformation.
        //---------------------------------------        
        void leftRotation(Node** headPtr){
          Node* r = right;
          
          r->setParent(parent);          

          //Change root of AVL tree
          if (parent == NULL){
            *headPtr = right;
          }
          else{
            if (parent->getLeftChild() == this){
              parent->setLeftChild(r);
            }
            else{
              parent->setRightChild(r);
            }
          }

          this->setRightChild(r->getLeftChild());
          if (this->getRightChild() != NULL){
            this->getRightChild()->setParent(this);
          }
          
          this->setParent(r);
          r->setLeftChild(this);  
          
          calculateHeight();      
          parent->calculateHeight();
        }
        
        //---------------------------------------
        // AVL left-right rotation.
        //---------------------------------------
        void leftRightRotation(Node** headPtr){
          this->left->leftRotation(headPtr);
          rightRotation(headPtr);
        }
        
        //---------------------------------------
        // AVL right-left rotation.
        //---------------------------------------
        void rightLeftRotation(Node** headPtr){
          this->right->rightRotation(headPtr);
          leftRotation(headPtr);
        }
                
        //---------------------------------------
        // Calculates the balance factor of node
        // by taking the difference in heights
        // of the node's children.
        //---------------------------------------
        void calculateBalance(int left_height, int right_height){
          balance = left_height - right_height;
        }
        
        //---------------------------------------
        // Calcualtes the height of the node by
        // taking the max of the child heights.
        //---------------------------------------
        void calculateHeight(){
          if (left == NULL && right == NULL){
            height = 0;
            calculateBalance(0, 0);
          }
          else if (left == NULL){
            height = right->getHeight() + 1;
            calculateBalance(0, right->getHeight() + 1);
          }
          else if (right == NULL){
            height = left->getHeight() + 1;
            calculateBalance(left->getHeight() + 1, 0);
          }
          else if (left->getHeight() > right->getHeight()){
            height = left->getHeight() + 1;
            calculateBalance(left->getHeight() + 1, right->getHeight() + 1);
          }
          else{
            height = right->getHeight() + 1;          
            calculateBalance(left->getHeight() + 1, right->getHeight() + 1);
          }
        }
        
        //---------------------------------------
        // Increments frequency counter.
        //---------------------------------------
        void updateFrequency(){ frequency++; }
        
        //---------------------------------------
        // Setter methods.
        //---------------------------------------
        void setParent(Node* p){ parent = p; }
        void setLeftChild(Node* l){ left = l; }
        void setRightChild(Node* r){ right = r; }
        void setBalance(int b){ balance = b; }
        
        //---------------------------------------
        // Getter methods.
        //---------------------------------------
        Node* getParent(){ return parent; }
        Node* getLeftChild(){ return left; }
        Node* getRightChild(){ return right; }
        int getBalance(){ return balance; }
        int getHeight(){ return height; }
        string getWord(){ return word; }
        int getFrequency(){ return frequency; }   
    };
    
    //-----------------------------------------
    // Private member variables.
    //-----------------------------------------
    int size;    //Size of tree, |V|
    Node* head;  //Pointer to root node of the tree
    
    //-----------------------------------------
    // Searches AVL tree for node.
    // Returns the instance if found.
    // Returns the last node if not found.
    //-----------------------------------------
    Node* searchTree(Node* node, string word){
      if (node == NULL){
        return NULL;
      }
      Node* position = node;
      
      if (word == position->getWord()){
        return position;
      }
      else if (word > position->getWord()){
        position = searchTree(position->getRightChild(), word);
      }
      else if (word < position->getWord()){
        position = searchTree(position->getLeftChild(), word);
      }      
      
      //Return last leaf
      if (position == NULL){
        return node;
      }
      return position;
    }    

    //-----------------------------------------
    // Updates the branch of the tree starting 
    // from a given position to the root.
    //-----------------------------------------
    void updateHeight(Node* position){
      while (position != NULL){
        position->calculateHeight();
        position = position->getParent();
      }
    }

    //-----------------------------------------
    // Function balances the AVL tree.
    //-----------------------------------------
    void balanceAVLTree(Node* node, Node** headPtr){
      while (node != NULL){
        if (node->getBalance() > 1 || node->getBalance() < -1){
          node->balanceNode(headPtr);
        }
        updateHeight(node);
        node = node->getParent();
      }
    }

  public:
    //-----------------------------------------
    // Default constructor.
    //-----------------------------------------
    AVLTree(){
      size = 0;
      head = NULL;
    }
    
    //-----------------------------------------
    // Adds word to AVL tree node.
    // If the word is present, increment count. 
    //-----------------------------------------
    void add(string word){
      //If tree is empty we set the word to the root of the tree
      if (head == NULL){
        Node* rootWord = new Node(word, NULL);
        head = rootWord;
        size++;
        return;
      }

      //Search the tree for the word
      Node* position = searchTree(head, word);
              
      if (position->getWord() == word){
        position->updateFrequency();
      }
      //Insert word
      else{
        Node* newWord = new Node(word, position);
        
        if (word > position->getWord()){
          position->setRightChild(newWord);          
        }
        else{
          position->setLeftChild(newWord);
        }        
        updateHeight(newWord);
        Node** headPtr = &head;
        balanceAVLTree(newWord, headPtr);
        updateHeight(newWord);
        size++;
      }
    }
    
    //-----------------------------------------
    // Checks for word in AVL tree.
    // If found, output "word: frequency"
    // Else, output "not found"
    //-----------------------------------------
    void find(string word){
      Node* position = searchTree(head, word);
      
      if (position == NULL){
        cout << "\n\t" + word + " not found in the text file.";
      }      
      else if (position->getWord() == word){
        cout << "\n\t" << position->getWord() + " found " << position->getFrequency() << " times in the text file.\n";        
      }
      else cout << "\n\t" + word + " not found in the text file.\n";
    }
    
    //-----------------------------------------
    // Prints the size of the AVL tree.
    //-----------------------------------------
    void printSize(){
      cout << "\nThere are " << size << " unique words in the text file.\n";
    }     
    
    //-----------------------------------------
    // Prints the top x most frequent words.
    //-----------------------------------------
    void printMostFrequentWords(int x){
      list<Node*> queue;  //For BFS
      Node* node;  //A single node
      int* frequency = new int[x];  //Frequency array
      string* words = new string[x];  //Word array, parallel to frequency[]
      
      //Initialize arrays      
      for (int i = 0; i < x; i++){
        frequency[i] = 0;
        words[i] = "";     
      }
      
      queue.push_back(head);
      node = queue.front();
      
      //BFS
      while (!queue.empty()){
        //If the node.freqeucny is larger than the smallest element in the array
        if (node != NULL && node->getFrequency() > frequency[x - 1]){
          frequency[x - 1] = node->getFrequency();  
          words[x - 1] = node->getWord();
          
          //Insertion sort
          for (int i = 0; i < x; i++){
            int j = i;
          
            while (j > 0 && frequency[j - 1] < frequency[j]){
              //Swap values
              int ftemp = frequency[j];
              frequency[j] = frequency[j - 1]; 
              frequency[j - 1] = ftemp;
              
              string wtemp = words[j];
              words[j] = words[j - 1];
              words[j - 1] = wtemp;
              j--;
            }
          }
        }
        
        //Enqueue
        if (node->getLeftChild() != NULL) queue.push_back(node->getLeftChild());
        if (node->getRightChild() != NULL) queue.push_back(node->getRightChild());
        queue.pop_front();
        if (!queue.empty()) node = queue.front();
      }       
      
      cout << "\nTop " << x << " frequent words in the text file.\n\n";
      for (int i = 0; i < x; i++){
        if (words[i] != "") cout << "\t" << words[i] << " occurs " << frequency[i] << " times in the text file.\n";              
      }
      delete[] frequency, words; 
    }
    
    //-----------------------------------------
    // Outputs all words in AVL tree having
    // given frequency.
    //-----------------------------------------
    void printWordsWithFrequency(int frequency){
      list<string> words;  //List of words matching search criteria
      list<Node*> queue;   //For BFS
      Node* node;	   //A single node
      
      queue.push_back(head);
      node = queue.front();
      
      while (!queue.empty()){
        if (node->getFrequency() == frequency){
          words.push_back(node->getWord());
        }
        
        //BFS
        if (node->getLeftChild() != NULL) queue.push_back(node->getLeftChild());
        if (node->getRightChild() != NULL) queue.push_back(node->getRightChild());
        queue.pop_front();
        if (!queue.empty()) node = queue.front();
      }
      
      cout << "\nWords which occur " << frequency << " times in the text file: \n\n";
      while (!words.empty()){
        cout << "\t" << words.front() << endl;
        words.pop_front();
      }
    }    
};

//-----------------------------------------
// Function signatures.
//-----------------------------------------
string RemoveIllegalCharacters(string);

//-----------------------------------------
// Main function.
//-----------------------------------------
int main(){
  ifstream fin;   //File stream
  string file;    //.txt file to be read
  string word;    //A single word in the file
  AVLTree words;  //All unique words in the text file
  int total = 0;  //All words in file
  string input;   //Search argument for word search 
  int frequency;  //Search argument for frequency search
  int topwords;   //Search argument for most frequent words
   
  cout << "\nEnter .txt file to be analyzed: ";
  cin >> file;
  
  fin.open(file.c_str(), ios::in);
  
  //Validation logic
  if(!fin.is_open()){  
    cout << "\nText file could not be read. (Try using the full file path)\n\n";
    fin.close();
    return 0;
  }
  
  //Read file contents
  while(!fin.eof()){
    fin >> word;
    word = RemoveIllegalCharacters(word);
    words.add(word);
    total++;
  }
  fin.close();
  
  cout << "\nThere are " << total << " words in the text file.\n";
  words.printSize();
  
  while (topwords != -1){
    cout << "\nStop most frequent words list at ('-1' to quit): ";
    cin >> topwords;
    if (!cin){
      cout << "\nOops, you broke the stream.\n";
      return 1;
    }
    if (topwords != -1) words.printMostFrequentWords(topwords);
  }
  
  while (input != "n"){
    cout << "\nSearch by term ('n' to quit): ";
    cin >> input;
    if (!cin){
      cout << "\nOops, you broke the stream.\n";
      return 1;
    }
    if (input != "n") words.find(input);
  }  
  
  while (frequency != -1){
    cout << "\nSearch by frequency ('-1' to quit): ";
    cin >> frequency;
    if (!cin){
      cout << "\nOops, you broke the stream.\n";
      return 1;
    }
    if (frequency != -1) words.printWordsWithFrequency(frequency);
  }
  return 0;
}

//-----------------------------------------
// Function removes all non ('a', 'z') 
// characters from input string.
// Input: character string
// Output: string with ('a', 'z') elements
//-----------------------------------------
string RemoveIllegalCharacters(string word){
  int length = word.length();
  
  for (int i = 0; i < length; i++){
    char c = word[i] = tolower(word[i]);
        
    if (c < 'a' || c > 'z'){
      word.erase(i--, 1);
      length--;
    }
  }
  return word;
}
