## 流程
整個演算法的流程如下：

1. 根據指令讀取 input file 並將其中的資訊儲存下來

2. 接著依照讀取的指令執行不同的處理

	* 如果讀取的指令是 **of**
	
			建立一個輸出檔並將給定的 graph 的 routing table 列印至輸出檔

	* 如果讀取的指令是 **rm**  
	
			繼續等待使用者輸入欲移除的 router 後將之從 graph 上移除，此步驟會反覆進行直到使用者輸入的指令不再是 rm

3.   程式執行完畢

## 執行細節
首先描述演算法執行過程中使用到的 function 的功能

1. **void** releaseAllocatedResource(**int**** adjacencyMatrix, **int** routerNumber)

		這個函數用來釋放因儲存 graph 所使用、動態配置的記憶體空間

2. **int** minimalDistanceIndex(**int*** distFromSource, **bool*** shortestPathSet, **int** remainingRouterCnt)

		這個函數會回傳 shortestPathSet 中所有尚未被檢查過的 node 中擁有最小 distance 的 node index

3. **void** printNextRouter(**int*** parentOfRouter, **int** destination, **int** removeRouterOrNot, **int*** remainingRouterIndex, **FILE*** outputFile)

		這個函數用來尋找從 source 出發走到 destination 的最短路徑中第一個經過的 router index，並將結果列印至 outputFile

4. **void** printRoutingTable(**int*** distFromSource, **int*** parentOfRouter, **int** originRouterCnt, **int** remainingRouterCnt, **int** sourceRouter, **int** removeRouterOrNot, **int*** remainingRouterIndex, **FILE*** outputFile)

		這個函數用來將依照各個使用者指令處理後得到的最終 graph 的 routing table 列印至 outputFile，過程中會呼叫 printNextRouter 函數找到最短路徑中第一個經過的 routerIndex

5. **void** DijkstraAlgorithm(**int**** adjacencyMatrix, **int** originRouterCnt, **int** remainingRouterCnt, **int** sourceRouter, **int** removeRouterOrNot, **int*** remainingRouterIndex, **FILE*** outputFile)

		這個函數使用 Dijkstra's Algorithm 尋找 sourceRouter 到所有其他 router 的最短路徑並呼叫 printRoutingTable 函數將結果列印至 outputFile

6. **void** removeCertainRouter(**int**** adjacencyMatrix, **int** routerNumber, **int** removedRouterIndex)

		這個函數用來將使用者欲移除的 router 從 graph 中移除

7. **int**** NewMatrixAndRemainingRouter(**int**** adjacencyMatrix, **int** routerNumber, **int*** remainingRouterCnt, **int**** remainingRouterIndex)

		因為使用者移除了某個 router 後整個 graph 的性質就完全不同了，因此這個函數用來讀取舊有的、已經有部分 router 被移除的 graph 資訊並製作一個 newGraph 後回傳
