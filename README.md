## 流程  
  
整個演算法的流程如下：  

1. 根據讀取指令讀取 input file 並將其中的資訊儲存下來  
  
2. 接著反覆的依照讀取的指令執行不同的處理
 + 如果讀取的指令是 **of**  
建立一個輸出檔並將給定的 graph 的 routing table 印出來  
  
 + 如果讀取的指令是 **rm**  
繼續等待使用者輸入欲移除的 router 後將之從 graph 上移除  

3.   程式執行完畢
