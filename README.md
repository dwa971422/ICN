# R07543096、王識堯、HW3_Report

## 程式碼編譯指令與執行範例
首先是程式碼的編譯指令

	gcc printRoutingTable.c -o printRoutingTable -std=gnu11

接著是程式碼的執行範例

	./printRoutingTable //執行 printRoutingTable 程式
	lf test.txt
	rm r2
	rm r4
	of //程式執行完畢，在程式碼目錄下產生檔名為 test_out2.txt 的輸出檔

## 流程
整個演算法的流程如下：

1. 根據指令讀取 input file 並將其中的資訊儲存下來

2. 接著依照讀取的指令執行不同的處理

	* 如果讀取的指令是 **of**
	
			建立一個輸出檔並將給定的 graph 的 routing table 列印至輸出檔

	* 如果讀取的指令是 **rm**  
	
			繼續等待使用者輸入欲移除的 router 後將之從 graph 上移除，此步驟會反覆進行直到使用者輸入的指令不再是 rm

3.   程式執行完畢

## 函數功能介紹
首先描述演算法執行過程中使用到的不同 function 的功能

1. **void** releaseAllocatedResource(**int**** adjacencyMatrix, **int** routerNumber)

		這個函數用來釋放因儲存 graph 所使用、動態配置的記憶體空間

2. **int** minimalDistanceIndex(**int*** distFromSource, **bool*** shortestPathSet, **int** remainingRouterCnt)

		這個函數會回傳 shortestPathSet 中所有尚未被檢查過的 node 中擁有最小 distance 的 node index

3. **void** printNextRouter(**int*** distFromSource, **int*** parentOfRouter, **int** destination, **int** removeRouterOrNot, **int*** remainingRouterIndex, **FILE*** outputFile)

		這個函數用來尋找從 source 出發走到 destination 的最短路徑中第一個經過的 router index，並將結果列印至 outputFile

4. **void** printRoutingTable(**int*** distFromSource, **int*** parentOfRouter, **int** originRouterCnt, **int** remainingRouterCnt, **int** sourceRouter, **int** removeRouterOrNot, **int*** remainingRouterIndex, **FILE*** outputFile)

		這個函數用來將依照各個使用者指令處理後得到的最終 graph 的 routing table 列印至 outputFile，過程中會呼叫 printNextRouter 函數找到最短路徑中第一個經過的 routerIndex

5. **void** DijkstraAlgorithm(**int**** adjacencyMatrix, **int** originRouterCnt, **int** remainingRouterCnt, **int** sourceRouter, **int** removeRouterOrNot, **int*** remainingRouterIndex, **FILE*** outputFile)

		這個函數使用 Dijkstra's Algorithm 尋找 sourceRouter 到所有其他 router 的最短路徑並呼叫 printRoutingTable 函數將結果列印至 outputFile

6. **void** removeCertainRouter(**int**** adjacencyMatrix, **int** routerNumber, **int** removedRouterIndex)

		這個函數用來將使用者欲移除的 router 從 graph 中移除

7. **int**** NewMatrixAndRemainingRouter(**int**** adjacencyMatrix, **int** routerNumber, **int** remainingRouterCnt, **int**** remainingRouterIndex)

		因為使用者移除了某個 router 後整個 graph 的性質就完全不同了，因此這個函數利用已經有部分 router 被移除的 graph matrix 來製作一個 new Graph matrix 後回傳；同時，這個函數也會建立一個記錄著剩下來的 router 的實際 index 的 array

## 重要執行細節
在**流程**的部分已經提到本演算法的執行步驟，這裡附上一些我覺得比較重要的程式碼部分來更清楚的描述一些功能如何實作

1. 讀取檔案後，首次輸入的指令為 **of**

		fileName[strlen(fileName) - strlen(".txt")] = '\0';
		strcat(fileName, "_out2.txt");
		FILE* result = fopen(fileName, "w");
		
	這段程式碼會將 input file name 轉換成助教指定的 output file name 後再產生一該檔名的 output file
	
		for (int i = 0; i < routerNumber; i++)
			DijkstraAlgorithm(adjacencyMatrix, routerNumber, routerNumber, i, removeRouterOrNot, remainingRouterIndex, result);
			
	產生了 ouput file 後，為了列印出每一個 router 的 routing table，便執行一個 **for** 迴圈，每一次都把 i 當作 source router 來執行 **DijkstraAlgorithm** 函數，如此一來便能取得每一個 router 的 routing table
	
2. 讀取檔案後，首次輸入的指令為 **rm**

		char removeRouterName[REMOVE_ROUTER_NAME_LENGTH];
		
		scanf("%s", removeRouterName);
		char* digitPtr;
		for (int i = 0; i < strlen(removeRouterName); i++) {
			if (isdigit(removeRouterName[i])) {
				digitPtr = &removeRouterName[i];
				break;
			}
		}
		int removedRouterIndex = atoi(digitPtr);
		
		removeCertainRouter(adjacencyMatrix, routerNumber, removedRouterIndex);
		int** newMatrix = NewMatrixAndRemainingRouter(adjacencyMatrix, routerNumber, remainingRouterCnt, &remainingRouterIndex);

	這段程式碼首先建立一個用來儲存使用者輸入的、欲移除的 router 的名稱的變數，接著分析該 router 的 index 後將該 router 從 graph 中移除，移除後再利用現有的 graph matrix 資訊建立 new graph matrix

3. 後續的指令接收

		while (1) {
			scanf("%s", operationName);
			if (strncmp(operationName, "rm", 2) != 0)
				break;
			else {
				remainingRouterCnt--;
				releaseAllocatedResource(newMatrix, remainingRouterCnt);
				free(remainingRouterIndex);
			
				scanf("%s", removeRouterName);
				...

	這段程式碼是一個 while loop，原因在於使用者可能會持續移除不同的 router，因此讓迴圈一直跑直到使用者輸入的指令不再是 **rm** 為止；每當使用者輸入一次 **rm** 指令時意味著原有的 graph matrix 要繼續調整(即移除 router)，這代表剛剛建立的 new graph matrix 是過時的資訊因此不再有效而應當把它佔據的記憶體空間釋放，同理，產生 new graph matrix 同時建立的、記錄著剩下來的 router 的實際 index 的 array 也因為佔據額外的記憶體空間而應該要將之釋放。接下來便是移除使用者欲移除的 router、以原有的 graph matrix 建立 new graph matrix，因為釋放空間之後的行為與第一次收到 **rm** 指令所做的事情極為相似，因此程式碼中以一個 **...** 代替

4. break while loop

		...
		if (remainingRouterCnt <= 0)
			fprintf(result, "there is no router left! every router is removed!\n");
		else {
			for (int i = 0; i < remainingRouterCnt; i++)
				DijkstraAlgorithm(newMatrix, routerNumber, remainingRouterCnt, i, removeRouterOrNot, remainingRouterIndex, result);
		}

	從 while loop 離開意味著使用者輸入的指令是 **of**，因此同樣需要先建立一個輸出檔，檔名維持助教指定的格式，行為與第一次收到 **of** 指令時為了建立輸出檔所做的事情一模一樣，因此以 **...** 代替。之後的行為則依照剩下來的 router 個數是否大於零而有所不同：如果剩下來的 router 個數小於等於零，意味著所有 router 都被使用者移除了，也就不會再有什麼 routing table 了，故輸出一行文字；若剩下來的 router 個數大於零，則直接執行 **DijkstraAlgorithm** 函數取得剩下每一個 router 的 routing table
