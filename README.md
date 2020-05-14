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
首先描述演算法執行過程中使用到的 function  的功能

1. **void** releaseAllocatedResource(**int**** adjacencyMatrix, **int** routerNumber)  

		這個函數用來釋放因儲存 graph 所使用、動態配置的記憶體空間

2. **int** minimalDistanceIndex(**int*** distFromSource, **bool*** shortestPathSet, **int** remainingRouterCnt)  

		這個函數會回傳 shortestPathSet 中所有尚未被檢查過的 node 中擁有最小 distance 的 node index

3. **void** printNextRouter(**int*** parentOfRouter, **int** destination, **int** removeRouterOrNot, **int*** remainingRouterIndex, **FILE*** outputFile)

		這個函數用來尋找從 destination 出發到目的地的最短路徑中第一個走到的 router index，並將結果列印至 outputFile
