// 資訊二甲 11327136 陳彥均 11327127 林煒博
#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <chrono> // 計時用
#include <vector>
using namespace std;

// define functions
void task1();
void task2();
void task3();
void task4();

struct Order {
  int OID;        // 訂單編號
  int arrival;    // 下單時刻 
  int duration;   // 製作耗時
  int timeOut;    //逾時時刻
};

struct LogEntry {
  int OID;
  int CID;        // 廚師編號 (Chef ID)
  int Time;       // 取消時刻 (Abort) 或 完成時間 (Departure)
  int Delay;
};

class myQueue {
 private:
  struct Node {
    Order data;
    Node* next;
  };

  Node* front;
  Node* rear;
  int currentSize;
  int capacity;

 public:
  myQueue() {
    front = nullptr;
    rear = nullptr;
    currentSize = 0;
    capacity = 3;
  }

  ~myQueue() {
    Node* current = front;
    while (current != nullptr) {
      Node* nextNode = current->next;
      delete current;
      current = nextNode;
    }

    front = nullptr;
    rear = nullptr;
    currentSize = 0;
  }

  bool isEmpty() const {return currentSize == 0; }
  bool isFull() const {return currentSize >= capacity; }
  int size() const {return currentSize; }

  bool enqueue(Order item) {
    if (isFull()) {
      return false;
    }
    
    Node* newNode = new Node;
    newNode->data = item;
    newNode->next = nullptr;

    if (isEmpty()) {
      front = newNode;
      rear = newNode;
    } else {
      rear->next = newNode;
      rear = newNode;
    }
    currentSize++;
    return true;
  }

  bool dequeue(Order& item) {
    if (isEmpty()) {
      return false;
    }
    
    Node* temp = front;
    item = temp->data;

    front = front->next;

    if (front == nullptr) {
      rear = nullptr;
    }
    
    delete temp;
    currentSize--;

    return true;
  }

  bool peek(Order& item) {
    if (isEmpty()) {
      return false;
    }
    item = front->data;
    return true;
  }
};

void showMenu() {
  cout << "*** (^_^) Data Structure (^o^) ***" << endl;
  cout << "** Simulate FIFO Queues by SQF ***" << endl;
  cout << "* 0. Quit                        *" << endl;
  cout << "* 1. Sort a file                 *" << endl;
  cout << "* 2. Simulate one FIFO queue     *" << endl;
  cout << "* 3. Simulate two queues by SQF  *" << endl;
  cout << "* 4. Simulate some queues by SQF *" << endl;
  cout << "**********************************" << endl;
};

void ReadInput(int &command) {
  cout << "Input a command(0, 1, 2, 3, 4): ";
  if (!(cin >> command)) {
    cin.clear();
    string dummy;
    getline(cin, dummy);
    cout << endl << "Command does not exist!" << endl << endl;
    return;
  }

  if (command < 0 || command > 4) {
    cout << endl << "Command does not exist!" << endl << endl;
    return;
  }
};

string getInputID() {  
  string id;
  cout << endl << "Input a file number (e.g., 401, 402, 403, ...): ";
  cin >> id;
  return id;
}

void shellSort();

// global data
Order* sharedOrders = nullptr; // order array
int sharedCount = 0;           // all order
string loadedFileID = "";      // current file id

int main() {
  int command = 0;

  do {
    showMenu();
    ReadInput(command);
    switch (command) {
      case 1:
        task1();
        break;
      case 2:
        task2();
        break;
      case 3:
        task3();
        break;
      case 4:
        task4();
        break;
      case 0:
        return 0;
        break;
    }
  } while (command != 0);
  
  return 0;
}
void shellSort(Order* array, int n) {
  for (int gap = n / 2; gap > 0; gap /= 2) {
    for (int i = gap; i < n; i++) {
      Order temp = array[i];
      int j;
      for (j = i; j >= gap; j -= gap) {
        bool needShift = false;
        if (array[j - gap].arrival > temp.arrival) {
          needShift = true;
        } else if (array[j - gap].arrival == temp.arrival) {
          if (array[j - gap].OID > temp.OID) {
            needShift = true;
          }
        }

        if (needShift) {
          array[j] = array[j - gap];
        } else {
          break;
        }
      }
      array[j] = temp;
    }
  }
}

void task1() {
  string fileID = getInputID(); 
  string inputFileName = "input" + fileID + ".txt";
  string outputFileName = "sorted" + fileID + ".txt";

  // read
  auto startRead = chrono::high_resolution_clock::now(); // timer start

  ifstream inFile(inputFileName);
  if (!inFile) {
    cout << endl << "### " << inputFileName << " does not exist! ###" << endl << endl;
    return;
  }

  // 第一次讀取 計算資料筆數
  string headerLine;
  getline(inFile, headerLine); // 跳過第一行標題

  int totalOrders = 0;
  int temp; 
  // 嘗試讀取每一行的四個整數讀成功一次計數加一
  while (inFile >> temp >> temp >> temp >> temp) {
    totalOrders++;
  }

  Order* orderList = new Order[totalOrders];

  // 第二次讀取真正讀入資料
  inFile.clear();
  inFile.seekg(0); // 游標回到檔案開頭
  getline(inFile, headerLine); // 跳過標題

  for (int i = 0; i < totalOrders; i++) {
    inFile >> orderList[i].OID 
           >> orderList[i].arrival 
           >> orderList[i].duration 
           >> orderList[i].timeOut;
  }
  inFile.close();

  auto endRead = chrono::high_resolution_clock::now(); // timer end
  auto readTime = chrono::duration_cast<chrono::microseconds>(endRead - startRead).count();

  cout << endl;
  cout << "\tOID\tArrival\tDuration\tTimeOut" << endl;
  for (int i = 0; i < totalOrders; i++) {
      cout << "(" << (i + 1) << ")\t" 
           << orderList[i].OID << "\t" 
           << orderList[i].arrival << "\t" 
           << orderList[i].duration << "\t\t" 
           << orderList[i].timeOut << endl;
  }
    
  // sort
  auto stratSort = chrono::high_resolution_clock::now();
  shellSort(orderList, totalOrders);
  auto endSort = chrono::high_resolution_clock::now();
  auto sortTime = chrono::duration_cast<chrono::nanoseconds>(endSort - stratSort).count();

  // write in
  auto startWrite = chrono::high_resolution_clock::now();
  ofstream outFile(outputFileName);

  outFile << "OID\tArrival\tDuration\tTimeOut" << endl;
  for (int i = 0; i < totalOrders; i++) {
    outFile << orderList[i].OID << "\t"
            << orderList[i].arrival << "\t"
            << orderList[i].duration << "\t"
            << orderList[i].timeOut << endl;
  }
  outFile.close();

  auto endWrite = chrono::high_resolution_clock::now();
  auto writeTime = chrono::duration_cast<chrono::microseconds>(endWrite - startWrite).count();

  cout << "\nReading data: " << readTime << " us." << endl;
  cout << "\nSorting data: " << sortTime << " us." << endl;
  cout << "\nWriting data: " << writeTime << " us." << endl;
  cout << endl;
  delete[] orderList;
}

void task2() {
  string inputID = getInputID();

  if (loadedFileID != inputID) {
    if (sharedOrders != nullptr) {
      delete[] sharedOrders;
      sharedOrders = nullptr;
    }
    // read
    string fileName = "sorted" + inputID + ".txt";
    ifstream inFile(fileName);

    if (!inFile) {
      cout << endl << "### " << fileName << " does not exist! ###" << endl << endl;
      loadedFileID = ""; // read fail
      return;
    }

    string header;
    getline(inFile, header); // 跳過標題
    int temp;
    sharedCount = 0;
    while (inFile >> temp >> temp >> temp >> temp) {
      sharedCount++;
    }
    // 讀入資料
    sharedOrders = new Order[sharedCount];
    inFile.clear();
    inFile.seekg(0);
    getline(inFile, header);
    for (int i = 0; i < sharedCount; i++) {
      inFile >> sharedOrders[i].OID >> sharedOrders[i].arrival 
            >> sharedOrders[i].duration >> sharedOrders[i].timeOut;
    }
    inFile.close();
        
    // update loaded ID
    loadedFileID = inputID; 
  }

  // output original data
  cout << endl << "\tOID\tArrival\tDuration\tTimeOut" << endl;
  for (int i = 0; i < sharedCount; i++) {
    cout << "(" << (i + 1) << ")\t" 
         << sharedOrders[i].OID << "\t" 
         << sharedOrders[i].arrival << "\t" 
         << sharedOrders[i].duration << "\t\t" 
         << sharedOrders[i].timeOut << endl;
  }
  cout << endl;
  // 4. 單一佇列模擬 (所有變數皆為 Local)
  // main section:
  LogEntry* abortList = new LogEntry[sharedCount];
  int abortCount = 0;
  LogEntry* timeoutList = new LogEntry[sharedCount];
  int timeoutCount = 0;
  myQueue queue;
  int chefIdleTime = 0; // 廚師閒置時間

  for (int i = 0; i < sharedCount; i++) {
    Order curr = sharedOrders[i];
    
    if (curr.timeOut < curr.arrival || curr.timeOut < curr.arrival + curr.duration) {
      continue;
    }
    while (!queue.isEmpty() && chefIdleTime <= curr.arrival) {
      Order qOrder;
      queue.dequeue(qOrder); // 取出

      if (qOrder.timeOut < chefIdleTime) {
        // CID=1 (進過佇列)
        abortList[abortCount++] = {qOrder.OID, 1, chefIdleTime, chefIdleTime - qOrder.arrival};
      } else {
        // 製作
        int startCook = chefIdleTime;
        chefIdleTime += qOrder.duration;
        // 檢查是否製作逾時
        if (qOrder.timeOut < chefIdleTime) {
          timeoutList[timeoutCount++] = {qOrder.OID, 1, chefIdleTime, startCook - qOrder.arrival};
        }
      }
    }

    // 處理新訂單 curr
    if (chefIdleTime > curr.arrival) {
      // 廚師還在忙，需要進入佇列
      if (queue.isFull()) {
        abortList[abortCount++] = {curr.OID, 0, curr.arrival, 0};
      } else {
        queue.enqueue(curr);
      }
    } else {
      // 廚師閒置（chefIdleTime <= curr.arrival），直接製作
      chefIdleTime = curr.arrival;
      int startCook = chefIdleTime;
      chefIdleTime += curr.duration;
            
      if (curr.timeOut < chefIdleTime) {
        timeoutList[timeoutCount++] = {curr.OID, 1, chefIdleTime, startCook - curr.arrival};
      }
    }
  }

  // 處理佇列剩餘訂單
  while (!queue.isEmpty()) {
    Order qOrder;
    queue.dequeue(qOrder);
        
    if (qOrder.timeOut < chefIdleTime) {
      abortList[abortCount++] = {qOrder.OID, 1, chefIdleTime, chefIdleTime - qOrder.arrival};
    } else {
      int startCook = chefIdleTime;
      chefIdleTime += qOrder.duration;
      if (qOrder.timeOut < chefIdleTime) {
        timeoutList[timeoutCount++] = {qOrder.OID, 1, chefIdleTime, startCook - qOrder.arrival};
      }
    }
  }

  // wirte file
  double totalDelay = 0;
  for (int i = 0; i < abortCount; i++) {
    totalDelay += abortList[i].Delay;
  }
  for (int i = 0; i < timeoutCount; i++) {
    totalDelay += timeoutList[i].Delay;
  }  
  float failRate = 0.0;
  if (sharedCount > 0) {
    failRate = (float)(abortCount + timeoutCount) / sharedCount * 100.0;
  }
  string outputFileName = "one" + loadedFileID + ".txt";
  ofstream outFile(outputFileName);
    
  outFile << "\t[Abort List]" << endl;
  outFile << "\tOID\tCID\tDelay\tAbort" << endl;
  for (int i = 0; i < abortCount; i++) {
    outFile << "[" << (i+1) << "]\t" 
        << abortList[i].OID << "\t" 
        << abortList[i].CID << "\t" 
        << abortList[i].Delay << "\t" 
        << abortList[i].Time << endl;
  }

  outFile << "\t[Timeout List]" << endl;
  outFile << "\tOID\tCID\tDelay\tDeparture" << endl;
  for (int i = 0; i < timeoutCount; i++) {
    outFile << "[" << (i+1) << "]\t" 
        << timeoutList[i].OID << "\t" 
        << timeoutList[i].CID << "\t" 
        << timeoutList[i].Delay << "\t" 
        << timeoutList[i].Time << endl;
  }

  outFile << "[Total Delay]" << endl;
  outFile << fixed << setprecision(0) << totalDelay << " min." << endl;
  outFile << "[Failure Percentage]" << endl;
  outFile << fixed << setprecision(2) << failRate << " %" << endl;
  outFile.close();

  delete[] abortList;
  delete[] timeoutList;
}

void task3() {
  if (sharedOrders == nullptr) {
    cout << endl << "### Execute command 2 first! ###" << endl << endl;
    return;
  }
  Order *ordersCopy = new Order[sharedCount];
  for (int i = 0; i < sharedCount; i++) {
    ordersCopy[i] = sharedOrders[i];
  }

  // initialize local data
  LogEntry* abortList = new LogEntry[sharedCount];
  int abortCount = 0;
  LogEntry* timeoutList = new LogEntry[sharedCount];
  int timeoutCount = 0;
  myQueue chef1_queue;
  myQueue chef2_queue;
  int chef1_idle_time = 0;  // 廚師1閒置時間
  int chef2_idle_time = 0;  // 廚師2閒置時間


  for (int i = 0; i < sharedCount; i++) {
    Order curr = ordersCopy[i];
    bool work = false;
    if (!chef1_queue.isEmpty()) {
      Order o1;
      chef1_queue.peek(o1);
      if (chef1_idle_time <= curr.arrival) {
        chef1_queue.dequeue(o1);
        work = true;
        int start1 =  chef1_idle_time;
        chef1_idle_time += o1.duration;
        
        if (o1.timeOut < chef1_idle_time) {
          timeoutList[timeoutCount++] = {o1.OID, 1, chef1_idle_time, start1 - o1.arrival};
        }


      }
    }

    if (!chef2_queue.isEmpty()) {
      Order o2;
      chef2_queue.peek(o2);
      if (chef2_idle_time <= curr.arrival) {
        chef2_queue.dequeue(o2);
        work = true;
        int start2 = chef2_idle_time;
        chef2_idle_time += o2.duration;
        
        if (o2.timeOut < chef2_idle_time) {
          timeoutList[timeoutCount++] = {o2.OID, 2, chef2_idle_time, start2 - o2.arrival};
        }
      }
    }

    bool chef1_idle = (chef1_idle_time <= curr.arrival && chef1_queue.isEmpty());
    bool chef2_idle = (chef2_idle_time <= curr.arrival && chef2_queue.isEmpty());
    int q1 = chef1_queue.size();
    int q2 = chef2_queue.size();
    //case 1
    if (chef1_idle && !chef2_idle) {
      chef1_idle_time = curr.arrival;
      int start1 = chef1_idle_time;
      chef1_idle_time += curr.duration;

      if (curr.timeOut < chef1_idle_time) {
        timeoutList[timeoutCount++] = {curr.OID, 1, chef1_idle_time, start1 - curr.arrival};
      }
      continue;
    }
    if (chef2_idle && !chef1_idle) {
      chef2_idle_time = curr.arrival;
      int start = chef2_idle_time;
      chef2_idle_time += curr.duration;

      if (curr.timeOut < chef2_idle_time) {
        timeoutList[timeoutCount++] = {curr.OID, 2, chef2_idle_time, start - curr.arrival};
      }
      continue;
    }
    //case 2
    if (chef1_idle && chef2_idle) {
      chef1_idle_time = curr.arrival;
      int start1 = chef1_idle_time;
      chef1_idle_time += curr.duration;

      if (curr.timeOut < chef1_idle_time) {
        timeoutList[timeoutCount++] = {curr.OID, 1, chef1_idle_time, start1 - curr.arrival};
      }
      continue;
    }
    // case 3 + case 4
    if (q1 <= q2) {   // 優先塞 chef1
      if (!chef1_queue.isFull()) {
        chef1_queue.enqueue(curr);   // case 3
      } else if (!chef2_queue.isFull()) {
        chef2_queue.enqueue(curr);   // case 3 (第二選擇)
      } else {
        // case 4：兩個都滿
        abortList[abortCount++] = {curr.OID, 0, curr.arrival, 0};
      }
    } else { // q2 比較短
      if (!chef2_queue.isFull()) {
        chef2_queue.enqueue(curr);   // case 3
      } else if (!chef1_queue.isFull()) {
        chef1_queue.enqueue(curr);   // case 3
      } else {
        // case 4：兩個都滿
        abortList[abortCount++] = {curr.OID, 0, curr.arrival, 0};
      }
}

    
  }
  while (!chef1_queue.isEmpty()) {
    Order o;
    chef1_queue.dequeue(o);
    if (o.timeOut < chef1_idle_time) {
      abortList[abortCount++] = {o.OID, 1, chef1_idle_time, chef1_idle_time - o.arrival};
    } else {
      int start1 = chef1_idle_time;
      chef1_idle_time += o.duration;

      if (o.timeOut < chef1_idle_time) {
        timeoutList[timeoutCount++] = {o.OID, 1, chef1_idle_time, start1 - o.arrival};
      }
    }
  }
  while (!chef2_queue.isEmpty()) {
    Order o;
    chef2_queue.dequeue(o);
    if (o.timeOut < chef2_idle_time) {
      abortList[abortCount++] = {o.OID, 2, chef2_idle_time, chef2_idle_time - o.arrival};
    } else {
      int start2 = chef2_idle_time;
      chef2_idle_time += o.duration;

      if (o.timeOut < chef2_idle_time) {
        timeoutList[timeoutCount++] = {o.OID, 2, chef2_idle_time, start2 - o.arrival};
      }
    }
  }

  double totalDelay = 0;
  for (int i = 0; i < abortCount; i++) {
    totalDelay += abortList[i].Delay;
  }
  for (int i = 0; i < timeoutCount; i++) {
    totalDelay += timeoutList[i].Delay;
  }
  double failRate = 0;
  if (sharedCount > 0) {
    failRate = (abortCount + timeoutCount) * 100.0 / sharedCount;
  }
  string outFileName = "two" + loadedFileID + ".txt";
  ofstream outFile(outFileName);
  outFile << "\t[Abort List]" << endl;
  outFile << "\tOID\tCID\tDelay\tAbort" << endl;
  for (int i = 0; i < abortCount; i++) {
    outFile << "[" << (i+1) << "]\t" 
            << abortList[i].OID << "\t" 
            << abortList[i].CID << "\t" 
            << abortList[i].Delay << "\t" 
            << abortList[i].Time << endl;
  }
  outFile << "\t[Timeout List]" << endl;
  outFile << "\tOID\tCID\tDelay\tDeparture" << endl;
  for (int i = 0; i < timeoutCount; i++) {
    outFile << "[" << (i+1) << "]\t" 
            << timeoutList[i].OID << "\t" 
            << timeoutList[i].CID << "\t" 
            << timeoutList[i].Delay << "\t" 
            << timeoutList[i].Time << endl;
  }
  outFile << "[Total Delay]\n" << totalDelay << " min.\n";
  outFile << "[Failure Percentage]\n" << fixed << setprecision(2) << failRate << " %\n";
  outFile.close();
  delete[] abortList;
  delete[] timeoutList;
  delete[] ordersCopy;
  
}
int getvalidN() {
  int n;
  std::string a;
  while (1) {
    std::cout <<"\nInput the number of queues: ";;
    std::cin >> a;
    bool isvalid = true;
    bool isnum = true;
    for (int i = 0; i < a.length(); i++) {
      if (a[i] < '0' || a[i] > '9') {
        isnum = false;
        isvalid = false;
        break;
      }
    }
    if (a.length() > 2) {
      isvalid = false;
    }
    if (!isnum) {
      continue;
    } else if (!isvalid) {
      std::cout << "\n### It is NOT in [1,19] ###" << std::endl;
      continue;
    }
    n = std::stoi(a);
    if (n < 1 || n > 19) {
      std::cout << "\n### It is NOT in [1,19] ###" << std::endl;
    } else {
      return n;
    }
  }
}
void task4() {
  if (sharedOrders == nullptr) {
    cout << endl << "### Execute command 2 first! ###" << endl << endl;
    return;
  }

  int chefCount;
  chefCount = getvalidN();
  if (chefCount < 2) chefCount = 2;

    // copy orders
  Order *orders = new Order[sharedCount];
  for (int i = 0; i < sharedCount; i++){
    orders[i] = sharedOrders[i];
  }
    // create queue for each chef
  myQueue *queues = new myQueue[chefCount];
  int *idle = new int[chefCount];
  for (int c = 0; c < chefCount; c++){
    idle[c] = 0;
  }
  LogEntry *abortList = new LogEntry[sharedCount];
  LogEntry *timeoutList = new LogEntry[sharedCount];
  int abortCount = 0, timeoutCount = 0;

  for (int i = 0; i < sharedCount; i++) {
    Order cur = orders[i];

    //所有廚師先清空 queue（只要 idle_time ≤ arrival）

    for (int c = 0; c < chefCount; c++) {
      while (!queues[c].isEmpty() && idle[c] <= cur.arrival) {
        Order o;
        queues[c].dequeue(o);

        int start = idle[c];
        idle[c] += o.duration;

        if (o.timeOut < idle[c]){
          timeoutList[timeoutCount++] = {o.OID, c + 1, idle[c], start - o.arrival};
        }
      }
    }

    //找 idle 廚師
    vector<int> idleChefs;
    for (int c = 0; c < chefCount; c++){
      if (idle[c] <= cur.arrival){
        idleChefs.push_back(c);
      }
    }

    // Case 1/2：有人 idle → 編號最小 idle 的直接做
    if (!idleChefs.empty()) {
      int c = idleChefs[0];  // 編號最小

      idle[c] = cur.arrival;
      int start = idle[c];
      idle[c] += cur.duration;

      if (cur.timeOut < idle[c]){
        timeoutList[timeoutCount++] = {cur.OID, c + 1, idle[c], start - cur.arrival};
      }
        continue;
    }
    // 所有廚師都忙 → SQF 排隊
    int best = -1;
    int bestSize = 999;

    for (int c = 0; c < chefCount; c++) {
      int s = queues[c].size();
      if (s < bestSize) {
        bestSize = s;
        best = c;
      }
    }

        // Case 4
    bool allFull = true;
    for (int c = 0; c < chefCount; c++) {
      if (!queues[c].isFull()) {
        allFull = false;
      }
    }

    if (allFull) {
      abortList[abortCount++] = {cur.OID, 0, cur.arrival, 0};
      continue;
    }

    // 把 cur 放進最短 queue（滿就找下一個最短）
    bool queued = false;
    for (int k = 0; k < chefCount && !queued; k++) {
      // 找第 k 小的 queue
      int target = -1;
      int bestSize2 = 999;

      for (int c = 0; c < chefCount; c++) {
        int s = queues[c].size();
        if (s < bestSize2 && s >= bestSize) {
          bestSize2 = s;
          target = c;
        }
      } 
      if (!queues[target].isFull()) {
        queues[target].enqueue(cur);
        queued = true;
      }
    }
  }

  //所有 queue 的剩餘訂單

  for (int c = 0; c < chefCount; c++) {
    while (!queues[c].isEmpty()) {
      Order o;
      queues[c].dequeue(o);

      int start = idle[c];
      idle[c] += o.duration;

      if (o.timeOut < idle[c]){
        timeoutList[timeoutCount++] = {o.OID, c + 1, idle[c], start - o.arrival};
      }
    }
  }

  double totalDelay = 0;
  for (int i = 0; i < abortCount; i++) totalDelay += abortList[i].Delay;
  for (int i = 0; i < timeoutCount; i++) totalDelay += timeoutList[i].Delay;

  double failRate = (abortCount + timeoutCount) * 100.0 / sharedCount;
  string out;
  if (chefCount > 2) {
    out = "any" + loadedFileID + ".txt";
  } else if (chefCount == 2) {
    out = "two" + loadedFileID + ".txt";
  } else if (chefCount == 1) {
    out = "one" + loadedFileID + ".txt";
  }
  
  ofstream fout(out);

  fout << "\t[Abort List]\n";
  fout << "\tOID\tCID\tDelay\tAbort\n";
  for (int i = 0; i < abortCount; i++){
    fout << "[" << i + 1 << "]\t" 
         << abortList[i].OID << "\t" 
         << abortList[i].CID << "\t" 
         << abortList[i].Delay << "\t" 
         << abortList[i].Time << "\n";
  }
  fout << "\t[Timeout List]\n";
  fout << "\tOID\tCID\tDelay\tDeparture\n";
  for (int i = 0; i < timeoutCount; i++) {
    fout << "[" << i + 1 << "]\t" 
         << timeoutList[i].OID << "\t" 
         << timeoutList[i].CID << "\t" 
         << timeoutList[i].Delay << "\t" 
         << timeoutList[i].Time << "\n";
  }
  fout << "[Total Delay]\n" << totalDelay << " min.\n";
  fout << "[Failure Percentage]\n" << fixed << setprecision(2) << failRate << " %\n";

  fout.close();

  delete[] abortList;
  delete[] timeoutList;
  delete[] orders;
  delete[] queues;
  delete[] idle;
}
