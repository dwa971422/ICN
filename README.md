## 流程  
整個演算法的流程如下：  

1. 根據讀取指令讀取 input file 並將其中的資訊儲存下來  
  
2. 接著反覆的依照讀取的指令執行不同的處理  
  
	* 如果讀取的指令是 **of**  
	建立一個輸出檔並將給定的 graph 的 routing table 印出來  
  
	* 如果讀取的指令是 **rm**  
	繼續等待使用者輸入欲移除的 router 後將之從 graph 上移除  

3.   程式執行完畢

## 執行細節  
首先解釋演算法執行過程中使用到的 function  
  
1. **void** releaseAllocatedResource(**int**** adjacencyMatrix, **int** routerNumber)  
		這個函數用來釋放因儲存 graph 所使用、動態配置的記憶體空間
  
2. **int** minimalDistance(**int*** distFromSource, **bool*** shortestPathSet, **int** remainingRouterCnt)  
		這個函數會回傳 shortestPathSet 中所有尚未被檢查過的 node 中		擁有最小 distance 的 node index
  
3. **void** printNextRouter(**int*** parentOfRouter, **int** destination, **int** removeRouterOrNot, **int*** remainingRouterIndex, **FILE*** outputFile)
		
